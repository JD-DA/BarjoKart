#include <iostream>
#include <string>
#include <list>
#include "image.hpp"
#include "trajectoire.hpp"
#ifndef AUTOMATE_H
#define AUTOMATE_H

class Automate{
public:
	int xCourant;
	int yCourant;
	Image* img;
	Trajectoire* traj;
	std::string precedenteDirection;
	bool etapeDouble;

	//std::list<std::pair<int,int>> trajectoire;

	Automate(Image*, Trajectoire*);
	virtual ~Automate();

	std::pair<int,int> uncoup();
	void machine();
	std::string inverse(std::string);
};

#endif