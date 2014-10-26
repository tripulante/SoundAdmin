#ifndef OGGLOADER_H_
#define OGGLOADER_H_

#include <iostream>
#include <string>
/*Inclusiones de OGG*/
#include <ogg/ogg.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
/*openal*/
#include <AL/alut.h>
#include <AL/al.h>

using namespace std;
/**
 * @class OggLoader
 * @brief Clase que permite cargar un archivo .ogg y convertirlo a un buffer OpenAL.
 * @todo Optimizar la carga para que no sea tan lenta
 * @note Basado en un artículo previo desarrollado en www.gamedev.net por Anthony Yuen
 * @author John Jairo Palma Robayo
 * @file OggLoader.h
 * */
class OggLoader
{
public:
	
	/**
	 * Constructor de la clase.
	 * */
	OggLoader();
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~OggLoader();
	
	
	/**
	 * Método que carga un archivo .ogg y lo convierte a un buffer OpenAL
	 * @param path Cadena con el archivo
	 * @return buffer del archivo en OpenAL
	 * @throws Cadenas con excepción derivada de la carga del archivo o errores en OpenAL  
	 * */
	void loadFile(string path) throw (string);
	
	/**
	 * Método privado que se encarga de la carga del archivo
	 * <pre> El archivo ya fue cargado </pre>
	 * @return Buffer generado
	 * @throws Cadena con el error en caso de haber sido cargado 
	 * */
	bool loadFileOntoBuffer(ALuint buffer) throw (string);
	

private:
	/**
	 * Imprime la infomación del archivo vorbis
	 * <pre> El archivo ya fue cargado </pre>
	 * */
	void printFileInfo();
	
	/**
	 * Busca si hay problemas con OpenAL
	 * */
	void error() throw (string);
	
	/**
	 * Información del archivo
	 * */
	vorbis_info *info;
	
	/**
	 * Formato de la información AL
	 * */
	ALenum format;
	
	/**
	 * Archivo Vorbis
	 * */
	OggVorbis_File file;	
	
};

#endif /*OGGLOADER_H_*/
