/**
 * @mainpage Sinusoidal Mix Test using PortAudio
 * @brief Previamente la prueba estaba enfocada en la reproducción de un archivo de audio.
 * Posteriormente se optó por hacer mezcla de múltiples ondas sinusoidales y alterarlas 
 * en tiempo real.
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com) 
 * */

#ifndef SOUNDFILE_H_
#define SOUNDFILE_H_

#include <iostream>
/*Include portaudio*/
#include <portaudio.h>
#include <vector>
#include <map>
#include <string> 
#include <sndfile.hh>
#include <fstream>
#include <math.h>
#include <time.h>
#include "Source.h"
#include "pruebas.h"

/**
 * @def TABLE_SIZE
 * Tamaño de la tabla de senos para reproducir
 * */
#define TABLE_SIZE   (400)

/**
 * @def M_PI
 * Valor de Pi (en caso de no estar definido)
 * */
#ifndef M_PI
#define M_PI  (3.14159265)
#endif


using namespace std;


/**
* @class SoundFile 
* @brief Clase de prueba que permite usar múltiples fuentes y maneja portaudio
* @author John Jairo Palma Robayo
* @file SoundFile.h
*/
class SoundFile
{
	
private:
	/**
	* Archivo de audio a reproducir
	*/
	SndfileHandle* sndfile;
	
	/**
	* Flujo PortAudio
	*/	
	PaStream *stream;

	/**
	* Método que devuelve las samplerates posibles para un dispositivo
	* @param deviceNumber Número del dispositivo a consultar
	* @return Vector con los valores disponibles.
	*/
	vector<int>* getSampleRates(int deviceNumber);
	
	/**
	* Posición de lectura del archivo
	*/
	int position;

	/**
	* Tabla que genera ondas 
	*/
	float sine[TABLE_SIZE];

	/**
	* Número de canales del sistema
	*/
	int numchannels;
	
	int selectedChannel;

	/**
	 * Función de callback PortAudio
	 * */
	friend int fileCallback(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags, void *userData );
	
	/**
	* Inicializa los valores de la tabla de senos
	*/
	void initializeSineTable();

	/**
	* Mapa con las fuentes 
	*/
	map<string, Source*> sources;
	
public:
	
	/**
	* Constructor de la clase 
	* @param filename Nombre del archivo a usar
	* @param numchan Número de canales de salida del sistema
	*/
	SoundFile(const char* filename, int numchan);
	
	/**
	* Destructor de la clase
	*/
	virtual ~SoundFile();
	
	/**
	* Inicializa Portaudio
	* @return true si se pudo incializar correctamente, false si no
	*/
	bool startPortaudio();
	
	/**
	* Imprime la información de los dispositivos para que el usuario decida
	*/
	void printDeviceInfo();
	
	/**
	* Imprime la información de los APIs disponibles
	*/
	void printApiInfo();
	
	/**
	* Inicializa el stream portaudio
	* @param device Número del dispositivo a usar
	* @return false si no se pudo inicalizar el stream
	*/
	bool initialize(int device);

	/**
	* Pone a reproducir el archivo
	* @return false si no se pudo reproducir
	*/
	bool startPlaying();

	/**
	* Detiene la reproducción y cierra el stream
	* @return false si no se pudo detener/cerrar
	*/
	bool stopPlaying();

	/**
	* Finaliza portaudio
	*/
	void finishPortaudio();
	
	/**
	* Agrega una fuente
	* @param id Nombre de la fuente
	* @return false si no se pudo agregar (ya existía una fuente con ese id)
	*/
	bool addSource(string id);
	
	/**
	* Reproduce las fuentes (función privada de callback)
	* @param outputBuffer salida del sistema
	* @param framesPerBuffer número de datos a leer
	* @return paContinue
	*/
	int readFile(void *outputBuffer, unsigned long framesPerBuffer);
	
	/**
	* Cambia la fase de una fuente al azar
	*/
	void changeRandomPhase();
	
	/**
	* Cambia la ganancia de una fuente al azar
	*/
	void changeRandomGain();
	
	//TODO!
	void changeChannel();
	
};

#endif /*SOUNDFILE_H_*/
