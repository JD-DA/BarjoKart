#include <iostream>
#include <string>
#ifndef IMAGE_H
#define IMAGE_H


class Image{
public:
	int width;
	int height;
	
	//un tableau de lignes de caracteres representant les différenttes lignes de pixel
	// pixel blanc=b pixel noir=n pixel d'une couleur bizarre=g et pixel de la couleur d'arrivée comme définie dans le toml=a
	char** matrice; 

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
	bool verifierPixel(int,int);
	bool verifierArrivee(int,int);
	virtual ~Image();
};



#endif
