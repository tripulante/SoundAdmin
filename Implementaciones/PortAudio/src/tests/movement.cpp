/**
 * @brief Mueve una fuente como una esfera, para escuchar el movimiento suave
 * @author John Jairo Palma Robayo
 * @example movement.cpp 
 * */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include "pruebas.h"
#include "../soundadmin/SoundAdminPortaudio.h"

#if PRUEBA == 2

using namespace std;

/**
 * Administrador que se va a probar
 * */
SoundAdminPortaudio *test2;

/**
 * Permite el movimiento de fuentes en el espacio
 * */
float srcPos[3] = {0};

/**
 * Velocidades
 * */
float speedx, speedy, speedz; 
/**
 * Nos movemos en un área determinada por el usuario
 * */
float lim = 0.0f;
/**
 * caracter de salida
 * */
static char c;

/**
 * Fuente que se va a mover
 * */
string alias;

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
void* moveSource(void *);

/**
 * Función principal
 * */
int main(int argc, const char *argv[]){
	if(argc != 3){
		cout << "Error al cargar el sistema!" << endl << "Debe usarse: " << endl;
		cout << "./Uniandes3DAudio <archivocarga>" << endl;
		return 0;
	}
	else{
		speedx = speedy = 0.00001;
		speedz = 0.00003;
		srand(time(NULL));
		if(!loadfile(argv[1])){
			cout << "Error al cargar los archivos" << endl;
			return 0;
		}
		stringstream s;
		s << argv[2];
		s >> lim;
		if(lim == 0.0f)
			lim = 2.0f;
		pthread_t id;
		pthread_create(&id, NULL, moveSource, 0);
		while(c != 'o'){
			cin >> c;
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
			test2 = SoundAdminPortaudio::getSoundAdminInstance();
			test2->setConfiguration(SoundAdminPortaudio::SR_44100_KHZ, numchan, conf.c_str(),
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
					if(test2->addFile(desc, route)){
						test2->createSource(desc, desc);
						test2->playSource(desc);
						alias = desc;
					}
				}
			}
			else{
				cout << "comentario: " << line << endl;
			}
		}
		soundfiles.close();
	}
}

void * moveSource(void *){
	while(c != 'o'){
		float x, y, z;
		x = y = z = 0.0;
		if(srcPos[0] >= lim*2 || srcPos[0] <= -lim*2)
			speedx *= -1.0f;
		if(srcPos[1] >= lim || srcPos[1] <= -lim)
			speedy *= -1.0f;
		if(srcPos[2] >= lim || srcPos[2] <= -lim)
			speedz *= -1.0f;
		srcPos[0] += speedx;
		srcPos[1] += speedy;
		srcPos[2] += speedz;
		test2->getSourcePosition(alias, x, y, z);
		test2->updateSource(alias, srcPos[0], srcPos[1], srcPos[2]);
		test2->getSourcePosition(alias, x, y, z);
		cout << "Posición nueva: " << alias;
		cout << " (" << x << ", " << y << ", " << z << ")" << endl;
		for(int i = 0; i < 10000; i++);
	}
	return 0;
}
#endif
