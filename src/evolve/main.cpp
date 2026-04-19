#include <complex>
#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <filesystem>

#include <time.h> //For timing evolution

#include <Eigen/Dense> //For matrix diagonalization

#include "pole.hpp" //Header for methods written in pole.cpp
#include "params.hpp" //Global parameter struct
using namespace std;

/*
 * This program reads soliton parameters from file
 * then it calculates soliton positions at different 
 * times by diagonalizing Hirota matrices. It also
 * calculates pole velocities using eigenvectors.
 */

void readSolitonParams(fstream& file, vector<cplx>& k_s, vector<cplx>& b_s){
    //Read soliton parameters from file
    double k_re, k_im, b_re, b_im;
    while(file >> k_re >> k_im >> b_re >> b_im){
	k_s.emplace_back(complex(k_re, k_im));
	b_s.emplace_back(complex(b_re, b_im));
    }
}

int main(int argc, char * argv[]){

    clock_t time_start = clock();

    cout << "Evolving solitons " << endl;
    int pid = 0;
    if(argc > 1){
	stringstream ss(argv[1]);
	ss >> pid;
    }
    //Get global parameters
    string paramfile = "global.params";
    Params params = parseParams(paramfile, pid);

    cout << "# EVOLUTION PARAMETERS #" << endl << endl;
    cout << "	Evolving for t in [" << params.t_start << ", " << params.t_end << "]" << endl; 
    cout << "	timestep is : " << params.t_step << endl << endl;
    //Create output folder if doesn't exist
    filesystem::create_directory(params.output_folder);

    //Define different files to read/write from
    string polefile = params.output_folder + "poles.res";
    string velocityfile = params.output_folder + "pole_velocities.res";
    string solitonfile = params.output_folder + "solitons.params";

    //Initialize
    std::fstream file;
    std::fstream file2;
    vector<cplx> k_s{}; vector<cplx> b_s{};

    //Read generated soliton data
    file.open(solitonfile, ios::in);
    readSolitonParams(file, k_s, b_s);
    file.close();
    
    //Calculate determinants <=> evolve solitons

    file.open(polefile, ios::out);
    file2.open(velocityfile, ios::out);
    InitialData data(k_s, b_s, params.t_start, params.t_end, params.t_step); 
    getPoles(file, file2, data);	//Note : This is coded in separate file 
					//to improve compile time avoiding recompiling
					//Eigen every time
    file.close();

    clock_t time_end = clock();
    cout << "Total execution time for evolution script : " << (double)(time_end - time_start)/CLOCKS_PER_SEC << " s" << endl << endl;
}


