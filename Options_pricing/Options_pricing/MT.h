#ifndef MT_H
#define MT_H
#include <random>

class MT {
public:
	static double rand_unif();
	static double rand_norm();
private:
	MT();
	MT(const MT&) = delete;  //against copy
	MT& operator=(const MT&) = delete; //against assignment

	static MT& getInstance();

	std::mt19937 mt;
	std::uniform_real_distribution<double> uniformDist;
	std::normal_distribution<double> normalDist;
};



#endif // !MT_H
