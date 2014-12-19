#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <unordered_map>
#include <vector>

/*
    Dmitry Prokopenko
    NRU ITMO
    December 2014
*/

// If it works, then I wrote it. Otherwise, someone changed the code



const int paramsCount = 10;
const double lambda = 0.02;
double alfa = 0.005;

double med = 0;
double sum = 0;
double previousSum = 2;
double threshold = 0.01;

int maxRating;
int trains;
int tests;
int users;
int moviesCount;

struct Data {

    int user;
    int movie;
    int raiting;
    Data(int _user, int _movie, int _raiting) : user(_user),
        movie(_movie), raiting(_raiting) { }
};

std::unordered_map<int, double> bu;
std::unordered_map<int, double> bi;
std::unordered_map<int, std::vector<double> > qi;
std::unordered_map<int, std::vector<double> > pu;
std::vector<Data> data;

std::vector<double> makeParams() {

    std::vector<double> res(paramsCount);
    for (size_t i = 0; i < paramsCount; ++i) {

        res[i] = 0;
    }
    return res;
}

void readData() {

    std::cin >> maxRating >> users >> moviesCount >> trains >> tests;

    for (size_t id = 0, user, movie, raiting; id < trains; ++id) {
        std::cin >> user >> movie >> raiting;
        med += raiting;
        data.push_back(Data(user, movie, raiting));
        bu.insert(std::make_pair(user, 0));
        bi.insert(std::make_pair(movie, 0));
        qi.insert(std::make_pair(movie, makeParams()));
        pu.insert(std::make_pair(user, makeParams()));
    }
    med /= trains;
}

double scalar(int movie, int user) {
    
    double res = 0;
    for (size_t i = 0; i < paramsCount; ++i) {
        res += qi.at(movie)[i] * pu.at(user)[i];
    }
    return res;
}

double norimalize(std::vector<double> toNorm) {
    
    double res = 0;
    for (size_t i = 0; i < paramsCount; ++i) {
        res += toNorm[i] * toNorm[i];
    }
    return res;
}


void train() {

    // I got it , you got it. We got a magic ....
    // TODO: make a normal limits

    for (size_t id = 0; id < 150 && sum < previousSum; ++id) {
         
        previousSum = sum;
        sum = 0;
        for (size_t idx = 0; idx < data.size(); ++idx) {

            int user = data[idx].user;
            int movie = data[idx].movie;
            int raiting = data[idx].raiting;
            double delta = raiting - med - bu.at(user) - bi.at(movie) - scalar(movie, user);
            sum += delta * delta;

            bu[user] += alfa * (delta - lambda * bu.at(user));
            bi[movie] += alfa * (delta - lambda * bi.at(movie));

            // Update params

            for (size_t i = 0; i < paramsCount; ++i) {
                double _qi = qi[data[idx].movie][i];
                double _pu = pu[data[idx].user][i];
                qi[data[idx].movie][i] += alfa * (delta * _pu - lambda * _qi);
                pu[data[idx].user][i] += alfa * (delta * _qi - lambda * _pu);
            }
        }
        sum = sqrt(sum / data.size());
    }
}

void getans() {

    // cout << "done" << endl;
    std::cout.precision(7);
    std::cout << std::fixed;

    for (size_t id = 0; id < tests; ++id) {
        int user;
        int movie;
        std::cin >> user >> movie;
        double rui = med;
        if (bi.count(movie)) {
            rui += bi.at(movie);
        }
        if (bu.count(user)) {
            rui += bu.at(user);
        }
        if (bi.count(movie) && bu.count(user)) {
            rui += scalar(movie, user);
        }
        
        std::cout << rui << std::endl;
    }
}

int main() {

    readData();
    train();
    getans();
    return 0;
}
