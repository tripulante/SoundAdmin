#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>

using namespace std;
/**
 * @class Source
 * @brief Definición de una fuente estándar que sirva de referencia a las 
 * demás implementaciones del sistema.
 * @author John Jairo Palma Robayo
 * @file Source.h 
 * */
class Source{
	
protected:
	/**
	 * Posición en el espacio de la fuente
	 * pos[0] = x, pos[1] = y; pos[2] = z;
	 * */
	float pos[3];
	
	/**
	 * Indica si la fuente está actualmente en reproducción
	 * */
	bool playing;
	
	/**
	 * Identificador de la fuente
	 * */
	string id;
	
	/**
	 * Infica si la fuente es ambiente o no
	 * */
	bool ambient;

public:
	
	/**
	 * Constructor de la clase. La fuente empieza en el origen
	 * y no se está reproduciendo
	 * @param nuID Nueva Id que identifica a la fuente
	 * */
	Source(string nuID){
		id = nuID;
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = false;
		ambient = false;
	}
	
	/**
	 * Destructor de la fuente
	 * */
	virtual ~Source(){
		id = "";
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = false;
		ambient = false;
	}
	
	/**
	 * Actualiza la posición de la fuente
	 * @param x Nueva posición en x de la fuente
	 * @param y Nueva posición en y de la fuente
	 * @param z Nueva posición en z de la fuente
	 * */
	void update(float x, float y, float z){
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}
	
	/**
	 * @return Arreglo con la posición de la fuente
	 * */
	float* getPosition(){
		return pos;
	}
	
	/**
	 * Pone la fuente a sonar o la detiene
	 * */
	void togglePlaying(){
		playing = !playing;
	}
	
	/**
	 * @return true si la fuente se está reproduciendo, false si no.
	 * */
	bool isPlaying(){
		return playing;
	}
	
	/**
	 * Cambia el ID de la fuente
	 * */
	void setId(string nuID){
		id = nuID;
	}
	
	/**
	 * @return Id de la fuente
	 * */
	string getId(){
		return id;
	}
	
	/**
	 * @return booleano que indica si la fuente es ambiente
	 * */
	bool isAmbient(){
		return ambient;
	}
	
	/**
	 * Cambia la fuente a ambiente/no ambiente 
	 * @param amb Indicación si la fuente es ambiente
	 * */
	void setAmbient(bool amb){
		ambient = amb;
	}
};

#endif /*SOURCE_H_*/
