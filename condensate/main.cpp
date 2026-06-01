#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

struct Params{
    int N = 100.;
    double cond_k = 1.;
    double t_k = 2.;

    Params(const string& filename){
	fstream file;
	file.open(filename, ios::in);
	string s;
	getline(file, s); //SKIP FIRST LINE
	getline(file, s);
	stringstream ss(s);

	ss >> N >> cond_k >> t_k;
    }


    
};


int main(){
    fstream file;
    file.open("output.res", ios::out);
    
    Params p("condensate.params");
    double dk = (p.cond_k)/(p.N);
    double L = 2*M_PI*p.N/p.cond_k;
    double t_x0 = -1.05*L;

    for(int i = 1; i <= p.N; i++){
	double k = dk*i;
	file << k << " " << 0 << " " << 0 << " " << 1./k << endl;
    }
    
    file << p.t_k << " " << 0 << " " << t_x0 << " " << 1./p.t_k << endl;

}
