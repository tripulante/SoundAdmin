#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>

using namespace std;

/**
 * @class Source
 * @brief Definición de una fuente sinusoidal que se puede ubicar en el espacio
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
	
	/**
	 * Fase de la tabla con la cual trabaja esta fuente.
	 * A cada fuente se asigna una fase de una onda sinosoidal.
	 * */
	int phase;

	/**
	* Ganancia de reproducción de la fuente
	*/
	float gain;
	
	/**
	 * Avance de la fuente en la tabla sinusoidal.<br>
	 * A mayor avance, más alta frecuencia de la onda
	 * */
	int adv;

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
		gain = 0.0;
		phase = adv = 0;
	}
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~Source(){
		id = "";
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = false;
		ambient = false;
		gain = 0.0;
		phase = adv = 0;
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
	 * @param amb Indica si la fiente es ambiente
	 * */
	void setAmbient(bool amb){
		ambient = amb;
	}

	/**
	* Cambia la ganancia de la fuente
	* @param nGain Nuevo valor de ganancia (entre 0 y 1)
	*/
	void setGain(float nGain){
		gain = nGain;
	}

	/**
	* @return Ganancia de la fuente
	*/
	float getGain(){
		return gain;
	}
	
	/**
	* @param nuPh nueva fase de la fuente
	*/
	void setPhase(int nuPh){
		phase = nuPh;
	}

	/**
	* @return valor de la fase del sistema
	*/
	int getPhase(){
		return phase;
	}
	
	/**
	 * Cambia el valor de avance de la fuente (implicaría un cambio de altura)
	 * @param advance Nuevo avance de la fuente
	 * */
	void setAdvance(int advance){
		adv = advance;
	}
	
	/**
	 * @return Avance de la fuente
	 * */
	int getAdvance(){
		return adv;
	}
};

#endif /*SOURCE_H_*/
