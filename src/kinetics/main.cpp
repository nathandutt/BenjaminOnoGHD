#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <complex>

#include "kinetics.hpp" //Defines Bins and PoleData structs
			//also declares getEigen method for bins

using namespace std;

/*
 * Program reads poles from evolution output file, 
 * bins them into differents bins along axis
 * Then calculates local lax matrix eigenvalues
 * And writes the eigenvalues per bin to output file
*/

struct Config{

    double start = 0.;
    double end = 1000.;
    int nbins = 10;
    string pole_file = "output/poles.res";
    string velocity_file = "output/pole_velocities.res";
    string output_file = "output/bins.res";

};

struct BinnedAxis{
    
    //Fields
    double start;
    double end;
    double dx;
    double time;
    int num_bins;
    vector<Bin> bins;

    //Constructor
    BinnedAxis(double start, double end, int N) :
	start(start), end(end), num_bins(N)
    {
	bins = vector<Bin>{};	
	bins.reserve(num_bins);
	dx = (end - start)/N;

	for(int i = 0; i < N; i++){
	    bins.emplace_back(Bin(i*dx, (i+1)*dx));
	}
	
    }

    void addPole(const complex<double>& z, const complex<double>& vz){
	//Add pole to correct bin
	int bin_idx= floor(z.real()/dx);
	if((bin_idx < 0) || (bin_idx >= num_bins))
	    return;
	bins[bin_idx].add(z, vz);
    }

    void write(fstream& file) const{
	//Write binned data to file with json-ish format
	//See Bin::write for more info

	file << "Timestep{" << endl;
	file << "time : " << time << endl;
	file << "nbins : " << num_bins << endl;
	for(const Bin& bin : bins){
	    bin.write(file);
	}
	file << "}" << endl;
    }
};
int main(){
    //Initialize
    Config config;    
    fstream pfile, vfile, ofile;
    string pole_input, vel_input;

    //Open input and output files
    //TODO: add error handling?
    pfile.open(config.pole_file, ios::in);
    vfile.open(config.velocity_file, ios::in);
    ofile.open(config.output_file, ios::out);


    while(getline(pfile, pole_input) && getline(vfile, vel_input)){
	//Each line is a timestep, thus loop over all timesteps
	

	//Create bins
	BinnedAxis axis(config.start, config.end, config.nbins);

	//Extract pole data and pole velocity data from input lines
	stringstream ps(pole_input), vs(vel_input);

	//Set time
	ps >> axis.time; 

	//Add poles to bins
	double p_re, p_im, v_re, v_im;
	while((ps >> p_re >> p_im) && (vs >> v_re >> v_im)){
	    complex<double> z(p_re, p_im);
	    complex<double> vz(v_re, v_im);
	    axis.addPole(z, vz);
	}
	
	//For each bin, compute local Lax eigenvalues
	for(Bin& b : axis.bins){
	    cout << b.poles.size() << endl;
	    b.getEigen();		   //Note: also coded in seperate file, to
			 		   //minimize recompilation of Eigen lib
	}
	//Write bins to file for this timestep
	axis.write(ofile);
    }

    //Close files
    ofile.close();
    pfile.close();
    vfile.close();
}
