/**
 * @brief Programa de prueba para OpenAL usando OggVorbis
 * @note El proyecto no incluye archivos de audio, con el fin de disminuír su tamaño
 * en el CVS
 * @author John Jairo Palma Robayo
 * @example main.cpp 
 * */

#include <iostream>
/*Include openal*/
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
/*include opengl*/
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <pthread.h>
#include "OggLoader.h"

using namespace std;

/*loader Vorbis*/
OggLoader *loader;

bool loadfile();
void setListener();
void setSource();
/*display*/
void display();
bool update();
/*Funciones de Teclado*/
void specialKeys(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);

/**
 * Posiciones escucha
 * */
ALfloat lisPosition[] = {0.0, 0.0, 0.0};

/**
 * Posición fuente
 * */
ALfloat position[] = {0.0, 0, 0};

/*Fuente*/
ALuint source;
/**
 * Buffers OpenAL
 * */
ALuint buffer;
/**
 * Otros buffers OpenAL
 * */
ALuint buffers[2];
/**
 * Reproducción de la fuente
 * */
bool playing = false;

/**
 * Función que lanza en un hilo la reproducción de audio
 * */
void* funcThread(void * data);
/**
 * Nombre del archivo
 * */
string filename;
/**
 * ventana
 * */
int win;


int main(int argc, char** argv)
{
	if(argc != 2){
		cout << "No se han ingresado los parámetros correctamente " << endl;
		cout << "LoadOggFilesOpenALTest <NombreArchivo>" << endl;
		return EXIT_FAILURE;
		
	}
	else
		filename = argv[1];
	if (!alutInit (NULL, NULL))
	{
		ALenum error = alutGetError ();
		cout <<  alutGetErrorString (error) << endl;;
		exit (EXIT_FAILURE);
	}
	alGenSources(1, &source);
	setListener();
	if(!loadfile())
		exit(EXIT_FAILURE);
	pthread_t hiloid;
	pthread_create( &hiloid, NULL, 
			funcThread, 
			0);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;
	std::cout << "Hello world!" << std::endl;
	glutInitWindowSize(200, 200);
	win = glutCreateWindow("Ventana 1");
	glutDisplayFunc(display) ;
	glutKeyboardFunc(keyboard) ;
	glutSpecialFunc(specialKeys);

	glutMainLoop();


	return EXIT_SUCCESS;
}

bool loadfile(){

	loader = new OggLoader();
	try{
		cout << filename << endl;
		loader->loadFile(filename);
		alGenBuffers(1, &buffer);
		if(!loader->loadFileOntoBuffer(buffer))
			return false;
		//alSourceQueueBuffers(source, 2, buffers);
		alSourcei (source, AL_BUFFER, buffer);
		alSourcei (source, AL_LOOPING, AL_TRUE);
		setSource();
		alSourcePlay(source);
		return true;
	}
	catch (string error){
		cout << error << endl;
		return false;
	}
}

bool update(){
    /*int processed;
    bool active = true;
 
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
 
    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
 
        active = loader->loadFileOntoBuffer(buffer);
 
        alSourceQueueBuffers(source, 1, &buffer);
    }
    return active;*/
}

void setListener(){
	alListenerfv(AL_POSITION, lisPosition);

	if(alGetError() != AL_NO_ERROR){
		cout << "Error al posicionar a la escucha" << endl;
	}
}

void setSource(){
	alSourcefv(source, AL_POSITION, position);

	if(alGetError() != AL_NO_ERROR){
		cout << "Error al posicionar a la escucha" << endl;
	}

}

void* funcThread(void * data){
	while(update());
}

// ===================================================================
// void keyboard(int key, int x, int y)
// ===================================================================
void keyboard(unsigned char key, int x, int y) 
{
	switch(key)
	{
		case 'w': case 'W':
			lisPosition[1]++;
			setListener();
			break;
		case 's': case 'S':
			lisPosition[1]--;
			setListener();
			break;
		case 'a': case 'A':
			lisPosition[0]--;
			setListener();
			break;
		case 'd': case 'D':
			lisPosition[0]++;
			setListener();
			break;
	
		case 'r': case 'R':
			lisPosition[2]--;
			setListener();
			break;
	
		case 'f': case 'F':
			lisPosition[2]--;
			setListener();
			break;
		case 'p': case 'P':
			if(playing){
				alSourcePause(source);
			}
			else
				alSourcePlay(source);
			break;
	
		case 'l': case 'L':
			alSourceStop(source);
			break;
			
		case 'z': position[2]++;
			setSource();
			break;
		
		case 'Z': position[2]--;
			setSource();
			break;
	
		case 27:
			alSourceStop(source);
			alutExit();
			glutDestroyWindow(win);
			exit(EXIT_SUCCESS) ;
			break ;
			
		default: break;
	}
	cout << "x: " << position[0] << " y: " << position[1] << " z: " << position[2] << endl;
	glutPostRedisplay() ;
}

// ===================================================================
// void specialKeys(int key, int x, int y)
// =================================================================== 
void specialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_RIGHT:	position[0]++;
			setSource();
			glutPostRedisplay() ;
			break;
		case GLUT_KEY_LEFT: 	position[0]--;
			setSource();
			glutPostRedisplay() ;
			break;
		case GLUT_KEY_UP: 	position[1]--;
			setSource();
			glutPostRedisplay() ;
			break;
		case GLUT_KEY_DOWN: 	position[1]++;
			setSource();
			glutPostRedisplay() ;
		break;
	}
	cout << "x: " << position[0] << " y: " << position[1] << " z: " << position[2] << endl;
}

void display(){
	


}
