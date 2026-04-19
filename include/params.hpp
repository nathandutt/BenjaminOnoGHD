//Header to parse global.params
#pragma once
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

struct Params{
    int N;
    
    double x_start;
    double x_end;
    double num_bins;

    double t_start;
    double t_end;
    double t_step;

    double k_low;
    double k_upp;
    double k_step;
    double b_max;

    std::string output_folder;
};

Params parseParams(const std::string& file, int pid = 0, std::string output_folder = "output"){
    Params p;

    fstream fs;
    fs.open(file, std::ios::in);
    string line;
    while(getline(fs, line)){
	stringstream ss(line);
	string key, value;	

	ss >> key >> value;

	if(key=="N_solitons")
	    p.N = stoi(value);

	if(key=="x_start")
	    p.x_start = stod(value);

	if(key=="x_end")
	    p.x_end = stod(value);

	if(key=="num_bins")
	    p.num_bins = stod(value);

	if(key=="t_start")
	    p.t_start = stod(value);

	if(key=="t_end")
	    p.t_end = stod(value);

	if(key=="t_step")
	    p.t_step = stod(value);

	if(key=="k_low")
	    p.k_low = stod(value);

	if(key=="k_upp")
	    p.k_upp = stod(value);

	if(key=="k_step")
	    p.k_step = stod(value);

	if(key=="b_max")
	    p.b_max = stod(value);	
    }
    p.output_folder = output_folder + to_string(pid) + "/";
    return p;
}
