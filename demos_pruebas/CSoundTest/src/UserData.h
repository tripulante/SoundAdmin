/**
 * @mainpage CSound Test using HRTF
 * @brief Este proyecto desarrolla una prueba de interacción entre el api de CSound y
 * una instancia activa de éste en otro sistema.
 * @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
 * */

#ifndef USERDATA_H_
#define USERDATA_H_

#include <csound/csound.hpp>


/**
 * @class UserData
 * @brief Clase encargada de almacenar los datos a pasarse al thread
 * @author John Jairo Palma Robayo
 * @file UserData.h
 * */
class UserData
{
	
private:
	/**
	 * Objeto csound para trabajar
	 * */
	Csound *csound;
	/**
	 * Resultado de la compilación Csound
	 * */
	int result;
	
	/**
	 * Estamos tocando?
	 * */
	bool performing;
	
	/**
	 * parámetro para la función a enviar al Software bus
	 * */
	double degrees;
	/**
	 * parámetro para la función a enviar al Software bus
	 * */
	double distance;
	/**
	 * parámetro para la función a enviar al Software bus
	 * */
	double reverb;
	/**
	 * Valores a usarse como parámetros entre eventos del score. 
	 * */
	MYFLT* pvalues;
	
	
	
public:
	
	/**
	 * Constructor de la clase
	 * @param cs Objeto Csound para trabajar
	 * @param res Resultado de la compilación
	 * */
	UserData(Csound *cs, int res);
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~UserData();
	
	/**
	 * Activa/Desactiva el hecho de estar tocando
	 * */
	void togglePerform(void);
	
	/**
	 * Pregunta si estamos tocando
	 * @return Valor del atributo performing
	 * */
	bool isPerforming();
	
	/**
	 * @return Resultado de compilación Csound
	 * */
	int getResult();
	
	/**
	 * @return Objeto Csound compilando actualmente 
	 * */
	Csound* getCsound();
	
	/**
	 * Destruye el objeto csound al final de un performance
	 * */
	void destroyCsound();
	
	/**
	 * Cambia los datos de la interacción
	 * @param degrees Grados interacción
	 * @param reverb Reverberación
	 * @param distance Distancia
	 * */
	void setInteractionData(double degrees, double reverb, double distance);
	/**
	 * Obtiene los grados de la función
	 * */
	double getDegrees();
	/**
	 * Obtienen la reverberación de la función
	 * */
	double getReverb();
	/**
	 * Obtiene la distancia de la función
	 * */
	double getDistance();
	/**
	 * Obtiene el arreglo de valores P
	 * */
	MYFLT* getPval();
	/**
	 * Cambia los valores P
	 * @param val Apuntador a arreglo de valores p para un evento
	 * */
	void setPval(MYFLT* val);
	
	
};

#endif /*USERDATA_H_*/
