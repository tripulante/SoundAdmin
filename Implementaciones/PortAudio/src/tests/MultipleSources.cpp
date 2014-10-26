/**
 * @brief Prueba para ver cuantas fuentes puede soportar el sistema sobre un solo archivo
 * @author John Jairo Palma Robayo
 * @example MultipleSources.cpp
 * */

#include <iostream>
#include "../soundadmin/SoundAdminPortaudio.h"
#include "pruebas.h"
#include <portaudio.h>
#include <string>
#include <sstream>

using namespace std;

/**
 * Administrador del sistema de audio
 * */
SoundAdminPortaudio *admin;

/**
 * Crea y pone a funcionar un número de fuentes hasta que supere la tasa posible de CPU
 * */
void createAndActiveSources();

/**
 * Número de fuentes a probar
 * */
int numsources = 0;

/**
 * función principal
 * */
#if PRUEBA == 1
int main(int argc, const char *argv[]){

	if(argc != 6){
		cout << "Parámetros Ingresados Incorrectamente" << endl;
		cout << "Debe ingresar los siguientes parámetros: " << endl;
		cout << "./Uniandes3DAudio <numspeakers> <configfile> <samplerate> <audiofile> <no.fuentes>" << endl;
		return 0;
	}
	else{
		stringstream values(stringstream::in | stringstream::out);
		int chan = 0, samplerate = 0;
		values << argv[1] << " " << argv[3] << " " << argv[5];
		cout << values.str() << endl;
		values >> chan;
		values >> samplerate;
		values >> numsources;
		try{
			admin = SoundAdminPortaudio::getSoundAdminInstance();
			if(samplerate == 22050)
				admin->setConfiguration(SoundAdminPortaudio::SR_22050_HZ, chan, argv[2], SoundAdminPortaudio::MONO);
			else if(samplerate == 44100)
				admin->setConfiguration(SoundAdminPortaudio::SR_44100_HZ, chan, argv[2], SoundAdminPortaudio::MONO);
			else if(samplerate == 48000)
				admin->setConfiguration(SoundAdminPortaudio::SR_48000_HZ, chan, argv[2], SoundAdminPortaudio::MONO);
			else
				throw string("El sample rate solictado no es soportado\n");
			string route(argv[4]);
			if(!admin->addFile("testfile", route))
				throw string("Archivo no es válido\n");
			
			createAndActiveSources();
			char c;
			while(c != 'o')
				cin >> c;
			
			return 0;
		}
		catch(string s){
			cout << s << endl;
			return 0;
		}
	}
}
#endif

void createAndActiveSources(){
	stringstream s(stringstream::out|stringstream::app);
	int i = 0;
	bool end = false;
	
	while(!end && i < numsources){
		s.str("testsource");
		s << i;
		admin->createSource(s.str(), "testfile", true);
		admin->playSource(s.str());
		if(admin->getCpuLoad() > 0.8){
			cout << "Se crearon y reprodujeron " << i+1 << " fuentes" << endl;
			cout << "Carga CPU: " << admin->getCpuLoad() << endl;
			end = true;
		}
		i++;
	}
	
}
