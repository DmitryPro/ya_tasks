#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <hash_map>
#include <map>
#include <vector>

using namespace std;

const int paramsCount = 4;
const double lambda = 0.02;
const double gamma = 0.005;

double u;
double sum;
double previousSum;

int maxRating;
int trains;
int tests;
int users;
int moviesCount;

map<int, double> bu;
map<int, double> bi;
map<int, vector<double>> qi;
map<int, vector<double>> pu;
map<int, map<int, int>> data;

vector<double> makeParams(){
	vector<double> res(paramsCount);
	for (int i = 0; i < paramsCount; i++){
		res[i] = rand();
	}
	return res;
}

void readdata(){

	cin >> maxRating >> users >> moviesCount >> trains >> tests;
	for (int i = 0, user, movie, raiting; i < trains; i++){

		cin >> user >> movie >> raiting;
		map<int, int> currentUser;
		if (data.count(user)){
			currentUser = data.at(user);
		}
		currentUser.insert(make_pair(movie, raiting));
		data.insert(make_pair(user, currentUser));

		bu.insert(make_pair(user, 0.1));
		bi.insert(make_pair(movie, 0.1));
		qi.insert(make_pair(movie, makeParams()));
		pu.insert(make_pair(user, makeParams()));
	}
}

void train(){

}

void getans(){

}

int main(){

	readdata();
	train();
	getans();

	return 0;
}