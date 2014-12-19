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



const int paramsCount = 4;
const double lambda = 0.02;
static const double gamma = 0.005;

double med;
double sum = 1e15;
double previousSum = 1e16;

int maxRating;
int trains;
int tests;
int users;
int moviesCount;

std::map<int, double> bu;
std::map<int, double> bi;
std::map<int, std::vector<double> > qi;
std::map<int, std::vector<double> > pu;
std::map<int, std::map<int, int> > data;

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
        std::map<int, int> currentUser;
        if (data.count(user)) {
            currentUser = data.at(user);
        }
        currentUser.insert(std::make_pair(movie, raiting));
        data.insert(std::make_pair(user, currentUser));
        bu.insert(std::make_pair(user, 0.1)); // TODO : make a some tests for change parametrs
        bi.insert(std::make_pair(movie, 0.1));
        qi.insert(std::make_pair(movie, makeParams()));
        pu.insert(std::make_pair(user, makeParams()));
    }
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

    int countMarks = 0;
    double raitingSum = 0;

    for (std::map<int, std::map<int, int> >::iterator it = data.begin(); it != data.end(); ++it) {
        for (std::map<int, int>::iterator info = it->second.begin();
            info != it->second.end(); ++info) {

            raitingSum += info->second;
            countMarks++;
        }
    }

    med = raitingSum / countMarks;

    // I got it , you got it. We got a magic ....
    // TODO: make a normal limits

    for (size_t id = 0; id < 1000 && sum < previousSum; ++id) {

        for (std::map<int, std::map<int, int> >::iterator it = data.begin();
            it != data.end(); ++it) {
            for (std::map<int, int>::iterator info = it->second.begin();
            info != it->second.end(); ++info) {

                int realRui = info->second;
                double rui = med +
                    bi.at(info->first) +
                    bu.at(it->first) +
                    scalar(info->first, it->first);
                double delta = realRui - rui;

                // Update bu and bi
                bu[it->first] = static_cast<double>(bu[it->first]) +
                                  gamma * (delta - lambda * static_cast<double>(bu[it->first]));
                bi[info->first] = static_cast<double>(bi[info->first]) + 
                                  gamma * (delta - lambda * static_cast<double>(bi[info->first]));

                // Update params

                std::vector<double> _qi(paramsCount);
                std::vector<double> _pu(paramsCount);

                for (size_t i = 0; i < paramsCount; ++i) {
                    _qi[i] = qi.at(info->first)[i] + 
                        gamma * (delta * static_cast<double>(pu.at(it->first)[i]) - 
                        lambda * static_cast<double>(qi.at(info->first)[i]));
                    _pu[i] = pu.at(it->first)[i] +
                        gamma * (delta * static_cast<double>(qi.at(info->first)[i]) - 
                        lambda * static_cast<double>(pu.at(it->first)[i]));
                }

                qi[info->first] = _qi;
                pu[it->first] = _pu;
            }
        }

        previousSum = sum;
        sum = 0;

        for (std::map<int, std::map<int, int> >::iterator it = data.begin();
            it != data.end(); it++) {
            for (std::map<int, int>::iterator info = it->second.begin();
                info != it->second.end(); info++) {
                
                double rui = med +
                    bi.at(info->first) +
                    bu.at(it->first) +
                    scalar(info->first, it->first);

                sum += (info->second - rui) * (info->second - rui) + lambda * 
                       (bi.at(info->first) * bi.at(info->first) +
                        bu.at(it->first) * bu.at(it->first) + norimalize(qi.at(info->first))
                       + norimalize(pu.at(it->first)));
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
