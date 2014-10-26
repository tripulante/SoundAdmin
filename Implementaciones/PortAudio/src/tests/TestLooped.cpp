/**
 * @brief  Archivo de prueba que permite verificar lo siguiente: <dl>
 * <dt> Asociar / Disociar una fuente a un archivo
 * <dt> Reproducir una fuente en loop / Reproducirla una vez
 * <dt> Destruir una archivo / Disociar las fuentes asociadas
 * </dl>
 * @author John Jairo Palma Robayo
 * @example TestLooped.cpp
 * */
#include "pruebas.h"
#if PRUEBA == 0
#include "../soundadmin/SoundAdminPortaudio.h"
#include <string>
#include <sstream>

using namespace std;

/**
 * Administrador de prueba del sistema
 * */
SoundAdminPortaudio* testadmin;

int main(int argc, const char *argv[]){
	
	if(argc != 4){
		cout << "No hay suficientes argumentos" << endl;
		cout << "./Uniandes3DAudio <Nombre Archivo Prueba> " << endl;
		cout <<	"<numspeakers> <archivoConfiguracion>"<< endl;
	}
	
	try{
		int channels = 0;
		string file(argv[1]);
		stringstream values(stringstream::in | stringstream::out);
		values << argv[2];
		cout << values.str() << endl;
		values >> channels;
		if(channels < 2){
			channels = 2;
			cout << "Canales incompletos: Trabajando en Stereo" << endl;
		}
		testadmin = SoundAdminPortaudio::getSoundAdminInstance();
		testadmin->setConfiguration(SoundAdminPortaudio::SR_44100_HZ, channels, argv[3],
				SoundAdminPortaudio::MONO);
		if(!testadmin->addFile("testFile", file)){
			throw string("No se pudo cargar el archivo...");
		}
		testadmin->createSource("testsrc", "testFile", false);
		testadmin->playSource("testsrc",false);
		char c;
		while(c != 'o'){
			cin >> c;
			switch(c){
			case 'a': 
				testadmin->attachSource("testsrc", "testFile");
				cout << "Fuente asociada" << endl;
				break;
			case 'd': 
				testadmin->detachSource("testsrc");
				cout << "Fuente Disociada" << endl;
				break;
			case 'w': 
				if(testadmin->deleteFile("testFile"))
					cout << "archivo borrado" << endl;
				else
					cout << "archivo no se pudo borrar" << endl;
				break;
			case 'p': 
				if(testadmin->isPlaying("testsrc"))
					testadmin->pauseSource("testsrc");
				else
					if(testadmin->isLooped("testsrc"))
						testadmin->playSource("testsrc");
					else
						testadmin->playSource("testsrc", false);
					break;
			case 'f': 
				if(testadmin->addFile("testFile", file))
					cout << "archivo creado" << endl;
				else
					cout << "archivo no se pudo crear" << endl;
				break;
			case 's': 
				if(testadmin->destroySource("testsrc"))
					cout << "Fuente destruida" << endl;
				else
					cout << "Fuente no se pudo destruir" << endl;
				break;
			case 'r': 
				testadmin->createSource("testsrc", "testFile", false); break;
			case 'y': 
				if(testadmin->isPlaying("testsrc"))
					testadmin->stopSource("testsrc");
				else
					if(testadmin->isLooped("testsrc"))
						testadmin->playSource("testsrc");
					else
						testadmin->playSource("testsrc", false);
				break;
			case 'l': 
				if(!testadmin->isLooped("testsrc"))
					testadmin->loopSource("testsrc", true);
				else
					testadmin->loopSource("testsrc", false);
			default: break;
			}
		}
	}
	catch(string s){
		cout << s << endl;
	}
	return 0;
}

#endif
