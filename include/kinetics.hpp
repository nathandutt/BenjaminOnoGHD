#pragma once
#include <vector>
#include <complex>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

struct PoleData{
    //to store Z and Z_dot
    complex<double> z;
    complex<double> zdot;
};

struct Bin{
    //Struct to bin different poles
    double start;
    double end;
    vector<PoleData> poles;
    vector<complex<double>> eigenvalues;
    Bin(double start, double end) : 
	start(start), end(end) {
	    poles = vector<PoleData>{};
	    eigenvalues = vector<complex<double>>{};
	}

    void add(complex<double> z, complex<double> zdot){
	poles.emplace_back(PoleData{z, zdot});
    }

    void getEigen();  //Calculates eigenvalues of lax matrix of poles in bin;

    void write(fstream& file) const{
	file << "Bin{" << endl;
	file << "	" << "start : " << start << "," << endl;
	file << "	" << "end : " << end << "," << endl;
	file << "	" << "eigenvalues : [" << endl;
	file << "	";
	for(const complex<double>& c : eigenvalues){
	    file << c << ", ";
	}
	file << endl <<"	]" << endl;
	file << "}" << endl;
    }
};


