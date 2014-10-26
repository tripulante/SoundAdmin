/**
 * @mainpage Aeon API...
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @brief Este documento contiene la referencia del API desarrollado como proyecto de grado
 * para Ingeniería de Sistemas y Computación, Universidad de los Andes, durante el segundo 
 * semestre de 2007, usando OpenAL como librería subyacente
 * */

#ifndef ISOUNDADMINISTRATOR_H_
#define ISOUNDADMINISTRATOR_H_
/*inclusiones del sistema*/
#include <string>
#include <list>
#include <exception>
/*namespaces*/
using namespace std;

/**
 * @class ISoundAdministrator
 * @brief Interfaz que define los métodos que han de ser implementados
 * por cada API posterior
 * @todo Tener un método que permita averiguar los archivos que no están 
 * atados a ninguna fuente/las fuentes que no tienen archivos asociados.
 * @todo Definir los métodos que permiten la implementación de múltiples cajas acústicas. 
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * @file ISoundAdministrator.h
 * */
class ISoundAdministrator
{
	
public:
	
	/**
	 * Enum para determinar el sample rate del aparato a usar
	 * */
	//enum SampleRate{44100_KHz, 48000_KHz};
	
	/**
	 * Enum para determinar el tamaño de palabra del sistema
	 * */
	//enum BitRate{24_BIT, 16_BIT};
	
	//funciones encargadas de inicialización del API.
	
	/**
	 * Se encarga de inicializar el administrador de acuerdo al API 
	 * @throws string en caso que no se pueda inicializar el API
	 * */
	virtual void initializeAdministrator() throw (string) = 0;
	
	/**
	 * Cierra el administrador de acuerdo al API que subyace en la 
	 * implementación.
	 * @throws string en caso que no se pueda cerrar el API
	 * */
	virtual void closeAdministrator() throw (string) = 0;
	
	/**
	 * Se encarga de configurar el sistema de acuerdo a lo que se va a usar
	 * @param sr Sample Rate del programa
	 * @param br Bitrate (Tamaño de palabra) a usar
	 * @param numSpeakers Número de parlantes a usar
	 * */
	//virtual void setConfiguration(SampleRate sr, BitRate br, int numParlantes) = 0;
	
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
	virtual bool createSource(string id, string fileDesc, bool amb) throw (string) = 0;
	
	//reproducción de fuentes
	
	/**
	 * Destruye la fuente. En este caso la separa del objeto audio
	 * con el que esté asociada y la elimina
	 * @param id Identificador de la fuente a eliminar 
	 * */
	virtual void destroySource(string id) throw (string)= 0;
	
	/**
	 * Indica que la fuente con id <b>id</b> debe empezar a sonar
	 * Si la fuente está sonando o no existe, el comando no hace nada
	 * @param id Identificador de la fuente que se quiere hacer sonar
	 * @param loop Indica si el sonido debe ser reproducido constantemente
	 * */
	virtual void playSource(string id, bool loop) = 0;
	
	/**
	 * Detiene la reproducción de una fuente en el momento actual
	 * @param id Identificador de la fuente a pausar.
	 * */
	virtual void pauseSource(string id) = 0;
	
	/**
	 * Detiene la reproducción de una fuente. En el momento que se decida
	 * resumir la reproducción la fuente empieza desde el principio 
	 * @param id Identificador de la fuente a detener
	 * */
	virtual void stopSource(string id) = 0;
	
	/**
	 * Libera la fuente del archivo fuente que está usando en el momento
	 * como fuente sonora
	 * @param id identificador de la fuente 
	 * */
	virtual void detachSource(string id) = 0;
	
	/**
	 * reasigna a la fuente el archivo señalado por desc
	 * @param id Identificador de la fuente
	 * @param desc Descripción de la fuente
	 * */
	virtual bool attachSource(string id, string desc) = 0;
	
	/**
	 * reproduce todas las fuentes en el sistema
	 * */
	virtual void playAllSources() = 0;

	/**
	 * Pausa todas las fuentes activas en el sistema
	 * */
	virtual void pauseAllSources() = 0;
	
	/**
	 * Detiene todas las fuentes en el sistema
	 * */
	virtual void stopAllSources() = 0;
	
	/**
	 * Retorna una lista con los nombres de las fuentes en el sistema
	 * @return Lista con nombres de fuentes
	 * */
	virtual list<string>* getSourceIds() = 0;
	
	/**
	 * retorna una lista con los nombres de las fuentes en reproducción
	 * @return Lista de nombres de fuentes activas
	 * */
	virtual list<string>* getActiveSources() = 0;
	
	/**
	 * Destruye todas las fuentes
	 * */
	virtual void destroyAllSources( ) = 0;
	
	/**
	 * indica si existe un archivo con una descripción dada
	 * @param id identificador de la fuente
	 * @return true si existe una fuente con nombre <b>id</b>. false si no  
	 * */
	virtual bool existsSource(string id) = 0;
	
	//Carga de archivos
	
	/**
	 * retorna una lista con las descripciones de todos los archivos
	 * @return lista de descripciones
	 * */
	virtual list<string>* getFileDescriptions() = 0; 
	
	/**
	 * Carga los archivos indicados en un archivo junto con su
	 * descripción.
	 * Formato del archivo:
	 * <descripción del archivo>:<ruta_archivo>
	 * ;<comentario>
	 * @param dataFile Archivo con las rutas a los archivos de audio y su descripción
	 * @throw SoundException el archivo no se pudo abrir o no existe
	 * @throw SoundException el archivo tiene un formato incorrecto
	 * */
	//virtual void loadFiles(string dataFile) throw (string)= 0;
	
	/**
	 * Carga un archivo individual.
	 * @param description descripción del archivo
	 * @param path Ruta al archivo
	 * @throw string Mensaje de error si el archivo no pudo ser cargado.
	 * */
	virtual void loadFile(string description, string path) = 0;
	
	/**
	 * Borra todos los archivos cargados
	 * */
	virtual void cleanFiles() = 0;
	
	/**
	 * indica si existe un archivo con una descripción dada
	 * @param desc Descripción del archivo
	 * @return true si existe un archivo con descripción <b>desc</b>. false si no  
	 * */
	virtual bool fileExists(string desc) = 0;
	
	//movimiento de fuentes 
	/**
	 * Actualiza la ubicación de una fuente en el espacio
	 * @param id identificador de la fuente
	 * @param x Posición nueva de la fuente en x
	 * @param y Posición nueva de la fuente en y
	 * @param z Posición nueva de la fuente en z
	 * */
	virtual void updateSource(string id, float x, float y, float z) = 0;
	
	//sonido ambiente, relacionado con el listener (escucha)
	
	/**
	 * Asocia una fuente a la escucha de manera que se mueve con ella
	 * y tiene la misma amplitud en todos los puntos.
	 * @param id Identificador de la fuente a asociar
	 * @param ambient Indica si la fuente es ambiente o no
	 * @throws string si la fuente no existe o ya fue asociada al listener
	 * */
	virtual void useSourceAsAmbient(string id, bool ambient) throw (string) = 0;
	
	/**
	 * Permite consultar si una fuente es ambiente o no
	 * @param id Identificador de la fuente a consultar
	 * @return true si la fuente es ambiente o false si no o no existe 
	 * */
	virtual bool isAmbient(string id) = 0;
	
	/**
	 * Actualiza la ubicación del listener en el espacio
	 * @param x Posición nueva del listener en x
	 * @param y Posición nueva del listener en y
	 * @param z Posición nueva del listener en z
	 * */
	virtual void updateListener(float x, float y, float z) = 0;
	
	/**
	 * Separa al listener de todas las fuentes asociadas a él.
	 * */
	virtual void cleanAmbientSources() = 0;
	
};

#endif /*ISOUNDADMINISTRATOR_H_*/
