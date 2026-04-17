#include "npy.hpp"
#include <Eigen/Dense>
#include <complex>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "pole.hpp"
using namespace std;

//Config struct for all parameters
struct Config{
    double tstep = 9.9;
    double tstart = 0.;
    double tend = 10.;
    string writefile = "output/poles.res";
    string velocityfile = "output/pole_velocities.res";
    string paramfile = "output/solitons.params";
    bool random = false; //Generate random parameters for solitons
};

void readParams(fstream& file, vector<cplx>& k_s, vector<cplx>& b_s){
    //Read soliton parameters from file
    double k_re, k_im, b_re, b_im;
    while(file >> k_re >> k_im >> b_re >> b_im){
	k_s.emplace_back(complex(k_re, k_im));
	b_s.emplace_back(complex(b_re, b_im));
    }

}

int main(){
    //Initialize
    std::fstream file;
    std::fstream file2;
    Config config; // create config
    vector<cplx> k_s{}; vector<cplx> b_s{};

    //Read generated soliton data
    file.open(config.paramfile, ios::in);
    readParams(file, k_s, b_s);
    file.close();
    
    //Calculate determinants <=> evolve solitons

    file.open(config.writefile, ios::out);
    file2.open(config.velocityfile, ios::out);
    InitialData data(k_s, b_s, config.tstart, config.tend, config.tstep); 
    getPoles(file, file2, data);	//Note : This is coded in separate file 
					//to improve compile time avoiding recompiling
					//Eigen every time
    file.close();
}


