#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>

using namespace std;

/**
 * @class Source
 * @brief Clase que permite modelar una fuente sonora en el espacio
 * @author John Jairo Palma Robayo
 * @file Source.h
 * */
class Source{
	
private:
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
	float pitch;

	/**
	* Ganancia de reproducción de la fuente
	*/
	float gain;
	
	/**
	 * Posición en la que se encuentra avanzada la reproducción del archivo 
	 * */
	int adv;
	
	/**
	 * Nombre del archivo de la fuente 
	 * */
	string fileName;
	
	/**
	 * Indica si la fuente debe ser reproducida constantemente
	 * */
	bool loop;

public:
	
	/**
	 * Constructor de la clase. La fuente empieza en el origen
	 * y no se está reproduciendo
	 * @param nuID Nueva Id que identifica a la fuente
	 * */
	Source(string nuID){
		id = nuID;
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = ambient = loop = false;
		gain = pitch = 0.0;
		adv = 0;
		fileName = "";
	}
	
	virtual ~Source(){
		id = fileName = "";
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = ambient = loop =false;
		gain = pitch = 0.0;
		adv = 0;
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
	 * Pone la fuente a sonar
	 * */
	void playSource(){
		playing = true;
	}
	
	/**
	 * pausa la fuente
	 * */
	void pauseSource(){
		playing = false;
	}
	
	/**
	 * Detiene la reproducción de la fuente
	 * */
	void stopSource(){
		playing = false;
		adv = 0;	
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
	* @param nuPh Nueva altura (pitch) de la fuente
	*/
	void setPitch(float nuPh){
		pitch = nuPh;
	}

	/**
	* @return Altura (pitch) actual de la fuente.
	*/
	float getPitch(){
		return pitch;
	}
	
	/**
	 * Cambia la posición dentro del archivo
	 * @param advance Nuevo avance de la fuente (posición en la que se encuentra en el archivo)
	 * */
	void setAdvance(int advance){
		adv = advance;
	}
	
	/**
	 * @return Posición en la que se encuentra ubicada la fuente actualmente en el archivo
	 * */
	int getAdvance(){
		return adv;
	}
	
	/**
	 * Cambia la descripción del archivo que usa la fuente
	 * @param file Nueva descripción del archivo a usar por la fuente
	 * */
	void setFileName(string file){
		fileName = file;
	}
	
	/**
	 * @return Descripción asociada a la fuente
	 * */
	string getFileName(){
		return fileName;
	}
	
	/**
	 * Indica si la fuente se debe reproducir constantemente
	 * @param looped Indica si la fuente debe reproducirse constantemente
	 * */
	void setLooped(bool looped){
		loop = looped;
	}
	
	/**
	 * @return Boolean que indica si la fuente se reproduce constantemente o no
	 * */
	bool isLooped(){
		return loop;
	}
};

#endif /*SOURCE_H_*/
