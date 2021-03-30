#include <iostream>
#include <cstdlib>
#include <stdio.h>

#include <png.h>
#include <png++/png.hpp>

#include <toml++/toml.h>

#include "image.hpp"
Image::Image(){
}

Image::~Image(){
	for (int i = 0; i < this->height; i++)
	{
		delete(matrice[i]);
	}
	delete(matrice);
	/*for (std::list<std::pair<int,int>>::iterator i = zoneArrive.begin(); i != zoneArrive.end(); ++i)
	{
		delete(i);
	}*/
}

void Image::build_image(std::string fichier){
	std::cout<<"Construction de l'image..."<<std::endl;
	std::cout<<fichier<<std::endl;

	png::image< png::rgba_pixel > image(fichier);

	int nbPixelArrive=0;

	this->width=image.get_width();
	this->height=image.get_height();
	std::cout<<"Largeur : "<<width<<" Hauteur : "<<height<<std::endl;
	int r;
	int g;
	int b;
	//int a;
	this->matrice = new char*[height];
	//int size = 0;
	//bool test;
		for (unsigned int i = 0; i <(unsigned int) this->height; ++i)
		{ 
			this->matrice[i]= new char[width];
			for(unsigned int j=0;j<(unsigned int) this->width;++j){
				r = image.get_pixel(j,i).red;
				g = image.get_pixel(j,i).green;
				b = image.get_pixel(j,i).blue;
				//a=image.get_pixel(j,i).alpha;
				//std::cout<<r<<' '<<g<<' '<<b<<std::endl;
				if((r==255 and g==255 and b==255)){
					this->matrice[i][j]='b';
					//putchar(' ');
				}else if(r==arrivalColorR and g==arrivalColorG and b==arrivalColorB){
					this->matrice[i][j]='a';
					nbPixelArrive++;
					zoneArrive.push_back(std::pair<int,int>(i,j));
					//putchar('a');
				}else if(r==0 and g==0 and b==0){
					this->matrice[i][j]='n';
					//putchar('n');
				}else{
					this->matrice[i][j]='g';
					//putchar('g');
				}

			}
			//putchar('\n');	
		}
		std::cout<<"Image contruite !"<<std::endl;
		std::cout<<"Nb de pixel de la zone d'arrivée : "<<nbPixelArrive<<std::endl;
		std::cout<<"Taille de la liste : "<<zoneArrive.size()<<std::endl;
}

void Image::load_data(std::string fpng,std::string ftoml){
	toml::table tbl;
	try{
		tbl = toml::parse_file(ftoml);
		//std::cout<<tbl<<std::endl;
		std::cout<<"Lecture du .toml..."<<std::endl;
		maxAcceleration = tbl["acc_max"].value<int>().value();
		arrivalColorR = (tbl["couleur_arrivee"][0].value<int>().value());
		arrivalColorG = (tbl["couleur_arrivee"][1].value<int>().value());
		arrivalColorB = (tbl["couleur_arrivee"][2].value<int>().value());
		departureX= (tbl["depart"]["x"].value<int>().value());
		departureY= (tbl["depart"]["y"].value<int>().value());

		std::cout<<"Acceleration Max : "<<maxAcceleration<<std::endl;
 		std::cout<<"Couleur d'arrivée :"<<arrivalColorR<<' '<<arrivalColorG<<' '<<arrivalColorB<<std::endl;
		std::cout<<"Position de départ : x->"<<departureX<<" y->"<<departureY<<std::endl;

		build_image(fpng);

	}catch(const toml::parse_error& err){
		std::cerr<<"La lecture du toml a échouée :\n"<<err<<"\n";
	}


}

/*
Permet de verifier le bonne lecture du fichier png via la creation d'un fichier pgm copiant l'image et donnant aux differentes zones un niveau de gris different.
*/
void Image::affichage(char* fichierSortie){
FILE* fichier = fopen(fichierSortie,"w");
if(fichier!=NULL){
	fputs("P2",fichier);
	fputs("\n",fichier);
	fprintf(fichier, "# image générée par Image::affichage\n");
	fprintf(fichier, "%d\t", width);
	fprintf(fichier, "%d\n", height);
	fprintf(fichier, "%d\n", 255);
std::cout<<"Écriture PGM..."<<std::endl;

	

	int num;
	for(int i=0; i<height;i++){
		for(int j=0;j<width;j++){
			char cara =matrice[i][j];
			if(cara=='b'){
				//utilisé pour afficher les directions
				/*if(i<2*(j)+(y-2*x)){
					if(i>-0.5*j+(y+0.5*x)){
						if(i>0.5*j+(y-0.5*x)){
							num = 200;
						}else{
							num = 150;
						}
					}else{
						if(i>-2*j+(y+2*x)){
							num = 100;
						}else{
							num = 50;
						}
					}
				}else{
					if(i>-0.5*j+(y+0.5*x)){
						if(i>-2*j+(y+2*x)){
							num = 250;
						}else{
							num = 200;
						}
					}else{
						if(i>0.5*j+(y-0.5*x)){
							num = 150;
						}else{
							num = 100;
						}
					}
				}*/
				num = 255;
				
			}else if(cara=='n'){
				num = 0;
			}else if (cara=='a'){
				num = 50;
			}else{
				//std::cout<<"Charactère bizarre : i="<<i<<" j="<<j<<std::endl;
				num = 100;
			}
			fprintf(fichier, "%d ", num);
		}
		//putchar('\n');
	}
	fclose(fichier);
	std::cout<<"L'image a été écrite, allez voir :"<<fichierSortie<<std::endl;
}
else{
	std::cout<<"Erreur d'ouverture du fichier"<<std::endl;
}
}

void Image::affichageDirection(const char* fichierSortie, int x,int y){
FILE* fichier = fopen(fichierSortie,"w");
if(fichier!=NULL){
	fputs("P2",fichier);
	fputs("\n",fichier);
	fprintf(fichier, "# image générée par Image::affichage\n");
	fprintf(fichier, "%d\t", width);
	fprintf(fichier, "%d\n", height);
	fprintf(fichier, "%d\n", 255);
std::cout<<"Écriture PGM..."<<std::endl;

	
	int num;
	for(int i=0; i<height;i++){
		for(int j=0;j<width;j++){
			char cara =matrice[i][j];
			if(cara=='b'){
				
				if(i<2*(j)+(y-2*x)){
					if(i>-0.5*j+(y+0.5*x)){
						if(i>0.5*j+(y-0.5*x)){
							num = 200;
						}else{
							num = 150;
						}
					}else{
						if(i>-2*j+(y+2*x)){
							num = 100;
						}else{
							num = 50;
						}
					}
				}else{
					if(i>-0.5*j+(y+0.5*x)){
						if(i>-2*j+(y+2*x)){
							num = 250;
						}else{
							num = 200;
						}
					}else{
						if(i>0.5*j+(y-0.5*x)){
							num = 150;
						}else{
							num = 100;
						}
					}
				}
				
			}else if(cara=='n'){
				num = 0;
			}else if (cara=='a'){
				num = 50;
			}else{
				num = 100;
			}
			fprintf(fichier, "%d ", num);
		}
	}
	fclose(fichier);
	std::cout<<"L'image a été écrite, allez voir :"<<fichierSortie<<std::endl;
}
else{
	std::cout<<"Erreur d'ouverture du fichier"<<std::endl;
}
}

bool Image::verifierPixel(int x,int y){
	return matrice[y-1][x-1]!='n';
}

bool Image::verifierPixel2(int x,int y){
	return matrice[y][x]!='n';
}


bool Image::verifierPixel3(int x,int y){
	bool test= matrice[y][x]!='n';
	if(x-1>0){
		test = test and matrice[y][x-1]!='n';
	}
	if(y-1>0){
		test = test and matrice[y-1][x]!='n';
	}
	if(y+1<height){
		test = test and matrice[y+1][x]!='n';
	}
	if(x+1<width){
		test = test and matrice[y][x+1]!='n';
	}
	return test;
}

bool Image::verifierArrivee(int x,int y){
	return matrice[y-1][x-1]=='a';
}

bool Image::verifierArrivee2(int x,int y){
	return matrice[y][x]=='a';
}

/**
* renvoi une paire de coordonnées de type y/x  correspondant au centre de la zone d'arrivée(origine en haut à gauche)
**/
std::pair<int,int> Image::centreZoneArrivee(){
	std::list<std::pair<int,int>>::iterator i = zoneArrive.begin();
	int res=0;
	while(res<zoneArrive.size()/2){
		i++;
		res++;
	}
	return (*i);
}



/**
* Pour un point donné, renvoie la direction vers laquelle se trouve le plus grand nombre de pixel qui composent la zone d'arrivée
*
**/
std::map<std::string,int> Image::direction(int x,int y){
	std::map<std::string,int> map;
	int n = 0;
	map.insert(std::pair<std::string,int>("n",0));
	map.insert(std::pair<std::string,int>("e",0));
	map.insert(std::pair<std::string,int>("s",0));
	map.insert(std::pair<std::string,int>("o",0));
	map.insert(std::pair<std::string,int>("ne",0));
	map.insert(std::pair<std::string,int>("no",0));
	map.insert(std::pair<std::string,int>("so",0));
	map.insert(std::pair<std::string,int>("se",0));
	int e = 0;
	int s = 0;
	int o = 0;
	int ne = 0;
	int no = 0;
	int se = 0;
	int so = 0;

	//float diff = y-x;

	for (std::list<std::pair<int,int>>::iterator it = zoneArrive.begin(); it != zoneArrive.end(); ++it)
	{
		int xa=(*it).second;
		int ya=(*it).first;

		if(ya<2*xa+(y-2*x)){
			if(ya>-0.5*xa+(y+0.5*x)){
				if(ya>0.5*xa+(y-0.5*x)){
					++se;
					map["se"]++;
				}else{
					++e;
					map["e"]++;
				}
			}else{
				if(ya>-2*xa+(y+2*x)){
					++ne;
					map["ne"]++;
				}else{
					++n;
					map["n"]++;
				}
			}
		}else{
			if(ya>-0.5*xa+(y+0.5*x)){
				if(ya>-2*xa+(y+2*x)){
					++s;
					map["s"]++;
				}else{
					++so;
					map["so"]++;
				}
			}else{
				if(ya>0.5*xa+(y-0.5*x)){
					++o;
					map["o"]++;
				}else{
					++no;
					map["no"]++;
				}
			}
		}
			
	}
	std::cout<<"n "<<n<<" ne "<<ne<<" e "<<e<<" se "<<se<<" s "<<s<<" so "<<so<<" o "<<o<<" no "<<no<<std::endl;
	
return map;


}

/**
* Renvoi un int qui correpond au potentiel de ce point dans cette direction, c'est à dire la distance qui le sépare du prochain obstacle
*
**/
int Image::potentiel(std::string dir, int x,int y){
	int pasX;
	int pasY;

	if(strcmp(dir.c_str(),"n")==0){
			pasX=0;
			pasY=-1;
		}else if(strcmp(dir.c_str(),"no")==0){
			pasX=-1;
			pasY=-1;
		}else if(strcmp(dir.c_str(),"o")==0){
			pasX=-1;
			pasY=0;
		}else if(strcmp(dir.c_str(),"so")==0){
			pasX=-1;
			pasY=1;
		}else if(strcmp(dir.c_str(),"s")==0){
			pasX=0;
			pasY=1;
		}else if(strcmp(dir.c_str(),"se")==0){
			pasX=1;
			pasY=-1;
		}else if(strcmp(dir.c_str(),"e")==0){
			pasX=1;
			pasY=0;
		}else{ //ne
			pasX=1;
			pasY=-1;
		}
	int nbPasPotentiels = 0;
	x+=pasX;
	y+=pasY;
	while(verifierPixel2(x,y)){
		nbPasPotentiels++;
		x+=pasX;
		y+=pasY;
	}

	return nbPasPotentiels;
}

