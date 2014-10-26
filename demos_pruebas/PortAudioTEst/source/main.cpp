/** 
* @brief Permite cargar varias fuentes que usan ondas sinusoidales, las mezcla y las saca usando portaudio
* @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
* @example main.cpp
*/

#include <iostream>
/*Include portaudio*/
#include <portaudio.h>
/*include libsndfile*/
#include <sndfile.hh>
#include <string>
#include <fstream>
#include <sstream>
#include "SoundFile.h"

using namespace std;

/**
 * Administrador de las fuentes de audio
 * */
SoundFile* soundfile;

/**
* Carga un archivo de configuración del sistema
* @param path Ruta al archivo de configuración
* @return false si el archivo no se pudo abrir
*/
bool loadfile(const char* path);

int main(int argc, const char* argv[]){

	if(argc != 2){
		cout << "Ingresar Parámetros correctamente" << endl;
		return 0;
	}
// 	else{
// 		int numSources = atoi(argv[2]);
// 		cout << numSources << endl;
// 		if(numSources > 0){
// 			for(int i = 3; i < argc; i++){
// 				string str(argv[i]);
// 				ids.push_back(str);
// 				cout << str << endl;
// 			}
// 			if(ids.size() != (unsigned int) numSources)
// 			{
// 				cout << "Ingrese los parámetros correctamente" << endl;
// 				return EXIT_FAILURE;
// 			}
// 		}
// 	}
	
	if(!loadfile(argv[1]))
		return EXIT_FAILURE;
	

	if(soundfile->startPortaudio()){
		/*imprimimos info de apis*/
		soundfile->printApiInfo();
		/*imprimimos los dispositivos*/
		soundfile->printDeviceInfo();

		/*Vamos a usar la Audigy Para probar las salidas!*/
		cout << "Escoja el dispositivo por el que se desea salir" << endl;
		int device = 0;
		cin >> device;
		
// 		vector<string>::iterator it;	
// 		for(it = ids.begin(); it != ids.end(); it++){
// 			soundfile->addSource((*it));
// 		}

		if(!soundfile->initialize(device))
			return EXIT_FAILURE;
		
		if(!soundfile->startPlaying())
			return EXIT_FAILURE;
		cout << "Presione 's' para terminar" << endl;
		cout << "'a' para cambiar las ganancias de una fuente al azar" << endl;
		cout << "'p' para cambiar la fase de una fuente al azar" << endl;
		char c = 'a';
		do{
			cin >> c;
			if(c == 'a'){
				soundfile->changeRandomGain();
			}
			else if(c == 'p'){
				soundfile->changeRandomPhase();
			}
			else if(c == 'c')
				soundfile->changeChannel();
		}
		while(c != 's');
		if(!soundfile->stopPlaying())
			return EXIT_FAILURE;
		soundfile->finishPortaudio();
		delete soundfile;
		return 0;
	}
	else{
		return EXIT_FAILURE;
	}
}


bool loadfile(const char* path){
	ifstream file(path);
	if(file.is_open()){
		string line;
		getline(file, line);
		int numchannels;
		file >> numchannels;
		soundfile = new SoundFile(line.c_str(), numchannels);
		while(!file.eof()){
			getline(file, line);
			soundfile->addSource(line);
			cout << line << endl;
		}
		return true;
	}
	else{
		cout << "El archivo no se pudo abrir" << endl;
		return false;
	}
}
