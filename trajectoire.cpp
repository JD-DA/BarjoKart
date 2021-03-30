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
/**
* Permet d'initialiser la classe à partir de l'image. Dans des versions précédentes cette fonction effectuait d'autre tâches d'où la séparation avec le constructeur
*
**/
void Trajectoire::load_data(){
	departureX = img->departureX;
	departureY = img->departureY;
	maxAcceleration = img->maxAcceleration;
	std::cout<<"Chargement des données de la trajectoire..."<<std::endl;
	std::cout<<" x "<<departureX<<" y "<<departureY<<" maxAcceleration "<<maxAcceleration<<std::endl;
	size=0;

	for (int i=0; i < size; ++i)
			{
				std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			std::cout << "]"<< std::endl;
}

/**
* Permet de créer une fausse trajectoire totalement invalide mais qui a permis de tester les fonctions d'export
*
**/
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

/**
* Verifier la trajectoire qui est stockée au moment même dans la classe
* @return True si la trajectoire est valide
**/
bool Trajectoire::verifier_trajectoire(){
	std::cout<<"On lance la vérification..."<<std::endl;
	for (int i=0; i < size; ++i)
			{
				std::cout << "[" << etapes[i].first << "," << etapes[i].second << "],";
			}
			std::cout << "]"<< std::endl;
	for (int i = 1; i <= size; ++i)
	{

		if(manhattan(etapes[i].first-etapes[i-1].first,etapes[i].second-etapes[i-1].second)> maxAcceleration){
			std::cout<<"On a fait un tonneau..."<<std::endl;
			return false;
		}

		int xSuivant = departureX+etapes[i].first;
		int ySuivant= departureY+etapes[i].second;
		std::list<int> traj = traceSegment(departureX,departureY,xSuivant,ySuivant);
		departureX=xSuivant;
		departureY=ySuivant;
		//verification des pixels
		for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
			int x=(*it);
			it++;
			int y=(*it);
		    if(not(img->verifierPixel(x,y))){
		    	std::cout<<"On est rentré dans un mur..."<<std::endl;
				return false;
			}
		}	
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

/**
* Permet notamment de vérifier une trajectoire entre deux points.  
* @return True si le segment ne coupe aucun obstacle, False sinon
**/
bool Trajectoire::verifierSegment(int xd ,int yd ,int xa ,int ya){
    if(not(img->verifierPixel2(xa,ya))){
        return false;
    }
    std::list<int> traj = traceSegment(xd,yd,xa,ya);
    
    for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
        int x=(*it);
        it++;
        int y=(*it);
        bool test = (img->verifierPixel3(x,y));
        if(not(test)){
            std::cout<<std::endl;
            return false;
        }
    }
    return true;
}

/**
* Permet de lire et recuperer une trajectoire au format Toml, nous a permis de modifier facilement a la main une trajcetoire ou d'en crée une
*
**/
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

/**
* Algo de Bresenham
*
**/
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
                	liste.push_back(x1);
                			liste.push_back(y1);
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
                        {  
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
                	liste.push_back(x1);
                			liste.push_back(y1);
                    //tracePixel(x1, y1);
                    --y1;
                }
                while(y1 != y2);
            }
        }
    }
    // le pixel final (x2, y2) n’est pas tracé.
	return liste;
}

/**
* Ecrit la trajectoire au format Toml
*
**/
void Trajectoire::write(std::string nfichier){

    std::ofstream f_sortie(nfichier, std::ios::out | std::ios::binary);
    FILE* fichier = NULL;
 
    fichier = fopen(nfichier.c_str(), "w");
 
    if (fichier != NULL)
    {
        
        fprintf(fichier, "size = %d\n", size);
        fprintf(fichier, "trajectoire = {");


            for (std::map<int,std::pair<int,int>>::iterator i = etapes.begin(); i != etapes.end(); ++i)
            {
                fprintf(fichier, "%d=[%d,%d],", (*i).first,(*i).second.first,(*i).second.second);
            }
            
            fprintf(fichier, "}\n");
        fclose(fichier);
    }}

/**
* Ecrit la trajectoire au format demandé par le serveur
*
**/
void Trajectoire::writeServeur(std::string nom_fichier_sortie){

    std::ofstream f_sortie(nom_fichier_sortie, std::ios::out | std::ios::binary);
    
    for (std::map<int,std::pair<int,int>>::iterator i = etapes.begin(); i != etapes.end(); ++i)
    {
        f_sortie.write((char *) &((*i).second.first), sizeof(int));
        f_sortie.write((char *) &((*i).second.second), sizeof(int));
    }
    
    f_sortie.close();
        
}

/**
* Verifie q'un couple d'étapes nous permet bien de passer par la zone d'arrivée
*
**/
bool Trajectoire::verifierTrajectoireArrivee(std::pair<int,int> depart,std::pair<int,int> etape){
        int xa=depart.first+etape.first;
        int ya=depart.second+etape.second;

        if(img->verifierArrivee(xa,ya)){
            std::cout<<"On est arrivé directement sur l'arrivée !"<<std::endl;
            return true;
        }
        std::list<int> traj = traceSegment(depart.first,depart.second,xa,ya);
        
        
        for (std::list<int>::iterator it = traj.begin(); it != traj.end(); ++it){
            int x=(*it);
            it++;
            int y=(*it);
            if(img->verifierArrivee(x,y)){
                std::cout<<"On est passé par l'arrivé !"<<std::endl;
                return true;
            }
        }
        return false;

}

/**
* Permet d'inserer une etape dans la trajectoire
*
**/
void Trajectoire::inserer(std::pair<int,std::pair<int,int>> paire){
    std::pair<std::map<int,std::pair<int,int>>::iterator,bool> res = etapes.insert(paire);

    if(not(res.second)){
        std::cout<<"Erreur lors de l'insertion"<<std::endl;
    }else{
        ++size;
    }


}


/**
* Permet d'avoir la distance qui separe le pixel du centre de la zone d'arrivée et non la distance la plus courte du a une recherche lourde qui était trop souvent appelée
*
**/
int Trajectoire::distanceMiniArrivee(int x,int y){
    int dMin=img->height*2;
    std::pair<int,int> arrive = img->centreZoneArrivee();
    if(verifierSegment(x,y,arrive.second,arrive.first)){
            return manhattan(x-(arrive.second),y-(arrive.first));
        }
    return dMin;


} 

//Affiche la trajectoire
void Trajectoire::afficher(){
    for (std::map<int,std::pair<int,int>>::iterator i = etapes.begin(); i != etapes.end(); ++i)
    {
        std::cout <<(*i).first<< " : [" << (*i).second.first << "," << (*i).second.second << "],";
    }
    std::cout<<std::endl;
}

/**
* Va lire les etapes et va créer une liste d'etapes dont on peut aller en ligne droite entre elles.
* Autrement dit on va lire les etapes et supprimer celles qui sont inutile, donc celle entre deux 
* etapes qui peuvent être reliées entre elles sans obstacles qui les sépare.
**/
void Trajectoire::lisser(){
    
    etapesGenerales.push_back(std::pair<int,int>(departureX,departureY));
    int xCourant=departureX;
    int yCourant=departureY;

    int resX=xCourant;
    int resY=yCourant;

    std::cout<<"On lisse la trajectoire..."<<std::endl;
    //on part de l'origine et on va iterer chaque etape.
    

    for ( std::map<int,std::pair<int,int>>::iterator i = etapes.begin();i != etapes.end();i++)
    {
        xCourant += (*i).second.first;
        yCourant += (*i).second.second;
        std::cout<<(*i).first<<std::endl;

        if(not(verifierSegment(xCourant, yCourant, resX, resY))){
            etapesGenerales.push_back(std::pair<int,int>(xCourant-(*i).second.first,yCourant-(*i).second.second));
            for (std::list<std::pair<int,int>>::iterator i = etapesGenerales.begin(); i != etapesGenerales.end(); i++)
            {
                std::cout<<'['<<(*i).first<<' '<<(*i).second<<"],";
            }
            std::cout<<std::endl;
            resX=xCourant-(*i).second.first;
            resY=yCourant-(*i).second.second;
        }

    }
    int finX=departureX;
    int finY=departureY;
    for (std::map<int,std::pair<int,int>>::iterator i = etapes.begin(); i != etapes.end(); i++)
    {
        finX+=(*i).second.second;
        finY+=(*i).second.first;
    }
    etapesGenerales.push_back(std::pair<int,int>(finalX,finalY));

    std::cout<<"Trajectoire simplifiée, en coordonnées directes :"<<std::endl;
    for (std::list<std::pair<int,int>>::iterator i = etapesGenerales.begin(); i != etapesGenerales.end(); i++)
    {
        std::cout<<(*i).first<<' '<<(*i).second<<std::endl;
    }
}

/**
* Va lire les etapes generales et les remplir avec des etapes intermedieres. 
* De plus va créer pour chaque segemtn une phase d'acceleration puis une phase 
* de deceleration pour reduire le nombre d'étapes
*
**/
void Trajectoire::detaillerTrajectoire(){
    std::list<std::pair<int,int>>::iterator i = etapesGenerales.begin();
    int xd,yd;
    int xf,yf;
    int xmilieu,ymilieu;

    std::vector<std::pair<int,int>> etapesFines;

    float m;
    xd=(*i).first;
    yd=(*i).second;
    i++;
    while ( i != etapesGenerales.end())
    {
        etapesFines.push_back(std::pair<int,int>(0,0));
        xf=(*i).first;
        yf=(*i).second;
        xmilieu=(xd+xf)/2.0;
        ymilieu=(yd+yf)/2.0;

        m=(float)(yd-yf)/(float)(xd-xf);
        std::cout<<"\n\n\n\n\nxd/yd : "<<xd<<'/'<<yd<<"\txf/yf : "<<xf<<'/'<<yf<<std::endl;
        std::cout<<"Max acceleration : "<<maxAcceleration<<std::endl;
        // y=m(x-a)-b
        // y=m(x-xd)-yd
        //on a notre equation de droite

        
        int x;
        //recherche des pas x et y
        //on va incremeneter x jusqu'a ce que la mesure manhattan ne soit plus que juste inferieur a l'acceleration maxi
        if(xd>xf){
            x=xd-1;
            while(manhattan(x-xd,(m*(x-xd)+yd)-yd)<=maxAcceleration){
                --x;

            }
            ++x;
        }else if (xd<xf){
            x=xd+1;
            while(manhattan(x-xd,(m*(x-xd)+yd)-yd)<=maxAcceleration){
                ++x;

            }
            --x;
        }else{
            x=xd;
        }
        int pasX=x-xd;
        int pasY;
        if(xd==xf and yd<yf){ // cas d'une trajectoire verticale
            pasY=maxAcceleration;
        }else if(xd==xf and yd>yf){
            pasY=-maxAcceleration;
        }else{
            pasY=(m*(x-xd)+yd)-yd;
        }
        std::cout<<"\n\nPas x : "<<pasX<<"  pas y : "<<pasY<<std::endl;
        std::cout<<"Milieu x : "<<xmilieu<<"  Milieu y : "<<ymilieu<<" milieu par equation :"<<(m*(xmilieu-xd)+yd)<<std::endl;

        int xCourant=xd;
        int yCourant=yd;
        int pasXcourant=0;
        int pasYcourant=0;
        int pasXPrecedant=0;
        int pasYPrecedant=0;
        std::cout<<"Pos X:Y courant : "<<xCourant<<' '<<yCourant<<std::endl;
        
        std::vector<std::pair<int,int>> etapesAcceleration;
        std::vector<std::pair<int,int>> etapesAccelerationDeceleration;


        while(true){

            //trouver y
            int yCalc=(m*(xCourant+pasXcourant+pasX-xd)+yd);

            //verifier manhattan
            int pasXcourantInter=pasXcourant+pasX;


            //on peut avoir pas egal a max acceleration !

            //si le pas x nous fait aller trop vite on le réduit.
            while(manhattan(pasXcourantInter-pasXcourant,yCalc-yCourant- pasYcourant)>maxAcceleration and manhattan(pasXcourantInter-pasXcourant,yCalc-yCourant- pasYcourant)<maxAcceleration*2 and xd!=x){
                if(pasXcourantInter<0){
                    pasXcourantInter++;
                }else{
                    pasXcourantInter--;
                }
                yCalc=(m*(xCourant+pasXcourantInter-xd)+yd);
            }

            //inserer etape
            //lister etapes pour faire decelerer
            pasXcourant=pasXcourantInter;
            if(xd!=x){
                pasYcourant=yCalc-yCourant;
            }else{
                pasYcourant=pasY;
            }
            //si on continue d'accelerer alors que l'on à dépasser la moitié de l'étapes on n'aura pas assez de longueur pour freiner...
            // on a enlevé les = pour y (dans le cas où traj verticale)
            if(not(xCourant+pasXcourant>=std::min(xmilieu,xd) and pasXcourant+xCourant<=std::max(xmilieu,xd) and pasYcourant+yCourant>=std::min(ymilieu,yd) and pasYcourant+yCourant<=std::max(ymilieu,yd) )){
                
                break;
            }
            xCourant+=pasXcourant;
            yCourant+=pasYcourant;
            std::pair<int,int> laPaire = std::pair<int,int>(pasXcourant,pasYcourant);
            etapesFines.push_back(laPaire);
            etapesAcceleration.push_back(laPaire);
            etapesAccelerationDeceleration.push_back(laPaire);
            pasXPrecedant=pasXcourant;
            pasYPrecedant=pasYcourant;
            std::cout<<"\t\t\t\t\t\tPas X:Y courant : "<<pasXcourant<<' '<<pasYcourant<<std::endl;
            std::cout<<"\t\t\t\t\t\tX:Y courant : "<<xCourant<<' '<<yCourant<<std::endl;
            std::cout<<"\t\t\t\t\t\tX:Y laPaire : "<<laPaire.first<<' '<<laPaire.second<<std::endl;

        }
        //On regarde si on peut s'approcher du milieu en doublant la derniere etape d'acceleration
        
        if((xCourant+pasXPrecedant>=std::min(xmilieu,xd) and pasXPrecedant+xCourant<std::max(xmilieu,xd) and pasYPrecedant+yCourant>=std::min(ymilieu,yd) and pasYPrecedant+yCourant<=std::max(ymilieu,yd) )){
                std::cout<<"\t\t\t\t\t\tOn rajoute une etape d'acceleration"<<std::endl;
                std::cout<<"\t\t\t\t\t\tPas X:Y courant : "<<pasXPrecedant<<' '<<pasYPrecedant<<std::endl;
                std::cout<<"\t\t\t\t\t\tX:Y courant : "<<xCourant+pasXPrecedant<<' '<<yCourant+pasYPrecedant<<std::endl;
                std::pair<int,int> laPaire = std::pair<int,int>(pasXPrecedant,pasYPrecedant);
            etapesFines.push_back(laPaire);
            etapesAcceleration.push_back(laPaire);
            etapesAccelerationDeceleration.push_back(laPaire);
            xCourant+=pasXPrecedant;
            yCourant+=pasYPrecedant;
            }
        //on regarde maintenant si l'espace restant jusqu'au milieu du segmant est >= a la moitiée de l'étape precedente permettant alors de la rajouter.
        if((xCourant+1+pasXPrecedant/2>=std::min(xmilieu,xd) and pasXPrecedant/2+1+xCourant<std::max(xmilieu,xd) and pasYPrecedant/2+1+yCourant>=std::min(ymilieu,yd) and pasYPrecedant/2+1+yCourant<std::max(ymilieu,yd) )){
                std::cout<<"\t\t\t\t\t\tOn rajoute une demi etape d'acceleration"<<std::endl;
                std::cout<<"\t\t\t\t\t\tPas X:Y courant : "<<pasXPrecedant<<' '<<pasYPrecedant<<std::endl;
                std::cout<<"\t\t\t\t\t\tX:Y courant : "<<xCourant+pasXPrecedant<<' '<<yCourant+pasYPrecedant<<std::endl;
                std::pair<int,int> laPaire = std::pair<int,int>(pasXPrecedant,pasYPrecedant);
            etapesFines.push_back(laPaire);
            //etapesAcceleration.push_back(laPaire);
            etapesAccelerationDeceleration.push_back(laPaire);
            xCourant+=pasXPrecedant;
            yCourant+=pasYPrecedant;
            }




        
        //on vient de faire la periode d'acceleration, maintenant on decelere.
        //la derniere etapes d'acceleration doit peut-etre etre supprimée. A vérifier...
        //std::cout<<'[';
        for (std::vector<std::pair<int,int>>::reverse_iterator i = etapesAcceleration.rbegin(); i != etapesAcceleration.rend(); ++i)
        {
            //std::cout<<'['<<(*i).first<<';'<<(*i).second<<"],";
            etapesFines.push_back(std::pair<int,int>((*i).first,(*i).second));
            etapesAccelerationDeceleration.push_back(std::pair<int,int>((*i).first,(*i).second));
            xCourant+=(*i).first;
            yCourant+=(*i).second;
        }
        //std::cout<<std::endl;

        std::cout<<'[';
        for (std::vector<std::pair<int,int>>::iterator i = etapesAccelerationDeceleration.begin(); i != etapesAccelerationDeceleration.end(); ++i)
            {
                std::cout<<'['<<(*i).first<<';'<<(*i).second<<"],";
            }
        std::cout<<std::endl;
        

        

        std::cout<<"xd/yd : "<<xd<<'/'<<yd<<"\txf/yf : "<<xf<<'/'<<yf<<"\txCourant/yCourant : "<<xCourant<<'/'<<yCourant<<std::endl;
        std::cout<<"Dernire etape :  : "<<xf- xCourant <<'/'<<yf- yCourant<<std::endl;

        if(xf- xCourant==0 and yf- yCourant==0){
            std::cout<<"On est arrivé a bon port !"<<std::endl;
        }else if (manhattan(xf- xCourant,yf- yCourant)<=maxAcceleration){
            std::cout<<"L'espace restant correspond a une petite etape"<<std::endl;
            //l'espace restant jusqu'a la fin est inferieur a l'acceleration maximale
            etapesFines.push_back(std::pair<int,int>(xf- xCourant,yf- yCourant));
            etapesAccelerationDeceleration.push_back(std::pair<int,int>(xf- xCourant,yf- yCourant));
            xCourant+=xf- xCourant;
            yCourant+=yf- yCourant;
            
        }else if(x==xd){
            //on a espace superieur à acc Max mais on est en trajectoire vertical
            bool yfPlusGrd=yf>yCourant;
            while((yCourant+pasY<yf and yfPlusGrd) or (yf<yCourant+pasY and not(yfPlusGrd))){
                etapesFines.push_back(std::pair<int,int>(0,pasY));
                etapesAccelerationDeceleration.push_back(std::pair<int,int>(0,pasY));
                yCourant+=pasY;
                
            }
            std::cout<<"On rajoute une petite etape : xc/yc "<<xCourant<<'/'<<yCourant<<std::endl;
            //ON revient au cas precedent...
            etapesFines.push_back(std::pair<int,int>(0,yf- yCourant));
            etapesAccelerationDeceleration.push_back(std::pair<int,int>(0,yf- yCourant));
            yCourant+=yf- yCourant;
        }else{
            //l'espace restant est superieur à l'acceleration maximale
            std::cout<<"L'espace restant est superieur a celui d'une etape de base\t";
            bool xfPlusGrd=xf>xCourant;
            while((xCourant+pasX<xf and xfPlusGrd) or (xf<xCourant+pasX and not(xfPlusGrd))){
                etapesFines.push_back(std::pair<int,int>(pasX,(m*(xCourant+pasX-xd)+yd)- yCourant));
                etapesAccelerationDeceleration.push_back(std::pair<int,int>(pasX,(m*(xCourant+pasX-xd)+yd)- yCourant));
                xCourant+=pasX;
                yCourant=(m*(xCourant-xd)+yd);
            }
            std::cout<<"On rajoute une petite etape : xc/yc "<<xCourant<<'/'<<yCourant<<std::endl;
            //On revient au cas precedent...
            etapesFines.push_back(std::pair<int,int>(xf- xCourant,yf- yCourant));
            etapesAccelerationDeceleration.push_back(std::pair<int,int>(xf- xCourant,yf- yCourant));
            xCourant+=xf- xCourant;
            yCourant+=yf- yCourant;
           
        }

        std::cout<<'[';
        for (std::vector<std::pair<int,int>>::iterator i = etapesAccelerationDeceleration.begin(); i != etapesAccelerationDeceleration.end(); ++i)
            {
                std::cout<<'['<<(*i).first<<';'<<(*i).second<<"],";
            }
        std::cout<<std::endl;
        xd=xf;
        yd=yf;
        i++;
        
    }
    int xTest=departureX;
    int yTest=departureY;
    std::cout<<'[';
    for (std::vector<std::pair<int,int>>::iterator i = etapesFines.begin(); i != etapesFines.end(); ++i)
        {
            std::cout<<'['<<(*i).first<<';'<<(*i).second<<"],";
            xTest+=(*i).first;
            yTest+=(*i).second;
        }
    std::cout<<std::endl;
    std::cout<<"Remplacement de la Trajectoire... "<<std::endl;

    this->remplacerTrajectoire(etapesFines);
}

/**
* Remplace la trajectoire de sommet en sommet trouvé par l'algorithme de Dijkstra par celle affinée ci-dessus
*
**/
void Trajectoire::remplacerTrajectoire(std::vector<std::pair<int,int>> trajectoire){
    etapes.clear();
    size=0;
    int res=0;
    for (std::vector<std::pair<int,int>>::iterator i = trajectoire.begin(); i != trajectoire.end(); ++i)
    {
        this->inserer(std::pair<int,std::pair<int,int>>(res++,std::pair<int,int>((*i).first,(*i).second)));
    }
    this->afficher();
}

void Trajectoire::reset(){
    etapes.clear();
    etapesGenerales.clear();
    size=0;
}






