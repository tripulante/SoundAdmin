#ifndef SPEAKER_H_
#define SPEAKER_H_
#include <gmtl/gmtl.h>
#include <gmtl/Vec.h>
#include <gmtl/Math.h>
#include <iostream> 
#include <sstream>

using namespace gmtl;
using namespace gmtl::Math;
using namespace std;
/**
 * @class Speaker
 * @brief Define un parlante en el espacio
 * @file Speaker.h
 * */
class Speaker{
	
private:
	
	/**
	 * Ángulo horizontal del speaker
	 * */
	float azimuth;
	
	/**
	 * Elevación del speaker
	 * */
	float elevation;
	/**
	 * Número del speaker
	 * */
	int numspeaker;
	
	/**
	 * Vector al que se encuentra orientado el altoparlante
	 * */
	Vec3f vector;
	
//	métodos privados
	
	/**
	 * Convierte las coordenadas polares del speaker a un vector cartesiano y lo guarda
	 * en el vector
	 * @pre El vector polar es unitario
	 * @note El vector queda en sistema de coordenadas tipo OpenGL 
	 * */
	void createVectorFromAngles(){
//		Convertimos a radianes
		//TODO Terminar de revisar la implementación de los ángulos
//		float x = cos(deg2Rad(azimuth))*cos(deg2Rad(elevation));
//		float y = sin(deg2Rad(azimuth))*cos(deg2Rad(elevation));
//		float z = sin(deg2Rad(elevation));
		float x = cos(deg2Rad(azimuth))*cos(deg2Rad(elevation));
		float y = sin(deg2Rad(elevation));
		float z = sin(deg2Rad(azimuth))*cos(deg2Rad(elevation));
		//vector
		vector[0] = (abs(x) > (1e-06f)) ? x : 0.0; // x;
		vector[1] = (abs(y) > (1e-06f)) ? y : 0.0; // y;
		vector[2] = (abs(z) > (1e-06f)) ? z : 0.0; // z;
	}
public:
	
	/**
	 * Crea un nuevo speaker
	 * @param nuAz Ángulo horizontal del speaker
	 * @param nuElev elevación del speaker
	 * @param num Número del speaker 
	 * */
	Speaker(float nuAz, float nuElev, int num){
		azimuth = nuAz;
		elevation = nuElev;
		numspeaker = num;
		createVectorFromAngles();
	}
	/**
	 * Destructor
	 * */
	virtual ~Speaker(){
		azimuth = elevation = 0.0;
		numspeaker = -1;
	}
	
	/**
	 * Cambia los ángulos del vector
	 * @param nuAz Nuevo ángulo de orientación (azimuth)
	 * @param nuElev Nuevo ángulo de elevación
	 * */
	void setAngles(float nuAz, float nuElev){
		azimuth = nuAz;
		elevation = nuElev;
		createVectorFromAngles();
	}
	
	/**
	 * @return Vector en coordenadas cartesianas del speaker
	 * */
	Vec3f getVector(){
		return vector;
	}
	
	/**
	 * @return Número del parlante
	 * */
	int getSpeakerNumber(){
		return numspeaker;
	}
	
	/**
	 * @return Versión Texto del Speaker (para debug)
	 * */
	string toString(){
		stringstream sstream("");
		sstream << "Speaker No. " << numspeaker+1 << " Vector: " << vector << endl;
		return sstream.str();
	}
	
};

#endif /*SPEAKER_H_*/
