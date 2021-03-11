#include <iostream>
#include <string>
#include <list>
#include "image.hpp"
#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H


class Trajectoire{
public:
	int size;

	//std::map<int,int*> etapes;
	std::map<int,std::pair<int,int>> etapes;


	int departureX;
	int departureY;
	int maxAcceleration;

	Image* img;


	Trajectoire(Image*);
	void build_fake_trajectoire();
	bool verifier_trajectoire();
	void load_data();

	void write(std::string);
	void load(std::string);

	int manhattan(int x, int y);
	virtual ~Trajectoire();

	std::list<int> traceSegment(int,int,int,int);

};



#endif
