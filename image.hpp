#include <iostream>
#include <string>
#include <list>
#include <map>

#ifndef IMAGE_H
#define IMAGE_H


class Image{
public:
	int width;
	int height;
	
	//un tableau de lignes de caracteres representant les différenttes lignes de pixel
	// pixel blanc=b pixel noir=n pixel d'une couleur bizarre=g et pixel de la couleur d'arrivée comme définie dans le toml=a
	char** matrice; 
	int** distanceObstacle;
	std::list<std::pair<int,int>> zoneArrive;

	int departureX;
	int departureY;
	int maxAcceleration;

	int arrivalColorR;
	int arrivalColorG;
	int arrivalColorB;

	Image();
	void build_image(std::string);
	void load_data(std::string,std::string);
	void affichage(char*);
	void affichageDirection(const char*,int,int);
	bool verifierPixel(int,int);
	bool verifierPixel2(int,int);
	bool verifierArrivee(int,int);
	bool verifierArrivee2(int,int);
	std::pair<int,int> centreZoneArrivee();

	int distanceMiniArrivee(int,int);
	virtual ~Image();

	std::map<std::string,int> direction(int,int);
	int potentiel(std::string,int,int);
	int indiceDirection(int,int);
};



#endif
