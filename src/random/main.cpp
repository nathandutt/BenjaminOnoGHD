//Quick script to generate N random solitons for benjamin ono.
//Generates k parameters with a fixed minimum distance, redrawing while not 
//true

#include <random>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include <set>
#include "params.hpp"
#include <filesystem>

using namespace std;

double smallestDistance(const std::set<double>& s, double target) {
    //Just to find smallest distance to set storing previously drawn ks
    if (s.empty()) return -1.; // Handle empty set
    auto it = s.lower_bound(target);
    if (it == s.begin()) return std::abs(target - *it);
    if (it == s.end()) return std::abs(target - *s.rbegin());
    double upper = *it;
    double lower = *(--it);
    double f = (std::abs(upper - target) < std::abs(lower - target)) ? upper : lower;
    return std::abs(f - target);
}

std::vector<double> drawRepulsion(const Params& p, mt19937& gen){
    //To draw the ks with set repulsion
    std::set<double> pts{};
    std::uniform_real_distribution<> dis1(p.k_low, p.k_upp);

    int added = 0;
    int tries = 0;
    while(added < p.N){
	tries+=1;
	if(tries % 20 == 0)
	    cout << "Redrawn " << tries << " times" << endl;
	double new_k = dis1(gen);
	double dist = smallestDistance(pts, new_k);
	
	if(dist < 0. || dist > p.k_step){
	    pts.insert(new_k);
	    tries = 0;
	    added += 1;
	}
    }
    std::vector<double> ks;
    ks.reserve(p.N);
    for(const double k: pts)
	ks.emplace_back(k);
    return ks;
}

int main(int argc, char * argv[]){

    cout << endl << "Generating random soliton initial parameters" << endl;
    int pid = 0;
    if(argc > 1){
	stringstream ss(argv[1]);
	ss >> pid;
    }
    //Get global parameters
    string paramfile = "global.params";
    Params p = parseParams(paramfile, pid);

    cout << "# RANDOM PARAMETERS # " << endl << endl;
    cout << "	Generating " << p.N << " solitons" << endl;
    cout << "	k selected in [" << p.k_low << ", " << p.k_upp <<"]" << endl;
    cout << "	with minimum distance between ks: "  << p.k_step << endl << endl;

    //Create output folder if doesn't exist
    filesystem::create_directory(p.output_folder);

    //Initialize random number generator with seed from random device
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis2(0., p.b_max);

    //File to write soliton parameters to
    string writefile = p.output_folder + "solitons.params";
    fstream file;
    file.open(writefile, std::ios::out);

    //Initialize and fill b vector
    vector<double> bs;
    bs.reserve(p.N);

    for(int i = 0; i<p.N; i++){
	double gen_b = dis2(gen);
	bs.emplace_back(gen_b);
    }

    //Draw k parameters
    vector<double> ks = drawRepulsion(p, gen);

    //Write parameters to file
    //For generalizability purposes, k and b are written as 
    //"k.real() k.imag() b.real() b.imag()"
    //Because we might want to take k non real sometime in future.

    for(int i = 0; i < p.N; i++){
	file << ks[i] << " " << 0 << " " << bs[i] << " " << (1./ks[i]) << std::endl;
    }
    file.close();
}


