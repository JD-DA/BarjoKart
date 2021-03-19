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
	bool verifierTrajectoireArrivee(std::pair<int,int> ,std::pair<int,int> );
	void load_data();
	bool verifierSegment(int,int,int,int);
	int distanceMiniArrivee(int x,int y);

	void write(std::string);
	void writeServeur(std::string);
	void load(std::string);

	int manhattan(int x, int y);
	virtual ~Trajectoire();

	std::list<int> traceSegment(int,int,int,int);

	void inserer(std::pair<int,std::pair<int,int>>);

	

};



#endif
