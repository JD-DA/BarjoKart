#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "image.hpp"


#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H


class Trajectoire{
public:
	int size;

	//std::map<int,int*> etapes;
	std::map<int,std::pair<int,int>> etapes;
	std::list<std::pair<int,int>> etapesGenerales;


	int departureX;
	int departureY;
	int finalX;
	int finalY;
	int maxAcceleration;

	Image* img;
	


	Trajectoire(Image*);

	void build_fake_trajectoire();
	bool verifier_trajectoire();
	bool verifierTrajectoireArrivee(std::pair<int,int> ,std::pair<int,int> );
	void load_data();
	bool verifierSegment(int,int,int,int);
	int distanceMiniArrivee(int x,int y);
	void afficher();
	void lisser();

	void write(std::string);
	void writeServeur(std::string);
	void load(std::string);

	int manhattan(int x, int y);
	virtual ~Trajectoire();

	std::list<int> traceSegment(int,int,int,int);

	void inserer(std::pair<int,std::pair<int,int>>);
	void detaillerTrajectoire();
	void remplacerTrajectoire(std::vector<std::pair<int,int>> );
	void reset();

	

};



#endif
