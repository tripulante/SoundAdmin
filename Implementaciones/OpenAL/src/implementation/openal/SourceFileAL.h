#ifndef SOURCEFILEAL_H_
#define SOURCEFILEAL_H_

/*inclusiones del sistema*/
#include <string>
#include <iostream>
#include <AL/al.h>
#include <AL/alut.h>

/*inclusiones del proyecto*/

/*namespace*/
using namespace std;

/**
 * @class SourceFileAL
 * @brief Clase que define un archivo en OpenAL
 * @author John Jairo Palma Robayo
 * @file SourceFileAL.h
 * */
class SourceFileAL
{
private:
	
	/**
	 * Descripción del archivo
	 * */
	string description;
	
	/**
	 * Buffer generado a partir de la carga del archivo
	 * */
	ALuint buffer;
public:
	
	/**
	 * Constructor de la clase. Construye una nueva clase a partir
	 * de un buffer para OpenAL y su descripción
	 * <pre>: buf != NULL, desc != "" </pre>
	 * @param desc descripción asociada al archivo
	 * @param buf Buffer OpenAL asociado al archivo
	 * */
	SourceFileAL(string desc, ALuint buf);
	
	/**
	 * Destructor de la clase. Elimina la descripción de la clase y elimina el 
	 * buffer asociado a la clase.
	 * <pre>: El buffer no está asociado a ninguna fuente. </pre>
	 * */
	virtual ~SourceFileAL();
	
	/**
	 * @return buffer AL generado para este archivo
	 * */
	ALuint getBuffer();
	
	/**
	 * Cambia el buffer asignado para este archivo
	 * @param buf Nuevo buffer AL designado para este archivo
	 * */
	void setBuffer(ALuint buf);
	
	/**
	 * @return descripción asociada a este archivo
	 * */
	string getDescription();
	
	/**
	 * Cambia la descripción del archivo
	 * @param nuDesc Nueva descripción asociada a este archivo
	 * */
	void setDescription(string nuDesc);
};

#endif /*SOURCEFILEAL_H_*/
