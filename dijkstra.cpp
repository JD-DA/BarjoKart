#include <iostream>
#include <string>
#include <list>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <limits>
#include <set>
#include "dijkstra.hpp"


using namespace std;

Dijkstra::Dijkstra(Image* pt, Trajectoire* tr){
	img=pt;
	xCourant=img->departureX;
	yCourant=img->departureY;
	maxAcceleration=img->maxAcceleration;
	traj=tr;
	cout<<"nbNodes "<<NbNodes<<endl;
	this->voisinExiste = new int*[img->height];
}

Dijkstra::~Dijkstra(){
	/*for (int i = 0; i < img->height; ++i)
	{
		delete(voisinExiste[i]);
	}
	delete(voisinExiste);*/
	
}
 
auto comp = [](const pair<int, int> &a, const pair<int, int> &b) {return a.second > b.second; };
 
 /**
 * Algorithme de Dijkstra
 *
 **/
void Dijkstra::algoDijkstra () {
 //crédit : 40tude.fr
	cout<<"on lance Dijkstra"<<endl;
  
  int StartNode = 0;
 
  vector<int>* Distances = new vector<int>(img->height*img->width, numeric_limits<int>::max());                   
 
  (*Distances)[StartNode] = 0;                                                      
  vector<int>* Parents=new vector<int>(img->height*img->width, -1);                
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> Q(comp);
  Q.push(make_pair(StartNode, 0));
 
  while (!Q.empty()) {
    int v = Q.top().first;  
    int w = Q.top().second;
    Q.pop();
 
    if (w <= (*Distances)[v]) { 
                               
 
      for (const auto& i : G[v]) { 
        auto v2 = i.first;        
        auto w2 = i.second;
 
        if ((*Distances)[v] + w2 < (*Distances)[v2]) {
          (*Distances)[v2] = (*Distances)[v] + w2;    
          (*Parents)[v2] = v;
          Q.push(make_pair(v2, (*Distances)[v2]));
        }
      }
    }
  }
 
  /*for (auto i = 0; i != NbNodes; ++i) {
    cout << "\nPath from node " << StartNode << " to node " << i << " cost " << (*Distances)[i] << endl;
 
    cout << i;
    for (auto p = (*Parents)[i]; p != -1; p = (*Parents)[p])
      cout << " <- " << p;
    cout << endl;         
  }*/
  chemin.push_back(noeudArrivee);
  cout << noeudArrivee;
  for (auto p = (*Parents)[noeudArrivee]; p != -1; p = (*Parents)[p]){
     chemin.push_back(p);  
     cout << " <- " << p;                                             
    }
    cout << endl;

    delete(Parents);
    delete(Distances);
}


/**
* Fonction importante qui va créer un graphe basé sur l'image, pour ne pas avoir un graphe dont chaque sommet correspond a un pixel 
* ici les sommets sont séparé d'une distance de maxAcceleration/2
* Créer aussi les arêtes entre chaque sommet en verifiant qu'aucun obstacle ne passe au travers
* A été tenté a la fin de cette fonction de reduire le graphe au sous graphe contenu dans la figure mais cela créait un probleme au niveau de la numerotation 
* des sommets pour l'algorithme de dijkstra
**/
void Dijkstra::createGraph(){
	cout<<"Creation du graph"<<endl;
	int indexNoeud = 1;

	int maxAcceleration=(img->maxAcceleration)/2;

	int nb=0;

	for (int i = yCourant%maxAcceleration; i < img->height; i+=maxAcceleration)
	{
		for (int j = xCourant%maxAcceleration; j < img->width; j+=maxAcceleration)
		{
			nb++;
		}
	}

	cout<<" Creation du Graph : accMax : "<<maxAcceleration<<endl;
	cout<<" premier x : "<<xCourant%maxAcceleration<<" Premier y : "<<yCourant%maxAcceleration<<endl;

	vector<vector<pair<int, int>>> graph(nb);
	map<int,pair<int,int>> coordonnees;

	for (int i = yCourant%maxAcceleration; i < img->height; i+=maxAcceleration)
	{
		voisinExiste[i]=new int[img->width];
		for (int j = xCourant%maxAcceleration; j < img->width; j+=maxAcceleration)
		{
			if(j==xCourant and i==yCourant){
				voisinExiste[i][j]=0;
				coordonnees.insert(pair<int,pair<int,int>>(0,pair<int,int>(xCourant,yCourant)));
				cout<<" "<<0;
			}else{
				if(img->verifierPixel2(j,i)){
					cout<<" "<<indexNoeud;
					coordonnees.insert(pair<int,pair<int,int>>(indexNoeud,pair<int,int>(j,i)));
					voisinExiste[i][j]=indexNoeud++;

					
				}
				else{
					voisinExiste[i][j]=-1;
					cout<<' '<<-1;
				}
			}
			

		}
		cout<<endl;
	}

	cout<<"On a créé les noeuds"<<endl;
	int numNoeud;
	int numVoisin;
	for (int i = yCourant%maxAcceleration; i < img->height; i+=maxAcceleration)
	{
		for (int j = xCourant%maxAcceleration; j < img->width; j+=maxAcceleration)
		{
			numNoeud = voisinExiste[i][j];
			if(numNoeud>-1){
				
				if(i- maxAcceleration >0){ //n
					numVoisin=voisinExiste[i- maxAcceleration][j];
					if(numVoisin!=-1){
						if(traj->verifierSegment(j ,i ,j ,i-maxAcceleration)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					
				}
				if(i+ maxAcceleration <img->width){ //s
					numVoisin=voisinExiste[i+ maxAcceleration][j];
					if(numVoisin!=-1){
						if(traj->verifierSegment(j ,i ,j,i+ maxAcceleration)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					
				}
				if(j- maxAcceleration >0){ //o
					numVoisin=voisinExiste[i][j- maxAcceleration];
					if(numVoisin>0){
						if(traj->verifierSegment(j ,i ,j- maxAcceleration ,i)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					if(i+maxAcceleration<img->height){//so
						numVoisin=voisinExiste[i+maxAcceleration][j- maxAcceleration];
							if(numVoisin>0){
								if(traj->verifierSegment(j ,i ,j- maxAcceleration ,i+maxAcceleration)){
								graph[numNoeud].push_back(make_pair(numVoisin, 1));
							}
					}
					}
					if(i- maxAcceleration >0){ //no
					numVoisin=voisinExiste[i- maxAcceleration][j- maxAcceleration];
					if(numVoisin>0){
						if(traj->verifierSegment(j ,i ,j-maxAcceleration ,i-maxAcceleration)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					
				}
					
				}

				if(j+ maxAcceleration <img->width){ //e
					numVoisin=voisinExiste[i][j+ maxAcceleration];
					if(numVoisin>0){
						if(traj->verifierSegment(j ,i ,j+ maxAcceleration ,i)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					if(i+maxAcceleration<img->height){//se
						numVoisin=voisinExiste[i+maxAcceleration][j+ maxAcceleration];
							if(numVoisin>0){
								if(traj->verifierSegment(j ,i ,j+ maxAcceleration ,i+maxAcceleration)){
								graph[numNoeud].push_back(make_pair(numVoisin, 1));
							}
					}
					}
					if(i- maxAcceleration >0){ //ne
					numVoisin=voisinExiste[i- maxAcceleration][j+ maxAcceleration];
					if(numVoisin>0){
						if(traj->verifierSegment(j ,i ,j+maxAcceleration ,i-maxAcceleration)){
						graph[numNoeud].push_back(make_pair(numVoisin, 1));
					}
					}
					
				}
					
				}


			}
		}
	}
	int indice=0;
	//affiche les noeuds
	/*for (vector<vector<pair<int, int>>>::iterator i = graph.begin(); i != graph.end(); ++i)
		{
			cout<<indice++<<" : [";
			for (std::vector<pair<int,int>>::iterator j = (*i).begin(); j != (*i).end(); ++j)
			{
				cout<<'['<<(*j).first<<','<<(*j).second<<']';
			}
			cout<<']'<<endl;

			
		}*/
indice=0;
this->NbNodes=graph.size();
cout<<"Nombre de noeuds : "<<NbNodes<<endl;
	this->G=graph;
	this->coor=coordonnees;
}

/**
* Renvoie le noeud le plus proche de l'arrivée
*
**/
void Dijkstra::chercherNoeudArrive(){
	cout<<"Recherche du noeud d'arrivée"<<endl;
	int ndMax;
	int distanceMini=max(img->height,img->width);
	int distanceRes;
	this->noeudDansArrivee=false;
	for (std::map<int,pair<int,int>>::iterator i = coor.begin(); i != coor.end(); ++i)
	{
		if(img->verifierArrivee2((*i).second.first,(*i).second.second) ){
			ndMax= (*i).first;
			this->noeudDansArrivee=true;
			break;
		}
		distanceRes = traj->distanceMiniArrivee((*i).second.first,(*i).second.second);
		
		if(distanceMini>distanceRes){
			ndMax = (*i).first;
			distanceMini = distanceRes;
		}
	}

	cout<<"Le noeud le plus proche : "<<ndMax<<endl;
	this->noeudArrivee = ndMax;
	traj->finalX=coor[ndMax].first;
	traj->finalY=coor[ndMax].second;
}


/**
* Prend la suite de sommet trouvé par l'algo de dikkstra, tous espacés de moins que l'AccMax, et créé alors la trajectoire dans cette derniere classe
* Une étape correspond à une liason entre deux sommets.
**/
void Dijkstra::transformer(){
	int indice=1;
	int x=coor[0].first;
	int y=coor[0].second;
	cout<<"On traduit le chemin de sommet en sommet en une liste d'étapes de vitesse : "<<std::endl;
	cout<<"X & y de départ : "<<x<<' '<<y<<endl;
	cout<<"trajectoire= ["<<endl;

	cout<<"Taille chemin : "<<chemin.size()<<endl;
	if(not(chemin.empty())){
		auto i = (chemin.rbegin());
	i++;
	int xs,ys;

	while(i != chemin.rend()){
		xs=coor[*i].first-x;
		ys=coor[*i].second-y;
		cout<<"["<<xs<<','<<ys<<"],";
		
		x=coor[*i].first;
		y=coor[*i].second;

		++i;
		traj->inserer(pair<int,pair<int,int>>(indice++,pair<int,int>(xs,ys)));
	}
	cout<<"]"<<endl;
}else{
	cout<<"\n\n Un problème est survenu, aucune trajectoire n'a été trouvée...\n\n"<<endl;
}
	
	
	
}

void Dijkstra::reset(){
	chemin.clear();
}

