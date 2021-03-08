#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <png.h>
#include <png++/png.hpp>
#include <toml++/toml.h>
#include "image.hpp"



int main(){

	Image img = Image();
	
	//img.load_data("circuits/circuit0.png","circuits/circuit0.toml");
	//img.load_data("circuits/fusee.png","circuits/fusee.toml");
	img.load_data("circuits/trois_quatorze.png","circuits/trois_quatorze.toml");

	img.affichage("image.pgm");
}