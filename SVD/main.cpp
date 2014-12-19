#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <hash_map>
#include <map>
#include <vector>

/*
    Dmitry Prokopenko
    NRU ITMO
    December 2014
*/

// If it works, then I wrote it. Otherwise, someone changed the code



const int paramsCount = 3;
const double lambda = 0.02;
static const double alfa = 0.005;

double med;
double sum = 1e15;
double previousSum = 1e16;

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

std::map<int, double> bu;
std::map<int, double> bi;
std::map<int, std::vector<double> > qi;
std::map<int, std::vector<double> > pu;
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

    double totalRaiting = 0;

    for (size_t id = 0, user, movie, raiting; id < trains; ++id) {
        std::cin >> user >> movie >> raiting;
        totalRaiting += raiting;
        data.push_back(Data(user, movie, raiting));
        bu.insert(std::make_pair(user, 0.1)); // TODO : make a some tests for change parametrs
        bi.insert(std::make_pair(movie, 0.1));
        qi.insert(std::make_pair(movie, makeParams()));
        pu.insert(std::make_pair(user, makeParams()));
    }

    med = totalRaiting / data.size();
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
        for (size_t idx = 0; idx < data.size();++idx) {

                int realRui = data[idx].raiting;
                double rui = med +
                    bi.at(data[idx].movie) +
                    bu.at(data[idx].user) +
                    scalar(data[idx].movie, data[idx].user);
                double delta = realRui - rui;

                sum += delta * delta;

                // Update bu and bi
                bu[data[idx].user] = static_cast<double>(data[idx].user) +
                    alfa * (delta - lambda * static_cast<double>(data[idx].user));
                bi[data[idx].movie] = static_cast<double>(bi[data[idx].movie]) +
                    alfa * (delta - lambda * static_cast<double>(bi[data[idx].movie]));

                // Update params

                for (size_t i = 0; i < paramsCount; ++i) {
                    double _qi = qi[data[idx].movie][i];
                    double _pu = pu[data[idx].user][i];
                    qi[data[idx].movie][i] += alfa * (delta * _pu - lambda * _qi);
                    pu[data[idx].user][i] += alfa * (delta * _qi - lambda * _pu);
                }
            }
    }
}

void getans() {

    // cout << "done" << endl;

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
