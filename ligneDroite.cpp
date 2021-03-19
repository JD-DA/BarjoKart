#include <iostream>
#include <fstream>  //pour écrire dans un fichier
#include <map>

#include <string>

using namespace std;

//faire un define pour le chemin de la création du fichier


int main(){

	int acc_max = 33;

	std::map< int, int* > c1;


	int coords[2];
	int coords2[2];
	int coords3[2];
	int coords4[2];

	/*

	coords[0]= 38;
	coords[1]= 89;


	coords2[0] = 40;
	coords2[1] = 78;

	coords3[0] = 24;
	coords3[1] = 60;

	*/

	coords[0]= 1;
	coords[1]= 4;


	coords2[0] = 5;
	coords2[1] = -2;

	coords3[0] = -5;
	coords3[1] = 13;

	coords4[0] = 24;
	coords4[1] = 60;



	c1.insert(std::pair<int, int*>(1, coords));
	c1.insert(std::pair<int, int*>(2, coords2));
	c1.insert(std::pair<int, int*>(3, coords4));
	c1.insert(std::pair<int, int*>(4, coords3));

	std::map< int, int* >::iterator it;


	string const nomFichier("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");

	ofstream maTrajectoire("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");



	string const nomFichier2("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapesLD.txt");

	ofstream mesEtapesAjour("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapesLD.txt");


	if(maTrajectoire)  //On teste si tout est OK
		{

			for (it = c1.begin(); it != c1.end(); it++){

				maTrajectoire << "l'étape " << it->first <<" a " << c1[it->first][0] << "," << c1[it->first][1] << " comme coordonnées"<< endl;

			}
			
		}
	else {
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}


	for (it = c1.begin(); it != c1.end(); it++){

		int xAB = c1[ ( it -> first ) + 1 ] [0] - c1[ it -> first] [0];
		int yAB = c1[ ( it -> first ) + 1 ] [1] - c1[ it -> first ] [1];

		int xAC = c1[ ( it -> first ) + 2 ] [0] - c1[ it -> first] [0];
		int yAC =  c1[ ( it -> first ) + 2 ] [1] - c1[ it -> first ] [1];


		if( (xAB * yAC - xAC * yAB) == 0) {

			if (manhattan(int x, int y) < acc_max){

				mesEtapesAjour << "l'étape " << it->first <<" a " << c1[it->first][0] << "," << c1[it->first][1] << " comme coordonnées"<< endl;

				mesEtapesAjour << "l'étape " << (it->first)+2 <<" a " << c1[(it->first)+2][0] << "," << c1[(it->first)+2][1] << " comme coordonnées"<< endl;
				//penser à garder un bon ordre des étapes donc déplacer les coordonnées de l'étape i+2 à i+1
			}

			else {

				mesEtapesAjour << "l'étape " << it->first <<" a " << c1[it->first][0] << "," << c1[it->first][1] << " comme coordonnées"<< endl;

				mesEtapesAjour << "l'étape " << (it->first)+1 <<" a " << c1[(it->first)+1][0] << "," << c1[(it->first)+1][1] << " comme coordonnées"<< endl;

				mesEtapesAjour << "l'étape " << (it->first)+2 <<" a " << c1[(it->first)+2][0] << "," << c1[(it->first)+2][1] << " comme coordonnées"<< endl;
			}

			
		}

		else {

			mesEtapesAjour << "l'étape " << it->first <<" a " << c1[it->first][0] << "," << c1[it->first][1] << " comme coordonnées"<< endl;

			//problème : ca n'affiche pas les deux dernieres étapes 
		}
	}

	
	/*
	cout << "l'étape 2 a " << c1.at(2) << "comme coordonnées" << endl;
	cout << "l'étape 3 a " << c1.at(3) << "comme coordonnées" << endl;
	*/


	return 0;
}

