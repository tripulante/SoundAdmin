#ifndef LISTENER_H_
#define LISTENER_H_

#include <gmtl/gmtl.h>
#include <iostream>

using namespace gmtl;
using namespace std;

/**
 * @class Listener
 * @brief Clase que define una escucha (Listener)
 * @author John Jairo Palma Robayo
 * @file Listener.h
 * */
class Listener{
	
private:
	
	/**
	 * Posición de la escucha en el espacio
	 * */
	Vec3f position;
	
	/**
	 * Orientación de la escucha en el espacio
	 * */
	Vec3f orientation;
	
public:
	
	/**
	 * Constructor de la clase
	 * */
	Listener(){
	}
	
	/**
	 * Destructor de la clase
	 * */
	virtual ~Listener(){
		
	}
	
	/**
	 * @param pos nueva posición de la escucha
	 * */
	void setPosition(Vec3f pos){
		position = pos;
	}
	
	/**
	 * @param orient Nueva orientación de la escucha
	 * */
	void setOrientation(Vec3f orient){
		orientation = orient;
	}
	
	/**
	 * @return Posición de la escucha 
	 * */
	Vec3f getPosition(){
		return position;
	}
	
	/**
	 * @return Orientación de la escucha
	 * */
	Vec3f getOrientation(){
		return orientation;
	}
};

#endif /*LISTENER_H_*/
