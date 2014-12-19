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

// Если это работает , то это написал я. В противном случае бабайка залезла в мой код


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

void readData(){

	cin >> maxRating >> users >> moviesCount >> trains >> tests;

	for (size_t i = 0, user, movie, raiting; i < trains; i++){

		cin >> user >> movie >> raiting;

		map<int, int> currentUser;

		if (data.count(user)){
			currentUser = data.at(user);
		}

		currentUser.insert(make_pair(movie, raiting));
		data.insert(make_pair(user, currentUser));

		bu.insert(make_pair(user, 0.1)); // TODO : сделать пару тестов для изменения параметра
		bi.insert(make_pair(movie, 0.1)); 
		qi.insert(make_pair(movie, makeParams()));
		pu.insert(make_pair(user, makeParams()));
	}
}

double scalar(int movie, int user){
	
	double res = 0;
	for (size_t i = 0; i < paramsCount; i++){
		res += 0;
	}
	return res;
}

// Дать мозги , но не довести до восстания машин
void train(){

	int countMarks = 0;
	double raitingSum = 0;

	for (map<int, map<int, int>>::iterator it = data.begin(); it != data.end(); it++){
		for (map<int, int>::iterator info = it->second.begin(); info != it->second.end(); info++){

			raitingSum += info->second;
			countMarks++;
		}
	}

	// Ухууу мы получили средний рейтинг
	u = raitingSum / countMarks;

	// I got it , you got it. We got a magic ....
	// TODO придумать нормальные ограничения цикла

	for (size_t i = 0; i < 1000 && sum < previousSum; i++){

		for (map<int, map<int, int>>::iterator it = data.begin(); it != data.end(); it++){
			for (map<int, int>::iterator info = it->second.begin(); info != it->second.end(); info++){

				int realRui = info->second;
				double rui = u +
					bi.at(info->first) +
					bu.at(info->first) +
					scalar(info->first, it->first);
				double delta = realRui - rui;

				//Обновляем bu и bi
				bu[it->first] = bu[it->first] + gamma * (delta - lambda * bu[it->first]);
				bi[info->first] = bi[info->first] + gamma * (delta - lambda * bi[info->first]);

				//Обновляем параметры

				vector<double> _qi(paramsCount);
				vector<double> _pu(paramsCount);

				for (size_t i = 0; i < paramsCount; i++){
					_qi[i] = qi.at(info->first)[i] + 
						gamma * (delta * pu.at(it->first)[i] - lambda * qi.at(info->first)[i]);
					_pu[i] = pu.at(it->first)[i] +
						gamma * (delta * qi.at(info->first)[i]) - lambda * pu.at(it->first)[i]);
				}

				qi[info->first] = _qi;
				pu[it->first] = _pu;
			}
		}

		previousSum = sum;
		sum = 0;

		for (map<int, map<int, int>>::iterator it = data.begin(); it != data.end(); it++){
			for (map<int, int>::iterator info = it->second.begin(); info != it->second.end(); info++){


			}
		}


	}

}

void getans(){

}

int main(){

	readData();
	train();
	getans();

	return 0;
}