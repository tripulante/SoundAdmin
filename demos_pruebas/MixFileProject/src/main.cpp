/**
 * @brief Archivo de prueba que permite mezclar dos señales de audio distintas.
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @example main.cpp
 * */

#include <iostream>
#include "FileMixer.h"
#include <sstream>
using namespace std;

/**
 * Mezclador principal del sistema
 * */
FileMixer *mixer;

/**
 * Función principal del programa
 * */
int main(int argc, const char *argv[]){

	if(argc != 4){
		cout << "Los argumentos son incorrectos: ";
		cout << "Debe ingresar: ./MixFileProject <samplerate> <archivo1> <archivo2>" << endl;
		return -1;
	}
	else{
		stringstream sstream(stringstream::in | stringstream::out);
		sstream << argv[1];
// 		cout << sstream.str() << endl;
		int sampleRate;
		sstream >> sampleRate;
// 		cout << sampleRate << endl;
		try{
			mixer = new FileMixer(sampleRate);
			mixer->createSource("test1", argv[2]);
			mixer->createSource("test2", argv[3]);
			mixer->startPlaying();
			char c;
			while(c != 'o'){
				cin >> c;
				if(c == 'c')
					mixer->changeRandomGain();
			}
			delete mixer;
			return 0;
		}
		catch(string& s){
			cout << s << endl;
			return -1;
		}
	}
}
