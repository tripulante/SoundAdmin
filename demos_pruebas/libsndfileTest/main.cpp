/**
 * @brief Prueba OpenAL que permite cargar un archivo usando libsndfile 
 * y lo usa en un buffer OpenAL
 * @author John Jairo Palma Robayo
 * @example main.cpp 
 * */


#include <iostream>
#include <string>
/*includes libsndfile*/
#include <sndfile.hh>
/*Include openal*/
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
#include "SndfileOpenALLoader.h"
using namespace std;

/**Fuente OpenAL*/
ALuint source;
/**Buffer OpenAL*/
ALuint buffer;
/**
 * contexto OpenAL
 * */
ALCcontext *context = NULL;
/**
 * Device OpenAL
 * */
ALCdevice *device = NULL;

/**
 * Clase que carga un archivo y lo convierte a Formato Buffer OpenAL
 * */
SndfileOpenALLoader* loader;

int main(int argc, char **argv){
	
	if(argc != 2){
		cout << "No se han ingresado los parámetros correctamente " << endl;
		cout << "libsndfileTest <NombreArchivo>" << endl;
		cout << argc << endl;
		return EXIT_FAILURE;
		
	}
	else
		for(int i = 0; i < argc; i++)
			cout << argv[i] << endl;
	
	try{
		device = alcOpenDevice(NULL);
		if(!device){
			cout << "No se pudo abrir el dispositivo" << endl;
			exit(EXIT_FAILURE);
		}
		alutInitWithoutContext(NULL, NULL);
		context = alcCreateContext(device, NULL);
		if(!context){
			cout << "No se pudo crear un contexto" << endl;
			cout << alGetString(alcGetError(device)) << endl;
			exit(EXIT_FAILURE);
		}
		alcMakeContextCurrent(context);
		/*if(!alutInit(NULL, NULL)){
			cout << "Error al iniclaizar OpenAL" << endl;
			return EXIT_FAILURE;
		}*/
		loader = new SndfileOpenALLoader();
		string str(argv[1]);
		loader->loadFile(str);
		alGenBuffers(1, &buffer);
		alGenSources(1, &source);
		if(!alIsSource(source)){
			cout << "No generó la fuente" << endl;
			return EXIT_FAILURE;
		}
		if(!loader->loadBufferFromFile(buffer))
			return false;
		alSourcei(source, AL_BUFFER, buffer);
		alSourcei(source, AL_LOOPING, AL_TRUE);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
		float pos[]= {0,0,-1};
		alSourcefv(source, AL_POSITION, pos);
		alSourcef(source, AL_GAIN, AL_MAX_GAIN);
		alSourcePlay(source);
		cout << "Presione una tecla para detener" << endl;
		char in;
		cin >> in;
		alSourceStop(source);
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
		alutExit();
		return EXIT_SUCCESS;
	}
	catch (string s){
		cout << s << endl;
		return EXIT_FAILURE;
	}
}

