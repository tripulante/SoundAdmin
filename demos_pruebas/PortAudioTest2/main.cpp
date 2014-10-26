/**
* @mainpage Multiple Stream Test on Portaudio 
* @brief Clase que me permite probar el uso de múltiples streams en portaudio.
* Cada stream usaría un archivo diferente cargado desde un archivo de texto
* @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
*/

/**
* @brief Clase que me permite probar el uso de múltiples streams en portaudio.
* Cada stream usaría un archivo diferente cargado desde un archivo de texto
* @todo Realizar pruebas de cambio de coordenadas en tiempo real!
* @author John Jairo Palma Robayo (joh-palm@uniandes.edu.co - tripulante@gmail.com)
* @example main.cpp
*/

#import <iostream>
#import <fstream>
#import <string>
/*Importamos portaudio*/
#import <portaudio.h>
/*libsndfile*/
#import <sndfile.hh>
/*Contenedor stl*/
#import <vector>
#import <stack>
/*Threads mediante boost*/
#import <boost/thread.hpp>


using namespace std;

/*Declaración de funciones*/

/**
 * Imprime la información de apis disponibles (?)
 * */
void printApiInfo();

/**
 * Se encarga de imprimir la información de cada dispositivo de audio disponible
 * */
void printDeviceInfo();

/**
* Inicializa portaudio
* @return true si portaudio si pudo empezar, false si no.
*/
bool startPortaudio();

/**
 * Función de callback usada para la salida PortAudio
 * @param inputBuffer Toma datos de la entrada del dispositivo (no usado)
 * @param outputBuffer Envía datos al dispositivo de salida
 * @param framesPerBuffer Cuántos datos han de enviarse
 * @param timeInfo No usado
 * @param statusFlags No usado
 * @param userData Datos del usuario a usar en la función
 * @return paContinue para operación normal.
 * */
static int fileCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData );

/**
* Se encarga de cargar los archivos de audio necesarios
* @param filename Nombre del archivo del que se va a cargar lo demás
* @return false si no se pudo abrir el archivo.
*/
bool loadfile(char* filename);

/**
* Se encarga de crear los streams a partir de c/u de los archivos
* @param params Parámetros de los streams a crear
* @return true si se pudo crear al menos un stream
* @return false si no se pudo crear ningún stream
*/
bool createStreams(PaStreamParameters* params);

/**
* Inicia la ejecución de los streams
*/
bool startStreams();

/**
* detiene la ejecución de los streams
*/
bool stopStreams();

/*Declaración de variables globales a usar acá*/

/**
* Pila dedicada a cada stream
*/
vector<PaStream*> streams;

/**
* Vector dedicado a cada uno de los archivos
*/
vector<SndfileHandle*> files;

/*delaración del main*/

int main(){
	if(startPortaudio()){
		PaStreamParameters params; // parámetros de uso
		const PaDeviceInfo *deviceInfo; //información del device disponible
		
		/*imprimimos info de apis*/
		printApiInfo();
		/*imprimimos los dispositivos*/
		printDeviceInfo();

		/*Vamos a usar la Audigy Para probar las salidas!*/
		cout << "Escoja el dispositivo por el que se desea salir" << endl;
		int device = 0;
		cin >> device;
		if(device >= Pa_GetDeviceCount() )
		{
			cout << "Ha escogido un dispositivo inválido" << endl;
			return EXIT_FAILURE;
		}	
		params.device = device;
		deviceInfo = Pa_GetDeviceInfo(params.device);
		params.channelCount = deviceInfo->maxOutputChannels;
		params.hostApiSpecificStreamInfo = NULL;
		params.suggestedLatency = deviceInfo->defaultLowOutputLatency;
		params.sampleFormat = paFloat32;
		/*información de debug*/
		cout << deviceInfo->name << endl << endl;
		
		if(!loadfile("../data/testload.txt"))
			return EXIT_FAILURE;
		if(!createStreams(&params))
			return EXIT_FAILURE;
		if(!startStreams())
			return EXIT_FAILURE;
		Pa_Sleep(3*1000);
		if(!stopStreams())
			return EXIT_FAILURE;
		Pa_Terminate();
		/*borramos todos los archivos*/
		files.clear();
		return EXIT_SUCCESS;

	}
	else
		return EXIT_FAILURE;
}


bool startPortaudio(){
	PaError err;
	err = Pa_Initialize();
	if(err != paNoError){
		cout << "Error al incializar PortAudio " << Pa_GetErrorText(err) << endl;
		return false;
	}
	return true;
}
static int fileCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData ){
	/*Userdata: archivo audio*/
	/*
	* TODO Modificar para usar todos los canales disponibles.
	*/
	SndfileHandle *handle = (SndfileHandle *) userData;
	(void) inputBuffer;
	float* salida = (float*) outputBuffer;
	float* buffer = new float[framesPerBuffer];
	sf_count_t items = 0;
	items = handle->readf(buffer, framesPerBuffer);
	for(unsigned int i = 0; i < framesPerBuffer && items; i++){

		*salida++ = buffer[i];//izquierda
		*salida++ = buffer[i];//derecha
	}
	return 0;
}

bool loadfile(char* filename){
	ifstream is(filename);
	if(!is.is_open()){
		cout << "Error al cargar el archivo: " << filename << endl;
		return false;
	}
	else{
		/*asumimos sintaxis correcta*/
		string str;
		while(!is.eof()){
			getline(is, str);
			if(str[0] == ';')
				cout << "comentario: " + str << endl;
			else{
				//cout << str << endl;
				SndfileHandle *handle = new SndfileHandle(str.c_str(), SFM_READ, 0, 0);
				if(handle->refCount() != 0){
					cout << str << " SampleRate: " << handle->samplerate() << endl;
					files.push_back(handle);
				}
				else	
					cout << str + " no se pudo leer" << endl;
			}
		}
		is.close();
		if(files.size() > 0){
			cout << "Finalizada carga archivos: " << (int) files.size() << endl;
			return true;
		}
		else{
			cout << "No se cargó ningún archivo" << endl;
			return false;
		}
	}	
}

bool createStreams(PaStreamParameters* params){
	PaError err;
	vector<SndfileHandle*>::iterator it;
	for(it = files.begin(); it < files.end(); it++){
		SndfileHandle *handle = *it;
		//cout << handle->refCount() << endl;
		PaStream* stream;
		/*creamos el stream*/
		err = Pa_OpenStream(&stream, NULL, params, handle->samplerate(),
			handle->frames(), paClipOff, fileCallback, handle);
		if(err != paNoError){
			cout << "Error al abrir el stream: " << Pa_GetErrorText(err) << endl;
			return false;
		}
		streams.push_back(stream);
	}
	if(streams.size() > 0){
		cout << "streams agregados exitosamente" << endl;
		return true;
	}
	else{
		cout << "No se agregó ningún stream" << endl;
		return false;
	}
}

bool startStreams(){
	PaError err;
	vector<PaStream*>::iterator it;
	for(it = streams.begin(); it < streams.end(); it++){
		PaStream* stream = *it;
		/*iniciamos cada stream*/
		err = Pa_StartStream(stream);
		if(err != paNoError){
			cout << "Error al iniciar el stream: " << Pa_GetErrorText(err) << endl;
			return false;
		}
	}
	cout << "Todos los streams han empezado exitosamente" << endl;
	return true;
}

bool stopStreams(){
	PaError err;
	/*ciclo que va eliminado cada stream despues de cerrarlo*/
	while(!streams.empty()){
		PaStream* stream = streams.back();
		/*detenemos cada stream*/
		err = Pa_StopStream(stream);
		if(err != paNoError){
			cout << "Error al detener el stream: " << Pa_GetErrorText(err) << endl;
			return false;
		}
		Pa_CloseStream(stream);
		streams.pop_back();
	}
	cout << "Todos los streams han sido detenidos exitosamente" << endl;
	return true;
}

void printDeviceInfo(){
	
	const PaDeviceInfo *deviceInfo;
	int numdev = Pa_GetDeviceCount();
	cout << "Device Count " << numdev << endl << endl;
	for(int i = 0; i < numdev; i++){
		deviceInfo = Pa_GetDeviceInfo(i);
		cout << "Código Dispositivo: " << i << endl;
		cout << "Nombre Dispositivo: " << deviceInfo->name << endl;
		cout << "Número canales salida : " << deviceInfo->maxOutputChannels << endl;
		cout << "Sample rate: " << deviceInfo->defaultSampleRate << endl;
		cout << "API: " << deviceInfo->hostApi << endl;
		cout << "------------------------------" << endl << endl;
	}
	
}

void printApiInfo(){	
	const PaHostApiInfo *apiInfo;
	int numdev = Pa_GetHostApiCount();
	cout << "Api Count " << numdev << endl;
	for(int i = 0; i < numdev; i++){
		apiInfo = Pa_GetHostApiInfo(i);
		cout << "Código Api: " << i << endl;
		cout << "Nombre Api: " << apiInfo->name<< endl;
		cout << "Tipo : " << apiInfo->type << endl;
		cout << "------------------------------" << endl << endl;
	}
}
