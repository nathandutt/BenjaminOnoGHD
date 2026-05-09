#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem> //To create output directory

#include <vector>
#include <complex>

#include "kinetics.hpp" //Defines Bins and PoleData structs
			//also declares getEigen method for bins
#include "params.hpp"

using namespace std;

/*
 * Program reads poles from evolution output file, 
 * bins them into differents bins along axis
 * Then calculates local lax matrix eigenvalues
 * And writes the eigenvalues per bin to output file
*/

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
	    bins.emplace_back(Bin(start+i*dx, start+(i+1)*dx));
	}
	
    }

    void addPole(const complex<double>& z, const complex<double>& vz){
	//Add pole to correct bin
	int bin_idx= floor((z.real()-start)/dx);
	if((bin_idx < 0) || (bin_idx >= num_bins))
	    return;
	bins[bin_idx].add(z, vz);
    }

    void write(fstream& file) const{
	//Write binned data to file with json-ish format
	//See Bin::write for more info

	file << "Timestep{" << endl;
	file << "	time : " << time << endl;
	file << "	nbins : " << num_bins << endl;
	for(const Bin& bin : bins){
	    bin.write(file);
	}
	file << "}" << endl;
    }
};
int main(int argc, char * argv[]){

    cout << "Calculating local lax matrices and binning values" << endl;

    int pid = 0;
    if(argc > 1){
	stringstream ss(argv[1]);
	ss >> pid;
    }
    //Get global parameters
    string paramfile = "global.params";
    Params params = parseParams(paramfile, pid);

    cout << "# KINETIC ANALYSIS PARAMETERS #" << endl << endl;
    cout << "	X axis is [" << params.x_start << ", " << params.x_end << "]" << endl;
    cout << "	Axis is split into " << params.num_bins << " bins" << endl << endl;

    //Create output folder if doesn't exist
    filesystem::create_directory(params.output_folder);

    //Define input/ouptut filenames
    string pole_file = params.output_folder + "poles.res";
    string velocity_file = params.output_folder + "pole_velocities.res";
    string output_file = params.output_folder + "bins.res";

    //Initialize
    fstream pfile, vfile, ofile;
    string pole_input, vel_input;

    //Open input and output files
    //TODO: add error handling?
    pfile.open(pole_file, ios::in);
    vfile.open(velocity_file, ios::in);
    ofile.open(output_file, ios::out);


    while(getline(pfile, pole_input) && getline(vfile, vel_input)){
	//Each line is a timestep, thus loop over all timestep	

	//Create bins
	BinnedAxis axis(params.x_start, params.x_end, params.num_bins);

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
