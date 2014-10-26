/**
 * @brief El objetivo de esta prueba es verificar la interacción en tiempo real
 * con una instancia de CSound usando Head-Related Transfer Function (hrtf)
 * @author John Jairo Palma Robayo 
 * @todo TRabaja con GLUT, pero no empieza a trabajar hasta que no se presiona una tecla 
 * @todo Revisar el envío de eventos de score y la carga de archivos. CScore está comentado
 * @todo Revisar Loops!
 * @todo Utilizar vbap (posteriormente)
 * @note El proyecto no incluye archivos de audio, con el fin de disminuír su tamaño
 * en el CVS
 * @example main.cpp
 * */


#include <iostream>
#include "UserData.h"
/*inclusiones de CSound*/
#include <csound/csound.hpp>
#include <csound/cscore.h>
//#include <csound/csound.h>
/*inclusiones GL/GLUT*/
#include <GL/gl.h>
#include <GL/glut.h>


using namespace std;

/*Definición de funciones en el main*/

/**
 * Rutina que va a ser ejecutada por el thread
 * @param data Apuntador a los datos a utilizar por la rutina
 * */
uintptr_t threadRoutine(void *data);
/**
 * Función requerida por csound para el envío de eventos al score de la instancia de CSound en ejecución
 * @todo Al no poderse probar CScore la función está actualmente vacía
 * */
void cscore();

/*objetos y atributos*/


/**
 * Objeto Csound a utilizar
 * */
Csound *csoundObj;

/**
 * Modela los grados de la función de prueba de CSound
 * */
double degree; 

/**
 * Modela la distancia de la función de prueba
 * */
double aDistance;

/**
 * Modela la reverberación en la función de prueba
 * */
double reverb;

/**
 * Identificador del hilo de ejecución
 * */
void *threadId;

/**
 * estructura de datos utilizada para los cambios en tiempo real
 * */
UserData *ud;

/**
 * Número que permite identificar la ventana
 * */
int win;

/**
 * Función de display  
 * */
void display();

/**
 * Funciones de Teclado
 * */
void specialKeys(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);

/**
 * Método principal de la aplicación
 * */
int main(int argc, char** argv){

	

	/*
	 * Definimos los canales para hacer las pruebas
	 * */
	degree = 40;
	aDistance = 40;
	reverb = 20;
	csoundObj = new Csound();
	
	cout << csoundObj->GetMessageLevel() << endl;
	/*
	 * Inicialización CSound
	 * */
	

	
	int res = csoundObj->Compile("data/test.csd");	
	
	cscore();
	/*agregamos los eventos*/
	/*csound->InputMessage("f 2	0	0	1	\"03.wav\"	0	0	0\n");
			csoundObj->InputMessage("i 1 0 60\n");
			csoundObj->InputMessage("e\n");*/ 
	
	ud = new UserData(csoundObj, res);
	
	

	if(!res){
		csoundObj->InputMessage("f 2	0	0	1	\"03.wav\"	0	0	0\n");
		csoundObj->InputMessage("i 1 0 60\n");
		csoundObj->InputMessage("e\n");
		ud->togglePerform();
		/*lanzamiento del thread*/
		threadId = csoundCreateThread(threadRoutine, (void*)ud);
		if(!threadId)
			cout << "no se creó el hilo" << endl;
		cout << "Crea tl thread" << endl;
	}
	else{
		cout << "Error en compilación Csound" << endl;
		return 0;
	}
	
	/*inicialización glut*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;
	glutInitWindowSize(200, 200);
	win = glutCreateWindow("Ventana 1");
	glutDisplayFunc(display) ;
	glutKeyboardFunc(keyboard) ;
	glutSpecialFunc(specialKeys);

	glutMainLoop();

	return ud->getResult();	
}

uintptr_t threadRoutine(void *data){

	
	UserData *ud = (UserData*) data;
	/*verificamos que haya compilado Csound*/
	if(!ud->getResult()){
		/*Reproducimos el score*/
		while(!((ud->getCsound())->PerformKsmps()) && ud->isPerforming()){
			cout << "grados! " << ud->getDegrees() << endl;
			/*Actualizamos los canales de software bus*/
			ud->getCsound()->SetChannel("degree", ud->getDegrees());
			ud->getCsound()->SetChannel("distance", ud->getDistance());
			ud->getCsound()->SetChannel("reverb", ud->getReverb());
			ud->getCsound()->RewindScore();
		}
		/*Destruimos Csound al terminar?*/
		//cout << "Destruye CSound" << endl;
		//ud->destroyCsound();

	}
	//detenemos la reproducción
	if(ud->isPerforming())
		ud->togglePerform();
	return 1;

}



// Funciones GLUT para interacción con teclado y otros

/*
 * Interacción normal con teclado
 * */
void keyboard(unsigned char key, int x, int y) 
{
	switch(key)
	{
		case 'w': case 'W':
			aDistance++;
			break;
		case 's': case 'S':
			aDistance--;
			break;
		case 'a': case 'A':
			degree++;
			break;
		case 'd': case 'D':
			degree--;
			break;
		case 'r': case 'R':
			reverb++;
			break;
		case 'v': case 'V':
			reverb--;
			break;
		case 27:
			cout << "final de la prueba" << endl;
			ud->togglePerform();
			/*unimos el hilo*/
			csoundJoinThread(threadId);
			/*destruimos la instancia de Datos incluyendo CSound*/
			delete ud;
			glutDestroyWindow(win);
			exit(EXIT_SUCCESS) ;
			break ;
		default: break;
	}
	ud->setInteractionData(degree, reverb, aDistance);
	glutPostRedisplay() ;
}


/*
 * No se tiene ninguna acción todavía, así que se guarda la función para 
 * futuras interacciones
 * */
void specialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_RIGHT:	
			break;
		case GLUT_KEY_LEFT: 	
			break;
		case GLUT_KEY_UP: 	
			break;
		case GLUT_KEY_DOWN: 
			break;
	}

	glutPostRedisplay();
}

void display(){

}

void cscore(){
//	cscorePutString(csoundObj->GetCsound(), "f 2	0	0	1	\"03.wav\"	0	0	0");
//	cscorePutString(csoundObj->GetCsound(), "i 1 0 60");
//	cscorePutString(csoundObj->GetCsound(), "e");

}
