//Quick script to generate N random solitons for benjamin ono.

#include <random>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include <set>

using namespace std;

struct Config{
    double low_k = 0.1;
    double upper_k = 50.;
    double delta_k = 0.01;
    double b_max = 500.;
    int N_solitons = 1000;
    std::string writefile = "output/solitons.params";
};

double smallestDistance(const std::set<double>& s, double target) {
    //Just to find smallest distance to set
    if (s.empty()) return -1.; // Handle empty set
    auto it = s.lower_bound(target);
    if (it == s.begin()) return *it;
    if (it == s.end()) return *s.rbegin();
    double upper = *it;
    double lower = *(--it);
    double f = (std::abs(upper - target) < std::abs(lower - target)) ? upper : lower;
    return std::abs(f - target);
}

std::vector<double> drawRepulsion(const Config& c, mt19937& gen){
    std::set<double> pts{};
    std::uniform_real_distribution<> dis1(c.low_k, c.upper_k);
    std::uniform_real_distribution<> disb(0., c.b_max);

    int added = 0;
    int tries = 0;
    while(added < c.N_solitons){
	tries+=1;
	if(tries % 20 == 0)
	    cout << "Redrawn " << tries << " times" << endl;
	double new_k = dis1(gen);
	double dist = smallestDistance(pts, new_k);
	if(dist < 0. || dist > c.delta_k){
	    pts.insert(new_k);
	    tries = 0;
	    added += 1;
	}
    }
    std::vector<double> ks;
    ks.reserve(c.N_solitons);
    for(const double k: pts)
	ks.emplace_back(k);
    return ks;
}

int main(){
    std::fstream file;
    Config c;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis2(0., c.b_max);

    file.open(c.writefile, std::ios::out);

    vector<double> bs;
    bs.reserve(c.N_solitons);

    for(int i = 0; i<c.N_solitons; i++){
	double gen_b = dis2(gen);
	bs.emplace_back(gen_b);
    }

    vector<double> ks = drawRepulsion(c, gen);

    for(int i = 0; i < c.N_solitons; i++){
	file << ks[i] << " " << 0 << " " << bs[i] << " " << (1./ks[i]) << std::endl;
    }
    file.close();
}


