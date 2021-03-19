#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fstream>

#include <toml++/toml.h>

#include "trajectoire.hpp"

Trajectoire::Trajectoire(Image* pt){
	img=pt;
load_data();
}

Trajectoire::~Trajectoire(){
	
}

void Trajectoire::load_data(){
	departureX = img->departureX;
	departureY = img->departureY;
	maxAcceleration = img->maxAcceleration;
	std::cout<<"Chargement des données de la trajectoire..."<<std::endl;
	std::cout<<" x "<<departureX<<" y "<<departureY<<" maxAcceleration "<<maxAcceleration<<std::endl;
	//etapes.insert(std::pair<int,std::pair<int,int>>(0,std::pair<int,int>(0,0)));
	size=0;

	for (int i=0; i < size; ++i)
			{
				std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			std::cout << "]"<< std::endl;

	/*std::cout<<"Tab : "<<tab<<std::endl;
	std::cout<<"Etapes : "<<etapes<<std::endl;*/
}

void Trajectoire::build_fake_trajectoire(){
	
	for (int i = 1; i < 10; ++i)
	{
		etapes.insert(std::pair<int,std::pair<int,int>>(i,std::pair<int,int>(departureX+0*i,departureY-10*i)));
		size++;
		//std::cout<<etapes[i].first<<' '<<etapes[i].second<<std::endl;
	}

	for (int i=0; i < size; ++i)
			{
				std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			std::cout << "]"<< std::endl;
}


bool Trajectoire::verifier_trajectoire(){
	std::cout<<"On lance la vérification..."<<std::endl;
	for (int i=0; i < size; ++i)
			{
				std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			std::cout << "]"<< std::endl;
	for (int i = 1; i < size; ++i)
	{
		//std::cout<<"Pour "<<i<<" : ";

		if(manhattan(etapes[i].first-etapes[i-1].first,etapes[i].second-etapes[i-1].second)> maxAcceleration){
			std::cout<<"On a fait un tonneau..."<<std::endl;
			return false;
		}

		int xSuivant = departureX+etapes[i].first;
		int ySuivant= departureY+etapes[i].second;
		std::list<int> traj = traceSegment(departureX,departureY,xSuivant,ySuivant);
		departureX=xSuivant;
		departureY=ySuivant;
		//std::cout<<"On a calculé la droite entre "<<etapes[i-1].first<<" "<<etapes[i-1].second<<" et "<<etapes[i].first<<" "<<etapes[i].second<<std::endl;
		//verification des pixels
		for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
			int x=(*it);
			it++;
			int y=(*it);
		    if(not(img->verifierPixel(x,y))){
		    	std::cout<<"On est rentré dans un mur..."<<std::endl;
				return false;
			}
			//std::cout<<(*it)[0]<<' '<<(*it)[1]<<std::endl;
		}
		std::cout<<"  On a verifié les pixels "<<std::endl;
		//verification acceleration

		
		std::cout<<"On a passé manhattan "<<std::endl;	
	}
	if(img->verifierArrivee(departureX,departureY)){
		std::cout<<"On est arrivé !"<<std::endl;
		return true;
	}
	std::cout<<"On est arrivé au milieu de nul part..."<<std::endl;
	return false;
}

int Trajectoire::manhattan(int x, int y){
	return abs(x) + abs(y);
}

bool Trajectoire::verifierSegment(int xd ,int yd ,int xa ,int ya){
    if(not(img->verifierPixel(xa,ya))){
        return false;
    }
    std::list<int> traj = traceSegment(xd,yd,xa,ya);
    
    for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
        int x=(*it);
        it++;
        int y=(*it);
        if(not(img->verifierPixel(x,y))){
            return false;
        }
    }
    return true;
}

void Trajectoire::load(std::string ftoml){
	toml::table tbl;
	try{
		tbl = toml::parse_file(ftoml);
		
		std::cout<<"Lecture du .toml..."<<std::endl;
		size = tbl["size"].value<int>().value();
		for (int i = 1; i < size; ++i)
		{
			int x = tbl["trajectoire"][i][0].value<int>().value();
			int y = tbl["trajectoire"][i][1].value<int>().value();
			
  			auto ret = this->etapes.insert(std::pair<int,std::pair<int,int>>(i,std::pair<int,int>(x,y)));

			if (ret.second==false) {
			    std::cout << "element "<<i<<" already existed";
			    std::cout << " with a value of " << ret.first->second.first <<" "<<ret.first->second.first<< '\n';
			  }

			
		}
	}catch(const toml::parse_error& err){
		std::cerr<<"La lecture du toml a échouée :\n"<<err<<"\n";
	}
}

std::list<int> Trajectoire::traceSegment(int x1, int y1, int x2, int y2){
    int dx, dy;
    dx = x2 - x1; //distance entre point x1 et x2
    std::list<int> liste;

    if(dx != 0)
    {
        if(dx > 0 )
        {
            dy = y2 - y1; //distance entre point y1 et y2
            if(dy != 0)
            {
                if(dy > 0)
                {
                    if(dx >= dy)
                    {
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        
                        while(true)
                        {
                        	/*int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            x1 = x1 + 1;
                            if(x1 == x2)
                            {
                                break;
                            }
                            e -= dy;
                            if(e < 0)
                            {
                                ++y1; // déplacement diagonal
                                e += dx;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        dy = e * 2 ;
                        dx = dx * 2 ;
                        while(true)
                        {  
                        	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            ++y1;
                            if(y1 == y2)
                            {
                                break;
                            }
                            e -= dx;
                            if(e < 0)
                            {
                                ++x1;  // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }
                else
                {
                    if( dx >= -dy)
                    {
                        int e = dx;
                        dx = e * 2 ;
                        dy = dy * 2 ;
                        while(true)
                        {  
                        	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            ++x1;
                            if(x1 == x2)
                            {
                                break;
                            }
                            e = e + dy;

                            if(e < 0)
                            {
                                --y1;  // déplacement diagonal
                                e = e + dx ;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        // e est négatif
                        dy = e * 2 ;
                        dx = dx * 2 ;  
                        while(true)
                        {  
                            // déplacements verticaux
                            /*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                            std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            --y1;
                            if(y1 == y2)
                            {
                                break;
                            }
                            e += dx;
                            if(e > 0)
                            {
                                ++x1;  // déplacement diagonal
                                e += dy ;
                            }
                        }
                    }
                }
            }
            else
            {  
                do
                {
                	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                	std::cout<<x1<<' '<<y1<<std::endl;*/
                	liste.push_back(x1);
                			liste.push_back(y1);
                    //tracePixel(x1, y1);
                    ++x1;
                }
                while(x1 != x2);
            }
        }
        else // dx < 0
        { 
            dy = y2 - y1;
            if(dy != 0)
            {
                if(dy > 0)
                {
                    // vecteur oblique dans le 2d quadran
                    if(-dx >= dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
                        int e = dx; // e est négatif
                        dx = e * 2;
                        dy = dy * 2;
                        
                        while(true)
                        { 
                            // déplacements horizontaux
                            /*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                            std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            --x1;
                            if(x1 == x2)
                            {
                                break;
                            }
                            e += dy;
                            if(e >= 0)
                            {
                                ++y1;  // déplacement diagonal
                                e += dx;
                            }
                        }
                    }
                    else
                    {
                        // vecteur oblique proche de la verticale, dans le 3e octant
                        int e = dy;
                        dy = e * 2 ;
                        dx = dx * 2 ;
                        // e est positif
                        while(true)
                        {  // déplacements verticaux
                        	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            ++y1;
                            if(y1 == y2)
                            {
                                break;
                            }
                            e += dx;
                            if(e <= 0)
                            {
                                --x1;  // déplacement diagonal
                                e += dy ;
                            }
                        }
                    }
                }
                else
                {  // dy < 0 (et dx < 0)
                    // vecteur oblique dans le 3e cadran
                    if(dx <= dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est négatif
                        while(true)
                        {  // déplacements horizontaux
                        	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            --x1;
                            if(x1 == x2)
                            {
                                break;
                            }
                            e -= dy;
                            if(e >= 0)
                            {
                                --y1;  // déplacement diagonal
                                e += dx ;
                            }
                        }
                    }
                    else
                    {  // vecteur oblique proche de la verticale, dans le 6e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        // e est négatif
                        while(true)
                        { // déplacements verticaux
                        	/*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                        	std::cout<<x1<<' '<<y1<<std::endl;;*/
                			liste.push_back(x1);
                			liste.push_back(y1);
                            //tracePixel(x1, y1);
                            --y1;
                            if(y1==y2)
                            {
                                break;
                            }
                            e -= dx;
                            if(e >= 0)
                            {
                                --x1;  // déplacement diagonal
                                e += dy ;
                            }
                        }
                    }
                }
            }
            else
            {  // dy = 0 (et dx < 0)
                // vecteur horizontal vers la gauche
                do
                {
                	//int tab[] = {x1,y1};
                	liste.push_back(x1);
                			liste.push_back(y1);
                    //tracePixel(x1, y1);
                    --x1;
                }
                while(x1 != x2);
            }
        }
    }
    else // dx = 0
    {
        dy = y2 - y1;
        //cout<<"dx=0 dy="<<dy<<endl;
        if(dy != 0)
        {
            if(dy > 0)
            {
                // vecteur vertical croissant
                do
                {
                    /*int a,b;
                	a=x1;
                	b=x2;
                    int tab[] = {x1,y1};
                    std::cout<<x1<<' '<<y1<<std::endl;;*/
                	liste.push_back(x1);
                			liste.push_back(y1);
                    //tracePixel(x1, y1);
                    ++y1;
                }
                while(y1 != y2);
            }
            else
            { // dy < 0 (et dx = 0)
                //cout<<"haut"<<endl;
                // vecteur vertical décroissant
                do
                {
                	/*int a,b;
                	a=x1;
                	b=x2
                    int tab[] = {x1,y1};
                    std::cout<<x1<<' '<<y1<<std::endl;;*/
                	liste.push_back(x1);
                			liste.push_back(y1);
                    //tracePixel(x1, y1);
                    --y1;
                }
                while(y1 != y2);
            }
        }
    }


		/*for (std::list<int>::iterator it = liste.begin(); it != liste.end(); ++it){
		    int a = (*it);
		    it++;
		    int b = (*it);
			std::cout<<a<<' '<<b<<std::endl;
		}*/
    // le pixel final (x2, y2) n’est pas tracé.
	return liste;
}

void Trajectoire::write(std::string nfichier){

    std::ofstream f_sortie(nfichier, std::ios::out | std::ios::binary);
    FILE* fichier = NULL;
 
    fichier = fopen(nfichier.c_str(), "w");
 
    if (fichier != NULL)
    {
        
        fprintf(fichier, "size = %d\n", size);
        fprintf(fichier, "trajectoire = {");

            for (int i=0; i < size; ++i)
            {
                fprintf(fichier, "%d=[%d,%d],", i,etapes[i].first,etapes[i].second);
            }
            fprintf(fichier, "}\n");
        fclose(fichier);
    }}


void Trajectoire::writeServeur(std::string nom_fichier_sortie){

    std::ofstream f_sortie(nom_fichier_sortie, std::ios::out | std::ios::binary);
    
    for (int i=0; i < size; ++i)
    {
        f_sortie.write((char *) &(etapes[i].first), sizeof(int));
        f_sortie.write((char *) &(etapes[i].second), sizeof(int));
    }
    f_sortie.close();
        //std::ofstream maTrajectoire(fichier);


	/*if(maTrajectoire)  //On teste si tout est OK
		{std::cout<<"Ecriture du toml..."<<std::endl;
			maTrajectoire << "size = " << size << std::endl;
			maTrajectoire << "trajectoire = {" ;
			for (int i=0; i < size; ++i)
			{
				maTrajectoire <<i<< "=[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			maTrajectoire << "}"<< std::endl;
			
		}
	else {
		std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;
	}*/
    //maTrajectoire.close();
}


/*void Trajectoire::dijkstra(){

}*/


bool Trajectoire::verifierTrajectoireArrivee(std::pair<int,int> depart,std::pair<int,int> etape){
        int xa=depart.first+etape.first;
        int ya=depart.second+etape.second;

        if(img->verifierArrivee(xa,ya)){
            std::cout<<"On est arrivé directement sur l'arrivée !"<<std::endl;
            return true;
        }
        std::list<int> traj = traceSegment(depart.first,depart.second,xa,ya);
        //std::cout<<"On a calculé la droite entre "<<etapes[i-1].first<<" "<<etapes[i-1].second<<" et "<<etapes[i].first<<" "<<etapes[i].second<<std::endl;
        //verification des pixels
        
        
        for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
            int x=(*it);
            it++;
            int y=(*it);
            if(img->verifierArrivee(x,y)){
                std::cout<<"On est passé par l'arrivé !"<<std::endl;
                return true;
            }
            //std::cout<<(*it)[0]<<' '<<(*it)[1]<<std::endl;
        }
        return false;

}

void Trajectoire::inserer(std::pair<int,std::pair<int,int>> paire){
    //std::cout<<"Inserer reçoit : "<<paire.first<<" -> "<<paire.second.first<<" "<<paire.second.second<<" size : "<<size<<std::endl;
    etapes.insert(paire);
    ++size;
    /*for (int i=0; i < size; ++i)
            {
                std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
            }
            std::cout << "]"<< std::endl;*/
}



int Trajectoire::distanceMiniArrivee(int x,int y){
    int dMin=img->height*2;

    for (std::list<std::pair<int,int>>::iterator i = img->zoneArrive.begin(); i != img->zoneArrive.end(); ++i)
    {
        if(verifierSegment(x,y,(*i).first,(*i).second)){
            dMin = std::min(dMin,manhattan(x-(*i).first,y-(*i).second));
        }
    }
    return dMin;
} 







