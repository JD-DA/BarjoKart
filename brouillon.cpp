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
typedef std::map< int, int [2]> etapes;


int main(){

	string const nomFichier("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");

	ofstream maTrajectoire("/Users/salmabadri/Documents/Cours L3/Projet/mesEtapes.txt");

	if(maTrajectoire)  //On teste si tout est OK
		{
			maTrajectoire << "Bonjour, c'est seulement un test." << endl;
		}
	else {
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}

	/*
	map< int, int [2]> etapes;

	*/

	etapes c1;

	c1.insert(etapes::value_type(1, [38, 89]));
	c1.insert(etapes::value_type(2, [40, 78]));
	c1.insert(etapes::value_type(3, [24, 60]));


	cout << "l'étape 1 a " << c1.at(1) << "comme coordonnées" << endl;
	cout << "l'étape 2 a " << c1.at(2) << "comme coordonnées" << endl;
	cout << "l'étape 3 a " << c1.at(3) << "comme coordonnées" << endl;

	return 0;
}



