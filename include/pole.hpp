#pragma once
#include <complex>
#include <vector>
#include <fstream>
//We want to not use eigen here, only in cpp file,
//so compilation isn't too long
using cplx = std::complex<double>;
using std::vector;
using std::fstream;

struct InitialData{
    vector<cplx> k_s;
    vector<cplx> b_s;
    double tstart;
    double tend;
    double tstep;
    InitialData(vector<cplx> k_s, vector<cplx> b_s, double tstart, double tend, double tstep):
	k_s(k_s), b_s(b_s), tstart(tstart), tend(tend), tstep(tstep) {}
};

void getPoles(fstream& file, fstream& file2, const InitialData& data);
