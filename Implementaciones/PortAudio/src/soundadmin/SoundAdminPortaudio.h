/**
 * @mainpage SoundAdminPortaudio API
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @brief Este documento contiene la referencia del API desarrollado como proyecto de grado
 * para Ingeniería de Sistemas y Computación, Universidad de los Andes, durante el segundo 
 * semestre de 2007.
 * */
#ifndef SOUNDADMINPORTAUDIO_H_
#define SOUNDADMINPORTAUDIO_H_

//inclusiones de la clase
#include <portaudio.h>
#include <map>
#include <vector>
#include <memory>
#include <gmtl/gmtl.h>
#include "../vbap/Vbap.h"
#include "Listener.h"
#include "Source.h"
#include "AudioFileLoader.h"

//define para pruebas sinusoidales
//#define TABLE_SIZE   (400)
//#ifndef M_PI
//#define M_PI  (3.14159265)
//#endif

using namespace gmtl;
using namespace std; 


/**
 * @class SoundAdminPortaudio
 * @brief Clase principal del sistema. Se encarga de la inicialización del sistema,
 * la creación y manejo de fuentes y de la escucha y la comunicación con el API de salida
 * de audio (Portaudio), así como con los archivos.
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @file SoundAdminPortaudio.h 
 * */
class SoundAdminPortaudio
{
public:
	/**
	 * Describe los sample rate a ser usados por el sistema
	 * */
	enum SampleRate {
		SR_8000_HZ, 
		SR_11025_HZ,
		SR_16000_HZ,
		SR_22050_HZ, 
		SR_44100_HZ, 
		SR_48000_HZ,
		SR_96000_HZ
	};

	/**
	 * Valores que indican cómo va a usarse el sonido en el sistema
	 * */
	enum SoundDistribution {MONO, STEREO, DISTRIBUTED};

private: 

	/**
	 * Apuntador estático que define la instancia única 
	 * */
	static auto_ptr<SoundAdminPortaudio> singletonInstance;

	/**
	 * Función estática de callback usada por Portaudio para el stream
	 * */
	friend int portaudioCallback(const void *inputBuffer, void *outputBuffer,
			unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void *userData );

	/**
	 * Flujo Portaudio a través del cuál saldrán los datos
	 * */
	PaStream *stream;

	/**
	 * Escucha de la clase
	 * */
	Listener *listener;

	/**
	 * Mapa con las fuentes disponibles en el sistema
	 * */
	map<string, Source*> sources;

	/**
	 * Número de salidas del sistema
	 * */
	int numspeakers;

	/**
	 * Objeto que controla la espacialización del sistema (para más de 2 speakers)
	 * */
	Vbap *panning_algorithm;

	/**
	 * Indica cómo se va a usar el sonido en el espacio
	 * */
	SoundDistribution distribution;

	/**
	 * Tabla que genera ondas (usada para pruebas)
	 */
	//	float sine[TABLE_SIZE];

	/**
	 * Utilidad para la carga y lectura de archivos 
	 * */
	AudioFileLoader *loader;

	//Funciones privadas

	/**
	 * Detiene todas las fuentes en el sistema
	 * */
	void stopAllSources();

	/**
	 * Método que devuelve las samplerates posibles para un dispositivo
	 * @param deviceNumber Número del dispositivo a consultar
	 * @return Vector con los valores disponibles.
	 */
	vector<int>* getSupportedSampleRates(int deviceNumber);

	/**
	 * retorna los dispositivos disponibles para usar en el sistema
	 * @param numChannels Número de canales de salida para el sistema
	 * @param samplerate SampleRate a usar en el sistema 
	 * */
	vector<int>* availableDevices(int numChannels, int samplerate);

	/**
	 * Inicializa los valores de la tabla de senos
	 */
	//	void initializeSineTable();

	/**
	 * Inicia la reproducción del flujo portaudio
	 * @throw string En caso que no se pueda reproducir correctamente
	 * */
	void startReproduction() throw (string);

	/**
	 * Método de prueba usado para trabajar con ondas sinusoidales
	 * */
	//	void workWithSinusoidalSources(float **output, int framesPerBuffer);

	/**
	 * Constructor de la clase
	 * */
	SoundAdminPortaudio();
	
	//Funciones de inicialización del sistema

	/**
	 * Cierra el administrador de acuerdo al API que subyace en la 
	 * implementación.
	 * @throws string en caso que no se pueda finalizar el API
	 * */
	void closeAdministrator() throw (string);
	
	/**
	 * Se encarga de inicializar el administrador de acuerdo al API
	 * @throws string Mensaje en caso que no se pueda inicializar el API 
	 * */
	void initializeAdministrator() throw (string);

public:

	/**
	 * Destructor de la clase
	 * */
	virtual ~SoundAdminPortaudio();

	/**
	 * Función que retorna la instancia única (Singleton)
	 * @return Única instancia del sistema para 
	 * */
	static SoundAdminPortaudio* getSoundAdminInstance();	

	/**
	 * Se encarga de configurar el sistema de acuerdo a lo que se va a usar
	 * @param sr Sample Rate del programa
	 * @param numSpeak Número de parlantes a usar \f$(numSpeak \geq 2)\f$ 
	 * @param configFile Archivo de configuración a usar con los parlantes
	 * @param distr Configuración de salida del sonido
	 * @note Si \f$numSpeak = 2\f$ la salida será <i>Stereo</i>, aún si <i>distr</i> es Distributed.
	 * Ésto no afecta la distribución Mono
	 * @throws string Mensaje en caso que el archivo de configuración no exista
	 * @throws string Mensaje en caso que no se pueda crear correctamente el sistema.
	 * Esto implica lo siguiente:<dl>
	 * <dt>\f$\to\f$ No existe un dispositivo capaz de manejar el sample rate solicitado y/o
	 * el número de canales solicitado
	 * <dt>\f$\to\f$ El archivo de configuración es incorrecto
	 * </dl>
	 * */
	void setConfiguration(SampleRate sr, int numSpeak, const char *configFile, 
			SoundDistribution distr) throw (string);


	//Creación y manejo de fuentes
	/**
	 * Crea una fuente puntual en el origen asociada al archivo indicado por
	 * fileDesc. La fuente se asocia a una descripción del archivo asociada
	 * desde la carga
	 * @param id Identificador de la nueva fuente
	 * @param fileDesc Descripción del identificador
	 * @param amb bool que indica si la fuente es ambiente o no
	 * */
	void createSource(string id, string fileDesc = "", bool amb = false);

	/**
	 * Destruye la fuente. En este caso la separa del objeto audio
	 * con el que esté asociada y la elimina
	 * @param id Identificador de la fuente a eliminar
	 * @return false si no hay una fuente con ese id o no se pudo eliminar  
	 * */
	bool destroySource(string id);

	/**
	 * Indica que la fuente con id <b>id</b> debe empezar a sonar
	 * Si la fuente está sonando o no existe, el comando no hace nada
	 * @param id Identificador de la fuente que se quiere hacer sonar
	 * @param loop Indica si el sonido debe ser reproducido constantemente
	 * */
	void playSource(string id, bool loop = true);

	/**
	 * Consulta si la fuente está detenida o no
	 * @param id Identificador de la fuente a consultar
	 * @return true si la fuente está detenida, false de lo contrario 
	 * */
	bool isPlaying(string id);

	/**
	 * Detiene la reproducción de una fuente en el momento actual
	 * @param id Identificador de la fuente a pausar.
	 * */
	void pauseSource(string id);

	/**
	 * Detiene la reproducción de una fuente. En el momento que se decida
	 * resumir la reproducción la fuente empieza desde el principio 
	 * @param id Identificador de la fuente a detener
	 * */
	void stopSource(string id);

	/**
	 * Cambia la reproducción de la fuente a un loop
	 * @param id Identificador de la fuente a cambiar
	 * @param loop Booleano que indica si la fuente se reproduce en ciclo (loop) o no
	 * */
	void loopSource(string id, bool loop);
	
	/**
	 * Consulta si la fuente está reproduciendose en ciclo o no
	 * @param id Identificador de la fuente a consultar
	 * @return true si la fuente está en ciclo, false de lo contrario 
	 * */
	bool isLooped(string id);
	
	/**
	 * Libera la fuente del archivo fuente que está usando en el momento
	 * como fuente sonora <br>
	 * post: La fuente es detenida y asociada a un archivo
	 * @param id identificador de la fuente 
	 * */
	void detachSource(string id);

	/**
	 * reasigna a la fuente el archivo señalado por <i>desc</i><br>
	 * post: La fuente es detenida y asociada a un archivo.
	 * @note No hace nada si el archivo no existe
	 * @param id Identificador de la fuente
	 * @param desc Ruta al archivo a hacer el attachment 
	 * */
	bool attachSource(string id, string desc);

	/**
	 * Asocia una fuente a la escucha de manera que se mueve con ella
	 * y tiene la misma amplitud en todos los puntos.
	 * @param id Identificador de la fuente a asociar
	 * @param ambient Indica si la fuente es ambiente o no
	 * */
	void useSourceAsAmbient(string id, bool ambient);

	/**
	 * Permite consultar si una fuente es ambiente o no
	 * @param id Identificador de la fuente a consultar
	 * @return true si la fuente es ambiente o false si no o no existe 
	 * */
	bool isAmbient(string id);

	/**
	 * Actualiza la ubicación de una fuente en el espacio. Si la fuente no
	 * existe no hace nada
	 * @param id identificador de la fuente
	 * @param x Posición nueva de la fuente en x
	 * @param y Posición nueva de la fuente en y
	 * @param z Posición nueva de la fuente en z
	 * */
	void updateSource(string id, float x, float y, float z);
	
	/**
	 * Obtiene la ubicación de la fuente en el espacio. 
	 * @param id identificador de la fuente
	 * @param x Posición de la fuente en x
	 * @param y Posición de la fuente en y
	 * @param z Posición de la fuente en z
	 * @return true si la fuente existe
	 * */
	bool getSourcePosition(string id, float& x, float& y, float& z);

	// Movimiento del Listener (Escucha) en el espacio

	/**
	 * Actualiza la ubicación del listener en el espacio
	 * @param pos Posición de la fuente en el espacio
	 * @param up Orientación: Hacia dónde está mirando la escucha 
	 * */
	void updateListener(Vec3f pos, Vec3f up);
	
	/**
	 * Permite obtener la información de la escucha
	 * @param pos Referencia en la cual se guardará el valor de la posición
	 * @param up Referencia en la cual se guardará el valor de la orientación 
	 * */	
	void getListenerInfo(Vec3f &pos, Vec3f &up);

	//Actualizaciones adicionales de las fuentes

	/**
	 * Cambia la altura (pitch) de la fuente \n
	 * TODO Todavía no tiene uso adecuado
	 * @param id Identificador de la fuente
	 * @param bend cantidad en la que se va a hacer el cambio de altura \f$(0.0 < bend < 2.0)\f$
	 * */
	void setPitchBend(string id, float bend);

	/**
	 * Obtiene la altura (pitch) de la fuente
	 * @param id Identificador de la fuente
	 * @param bend Referencia en la que va a quedar la altura actual de la fuente
	 * @return true si la fuente existe
	 * */
	bool getPitchBend(string id, float& bend);
	
	/**
	 * Cambia el nivel de la fuente
	 * Aplica sólamente para fuentes ambiente
	 * @param id identificador de la fuente
	 * @param gain Nueva ganancia de la fuente \f$(0.0 \leq gain \leq 1.0)\f$
	 * */
	void setVolume(string id, float gain);
	
	/**
	 * Obtiene el nivel (volumen) de la fuente
	 * @param id identificador de la fuente
	 * @param gain Referencia en la que va a quedar la ganancia actual de la fuente
	 * @return true si la fuente existe
	 * */
	bool getVolume(string id, float& gain);

	//cambio de coordenadas de una fuente respecto al listener

	/**
	 * Cambia las coordenadas de una fuente para que sean relativas al listener
	 * @param src Vector a cambiar
	 * @return Nuevas coordenadas de la fuente relativas a la escucha 
	 * */
	//	Vec3f changeCoordinatesFromListener(Vec3f src);

	//manejo de archivos

	/**
	 * Se encarga de agregar un archivo para usarse en reproducción
	 * @param desc Descripción a la que va a estar asociado el archivo
	 * @param fileRoute Ruta del archivo
	 * @note El archivo no se agrega si ya existe otro con la misma descripción
	 * */
	bool addFile(string desc, string fileRoute);
	
	/**
	 * Se encarga de borrar un archivo y eliminar las referencias a éste desde las fuentes 
	 * @param desc Descripción de la fuente del archivo
	 * @return true si el archivo se pudo borrar exitosamente
	 * @return false si el archivo no se pudo borrar
	 * @return true si el archivo no existe
	 * */
	bool deleteFile(string desc);
	

	//información de debug y manejo de descripciones

	/**
	 * Retorna los id de todas las fuentes actualmente en el sistema
	 * @return vector con todos los id de las fuentes. Puede estar vacío en caso de no haber
	 * 	fuentes creadas todavía
	 * */
	vector<string>* getSourceIds();

	/**
	 * Método que retorna las descripciones de los archivos cargados
	 * @return Lista con las descirpciones de los archivos. Puede ser una lista vacía en caso de no habrse cargado nada
	 * */
	vector<string>* getFileDescriptions();

	//manejo de Portaudio

	/**
	 * Función encargada de la reproducción de las fuentes sonoras.
	 * @param outputBuffer Buffer de salida usado por PortAudio
	 * @param framesPerBuffer frames de salida del sistema
	 * @return paComplete al terminar la función
	 */
	int soundAdminCallback(void *outputBuffer, unsigned long framesPerBuffer);
	
	/**
	 * @return Carga de CPU del sistema actual
	 * */
	double getCpuLoad(){
		if(stream)
			return Pa_GetStreamCpuLoad(stream);
		return 0.0;
	}

};

#endif /*SOUNDADMINPORTAUDIO_H_*/
