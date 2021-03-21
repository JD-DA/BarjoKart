#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <png.h>
#include <png++/png.hpp>
#include <toml++/toml.h>
#include "image.hpp"
#include "trajectoire.hpp"
#include "automate.hpp"
#include "dijkstra.hpp"


int main(){

	Image img = Image();
	
	
	 //img.load_data("circuits/circuit0.png","circuits/circuit0.toml");
	img.load_data("circuits/fusee.png","circuits/fusee.toml");
	//img.affichage("image.pgm");
	//img.load_data("circuits/trois_quatorze.png","circuits/trois_quatorze.toml");
	Trajectoire tr = Trajectoire(&img);
	Dijkstra dij = Dijkstra(&img,&tr);
	dij.createGraph();
	dij.chercherNoeudArrive();
	dij.algoDijkstra();
	std::cout<<dij.noeudArrivee<<std::endl;

	dij.transformer();
	
	if(not(dij.noeudDansArrivee)){
		std::cout<<"On est presque arrivé ! \n\n";
		Automate aut = Automate(&img,&tr);
		aut.xCourant=dij.coor[dij.noeudArrivee].first;
		aut.yCourant=dij.coor[dij.noeudArrivee].second;
		std::cout<<"avant-dernieres coordonnées : x/y "<<aut.xCourant<<' '<<aut.yCourant<<std::endl;
		std::pair<int,int> laPaire = (aut.coupFinal());
		std::cout<<"Dernire etape : x/y "<<laPaire.first<<' '<<laPaire.second<<" Size : "<<tr.etapes.size()<<std::endl;
		int indice = (tr.etapes.size())+1;
		tr.inserer(std::pair<int,std::pair<int,int>>(indice,laPaire));

	}

	tr.afficher();
	

	tr.writeServeur("fusee3.txt");
	tr.write("test21marsFusee.toml");

	
	//aut.machine();
	//tr.write("testSamediSoir.toml");

	

	//img.direction(img.departureX,img.departureY);
	
	/*img.affichageDirection("direction/etape1.pgm", aut.xCourant,aut.yCourant);
	std::pair<int,int> lapaire = aut.uncoup();
	std::cout<<"1er coup, x : "<<lapaire.first<<" y : "<<lapaire.second<<"\n\n"<<std::endl;
	aut.xCourant+=lapaire.first;
	aut.yCourant+=lapaire.second;
	img.affichageDirection("direction/etape2.pgm", aut.xCourant,aut.yCourant);


	lapaire = aut.uncoup();
	std::cout<<"2eme coup, x : "<<lapaire.first<<" y : "<<lapaire.second<<"\n\n"<<std::endl;
	aut.xCourant+=lapaire.first;
	aut.yCourant+=lapaire.second;
	img.affichageDirection("direction/etape3.pgm", aut.xCourant,aut.yCourant);


	lapaire = aut.uncoup();
	std::cout<<"3eme coup, x : "<<lapaire.first<<" y : "<<lapaire.second<<"\n\n"<<std::endl;
	aut.xCourant+=lapaire.first;
	aut.yCourant+=lapaire.second;
	img.affichageDirection("direction/etape4.pgm", aut.xCourant,aut.yCourant);

	lapaire = aut.uncoup();
	std::cout<<"4eme coup, x : "<<lapaire.first<<" y : "<<lapaire.second<<"\n\n"<<std::endl;
	aut.xCourant+=lapaire.first;
	aut.yCourant+=lapaire.second;
	img.affichageDirection("direction/etape5.pgm", aut.xCourant,aut.yCourant);


	lapaire = aut.uncoup();
	std::cout<<"5eme coup, x : "<<lapaire.first<<" y : "<<lapaire.second<<"\n\n"<<std::endl;
	aut.xCourant+=lapaire.first;
	aut.yCourant+=lapaire.second;
	img.affichageDirection("direction/etape6.pgm", aut.xCourant,aut.yCourant);
*/

	
	//tr.build_fake_trajectoire();
	//tr.traceSegment(10,10,20,25);
	//tr.write("sortie.toml");
	//tr.load("sortie.toml");
	//std::cout<<"Verification de la trajectoire..."<<std::endl;
	//std::cout<<tr.verifier_trajectoire()<<std::endl;
}