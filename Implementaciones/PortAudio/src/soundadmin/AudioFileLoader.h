#ifndef AUDIOFILELOADER_H_
#define AUDIOFILELOADER_H_

#include <string>
#include <sndfile.hh>
#include <iostream>
#include <vorbis/vorbisfile.h>
#include <map>
#include <vector>
#include <memory>

using namespace std;


/**
 * @class AudioFileLoader
 * @brief Se encarga de la carga de archivos y su lectura
 * @author John Jairo Palma Robayo
 * @file AudioFileLoader.h
 * */
class AudioFileLoader
{

private:
	
	/**
	 * Mapa con los archivos en formato legible por libsndfile
	 * */
	map<string, SndfileHandle*> libsndfiles;
	
	/**
	 * Archivos ogg vorbis
	 * */
	map<string, OggVorbis_File*> oggFiles;
	
	/**
	 * Lee los contenidos de un archivo ogg y los mete un un buffer
	 * @param buffer Buffer en el que se colocarán los contenidos leídos
	 * @param pos Posición a partir de la cual se leerá el archivo
	 * @param framesToRead Cantidad de datos a leer
	 * @param loop Booleano que indica si la fuente debe reproducirse en loop o no
	 * @return Cantidad de datos leídos efectivamente
	 * */
	int readFromOgg(OggVorbis_File *file, float *buffer, int *pos, int framesToRead, bool loop);
	
	/**
	 * Lee los contenidos de un archivo y los mete un un buffer
	 * @param buffer Buffer en el que se colocarán los contenidos leídos
	 * @param pos Posición a partir de la cual se leerá el archivo
	 * @param framesToRead Cantidad de datos a leer
	 * @param loop Booleano que indica si la fuente debe reproducirse en loop o no
	 * @return Cantidad de datos leídos efectivamente
	 * */
	int readFromPcm(SndfileHandle *file, float *buffer, int *pos, int framesToRead, bool loop);
	
	/**
	 * Imprime la información de un archivo ogg
	 * Información de debug sólamente
	 * */
	void printOggFileInfo(OggVorbis_File file);
	
	/**
	 * Imprime la información de un archivo soportado por libsndfile
	 * Información de debug sólamente
	 * */
	void printSndfileInfo(SndfileHandle handle);
	
	/**
	 * Método encargardo de iterar por todos los archivos y destruirlos
	 * @throw string Alguno de los buffers no pudo ser destruido
	 * */
	void destroyAllFiles() throw (string);
	
public:
	
	/**
	 * Constructor de la clase
	 * */
	AudioFileLoader();
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~AudioFileLoader();
	
	/**
	 * Método encargado de cargar un archivo.
	 * @note No se permiten descripciones con la cadena vacía ("");
	 * @param desc Descripción del archivo
	 * @param file Ruta al archivo.
	 * @throw string Si el archivo no se puede abrir.
	 * @throw string Si el archivo no se pudo cargar.
	 * @throw string Si la descripción es inválida ("");
	 * */
	void loadFile(string desc, string file) throw (string);
	
	/**
	 * Método encargado de borrar un archivo
	 * <pre> Existe un archivo con esa descripción </pre>
	 * @param desc Descrición del archivo a borrar
	 * @return true Si el archivo fue borrado exitosamente
	 * @return false Si no se pudo borrar el archivo
	 * */
	bool deleteFile(string desc);
	
	/**
	 * Lee los contenidos de un archivo y los mete un un buffer
	 * @param buffer Buffer en el que se colocarán los contenidos leídos
	 * @param desc Descripción del archivo
	 * @param pos Posición a partir de la cual se leerá el archivo
	 * @param framesToRead Cantidad de datos a leer
	 * @param loop Booleano que indica si la fuente debe reproducirse en loop o no
	 * @return Cantidad de datos leídos efectivamente
	 * */
	int readFromFile(float *buffer, string desc, int *pos, int framesToRead, bool loop);
	
	/**
	 * verifica que haya un archivo asociado a una descripción
	 * @param desc Descripción a verificar
	 * @return true si existe un archivo asociado a esa descipción
	 * */
	bool fileExists(string desc);
	
	/**
	 * Método que retorna las descripciones de los archivos cargados
	 * @return Lista con los archivos. Puede ser una lista vacía en caso de no haberse cargado nada
	 * */
	vector<string>* getFileDescriptions();
};

#endif /*AUDIOFILELOADER_H_*/
