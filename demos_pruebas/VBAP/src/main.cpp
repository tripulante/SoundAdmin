/**
 * @mainpage VBAP
 * @brief Desarrollo de una librería que implementa el algoritmo Vector base Amplitude Panning,
 *  propuesto por Ville Pulkki (http://www.acoustics.hut.fi/~ville/). Se basa en una 
 * implementación propuesta en Creating Auditory Displays with Multiple Loudspeakers Using VBAP: A Case Study with DIVA Project
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * */

#include <iostream>
#include "Speaker.h"
#include "Triangle.h"
#include "Vbap.h"
/**
 * 
 * @brief Ejemplo de uso de VBAP.
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @example main.cpp
 * */

using namespace std;

Vbap *v;

int main(int argc, const char* argv[] ){
	cout << "Vbap Test" << endl;
	
	if(argc != 3){
		cout << "Ingrese Los argumentos Correctamente!" << endl;
		cout << "VBAP <numspeakers> <ruta_archivo_configuración>" << endl;
		return 0;
	}
	else{
		int numspeak = atoi(argv[1]);
		v = new Vbap(numspeak);
		if(!v->createSpeakers(argv[2])){
			cout << "Error al cargar el archivo" << endl;
			return -1;
		}
		v->createAllTriangles();
		v->removeTriangles();
		pair<Vec3f, int*>* testPair;
		char c;
		float x, y, z;
		x = y = z = 0.0f;
		while(c != 'o'){
			cin >> c;
			switch(c){
				case 'w': y += 0.1;
				break;
				case 's': y -= 0.1;
				break;
				case 'a': x -= 0.1;
				break;
				case 'd': x += 0.1;
				break;
				case 'r': z += 0.1;
				break;
				case 'f': z -= 0.1;
				break;
				default: break;
			}
			testPair = v->selectTriangle(x, y, z);
			cout << "Ganancia: " << testPair->first << " Speakers No. (";
			for(int i = 0; i < 3; i++)
				cout << testPair->second[i] << ", ";
			cout << ")" << endl;
		}
		delete v;
		return 0;
	}
}
