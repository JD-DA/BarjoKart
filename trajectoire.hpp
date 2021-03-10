#include <iostream>
#include <string>
#include <list>
#include "image.hpp"
#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H


class Trajectoire{
public:
	int size;

	std::map<int,int*> etapes;


	int departureX;
	int departureY;
	int maxAcceleration;

	Image* img;


	Trajectoire(Image*);
	void build_fake_trajectoire();
	std::list<int*> bresenham(int,int,int,int);
	bool verifier_trajectoire();
	void load_data(int,int,int);

	void write(char*);
	void load(std::string);

	int manhattan(int x, int y);
	virtual ~Trajectoire();
};



#endif
