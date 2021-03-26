#include <iostream>
#include <string>
#include <list>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <limits>
#include "image.hpp"
#include "trajectoire.hpp"


#ifndef DIJKSTRA_H
#define DIJKSTRA_H


class Dijkstra{
public:
	int xCourant;
	int yCourant;
	Image* img;
	Trajectoire* traj;
	int NbNodes;
	int maxAcceleration;
	bool noeudDansArrivee;

	int noeudArrivee;

	int** voisinExiste;

	void algoDijkstra ();
	void createGraph();
	void chercherNoeudArrive();
	void transformer();

	Dijkstra(Image*, Trajectoire*);
	virtual ~Dijkstra();

	std::vector<std::vector<std::pair<int, int>>> G;

	std::map<int,std::pair<int,int>> coor;

	std::vector<int> chemin;

	
};

#endif