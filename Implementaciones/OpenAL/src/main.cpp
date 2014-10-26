/**
 * @brief Programa de prueba para OpenAL usando la base del sistema desarrollado
 * @note El proyecto no incluye archivos de audio, con el fin de disminuír su tamaño
 * en el CVS
 * @author John Jairo Palma Robayo - Oscar Javier Chavarro (jedilink@users.sourceforge.net)
 * @example main.cpp
 * */
//===========================================================================
//= Plantilla para el uso de OpenGL y glut.                                 =
//= Para compilar en Unix (Linux en Intel o en Sparc):                      =
//= g++ -L/usr/X11R6/lib -o programa principal.C                            =
//=                                  -lglut -lGLU -lGL -lX11 -lXmu -lXi -lm =
//= Para compilar en cygwin:                                                =
//= g++ -o programa principal.C -lglut32 -lGLU32 -lOpenGL32 -lm             =
//=-------------------------------------------------------------------------=
//= Este programa fue elaborado por Oscar Chavarro                          =
//=     (jedilink@users.sourceforge.net)                                    =
//= Modificado por John Jairo Palma Robayo para trabajar con OpenAL	    	=
//= y se rige bajo la licencia GPL de la GNU (http://www.gnu.org)           =
//===========================================================================

#include <pthread.h>
// Se incluyen los encabezados standard de C/C++
#include <stdio.h>
#include <stdlib.h>

// Se incluye el encabezado de la libreria GLUT, que a su vez llama a GL y GLU
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <iostream>

/*Include openal*/
#include "interface/ISoundAdministrator.h"
#include "implementation/openal/SoundAdministratorOpenAL.h"
#include <string>

#include <vrpn_Shared.h>
#include <vrpn_Tracker.h>

/**
 * @def Definición para limpiar la pantalla
 * */
#define clrscr() printf("%c[2J%c[1;1H", 27, 27)

using namespace std;

double posx = 0.0;
double posy = 0.0;
double posz = 0.0;

double posx2 = 0.0;
double posy2 = 0.0;
double posz2 = 0.0;

/**
 * Instancia del administrador auditivo
 * */
ISoundAdministrator *admin; 
/**
 * Archivo correspondiente al primer tracker.
 * */
string fileTracker1;
/**
 * Archivo correspondiente al segundo tracker.
 * */
string fileTracker2;
/**
 * Indicador de fuente ambiente para el tracker 1
 * */
bool ambientT1 = false;
/**
 * Indicador de fuente ambiente para el tracker 2
 * */
bool ambientT2 = false;

#define NMAX 500
double arrx[NMAX];
double arry[NMAX];
double arrz[NMAX];

double arrx2[NMAX];
double arry2[NMAX];
double arrz2[NMAX];

void
inspunto()
{
    int i;

    for ( i = NMAX-1; i > 0; i-- ) {
	arrx[i] = arrx[i-1];
	arry[i] = arry[i-1];
	arrz[i] = arrz[i-1];
    }
    arrx[0] = posx;
    arry[0] = posy;
    arrz[0] = posz;
}

void
inspunto2()
{
    int i;

    for ( i = NMAX-1; i > 0; i-- ) {
	arrx2[i] = arrx2[i-1];
	arry2[i] = arry2[i-1];
	arrz2[i] = arrz2[i-1];
    }
    arrx2[0] = posx2;
    arry2[0] = posy2;
    arrz2[0] = posz2;
}

int done = 0;       // Signals that the program should exit

int
initTracker (int argc, char * argv []);

//-------------------------------------
class device_info {
    public:
        char                *name;
        vrpn_Tracker_Remote *tkr;
};
device_info *dev = 0;
    device_info data;


//===========================================================================
//= Definicion de constantes, macros y variables globales                   =
//===========================================================================

// Tamanno inicial de la ventana
#define VENTANA_XTAM 640
#define VENTANA_YTAM 480

// Tamanno de la ventana en un momento dado (Variables de estado)
int XTAM_actual = VENTANA_XTAM;
int YTAM_actual = VENTANA_YTAM;

/**
* Funciones especiales para audio
*/

/**
* Ubica a la fuente en el espacio
*/
/*
void setListener(){
	alListenerfv(AL_POSITION, lisPosition);

	if(alGetError() != AL_NO_ERROR){
		cout << "Error al posicionar a la escucha" << endl;
	}
}
*/
/**
* Ubica las fuentes de audio en el espacio
*/
void setSource(){
	admin->updateSource("Tracker0", (float) posx, (float) posy, (float) posz);
	admin->updateSource("Tracker1", (float) posx2, (float)posy2, (float)posz2);
}

//===========================================================================
//= Definicion de las funciones tipo "callback", las cuales son llamadas    =
//= asincronicamente por GLUT en el momento que se detecten eventos.        =
//===========================================================================

static void
callback_nuevo_tam(int nuevo_ancho, int nuevo_alto)
/**
Esta funcion es llamada de manera asincronica por GLUT cuando el usuario
cambia el tamanno de la ventana.  Notese que para que la proyeccion del
mundo 3D a la ventana 3D coincida con el area de la ventana en su nuevo
tamanno, debe reconfigurarse OpenGL para que lo tenga en cuenta en sus
matrices de proyeccion (i.e. se tendra que llamar a glViewport con los
nuevos valores).  Las variables `nuevo_ancho` y `nuevo_alto` indican las
dimensiones del area de visualizacion dentro de la ventana.
*/
{
    XTAM_actual = nuevo_ancho;
    YTAM_actual = nuevo_alto;
}

static void
callback_teclado(unsigned char tecla, int x, int y)
/**
Esta funcion es llamada de manera asincronica por GLUT cuando el usuario
oprime una tecla "ASCII", es decir, cuando la tecla posee un codigo ASCII
correspondiente. Notese que teclas como "F1", "SHIFT", "Caps Lock" o las 
teclas del cursor no tienen una equivalente ASCII, por lo cual se manejan
en la funcion `callback_teclado_especial`, o no se soportan.
*/
{
    //- Actualice el estado de ejecucion del programa en funcion del evento -
    switch ( tecla ) {
      case 27:
        printf("Salgo del programa!\n\a");
        delete admin;
        fflush(stdout);
        exit(1);
      case 'p':
        printf("Camara en modo perspectiva!\n\a");
        fflush(stdout);
        break;
	case 'u': 
		admin->stopSource("Tracker0");
		break;
	case 's':
		admin->stopSource("Tracker1");
		break;
	case '1':
		admin->playSource("Tracker0", true);
		break;
	case '2':
		admin->playSource("Tracker1", true);
		break;
	case 'c': case 'C':
		ambientT1 = !ambientT1;
		admin->useSourceAsAmbient("Tracker0", ambientT1);
		break;
	case 'v': case 'V':
		ambientT2 = !ambientT2;
		admin->useSourceAsAmbient("Tracker1", ambientT2);
		break;
      default:
        printf("Tecla no asignada!\n\a");
        fflush(stdout);
        return;
    }

    //- Dispare un evento de redibujado (se llamara la funcion de dibujado) -
    glutPostRedisplay();
}

static void
callback_teclado_especial(int keycode, int x, int y)
/**
Esta funcion es llamada de manera asincronica por GLUT cuando el usuario
oprime una tecla "no ASCII", como las flechas de cursor. Notese que cada 
tecla "no ASCII" esta identificada mediante una constante. Para ver las 
teclas especiales soportadas por GLUT, observe el contenido del encabezado
"glut.h" (usualmente en /usr/X11R6/include/GL, si no puede buscarlo con
el comando find / -name "glut.h" 2> /dev/null).
*/
{
    //- Actualice el estado de ejecucion del programa en funcion del evento -
    switch ( keycode ) {
      case GLUT_KEY_LEFT:
        printf("Selecciono la cosa anterior!\n\a");
        fflush(stdout);
        break;
      case GLUT_KEY_RIGHT:
        printf("Selecciono la siguiente cosa!\n\a");
        fflush(stdout);
        break;
      default:
        printf("Tecla especial no asignada!\n\a");
        fflush(stdout);
        return;
    }

    //- Dispare un evento de redibujado (se llamara la funcion de dibujado) -
    glutPostRedisplay();
}

static void
callback_animar_modelo(void)
{
    glutPostRedisplay();
}

static void
callback_pintar_modelo(void)
/**
Esta funcion es llamada de manera asincronica por GLUT cuando ocurre cualquier
circunstancia que implique la necesidad de redibujar la grafica en la ventana.
Dichas circunstancias son tipicamente generadas por movimientos de la ventana,
restauraciones a partir de un estado minimizado y respuesta a eventos como
activacion de teclas o movimientos de teclado.
*/
{
//        dev->tkr->mainloop();

    inspunto();
    inspunto2();
	/*posicionamos la fuente sonora*/
	setSource();

    glViewport(0, 0, XTAM_actual, YTAM_actual);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
/*
    gluLookAt(0, 0, 0, 
              0, -10, 0,  
              0, 0, 1);
    gluPerspective(60, 1, 0.1, 100);
*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//-----------------------------------------------------------------------
    glPushMatrix();
    glColor3d(0, 1, 0);

    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    glColor3d(1, 0, 0);
    int i;
    for ( i = NMAX-1; i >= 0; i-- ) {
        glVertex3d(arrx[i]/4, arry[i]/4, arrz[i]/4);
    }
    glEnd();

    glLineWidth(2);

    glColor3d(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    for ( i = NMAX-1; i >= 0; i-- ) {
        glVertex3d(arrx2[i]/4, arry2[i]/4, arrz2[i]/4);
    }
    glEnd();


    glPopMatrix();

	//- Finalmente, se muestra la escena pintada y OpenGL pintara "atras" ---
    glFlush();
    glutSwapBuffers();
}



//===========================================================================
//= Programa principal                                                      =
//===========================================================================

int
main(int argc, char **argv)
{
	if(argc != 4){
		printf("ERROR: Los parámetros están incompletos\n");
		printf("OpenALTest <ruta Tracker> <Nombre Archivo Tracker 1> <Nombre Archivo Tracker 2> \n");
		exit(1);
	}
	else{
		fileTracker1 = argv[2];
		fileTracker2 = argv[3];
	}
	
	/*inicialización audio*/
	try{
		admin = new SoundAdministratorOpenAL();
		admin->loadFile("Tracker0", fileTracker1);
		admin->loadFile("Tracker1", fileTracker2);
		admin->createSource("Tracker0", "Tracker0", ambientT1);
		admin->createSource("Tracker1", "Tracker1", ambientT2);
		printf("fin?\n");
		
		admin->playAllSources();
	}
	catch (string str){
		printf("Error al trabajar con ALUT: %s", str.c_str());
		exit(EXIT_FAILURE);
	}
	//- (1/3): Inicializacion de la GUI -------------------------------------
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(VENTANA_XTAM, VENTANA_YTAM);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    if ( glutCreateWindow("PLANTILLA OPENGL/GLUT") == GL_FALSE ) {
        printf("ERROR: No se puede crear una ventana grafica!\n");
        printf("Revise que el sistema X11 este funcionando.\n");
        exit(1);
    }

    initTracker(argc, argv);
  
	//- (2/3): Asignacion de eventos a funciones asincronicas o "callbacks" -
    glutReshapeFunc(callback_nuevo_tam);
    glutKeyboardFunc(callback_teclado);
    glutSpecialFunc(callback_teclado_especial);
    glutDisplayFunc(callback_pintar_modelo);
    glutIdleFunc(callback_animar_modelo);

	//- (3/3): Ejecucion de la GUI: GLUT toma el control y llama callbacks --
    glutMainLoop();
    return 0;
}

//===========================================================================
//= Fin de archivo                                                          =
//===========================================================================



//-------------------------------------
// This section contains the data structure that is used to determine how
// often to print a report for each sensor of each tracker.  Each element
// contains the number of reports to skip between printing and a counter
// that is used by the callback routine to keep track of how many it has
// skipped.  There is an element for each possible sensor.  A new array of
// elements is created for each new tracker object, and a pointer to it is
// passed as the userdata pointer to the callback handlers.  A separate
// array is kept for the position, velocity, and acceleration for each
// tracker.  The structure passed to the callback handler also has a
// string that is the name of the tracker.

class t_sensor_counter {
    public:
        int stride;     // Stride of 1 == print every, 2 is every other, ...
        int count;      // How many we have gotten
};
class t_user_callback {
    public:
        char                t_name[vrpn_MAX_TEXT_LEN];
        t_sensor_counter    t_sensor_list[vrpn_TRACKER_MAX_SENSORS];
};


/*****************************************************************************
 * Callback handlers                                                         *
 *****************************************************************************/

void VRPN_CALLBACK handle_tracker_pos_quat (void *userdata, const vrpn_TRACKERCB t)
{
        t_user_callback *t_data = (t_user_callback *)userdata;

        // See if we have gotten enough reports from this sensor that we should
        // print this one.  If so, print and reset the count.
        if ( ++t_data->t_sensor_list[t.sensor].count >= t_data->t_sensor_list[t.sensor].stride ) {
                t_data->t_sensor_list[t.sensor].count = 0;
		if ( t.sensor == 0 ) {
/*
                printf("(1)Tracker %s, sensor %ld:\n        pos (%5.2f, %5.2f, %5.2f); quat (%5.2f, %5.2f, %5.2f, %5.2f)\n",
                        t_data->t_name,
                        t.sensor,
                        t.pos[0], t.pos[1], t.pos[2],
                        t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
*/
		posx = t.pos[0];
		posy = t.pos[1];
		posz = t.pos[2];
		}
		if ( t.sensor == 1 ) {
		posx2 = t.pos[0];
		posy2 = t.pos[1];
		posz2 = t.pos[2];
		}
        }
}

void *
hilofuncion(void *)
{

    while ( 1 ) {
        dev->tkr->mainloop();
        vrpn_SleepMsecs(1);
    }

    return 0;
}

int
initTracker (int argc, char * argv [])
{
    int tracker_stride = 1;         // Every nth report will be printed
    int print_for_tracker = 1;       // Print tracker reports?
    int print_for_text = 1;          // Print warning/error messages?

    int i = 1;

    // Parse arguments, creating objects as we go.  Arguments that
    // change the way a device is treated affect all devices that
    // follow on the command line.

            // Name the device and open it as everything
            dev = &data;
            dev->name = argv[i];
            dev->tkr = new vrpn_Tracker_Remote(dev->name);
            if ( (dev->tkr == NULL) ) {
                fprintf(stderr,"Error opening %s\n", dev->name);
                return -1;
            } else {
                printf("Opened %s as:", dev->name);
            }

            // If we are printing the tracker reports, prepare the
            // user-data callbacks and hook them up to be called with
            // the correct data for this device.
            if ( print_for_tracker ) {
                int j;
                vrpn_Tracker_Remote *tkr = dev->tkr;
                t_user_callback *tc1 = new t_user_callback;
                t_user_callback *tc2 = new t_user_callback;
                t_user_callback *tc3 = new t_user_callback;

                if ( (tc1 == NULL) || (tc2 == NULL) || (tc3 == NULL) ) {
                    fprintf(stderr,"Out of memory\n");
                }
                printf(" Tracker");

                // Fill in the user-data callback information
                strncpy(tc1->t_name, dev->name, sizeof(tc1->t_name));
                strncpy(tc2->t_name, dev->name, sizeof(tc2->t_name));
                strncpy(tc3->t_name, dev->name, sizeof(tc3->t_name));
                for (j = 0; j < vrpn_TRACKER_MAX_SENSORS; j++) {
                    tc1->t_sensor_list[j].count = 0;
                    tc1->t_sensor_list[j].stride = tracker_stride;
                    tc2->t_sensor_list[j].count = 0;
                    tc2->t_sensor_list[j].stride = tracker_stride;
                    tc3->t_sensor_list[j].count = 0;
                    tc3->t_sensor_list[j].stride = tracker_stride;
                }

                // Set up the tracker callback handlers
                tkr->register_change_handler(tc1, handle_tracker_pos_quat);
                //tkr->register_change_handler(tc2, handle_tracker_vel);
                //tkr->register_change_handler(tc3, handle_tracker_acc);
            }

            if (print_for_text) {
                printf(" Warning/Error");
            } else {
                vrpn_System_TextPrinter.remove_object(dev->tkr);
            }

            printf(".\n");
	    pthread_t hiloid;
            pthread_create( &hiloid, NULL, 
                hilofuncion, 
               0);
    return 0;
}
