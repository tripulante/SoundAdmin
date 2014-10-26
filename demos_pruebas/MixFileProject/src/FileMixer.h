/**
 * @mainpage File Mixer for Portaudio
 * @brief Proyecto de prueba que permite mezclar dos archivos en una misma salida 
 * usando Portaudio
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * */

#ifndef FILEMIXER_H_
#define FILEMIXER_H_

#include <portaudio.h>
#include <sndfile.hh>
#include <iostream>
#include <map>
#include <vector>
#include "Source.h"

using namespace std;

/**
 * @class FileMixer
 * @brief Clase encargada de mezclar las señales de archivos leídos usando libsndfile en 
 * una salida portaudio
 * @author John Jairo Palma Robayo
 * @file FileMixer.h
 * */
class FileMixer
{

private: 

	/**
	 * Función de callback Portaudio
	 * */
	friend int fileCallback(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags, void *userData );

	/**
	 * Fuentes del sistema 
	 * */
	map<string, Source*> sources;

	/**
	 * Flujo PortAudio a usar
	 * */
	PaStream *stream;

	/**
	 * @param deviceNumber Número de dispositivo a consultar
	 * @return vector con las sample rates soportadas por ese dispositivo
	 * */
	vector<int>* getSampleRates(int deviceNumber);

	/**
	 * Inicializa Portaudio
	 * @throw string si hubo algún problema al inicializar Portaudio
	 * */
	void startPortaudio() throw (string);

	/**
	 * Número de canales de salida
	 * */
	int numchannels;
	
	/**
	 * toma el formato de un archivo libsndfile y lo devuelve como una cadena legible
	 * @param format Formato a consultar
	 * @return cadena legible con la información del formato 
	 * */
	string getFormatAsString(int format);

public:
	
	/**
	 * Constructor de la clase
	 * @param sampleRate Sample rate a usar en el dispositivo de salida
	 * @throw string si el sistema no puede ser soportado
	 * @throw string en caso de errores con PortAudio
	 * */
	FileMixer(int sampleRate) throw (string);
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~FileMixer();

	/**
	* Reproduce las fuentes (función privada de callback)
	* @param outputBuffer salida del sistema
	* @param framesPerBuffer número de datos a leer
	* @return paContinue
	*/
	int mixFunc(void *outputBuffer, unsigned long framesPerBuffer);

	/**
	 * Crea una fuente
	 * @param id Identificador de la fuente
	 * @param fileRoute ruta al archivo
	 * @note No hace nada si ya existe una fuente con ese id o el archivo no se pudo abrir
	 * */
	void createSource(string id, const char* fileRoute);

	/**
	 * Empieza a reproducir el sistema.
	 * @throw string si hay problemas con PortAudio
	 * */
	void startPlaying() throw (string);

	/**
	 * Cambia al azar la ganancia de una fuente en el sistema
	 * */
	void changeRandomGain();
};

#endif /*FILEMIXER_H_*/
