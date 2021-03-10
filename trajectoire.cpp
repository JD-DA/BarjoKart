#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fstream>

#include <toml++/toml.h>

#include "trajectoire.hpp"

Trajectoire::Trajectoire(Image* pt):etapes(){
	img=pt;
	departureX=100;
	departureY=100;
}

Trajectoire::~Trajectoire(){
	
}

void Trajectoire::load_data(int x,int y,int max){
	departureX = x;
	departureY = y;
	maxAcceleration = max;
	int tab[] = {x,y};
	etapes.insert({0,tab});
	size=1;
}

void Trajectoire::build_fake_trajectoire(){
	int xp = departureX;
	int yp = departureY;
	for (int i = 1; i < 11; ++i)
	{
		int tab[] ={xp +=10,yp +=10};
		etapes.insert({i,tab});
		size++;
		std::cout<<etapes[i][0]<<' '<<etapes[i][1]<<std::endl;
	}
}

std::list<int*> Trajectoire::bresenham(int a, int b, int c, int d){
}

bool Trajectoire::verifier_trajectoire(){
	for (int i = 1; i < size; ++i)
	{
		std::list<int*> traj = bresenham(etapes[i-1][0],etapes[i-1][1],etapes[i][0],etapes[i][1]);
		/*std::list<int*>::iterator it = traj.begin();
		int x_1 = (*it)[0];
		int y_1 = (*it)[1];
		it++;
		int x_2 = (*it)[0];
		int y_2 = (*it)[1];
		for (it; it != traj.end(); ++it){
		    if(not(img->verifierPixel((*it)[0],(*it)[1]))){
				return false;
			}
			std::cout<<(*it)[0]<<' '<<(*it)[1]<<std::endl;
		}*/

		//verification des pixels
		for (std::list<int*>::iterator it = traj.begin(); it != traj.end(); ++it){
		    if(not(img->verifierPixel((*it)[0],(*it)[1]))){
				return false;
			}
			std::cout<<(*it)[0]<<' '<<(*it)[1]<<std::endl;
		}

		//verification acceleration

		if(manhattan(etapes[i][0]-etapes[i-1][0],etapes[i][1]-etapes[i-1][1])> maxAcceleration){
			return false;
		}	
	}
	if(img->verifierArrivee(etapes[size-1][0],etapes[size-1][1])){
		return true;
	}
	return false;
}

int Trajectoire::manhattan(int x, int y){
	return abs(x) + abs(y);
}

void Trajectoire::load(std::string ftoml){
	toml::table tbl;
	try{
		tbl = toml::parse_file(ftoml);
		//std::cout<<tbl<<std::endl;
		std::cout<<"Lecture du .toml..."<<std::endl;
		int size = tbl["size"].value<int>().value();
		for (int i = 0; i < size; ++i)
		{
			int tab[]={tbl["etapes"][i][0].value<int>().value(),tbl["etapes"][i][1].value<int>().value()};
			etapes.insert({i,tab});
		}
		

	}catch(const toml::parse_error& err){
		std::cerr<<"La lecture du toml a échouée :\n"<<err<<"\n";
	}
}







