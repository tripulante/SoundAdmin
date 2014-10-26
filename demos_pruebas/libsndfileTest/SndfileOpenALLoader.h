/**
 * @mainpage File Loader for OpenAL using Libsndfile
 * @brief Librería que se encarga de cargar archivos compatibles con libsndfile y los
 * convierte a buffers OpenAL
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * */

#ifndef SNDFILEOPENALLOADER_H_
#define SNDFILEOPENALLOADER_H_


#include <iostream>
#include <fstream>
#include <string>
/*includes libsndfile*/
#include <sndfile.hh>
/*Include openal*/
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>


using namespace std;
/**
 * @def BUFFER_SIZE
 * Tamaño del buffer a usar
 * */
#define BUFFER_SIZE (4096)

/**
 * @class SndfileOpenALLoader
 * @brief Clase que se encarga de cargar un archivo usando libsndfile y convertirlo
 * a un buffer OpenAL para uso
 * @todo Corregir errores presentes en la carga que evitan su uso adecuado.
 * @todo Corregir la carga del archivo. Está cambiando de acuerdo a el sample rate y bitrate
 * del archivo 
 * @author John Jairo Palma Robayo
 * @file SndfileOpenALLoader.h
 * */
class SndfileOpenALLoader
{
public:
	/**
	 * Constructor de la clase
	 * */
	SndfileOpenALLoader();
	
	/**
	 * Destructor de la clase 
	 * */
	virtual ~SndfileOpenALLoader();
	
	/**
	 * Método que se encarga de cargar el archivo
	 * @param file Ruta del archivo a abrir
	 * @return Nombre del buffer creado. AL_NONE si no se creó nada
	 * @throws string si el archivo no se pudo abrir
	 * @throws string si no se pudo crear buffer
	 * */
	void loadFile(string file) throw (string);
	
	/**
	 * Imprime la información de un archivo!
	 * */
	void printFileInfo();

	/**
	 * Método privado que permite crear el archivo copiando los datos básicos del archivo
	 * @return Nombre del buffer creado. AL_NONE si no se creó nada
	 * @throws string con la cadena de error
	 * */
	bool loadBufferFromFile(ALuint buffer) throw (string);
private:	
	/**
	 * Método privado que permite identificar que formato estamos usando para información
	 * @param format formato en entero de los datos
	 * @return cadena con el formato 
	 * */
	string getFormatAsString(int format);
	
	/**
	 * Método para consultar errores de OpenAL
	 * */
	void errorOpenAL() throw (string);
	
	/**
	 * Archivo de audio a usar.
	 * */
	SndfileHandle* handle;
	
	/**
	 * Formato del archivo de audio
	 * */
	ALenum format;
};

#endif /*SNDFILEOPENALLOADER_H_*/
