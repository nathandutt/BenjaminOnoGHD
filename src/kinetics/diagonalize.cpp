#include <iostream>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "kinetics.hpp"

//Type name changes declaration
using namespace std;
using Matrix = Eigen::MatrixXcd;
using Vector  = Eigen::VectorXcd;
using cplx = std::complex<double>;

std::vector<cplx> localZdot(const Bin& bin){
    //FIll with correct formula from matsuno
    int N = bin.poles.size();
    std::vector<cplx> zdots(N);
    for(int i = 0; i < N; i++){
	const cplx& zi = bin.poles[i].z;
	zdots[i] += 1./(zi - conj(zi));
	for(int j = 0; j < i; j++){
	    const cplx& zj = bin.poles[j].z;
	    zdots[i] += (1./(zj - zi) + 1./(zi - conj(zj)));
	    zdots[j] += (1./(zi - zj) + 1./(zj - conj(zi)));
	}

    }
    for(int i = 0; i< N; i++){
	zdots[i] *= cplx(0., 2.);
    }
    return zdots;
}
Matrix LaxMatrix(const Bin& bin){
    //Creates local lax matrix for bin

    //Initialize
    const vector<PoleData>& p = bin.poles;
    int N = (bin.poles).size();
    vector<cplx> zdots = localZdot(bin);
    Matrix lax(N, N);

    //Fill matrix
    for(int i = 0; i < N; i++){
	//lax(i, i) = ((bin.poles)[i]).zdot;
	lax(i, i) = zdots[i];
	for(int j = 0; j < i; j++){
	  lax(i, j) = cplx(0., -2.)/(p[i].z - p[j].z); 
	  lax(j, i) = cplx(0., -2.)/(p[j].z - p[i].z); 
	}
    }
    
    return lax;
}

void Bin::getEigen(){
    //Diagonalizes bin's local lax matrix, and fills eigenvalues field

    //Create lax matrix
    Matrix lax = LaxMatrix(*this);
    int N = lax.rows();

    if(N == 0) 	//Handle empty bin
	return;

    //Diagonalize lax matrix
    Eigen::ComplexEigenSolver<Matrix> solver(lax);
    Vector eigenvalues_vec = solver.eigenvalues();

    //Store eigenvalues in Bin's "eigenvalues" field
    eigenvalues.reserve(N);
    for(int i = 0; i < N; i++){
	eigenvalues.emplace_back(eigenvalues_vec(i));
    }

}
