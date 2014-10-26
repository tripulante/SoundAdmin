#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include <gmtl/gmtl.h>
#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <iostream>
#include <sstream>
#include "Speaker.h"

using namespace gmtl;
using namespace gmtl::Math;
using namespace std;

/**
 * @class SpeakerTriangle
 * @brief Define un triángulo entre speakers y algunas funciones útiles
 * @file Triangle.h 
 * */
class SpeakerTriangle{
private:
	
	/**
	 * Arreglo que contiene los 3 parlantes
	 * */
	Speaker* speakers[3];
	
	/**
	 * Matriz para calcular las ganancias
	 * */
	Matrix33f speakersMatrix;
//	métodos privados
	
	
public: 
	
	
	/**
	 * Constructor de la clase
	 * @pre Los parlantes no son nulos
	 * @param a Speaker que define la primer arista del triángulo
	 * @param b Speaker que define la segunda arista del triángulo
	 * @param c Speaker que define la tercer arista del triángulo
	 * */
	SpeakerTriangle(Speaker* a, Speaker* b, Speaker* c){
		
		speakers[0] = a;
		speakers[1] = b;
		speakers[2] = c;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++){
				speakersMatrix[i][j] = speakers[i]->getVector()[j];
			}
		gmtl::transpose(speakersMatrix);
		Matrix33f res;
		invertFull(res, speakersMatrix);
		if(res.isError()){
			cout << "Error al invertir la matriz!" << endl;
//			cout << toString() << endl;
		}
		else
			speakersMatrix = res;
	}
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~SpeakerTriangle(){
//		for(int i = 0; i < 3; i++)
//			delete speakers[i];
//		delete speakers;
//		cout << "Borrado Triángulo" << endl;
	}
	
	/**
	 * @return Retorna el arreglo con los parlantes contenidos en el triángulo
	 * */
	Speaker** getSpeakers(){
		return speakers;
	}
	
	/**
	 * @return Retorna la matriz para realizar los cálculos de ganancias
	 * */
	Matrix33f getMatrix(){
		return speakersMatrix;
	}
	
	/**
	 * Devuelve el volumen del sólido contenido por los vectores en el 
	 * triángulo dividido por el total de la longitud de los lados
	 * @param s Triángulo a calcular
	 * @return Volumen del triángulo. 0.0f si es muy pequeño
	 * */
	static float triangleVolume(SpeakerTriangle s){
		/*Calculamos el volumen usando triple producto*/
		Speaker** speakers = s.getSpeakers();
		Vec3f first = speakers[0]->getVector();
		Vec3f second = speakers[1]->getVector();
		Vec3f third = speakers[2]->getVector();
		Vec3f res;
		cross(res, first, second);
		normalize(res);
		float volume = abs((float)dot(res, third));
		float perimeter = abs(angleBetween(first, second)) + abs(angleBetween(second, third))+
			abs(angleBetween(third, first));
		if(perimeter > (1e-06))
			return (float)volume/perimeter;
		else
			return 0.0f;
//		return 0.0;
	}
	
	/**
	 * Retorna el ángulo entre los dos vectores
	 * @param a Vector a calcular
	 * @param b Vector a calcular
	 * @return Ángulo entre los dos vectores
	 * */
	static float angleBetween(Vec3f a, Vec3f b){
		float val = (float) (dot(a,b)/(length(a)*length(b)));
		val = (val > 1.0 ? 1.0 : val);
		val = (val < -1.0 ? -1.0 : val);
		return gmtl::Math::abs(gmtl::Math::aCos(val));
	}
	
	/**
	 * Convierte el triángulo a una cadena legible para debug
	 * @return Triángulo como cadena legible
	 * */
	string toString(){
		stringstream sstream("");
		sstream << "Triángulo: " << endl;
		for(int i = 0; i < 3; i++){
			sstream << speakers[i]->toString();
		}
		sstream << "Matriz Invertida: " << endl << speakersMatrix << endl;
		return sstream.str();
	}
	
	
	/**
	 * Verifica si existe un vértice del triángulo con ese número de speaker
	 * @param numspeaker Número del speaker a verificar
	 * @return true si uno de los vértices del triángulo tiene ese número de speaker
	 * */
	bool isTriangleVertex(int numspeaker){
		 return (speakers[0]->getSpeakerNumber() == numspeaker || 
				speakers[1]->getSpeakerNumber() == numspeaker ||
				speakers[2]->getSpeakerNumber() == numspeaker);
	}
};

#endif /*TRIANGLE_H_*/
