#include <iostream>
#include <fstream>  //pour écrire dans un fichier
#include <map>

#include <string>

using namespace std;

//faire un define pour le chemin de la création du fichier

/*

class Kart {

	int chemin(int x, int y, int acc_max);
public:

	void ligneDroite();
	Kart();
	~Kart();
	
};

*/

/*

	string const nomFichier("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");

	ofstream maTrajectoire("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");


	if(maTrajectoire)  //On teste si tout est OK
		{
			maTrajectoire << "Bonjour, c'est seulement un test." << endl;


		}
	else {
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}

*/


int main(){

	typedef std::map< int, int* > etapes;

	int coords[2]={0, 0};

	int* p(0);

	p= coords;

	*p= 38;
	*(p+1) = 89;


	etapes c1;

	printf("p= %p, coords[0]= %d \n", p, coords[0]);


	c1.insert(std::pair<int, int*>(1, coords));

	/*
	c1.insert(etapes::value_type(2, [40, 78]));
	c1.insert(etapes::value_type(3, [24, 60]));
	*/


	cout << "l'étape 1 a " << c1[1] << "comme coordonnées" << endl;

	/*
	cout << "l'étape 2 a " << c1.at(2) << "comme coordonnées" << endl;
	cout << "l'étape 3 a " << c1.at(3) << "comme coordonnées" << endl;
	*/

	p = 0;

	return 0;
}



