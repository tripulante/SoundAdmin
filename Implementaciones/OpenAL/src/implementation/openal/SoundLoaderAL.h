#ifndef SOUNDLOADERAL_H_
#define SOUNDLOADERAL_H_

/*Inclusiones del sistema*/
#include <list>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <AL/alut.h>
#include <AL/al.h>
#include <sndfile.hh>

/*inclusiones del proyecto*/
#include "SourceFileAL.h"
#include "lib/OggLoader.h"

/*namespaces*/
using namespace std;

/*Definiciones*/

/**
 * @class SoundLoaderAL
 * @brief Clase que se encarga de la carga de archivos para OpenAL
 * Convierte los archivos conseguidos en buffers para el uso
 * @author John Jairo Palma Robayo
 * @file SoundLoaderAL.h
 * */
class SoundLoaderAL
{
private:
	/**
	 * Lista de archivos AL creados por el sistema
	 * */
	list<SourceFileAL*> files;
	
	/**
	 * Método auxiliar que se encarga de crear un archivo a partir de un wav.
	 * @param wavfile Nombre del archivo del que se quiere crear el buffer 
	 * @return AL_NONE si el archivo no se pudo abrir.
	 * */
	ALuint createBufferFromWav(string wavfile);
	
	/**
	 * Método auxiliar que permite crear un buffer AL a partir de un archivo OggVorbis
	 * @param oggfile Nombre del archivo del que se quiere crear el buffer
	 * @return AL_NONE si el buffer no se pudo crear o el archivo no se pudo abrir
	 * */
	ALuint createBufferFromOgg(string oggfile);
	
	/**
	 * Método auxiliar que usa libsndfile para cargar archivos con otras extensiones
	 * @param file Nombre del archivo que desea usar para el buffer
	 * @return AL_NONE si el buffer no se puedo crear o el archivo no se pudo abrir
	 * */
	ALuint createBufferFromOtherExtension(string file);
public:
	
	/**
	 * Constructor de la clase
	 * */
	SoundLoaderAL();
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~SoundLoaderAL();
	
	/**
	 * Método encargado de cargar los distintos archivos y convertirlos en buffers OpenAL
	 * @param file Ruta/Nombre del archivo del cual se cargarán todos
	 * @throw exception Si el archivo no se puede abrir  
	 * */
	//void loadFiles(string file) throw (string);
	
	/**
	 * Método encargado de cargar un archivo.
	 * @param description Descripción del archivo
	 * @param file Ruta al archivo.
	 * @throw string Si el archivo no se puede abrir.
	 * @throw string Si el archivo no se pudo cargar.
	 * */
	void loadFile(string description, string file) throw (string);
	
	/**
	 * Mtodo encargardo de iterar por todos los archivos y destruirlos
	 * <pre> los archivos no tienen ningn source asociado </pre>
	 * @throw exception Alguno de los buffers no pudo ser destruido
	 * */
	void destroyAllFiles() throw (string);
	
	/**
	 * Mtodo encargado de localizar un archivo dada su descripción
	 * @param desc descripcin del archivo
	 * @return NULL si el archivo no existe. Apuntador al objeti si est.
	 * */
	SourceFileAL* findFile(string desc);
	
	/**
	 * Método que retorna las descripciones de los archivos cargados
	 * @return Lista con los archivos. Puede ser una lista vacía en caso de no habrse cargado nada
	 * */
	list<string>* getFileDescriptions();
	
};

#endif /*SOUNDLOADERAL_H_*/
