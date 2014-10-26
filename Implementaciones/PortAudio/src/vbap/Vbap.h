#ifndef VBAP_H_
#define VBAP_H_

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <gmtl/gmtl.h>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <algorithm>

#include "Speaker.h"
#include "Triangle.h"

/**
 * Volumen mínimo a tener en cuenta (para cálculos de triángulos)
 * */
#define MIN_VOL_PER_SIDE (0.01)

using namespace std;
using namespace gmtl;
using namespace gmtl::Math;

/**
 * @struct speakDist
 * @brief Estructura para calcular las intersecciones
 * @file Vbap.h
 * */
struct speakDist{
	/**
	 * Posición del Speaker i
	 * */
	int i;
	/**
	 * Posición del Speaker j
	 * */
	int j;
	/**
	 * Distancia entre los dos speakers
	 * */
	float distance;
	/**
	 * Operador de igualdad entre estructuras
	 * @param b Estructura a comparar
	 * @return true si son iguales
	 * */
	bool operator == (speakDist b){
		return (distance == b.distance && ((i == b.i && j == b.j) || (i == b.j && j == b.i)));
	}
	
	/**
	 * Operador para comparar las distancias entre dos speakers
	 * @param a Estructura a comparar
	 * @return true si \f$distance < a.distance\f$ 
	 * */
	bool operator < (speakDist a){
		return (distance < a.distance);
	}
};

/**
 * funcio&acuten estática que actua como comparador para ordenar usando algoritmos de STL
 * @param a Estructura a comparar
 * @param b Estructura a comparar
 * @return \f$(a.distance < b.distance)\f$
 * */
bool sortFunction(speakDist a, speakDist b);

/**
 * @class Vbap
 * @brief Clase principal del sistema. Recibe la elevación de los ángulos y \n el No. de
 * speakers y calcula las matrices
 * @note Implementa el algoritmo VBAP, desarrollado por Ville Pulkki y se basa en una 
 * implementación previa desarrollada por Pullki
 * @file Vbap.h 
 */
class Vbap
{
	/**
	 * Arreglo con todos los speakers disponibles
	 * */
	Speaker** totalSpeakers;
	
	/**
	 * Número de speakers creados
	 * */
	int numspeakers;
	
	/**
	 * lista con las distancias entre speakers
	 * */
	vector<speakDist> distBetweenSpeakers;
	
	/**
	 * Lista de todos los triángulos activos a usarse
	 * */
	vector<SpeakerTriangle*> activeTriangles;
	
	/**
	 * Matriz con las distancias entre speakers
	 * */
	float** distances;
	
	/**
	 * Verifica si hay intersección entre dos líneas de speakers y desactiva la conexión
	 * @pre \f$a.distance \leq b.distance\f$
	 * @post Si la línea es modificada, cambia la matriz de distancia de b entre speakers a 0.0
	 * @param a Línea a verificar
	 * @param b Línea a verificar
	 * */
	void intersection(speakDist a, speakDist b);
	
	/**
	 * Verifica si algún speaker se encuentra al interior de un triángulo
	 * @param a Triángulo a verificar
	 * @return true si tiene un Speaker dentro del triángulo
	 * */
	bool insideTriplet(SpeakerTriangle *a);
	
	/**
	 * Verifica si el triángulo tenía un lado cruzado (verificado en intersección)
	 * @param b Triángulo a verificar
	 * @return true si tiene un lado cruzado
	 * */
	bool hasCrossedSide(SpeakerTriangle *b);
	
	/**
	 * Imprime las matrices de distancia entre los speakers
	 * */
	void printDistanceMatrix();
	
	
public:
	/**
	 * Constructor de la clase
	 * */
	Vbap(int numspeak);
	/**
	 * Destructor de la clase
	 * */
	virtual ~Vbap();
	
	/**
	 * Crea los speakers a partir de un archivo de configuración
	 * */
	bool createSpeakers(const char* configFilePath);
	
	/**
	 * Crea todos los triángulos posibles con un área razonable 
	 * */
	void createAllTriangles();
	
	/**
	 * Remueve los triángulos inútiles
	 * */
	void removeTriangles();
	
	/**
	 * Selecciona un triángulo del conjunto de acuerdo a las posiciones entregadas
	 * @param x Posición en x de la fuente a manejar
	 * @param y posición en y de la fuente a manejar
	 * @param z posición en z de la fuente a manejar
	 * @return Par con la ganancia de la fuente y los parlantes asociados a la misma 
	 * */
	pair<Vec3f, int*>* selectTriangle(float x, float y, float z);
	
	/**
	 * Retorna la cantidad que se debe atenuar de la ganancia de una fuente de 
	 * acuerdo a qué tan lejos se encuentra de la escucha.
	 * La atenuación se realiza de acuerdo al modelo de distancia inversa:
	 * \f[ atten = \frac{1}{d^2}\f]
	 * con 
	 * \f[ d = ||listener - pos||\f] (es decir, la distancia entre fuente y escucha)
	 * @param pos Posición de la fuente
	 * @param listener Posición de la escucha
	 * @return -1 Si la fuente se encuentra muy cerca del Listener (\f$ d^2 < 1.0 \f$) 
	 * @return Si no, se devuelve la atenuación por distancia (\f$ 0.01 \leq atten \leq 1.0 \f$)
	 * */
	static float getDistanceAttenuation(Vec3f pos, Vec3f listener);
	
	/**
	 * Calcula la ganancia para una fuente en stereo
	 * @param x Posición en x de la fuente a manejar
	 * @param y posición en y de la fuente a manejar
	 * @param z posición en z de la fuente a manejar
	 * @return Vector con la ganancia para cada canal stereo (izquierda, derecha)
	 * */
	static Vec2f getStereoGain(float x, float y, float z);
};

#endif /*VBAP_H_*/
