#ifndef SOURCESTRUCTAL_H_
#define SOURCESTRUCTAL_H_

/*inclusiones del sistema*/
#include <string>
#include <AL/al.h>
#include <AL/alut.h>
#include <iostream>
#include <exception>

/*inclusiones del proyecto*/
#include "../../interface/Source.h"
#include "SourceFileAL.h"
/*namespace*/
using namespace std;

/**
 * @class SourceStructAL
 * @brief Clase que define una estructura que combina los archivos y las fuentes
 * @author John Jairo Palma Robayo
 * @file SourceStructAL
 * */
class SourceStructAL
{

private:
	
	/**
	 * Apuntador que define esta fuente para OpenAL
	 * */
	ALuint source;
	
	/**
	 * Archivo de audio asociado a esta fuente
	 * */
	SourceFileAL *file;
	
	/**
	 * Fuente en movimiento asociada a esta estructura
	 * */
	Source *src;
public:
	
	/**
	 * Constructor de la clase. Crea una nueva estructura AL para la reproduccin de la
	 * fuente
	 * @param f Archivo asociado a la fuente
	 * @param s fuente
	 * @throw exception Si no se puede crear la fuente OpenAL 
	 * */
	SourceStructAL(SourceFileAL *f, Source *s) throw (string);
	
	/**
	 * destructor de la clase
	 * */
	virtual ~SourceStructAL();
	
	/**
	 * Actualiza la posicin de la fuente
	 * @param x Nueva posicin en x de la fuente
	 * @param y Nueva posicin en y de la fuente
	 * @param z Nueva posicin en z de la fuente
	 * */
	void updateSource(float x, float y, float z);
	
	/**
	 * cambia el archivo asociado a la estructura
	 * @param f Nuevo archivo asociado a la estructura
	 * */
	void setFile(SourceFileAL* f);
	
	/**
	 * Cambia la fuente asociada a la estructura
	 * @param s Nueva fuente.
	 * */
	void setSource(Source* s);
	
	/**
	 * @return la fuente actual de la estructura
	 * */
	Source* getSource();
	
	/**
	 * @return Archivo actual asociado a la fuente
	 * */
	SourceFileAL* getFile();
	
	/**
	 * Pone a sonar la fuente. 
	 * <pre> La fuente es ubicada previamente de acuerdo a la posición del oyente si es ambiente </pre>
	 * @param loop indica si la fuente se reproduce constantemente
	 * */
	void playSource(bool loop);
	
	/**
	 * Detiene la fuente
	 * */
	void stopSource();
	
	/**
	 * Pausa la fuente
	 * */
	void pauseSource();
	
	/**
	 * Indica que la fuente debe ser ambiente o no
	 * @param ambient Booleano que indica si la fuente es (true) o no (false) ambiente
	 * */
	void setAsAmbient(bool ambient);
	
	/**
	 * Indica si la fuente actualmente es ambiente
	 * @return true si la fuente es ambiente, false si no
	 * */
	bool isAmbient();
};

#endif /*SOURCESTRUCTAL_H_*/
