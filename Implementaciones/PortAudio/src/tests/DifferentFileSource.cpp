/**
 * @brief Prueba que verifica la carga de distintos archivos y su uso en múltiples fuentes, además
 * de moverlas aleatoriamente de acuerdo a la entrada del usuario
 * @author John Jairo Palma Robayo
 * @example DifferentFileSource.cpp
 * */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pruebas.h"
#include "../soundadmin/SoundAdminPortaudio.h"

#if PRUEBA == 3
using namespace std;

/**
 * Administrador que se va a probar
 * */
SoundAdminPortaudio *test;

/**
 * Permite el movimiento de fuentes en el espacio
 * */
float srcPos[3] = {0};

/**
 * Se encarga de la construcción del sistema a partir de un archivo de configuración
 * @param file Archivo que se va a leer
 * @return true si al administrador se pudo crear exitosamente
 * */
bool loadfile(const char *file);

/**
 * Carga archivos de audio al sistema
 * Los archivos consisten de <descripcion>:<ruta> 
 * */
void loadAudioFiles(const char *srcs);

/**
 * Mueve una fuente de manera aleatoria
 * */
void moveSource();

/**
 * Función principal
 * */
int main(int argc, const char *argv[]){
	if(argc != 2){
		cout << "Error al cargar el sistema!" << endl << "Debe usarse: " << endl;
		cout << "./Uniandes3DAudio <archivocarga>" << endl;
		return 0;
	}
	else{
		srand(time(NULL));
		if(!loadfile(argv[1])){
			cout << "Error al cargar los archivos" << endl;
			return 0;
		}
		char c;
		while(c != 'o'){
			cin >> c;
			switch(c){
			case 'a': srcPos[0]--; break;
			case 'd': srcPos[0]++; break;
			case 'w': srcPos[1]++; break;
			case 's': srcPos[1]--; break;
			case 'r': srcPos[2]++; break;
			case 'f': srcPos[2]--; break;
			case 't': srcPos[0] = srcPos[1] = srcPos[2] = 0.0; break;
			default: break; 
			}
			moveSource();
		}
		return 0;
	}
}

bool loadfile(const char *file){
	ifstream arch(file);
	if(arch.is_open()){
		int numchan = 0;
		string conf, files;
		arch >> numchan;
		arch >> conf;
		arch >> files;
		try{
			test = SoundAdminPortaudio::getSoundAdminInstance();
			test->setConfiguration(SoundAdminPortaudio::SR_44100_KHZ, numchan, conf.c_str(),
					SoundAdminPortaudio::DISTRIBUTED);
			loadAudioFiles(files.c_str());
			arch.close();
			return true;
		}
		catch(string s){
			cout << s << endl;
			arch.close();
			return false;
		}
	}
	else{
		cout << "El archivo de configuración no se pudo abrir" << endl;
		return false;
	}
}

void loadAudioFiles(const char *srcs){
	ifstream soundfiles(srcs);
	if(soundfiles.is_open()){
		string line;
		while(!soundfiles.eof()){
			getline(soundfiles, line);
			if(line[0] != ';'){
				int pos = line.find_first_of(":");
				if(pos != (int)string::npos){
					string desc = line.substr(0, pos);
					string route = line.substr(pos+1);
					cout << desc << " " << route << endl;
					if(test->addFile(desc, route)){
						test->createSource(desc, desc);
						test->playSource(desc);
					}
				}
			}
			else{
				cout << "Comentario: " << line << endl;
			}
		}
		soundfiles.close();
	}
}

void moveSource(){
	vector<string> *vec = test->getSourceIds();
	float x, y, z;
	x = y = z = 0.0;
	
	int s = rand()%vec->size();
	string alias;
	int i = 0;
	for(; i < (int) vec->size() && i != s; i++);
	alias = vec->at(i);
	test->getSourcePosition(alias, x, y, z);
	cout << "Posición anterior: " << alias;
	cout << " (" << x << ", " << y << ", " << z << ")" << endl;
	test->updateSource(alias, srcPos[0], srcPos[1], srcPos[2]);
	test->getSourcePosition(alias, x, y, z);
	cout << "Posición anterior: " << alias;
	cout << " (" << x << ", " << y << ", " << z << ")" << endl;
}
#endif
