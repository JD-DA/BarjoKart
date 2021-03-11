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
	for (int i = 0; i < height; ++i)
	{
		delete(matrice[i]);
	}
	delete(matrice);
}

void Image::build_image(std::string fichier){
	std::cout<<"Construction de l'image..."<<std::endl;
	std::cout<<fichier<<std::endl;

	png::image< png::rgba_pixel > image(fichier);

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
					//putchar('a');
				}else if(r==0 and g==0 and b==0){
					//std::cout<<"000 Vrai"<<std::endl;
					this->matrice[i][j]='n';
					//putchar('n');
				}else{
					/*test=false;
					for (int v = 0; v < size; ++v)
					{

						int* tabtest = tab[v];
						if(tabtest[0]==r and tabtest[1]==g and tabtest[2]==b){
							test = true;
						}
					}
					if(!test){
						int* letab = new int[3];
						letab[0]=r;
						letab[1]=g;
						letab[2]=b;
						tab[size]= letab;
						size++;
					}*/
					this->matrice[i][j]='g';
					//putchar('g');
				}

			}
			//putchar('\n');	
		}
		std::cout<<"Image contruite !"<<std::endl;
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
	for(int i=0; i<width;i++){
		for(int j=0;j<height;j++){
			char cara =matrice[i][j];
			if(cara=='b'){
				num = 255;
			}else if(cara=='n'){
				num = 0;
			}else if (cara=='a'){
				num = 200;
			}else{
				std::cout<<"Charactère bizarre : i="<<i<<" j="<<j<<std::endl;
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

bool Image::verifierPixel(int x,int y){
	std::cout<<x-1<<';'<<y-1<<' '<<matrice[y-1][x-1];
	return matrice[y-1][x-1]!='n';
}

bool Image::verifierArrivee(int x,int y){
	return matrice[y-1][x-1]=='a';
}

