#include "pole.hpp"

#include <Eigen/Dense>
#include <iostream>
#include <string>

using namespace std;
using Matrix = Eigen::MatrixXcd;
using Vector  = Eigen::VectorXcd;
using cplx = std::complex<double>;

Matrix hirota(const vector<cplx>& k_s, const vector<cplx>& b_s, const double time){
    int N = k_s.size();
    //Create initial hirota matrix
    Matrix ret(N, N);
    for(int i =0; i<N; i++){
	ret(i, i) = k_s[i]*time + b_s[i];
	for(int j= 0; j < i; j++){
	    ret(i, j) = complex(0., -2.)/(k_s[i] - k_s[j]);
	    ret(j, i) = complex(0., -2.)/(k_s[j] - k_s[i]);
	}
    }
    return ret;
}

void increment(Matrix& h, const vector<cplx>& k_s, const double tstep){
    int N = k_s.size();
    //Function to increment matrix, instead of recreating it at each time
    for(int i =0; i < N; i++){
	h(i,i) += tstep*k_s[i];
    }
}

Vector diagonalize(const Matrix& h, Matrix& VR_, Matrix& VL_){
    //Diagonalize matrix, get eigenvalues and store eigenvectors too.
    Eigen::ComplexEigenSolver<Matrix> solver(h);

    Vector eigenvalues_vec = solver.eigenvalues();
    Matrix VR              = solver.eigenvectors();
    Matrix VL = VR.inverse();
    VR_ = VR; VL_ = VL;
    return eigenvalues_vec;
}

Vector getVelocity(const Matrix& k, const Matrix& VL, const Matrix& VR){
    int N = k.rows();
    Vector res(N);
    Matrix d = VL * k * VR;
    for(int i = 0; i < N; i++){
	res(i) = d(i, i);
    }
    return res;
}
void writePoles(fstream& file, const double time, const Vector& eigs){
    int N = eigs.size();
    file << time << " ";
    for(int i = 0; i < N; i++){
	file << eigs(i).real() << " " << eigs(i).imag() << " ";
    }
    file << endl;
}
void writeVelocity(fstream& file, const Vector& eigdot){
    int N = eigdot.size();
    for(int i = 0; i < N; i++){
	file << eigdot(i).real() << " "<< eigdot(i).imag() << " ";
    }
    file << endl;
}
void getPoles(fstream& file, fstream& file2, const InitialData& data){
    int N = (data.k_s).size();
    double tcurr = data.tstart;
    Matrix hMatrix = hirota(data.k_s, data.b_s, data.tstart);
    
    Matrix k(N, N);
    for(int i = 0; i < N; i++){
	k(i, i) = data.k_s[i];
	for(int j = 0; j < i; j++){
	    k(i, j) = 0.;
	    k(j, i) = 0.;
	}
    }
    while(tcurr < data.tend){
	Matrix VR(N, N), VL(N,N);
	Vector eigs = diagonalize(hMatrix, VR, VL);
	Vector eigdot = getVelocity(k, VL, VR);
	writePoles(file, tcurr, eigs);
	writeVelocity(file2, eigdot);	
	tcurr += data.tstep;
	increment(hMatrix, data.k_s, data.tstep);	
    } 
}


