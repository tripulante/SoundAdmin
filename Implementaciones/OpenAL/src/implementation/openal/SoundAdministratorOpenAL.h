#ifndef SOUNDADMINISTRATOROPENAL_H_
#define SOUNDADMINISTRATOROPENAL_H_

/*inclusiones del sistema*/
#include <iostream>
#include <list>
#include <string>
#include <AL/alut.h>
#include <AL/al.h>
#include <AL/alc.h>

/*inclusiones del proyecto*/
#include "../../interface/ISoundAdministrator.h"
#include "../../interface/Source.h"
#include "SourceFileAL.h"
#include "SoundLoaderAL.h"
#include "SourceStructAL.h"

/*namespaces*/
using namespace std;

/**
 * @class SoundAdministratorOpenAL
 * @brief Clase que se encarga de administrar las fuentes generadas mediante
 * OpenAL
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @file SoundAdministratorOpenAL.h
 * */
class SoundAdministratorOpenAL : public ISoundAdministrator
{

private:

	/**
	 * Lista de fuentes activas en el sistema
	 * */
	list<SourceStructAL*> active;

	/**
	 * Cargador de archivos
	 * */
	SoundLoaderAL* loader;
		
	/**
	 * Obtiene una estructura entre las fuentes activas.
	 * @param id Identificador de la fuente a buscar
	 * @return Pointer a la fuente obtenida.NULL si no existe.
	 * */
	SourceStructAL* getSourceActive(string id);
	
	/**
	 * Obtiene una estructura entre las fuentes ambientes.
	 * @param id Identificador de la fuente a buscar
	 * @return Pointer a la fuente obtenida.NULL si no existe.
	 * */
	SourceStructAL* getAmbientSource(string id);
	
	/**
	 * Contexto ALC para iniclaizar OpenAL
	 * */
	ALCcontext *context;
	
	/**
	 * Device ALC para que OpenAL pueda trabajar
	 * */
	ALCdevice *device;

public:

	/**
	 * Constructor de la clase
	 * */
	SoundAdministratorOpenAL();

	/**
	 * Destructor de la clase
	 * */
	virtual ~SoundAdministratorOpenAL();

	//Mtodos implementados de la clase heredada

	/**
	 * Se encarga de inicializar el administrador de acuerdo al API
	 * @throws string en caso que no se pueda inicalizar el API 
	 * */
	void initializeAdministrator() throw (string);

	/**
	 * Cierra el administrador de acuerdo al API que subyace en la 
	 * implementación.
	 * @throws string en caso que no se pueda inicalizar el API
	 * */
	void closeAdministrator() throw (string);

	/**
	 * Se encarga de configurar el sistema de acuerdo a lo que se va a usar
	 * @param sr Sample Rate del programa
	 * @param br Bitrate (Tamaño de palabra) a usar
	 * @param numSpeakers Número de parlantes a usar
	 * */
	//  void setConfiguration(SampleRate sr, BitRate br, int numParlantes)  ;

	//Creación, asignación y destrucción de fuentes

	/**
	 * Crea una fuente puntual en el origen asociada al archivo indicado por
	 * fileDesc. La fuente se asocia a una descripción del archivo asociada
	 * desde la carga
	 * @param id Identificador de la nueva fuente
	 * @param fileDesc Descripción del identificador
	 * @param amb bool que indica si la fuente es ambiente o no
	 * @return true Si la fuente pudo ser creada exitosamente
	 * @return false Si la fuente no pudo ser creada
	 * @throw string si el archivo descrito por fileDesc no existe
	 * @throw string si ya existe una fuente con el id solicitado
	 * */
	bool createSource(string id, string fileDesc, bool amb) throw (string);

	//reproducción de fuentes

	/**
	 * Destruye la fuente. En este caso la separa del objeto audio
	 * con el que esté asociada y la elimina
	 * @param id Identificador de la fuente a eliminar
	 * @throw string si la fuente no existe o no se pudo eliminar
	 * */
	void destroySource(string id) throw (string);

	/**
	 * Indica que la fuente con id <b>id</b> debe empezar a sonar
	 * Si la fuente está sonando o no existe, el comando no hace nada
	 * @param id Identificador de la fuente que se quiere hacer sonar
	 * @param loop Indica si el sonido debe ser reproducido constantemente
	 * */
	void playSource(string id, bool loop);

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
	 * Libera la fuente del archivo fuente que está usando en el momento
	 * como fuente sonora
	 * @param id identificador de la fuente 
	 * */
	void detachSource(string id);

	/**
	 * reasigna a la fuente el archivo señalado por desc
	 * @param id Identificador de la fuente
	 * @param desc Descripción del archivo a asociar
	 * */
	bool attachSource(string id, string desc);

	/**
	 * reproduce todas las fuentes en el sistema
	 * */
	void playAllSources();

	/**
	 * Pausa todas las fuentes activas en el sistema
	 * */
	void pauseAllSources();

	/**
	 * Detiene todas las fuentes en el sistema
	 * */
	void stopAllSources();

	/**
	 * Retorna una lista con los nombres de las fuentes en el sistema
	 * @return Lista con nombres de fuentes
	 * */
	list<string>* getSourceIds();

	/**
	 * retorna una lista con los nombres de las fuentes en reproducción
	 * @return Lista de nombres de fuentes activas
	 * */
	list<string>* getActiveSources();

	/**
	 * Destruye todas las fuentes
	 * */
	void destroyAllSources( );

	/**
	 * indica si existe un archivo con una descripción dada
	 * @param id identificador de la fuente
	 * @return true si existe una fuente con nombre <b>id</b>. false si no  
	 * */
	bool existsSource(string id);

	//Carga de archivos

	/**
	 * @return lista con las descrpiciones de los archivos del sistema
	 * */
	list<string>* getFileDescriptions(); 

	/**
	 * Carga los archivos indicados en un archivo junto con su
	 * descripción.
	 * Formato del archivo:
	 * <descripción del archivo>:<ruta_archivo>
	 * ;<comentario>
	 * @param dataFile Archivo con las rutas a los archivos de audio y su descripción
	 * @throw string el archivo no se pudo abrir o no existe
	 * @throw string el archivo tiene un formato incorrecto
	 * */
	//void loadFiles(string dataFile) throw (string);
	
	/**
	 * Carga un archivo individual.
	 * @param description descripción del archivo
	 * @param path Ruta al archivo
	 * @throw string Mensaje de error si el archivo no pudo ser cargado.
	 * */
	void loadFile(string description, string path) throw (string);

	/**
	 * Borra todos los archivos cargados
	 * */
	void cleanFiles();

	/**
	 * indica si existe un archivo con una descripción dada
	 * @param desc Descripción del archivo
	 * @return true si existe un archivo con descripción <b>desc</b>. false si no  
	 * */
	bool fileExists(string desc);

	//movimiento de fuentes 
	/**
	 * Actualiza la ubicación de una fuente en el espacio
	 * @param id identificador de la fuente
	 * @param x Posición nueva de la fuente en x
	 * @param y Posición nueva de la fuente en y
	 * @param z Posición nueva de la fuente en z
	 * */
	void updateSource(string id, float x, float y, float z);

	//sonido ambiente, relacionado con el listener (escucha)

	/**
	 * Asocia una fuente a la escucha de manera que se mueve con ella
	 * y tiene la misma amplitud en todos los puntos.
	 * @param id Identificador de la fuente a asociar
	 * @param ambient Indica si la fuente es ambiente o no
	 * @throws string si la fuente no existe o ya fue asociada al listener
	 * */
	void useSourceAsAmbient(string id, bool ambient) throw (string);

	/**
	 * Permite consultar si una fuente es ambiente o no
	 * @param id Identificador de la fuente a consultar
	 * @return true si la fuente es ambiente o false si no o no existe 
	 * */
	bool isAmbient(string id);
	
	/**
	 * Actualiza la ubicación del listener en el espacio
	 * @param x Posición nueva del listener en x
	 * @param y Posición nueva del listener en y
	 * @param z Posición nueva del listener en z
	 * */
	void updateListener(float x, float y, float z);

	/**
	 * Separa al listener de todas las fuentes asociadas a él.
	 * */
	void cleanAmbientSources();
};

#endif /*SOUNDADMINISTRATOROPENAL_H_*/
