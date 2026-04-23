#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "params.hpp"

using namespace std;

int simu = 1; //Number of simulations

fstream& skiplines(fstream& file, int n){
    string nul;
    for(int i = 0; i < n; i++)
	getline(file, nul);
    return file;
}

int main(){
    Params params = parseParams("../global.params");

    fstream file;
    fstream outfile;
    outfile.open("allbins.res", ios::out);

    string prefix = params.output_folder;
    cout << "Prefix " << prefix << endl;
    prefix.pop_back(); prefix.pop_back(); //Original prefix is generally like output0/
    prefix = "../" + prefix;
    string suffix = "/bins.res";
    //Read each simulation file

    int bins = params.num_bins;
    for(int i = 1; i <= simu; i++){
	//Open simulation output file
	string filename = prefix + to_string(i) + suffix;
	cout << "Reading data from simulation " << i << " in file " << filename << endl;

	file.open(filename, ios::in);

	//Check if file is open succesfully
	if(!file.is_open()){
	    string er = "File not found : " + filename;
	    throw std::runtime_error(er);
	}

	string timestep;
	int simtime = 0;
	while((getline(file, timestep)) && (timestep=="Timestep{")){
	    simtime++;
	    cout << simtime << endl;

	    //Get time
	    double time;
	    string tstring; getline(file, tstring);
	    stringstream t(tstring); string nu;
	    t >> nu >> nu >> time;
	    skiplines(file, 1);

	    //Loop over bins
	    for(int bin_id = 0; bin_id < bins; bin_id++){
		skiplines(file, 4);	
		char ch; double num1, num2;

		string eigs; getline(file, eigs); stringstream e(eigs);
		//Loop over eigenvalues
		while((e >> ch) && (ch == '(')){
		    e >> num1 >> ch >> num2 >> ch >> ch;
		    outfile << time << " " << bin_id << " " << num1 << endl;
		}
		skiplines(file, 2);
	    }
	    skiplines(file, 1);
	}
	file.close();	
    }
    outfile.close();
}
