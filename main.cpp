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
	//img.load_data("circuits/fusee.png","circuits/fusee.toml");
	//img.affichage("image.pgm");
	img.load_data("circuits/trois_quatorze.png","circuits/trois_quatorze.toml");
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

	tr.lisser();
	tr.detaillerTrajectoire();
	std::cout<<tr.verifier_trajectoire()<<std::endl;

	/*if(tr.verifier_trajectoire()==0){
		tr.reset();
		dij.reset();
		dij.algoDijkstra();
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
	}*/
	
	tr.writeServeur("pi21mars.txt");
	tr.write("pi21mars.toml");

	
	//aut.machine();

}