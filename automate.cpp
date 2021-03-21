#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <cstring>

#include "automate.hpp"

//deprécié

Automate::Automate(Image* pt, Trajectoire* tr){
	img=pt;
xCourant=img->departureX;
yCourant=img->departureY;
traj=tr;
}

Automate::~Automate(){
	
}


std::pair<int,int> Automate::uncoup(){
	std::cout<<"on Lance un coup !"<<std::endl;
	std::cout<<" x courant : "<<xCourant<<" y courant : "<<yCourant<<std::endl;
	std::map<std::string,int> map = img->direction(xCourant,yCourant);
	etapeDouble=false;
	
	//std::string directionInterdite = inverse(precedenteDirection);
	for (std::map<std::string,int>::iterator i = map.begin(); i != map.end(); ++i)
		{
			std::cout<<"["<<(*i).first<<";"<<(*i).second<<'],';
			

		}
		std::cout<<std::endl;
	//std::cout<<"Direction Intrdite : "<<directionInterdite<<std::endl;
	//map.erase(map.find(directionInterdite));
	for (std::map<std::string,int>::iterator i = map.begin(); i != map.end(); ++i)
		{
			std::cout<<"["<<(*i).first<<";"<<(*i).second<<"],";
			

		}
		std::cout<<std::endl;
	std::map<std::string,int> potentiel;
	std::string dir;
	int max=0;
	bool chercherParmiReste=false;
	for (std::map<std::string,int>::iterator i = map.begin(); i != map.end(); ++i)
		{
			//std::cout<<"Dir et nbdePixel "<<(*i).first<<" "<<(*i).second<<std::endl;
			if(max<(*i).second){
				dir=(*i).first;
				max=(*i).second;
			}

		}
	std::cout<<"On a trouvé la premiere direction : "<<dir<<std::endl;
	int coupPotentiel = img->potentiel(dir,xCourant,yCourant);
	std::cout<<"Son potentiel : "<<coupPotentiel<<std::endl;
	if(coupPotentiel>15){
		//on peut aller en direction du plsu grand nombre de pixel.
	}else{
		//on regarde dans les directions voisines.
		std::cout<<"On va aller voir allieurs quand même..."<<dir<<std::endl;
		std::pair<std::string,std::string> voisins;
		if(strcmp(dir.c_str(),"n")==0){
			voisins.first=std::string("no");
			voisins.second=std::string("ne");
		}else if(strcmp(dir.c_str(),"no")==0){
			voisins.first=std::string("n");
			voisins.second=std::string("o");
		}else if(strcmp(dir.c_str(),"o")==0){
			voisins.first=std::string("no");
			voisins.second=std::string("so");
		}else if(strcmp(dir.c_str(),"so")==0){
			voisins.first=std::string("o");
			voisins.second=std::string("s");
		}else if(strcmp(dir.c_str(),"s")==0){
			voisins.first=std::string("so");
			voisins.second=std::string("se");
		}else if(strcmp(dir.c_str(),"se")==0){
			voisins.first=std::string("s");
			voisins.second=std::string("e");
		}else if(strcmp(dir.c_str(),"e")==0){
			voisins.first=std::string("se");
			voisins.second=std::string("ne");
		}else if(strcmp(dir.c_str(),"ne")==0){
			voisins.first=std::string("n");
			voisins.second=std::string("e");
		}
		int v1potentiel=img->potentiel(voisins.first,xCourant,yCourant);
		int v2potentiel=img->potentiel(voisins.second,xCourant,yCourant);
		//on test la direction voisines qui compte le plus de pixel d'arrivé

		std::cout<<"Les directions voisines leurs nbPixel et potentiel: "<<voisins.first<<' '<<map[voisins.first]<<' '<<v1potentiel<<' '<<voisins.second<<' '<<map[voisins.second]<<' '<<v2potentiel<<std::endl;
		/*if(strcmp(voisins.first.c_str(),directionInterdite.c_str())==0){
			if(v2potentiel>15){
				dir=voisins.second;
				coupPotentiel=v2potentiel;
			}else{
				chercherParmiReste=true;
			}
		}else if(strcmp(voisins.second.c_str(),directionInterdite.c_str())==0){
			if(v1potentiel>15){
				dir=voisins.first;
				coupPotentiel=v1potentiel;
			}else{
				chercherParmiReste=true;
			}
		}else{*/
			if(map[voisins.first]>map[voisins.second]){
				if(v1potentiel>15){
					dir=voisins.first;
					coupPotentiel=v1potentiel;
				}else if(v2potentiel>15){
					dir=voisins.second;
					coupPotentiel=v2potentiel;
				}else{
					chercherParmiReste=true;
				}
			}else{
				if(v2potentiel>15){
					dir=voisins.second;
					coupPotentiel=v1potentiel;
				}else if(v1potentiel>15){
					dir=voisins.first;
					coupPotentiel=v2potentiel;
				}else{
					chercherParmiReste=true;
				}
			}
		//}
		std::cout<<"donc, on a dir, pot : "<<dir<<' '<<coupPotentiel<<std::endl;
	}
	if (chercherParmiReste)
	{
		std::cout<<"On doit aller chercher parmi le reste des directions"<<std::endl;
		//on n'a pas trouvé qu'aller vers la direction de la zone d'arrivée est viable donc cherche une direction libre
		int size=map.size();
		int max;
		while(size>0){
			 max=-1;
			for (std::map<std::string,int>::iterator i = map.begin(); i != map.end(); ++i)
			{
				std::cout<<(*i).first<<" ";
				if(max<(*i).second){
					dir=(*i).first;
					max=(*i).second;
					
				}

			}std::cout<<std::endl;
			int coupPotentiel = img->potentiel(dir,xCourant,yCourant);
			std::cout<<" nbPixel max, sa direction et son potentiel : "<<max<< " "<<dir<<" "<<coupPotentiel<<std::endl;
			/*if(coupPotentiel>15){
				break;
			}else{*/
				potentiel.insert(std::pair<std::string,int>(dir,coupPotentiel ));
				std::cout<<"on l'a inseré"<<std::endl;
				map.erase(map.find(dir));
				std::cout<<"dir est effacé de la map"<<std::endl;
			//}
			size--;

		}
		max=-1;
		//aucune direction n'a suffisemment de potentiel, on prend celle qui a le plus grand.
		if (size==0)
		{
			std::cout<<"On récupère la direction avec le plus de potentiel."<<std::endl;
			for (std::map<std::string,int>::iterator i = potentiel.begin(); i != potentiel.end(); ++i)
			{
				std::cout<<(*i).first<<' '<<(*i).second;
				if(max<(*i).second){
					std::cout<<" up ";
					dir=(*i).first;
					max=(*i).second;
				}

			}
			std::cout<<std::endl<<"final : "<<dir<<" avec "<<max<<std::endl;
			coupPotentiel=img->potentiel(dir,xCourant,yCourant);
		}
		std::cout<<std::endl<<"final : "<<dir<<" avec "<<max<<std::endl;
	}
	std::cout<<std::endl<<"final : "<<dir<<" avec "<<max<<std::endl;

	//on a notre direction, on va créer l'étape.
	int vitesse;

	if (coupPotentiel>30)
	{
		vitesse = 30;
		if(coupPotentiel>60){
			etapeDouble=true;
		}
	//}else if (coupPotentiel>15){
	//	vitesse = 15;
	}else{
		vitesse=coupPotentiel-1;
	}
	std::cout<<"La vitesse et sa direction : "<<vitesse<<" "<<dir<<std::endl;
	precedenteDirection=dir;
	if(strcmp(dir.c_str(),"n")==0){
			return std::pair<int,int>(0,-vitesse);
		}else if(strcmp(dir.c_str(),"no")==0){
			return std::pair<int,int>(-vitesse/2,-vitesse/2);
		}else if(strcmp(dir.c_str(),"o")==0){
			return std::pair<int,int>(-vitesse,0);
		}else if(strcmp(dir.c_str(),"so")==0){
			return std::pair<int,int>(-vitesse/2,vitesse/2);
		}else if(strcmp(dir.c_str(),"s")==0){
			return std::pair<int,int>(0,vitesse);
		}else if(strcmp(dir.c_str(),"se")==0){
			return std::pair<int,int>(vitesse/2,vitesse/2);
		}else if(strcmp(dir.c_str(),"e")==0){
			return std::pair<int,int>(vitesse,0);
		}else{ //ne
			return std::pair<int,int>(vitesse/2,-vitesse/2);
		}


	
}


std::pair<int,int> Automate::coupFinal(){
	std::cout<<"on lance le dernier coup !"<<std::endl;
	std::cout<<" x courant : "<<xCourant<<" y courant : "<<yCourant<<std::endl;
	std::map<std::string,int> map = img->direction(xCourant,yCourant);
	etapeDouble=false;
	
	std::map<std::string,int> potentiel;
	std::string dir;
	int max=0;
	bool chercherParmiReste=false;
	for (std::map<std::string,int>::iterator i = map.begin(); i != map.end(); ++i)
		{
			//std::cout<<"Dir et nbdePixel "<<(*i).first<<" "<<(*i).second<<std::endl;
			if(max<(*i).second){
				dir=(*i).first;
				max=(*i).second;
			}

		}
	std::cout<<"On a trouvé la premiere direction : "<<dir<<std::endl;
	int coupPotentiel = img->potentiel(dir,xCourant,yCourant);
	std::cout<<"Son potentiel : "<<coupPotentiel<<std::endl;
	
	//on a notre direction, on va créer l'étape.
	int vitesse;

	vitesse=coupPotentiel;

	std::cout<<"La vitesse et sa direction : "<<vitesse<<" "<<dir<<std::endl;
	precedenteDirection=dir;
	if(strcmp(dir.c_str(),"n")==0){
			return std::pair<int,int>(0,-vitesse);
		}else if(strcmp(dir.c_str(),"no")==0){
			return std::pair<int,int>(-vitesse/2,-vitesse/2);
		}else if(strcmp(dir.c_str(),"o")==0){
			return std::pair<int,int>(-vitesse,0);
		}else if(strcmp(dir.c_str(),"so")==0){
			return std::pair<int,int>(-vitesse/2,vitesse/2);
		}else if(strcmp(dir.c_str(),"s")==0){
			return std::pair<int,int>(0,vitesse);
		}else if(strcmp(dir.c_str(),"se")==0){
			return std::pair<int,int>(vitesse/2,vitesse/2);
		}else if(strcmp(dir.c_str(),"e")==0){
			return std::pair<int,int>(vitesse,0);
		}else{ //ne
			return std::pair<int,int>(vitesse/2,-vitesse/2);
		}


	
}

void Automate::machine(){
	//img.direction(img.departureX,img.departureY);
	//img.affichage("image.pgm");
	img->affichageDirection("direction/etape0.pgm", xCourant,yCourant);
	std::pair<int,int> etape = uncoup();
	std::pair<int,int> depart = std::pair<int,int>(xCourant,yCourant);
	std::cout<<"1er coup, x : "<<etape.first<<" y : "<<etape.second<<"\n\n"<<std::endl;
	xCourant+=etape.first;
	yCourant+=etape.second;


	int nbEtapes = 1;

	traj->inserer(std::pair<int,std::pair<int,int>>(nbEtapes++,std::pair<int,int>(etape.first,etape.second)));

	img->affichageDirection("direction/etape1.pgm", xCourant,yCourant);


	while(not(traj->verifierTrajectoireArrivee(depart,etape))){
		etape =uncoup();
		depart = std::pair<int,int>(xCourant,yCourant);
		std::cout<<"1er coup, x : "<<etape.first<<" y : "<<etape.second<<"\n\n"<<std::endl;
		xCourant+=etape.first;
		yCourant+=etape.second;

		traj->inserer(std::pair<int,std::pair<int,int>>(nbEtapes++,std::pair<int,int>(etape.first,etape.second)));




		std::string fichier = "direction/etape";
		fichier+=std::to_string(nbEtapes);
		fichier+=".pgm";
		std::cout<<fichier<<std::endl;
		std::cout<<fichier.c_str()<<std::endl;
		img->affichageDirection(fichier.c_str(), xCourant,yCourant);
		if(etapeDouble){
			traj->inserer(std::pair<int,std::pair<int,int>>(nbEtapes++,std::pair<int,int>(etape.first,etape.second)));
		}


	}
	


}

std::string Automate::inverse(std::string dir){
	if(strcmp(dir.c_str(),"n")==0){
			return std::string("s");
		}else if(strcmp(dir.c_str(),"no")==0){
			return std::string("se");
		}else if(strcmp(dir.c_str(),"o")==0){
			return std::string("e");
		}else if(strcmp(dir.c_str(),"so")==0){
			return std::string("ne");
		}else if(strcmp(dir.c_str(),"s")==0){
			return std::string("n");
		}else if(strcmp(dir.c_str(),"se")==0){
			return std::string("no");
		}else if(strcmp(dir.c_str(),"e")==0){
			return std::string("o");
		}else{ //ne
			return std::string("so");
		}
}