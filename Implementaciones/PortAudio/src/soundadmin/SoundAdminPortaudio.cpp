#include "SoundAdminPortaudio.h"

int portaudioCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData );

/**
 * Instancia única del sistema de audio
 * */
auto_ptr<SoundAdminPortaudio> SoundAdminPortaudio::singletonInstance;

SoundAdminPortaudio::SoundAdminPortaudio()
{
	numspeakers = 0;
	stream = NULL;
	srand(time(NULL));
	initializeAdministrator();
//	initializeSineTable();
	listener = new Listener();
	loader = new AudioFileLoader();
}

SoundAdminPortaudio::~SoundAdminPortaudio()
{
	closeAdministrator();
	delete listener;
	sources.clear();
	delete loader;
}

void SoundAdminPortaudio::initializeAdministrator() throw (string){
	PaError err;
	err = Pa_Initialize();
	string str = "";
	if(err != paNoError){
		str = string("Error al incializar PortAudio ") + string(Pa_GetErrorText(err));
		throw str;
	}
}

void SoundAdminPortaudio::closeAdministrator() throw (string){
	if(stream){
		stopAllSources();
		PaError err;
		string str;
		err = Pa_CloseStream(stream);
		if(err != paNoError){
			str = string("Error al Cerrar el Flujo PortAudio ") + string(Pa_GetErrorText(err));
			cout << str << endl;
		}
		Pa_Terminate();
		cout << "Se ha detenido el sistema " << endl;
	}
	else{
		cout << "No se ha inicializado nada! " << endl;
	}
}

void SoundAdminPortaudio::setConfiguration(SampleRate sr, int numSpeak,
		const char* configFile, SoundDistribution distr) throw (string){
	
	PaError err;
	if(stream){
		closeAdministrator();
		initializeAdministrator();
	}
	if(numSpeak < 2)
		throw new string("No se puede tener sonido espacializado con menos de 2 cajas");
	int sampleRate = 0;
	switch(sr){
		case SR_8000_HZ: sampleRate = 8000;
			break;
		case SR_11025_HZ: sampleRate = 11025;
			break;
		case SR_16000_HZ: sampleRate = 16000;
			break;
		case SR_22050_HZ: sampleRate = 22050;
			break;
		case SR_44100_HZ: sampleRate = 44100;
			break;
		case SR_48000_HZ: sampleRate = 48000;
			break;
		case SR_96000_HZ: sampleRate = 96000;
			break;
		default: throw string("Sample Rate no Soportado!");
	}
	vector<int> *supported = availableDevices(numSpeak, sampleRate);
	
	//tomamos el primer dispositivo. Todos deberían poder soportar el sistema
	if(supported->empty())
		throw string("No hay dispositivos disponibles que puedan manejar el sistema");

	int device = supported->front();
	PaStreamParameters params;
	params.device = device;
	const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(params.device);
	params.channelCount = deviceInfo->maxOutputChannels;
	params.hostApiSpecificStreamInfo = NULL;
	params.suggestedLatency = deviceInfo->defaultLowOutputLatency;
	params.sampleFormat = paFloat32;
	params.sampleFormat |= paNonInterleaved;

	//imprimimos información del dispositivo
	cout << "Código Dispositivo: " << device << endl;
	cout << "Nombre Dispositivo: " << deviceInfo->name << endl;
	cout << "Número canales salida : " << deviceInfo->maxOutputChannels << endl;
	cout << "Sample rate: " << deviceInfo->defaultSampleRate << endl;
	const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
	cout << "Código API: " << deviceInfo->hostApi << endl;
	cout << "Nombre API: " << apiInfo->name<< endl;
	cout << "Tipo : " << apiInfo->type << endl;
	cout << "------------------------------" << endl << endl;
	err = Pa_OpenStream(&stream, NULL, &params, sampleRate, 
			paFramesPerBufferUnspecified, paClipOff, portaudioCallback, this);

	if(err != paNoError){
		string str = string("Error al abrir el stream: ") + string(Pa_GetErrorText(err));
		throw str;
	}
	numspeakers = numSpeak;

	switch(distr){
		case STEREO: case DISTRIBUTED: 
			distribution = (numspeakers == 2 ? STEREO : DISTRIBUTED);
			break;		
		default: distribution = MONO;
			break;
	}
	// Creamos lo necesario para el algoritmo de espacialización
	if(panning_algorithm)
		delete panning_algorithm;
	if(distribution == DISTRIBUTED){
		//La distribución se crea solamente si hay más de 2 speakers y no es mono
		panning_algorithm = new Vbap(numspeakers);
		
		if(!panning_algorithm->createSpeakers(configFile))
			throw string("No se pudo usar el archivo de configuración! \n"
					"Por favor revíselo");
		panning_algorithm->createAllTriangles();
		panning_algorithm->removeTriangles();
	}
	delete supported;
	startReproduction();
}

void SoundAdminPortaudio::startReproduction() throw (string){
	if(stream){
		PaError err = Pa_StartStream(stream);
		if(err != paNoError){
			string str = string("Error al iniciar el stream: ") + string(Pa_GetErrorText(err));
			throw str;
		}
	}
	else
		throw string("El flujo portaudio no ha sido abierto");
}

//Manejo de instancias (Singleton)

SoundAdminPortaudio* SoundAdminPortaudio::getSoundAdminInstance(){
	if(singletonInstance.get() == NULL)
		singletonInstance.reset(new SoundAdminPortaudio);
	return singletonInstance.get();
}

//creación y manejo de fuentes

void SoundAdminPortaudio::createSource(string id, string fileDesc, bool amb){
	if(sources.count(id) == 0){
		Source *src = new Source(id);
		src->setFileName(fileDesc);
		src->setAdvance(0);
		src->setAmbient(amb);
		src->setGain(1);
		sources[id] = src;
	}
}

void SoundAdminPortaudio::stopAllSources(){
	if(Pa_IsStreamActive(stream))
		Pa_StopStream(stream);
	map<string, Source*>::iterator it;
	for(it = sources.begin(); it != sources.end(); it++)
		(it->second)->stopSource();
}

void SoundAdminPortaudio::playSource(string id, bool loop){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->playSource();
		src->setLooped(loop);
	}
}

void SoundAdminPortaudio::pauseSource(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->pauseSource();
	}
}

void SoundAdminPortaudio::stopSource(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->stopSource();
	}
}

bool SoundAdminPortaudio::isPlaying(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		return src->isPlaying();
	}
	return false;
}

void SoundAdminPortaudio::loopSource(string id, bool loop){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->setLooped(loop);
	}
}

bool SoundAdminPortaudio::isLooped(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		return src->isLooped();
	}
	return false;
}

void SoundAdminPortaudio::updateSource(string id, float x, float y, float z){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->update(x, y, z);
	}
}

bool SoundAdminPortaudio::attachSource(string id, string desc){
	if(loader->fileExists(desc)){
		if(sources.count(id) > 0){
			Source *src = sources[id];
			src->stopSource();
			src->setFileName(desc);
			return true;
		}
		return false;
	}
	return false;
}

void SoundAdminPortaudio::detachSource(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->stopSource();
		src->setFileName("");
	}
}

bool SoundAdminPortaudio::destroySource(string id){
	if(sources.count(id) > 0){
		sources.erase(id);
		return (sources.count(id) == 0); // si devuelve false es que no se eliminó
	}
	else{
		return false;
	}
}

void SoundAdminPortaudio::useSourceAsAmbient(string id, bool ambient){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		src->setAmbient(ambient);
	}
}

bool SoundAdminPortaudio::isAmbient(string id){
	if(sources.count(id) > 0){
		Source *src = sources[id];
		return src->isAmbient();
	}
	return false;
}

void SoundAdminPortaudio::setVolume(string id, float gain){
	if(sources.count(id) > 0){
		if(gain >= 0.0 && gain <= 1.0)
			sources[id]->setGain(gain);
	}
}

bool SoundAdminPortaudio::getVolume(string id, float& gain){
	if(sources.count(id) > 0){
		gain = sources[id]->getGain();
		return true;
	}
	return false;
}

void SoundAdminPortaudio::setPitchBend(string id, float bend){
	if(sources.count(id) > 0){
		if(bend >= 0.0 && bend <= 2.0)
			sources[id]->setPitch(bend);
	}
}

bool SoundAdminPortaudio::getPitchBend(string id, float& bend){
	if(sources.count(id) > 0){
		bend = sources[id]->getPitch();
		return true;
	}
	return false;
}

bool SoundAdminPortaudio::getSourcePosition(string id, float& x, float& y, float& z){
	if(sources.count(id) > 0){
		float *pos = sources[id]->getPosition();
		x = pos[0];
		y = pos[1];
		z = pos[2];
		return true;
	}
	return false;
}

//manejo listener

void SoundAdminPortaudio::updateListener(Vec3f pos, Vec3f up){
	listener->setPosition(pos);
	listener->setOrientation(up);
}

void SoundAdminPortaudio::getListenerInfo(Vec3f& pos, Vec3f& up){
	pos = listener->getPosition();
	up = listener->getOrientation();
}

//manejo de archivos

bool SoundAdminPortaudio::addFile(string desc, string fileRoute){
	if(loader->fileExists(desc))
		return false;
	else{
		try{
			loader->loadFile(desc, fileRoute);
			return true;
		}
		catch(string s)
		{
			cout << s << endl;
			return false;
		}
	}
}

bool SoundAdminPortaudio::deleteFile(string desc){
	if(loader->fileExists(desc)){
		map<string, Source*>::iterator it;
		for(it = sources.begin(); it != sources.end(); it++){
			Source *src = it->second;
			if(src->getFileName() == desc)
				detachSource(src->getId());
		}
		return loader->deleteFile(desc);
	}
	return true;
}

//Vec3f SoundAdminPortaudio::changeCoordinatesFromListener(Vec3f src){
//	//TODO No tiene en cuenta la orientación de la fuente
//	Vec4f s, list;
//	Vec3f temp = listener->getPosition();
//	for(int i = 0; i < 3; i++){
//		s[i] = src[i];
//		list[i] = temp[i];
//	}
//	s[3] = list[3] = 1;
//	
//	Matrix44f mat;
//	for(int i = 0; i < 4; i++)
//		mat[i][3] = list[i];
//	gmtl::transpose(mat);
//	Vec4f res = mat*s;
//	Vec3f result;
//	for(int i = 0; i < 3; i++)
//		result[i] = res[i];
//	return result;
//}

//información de debug y consulta de fuentes y archivos

vector<string>* SoundAdminPortaudio::getSourceIds(){
	vector<string>* vec = new vector<string>();
	map<string, Source*>::iterator it;
	for(it = sources.begin(); it != sources.end(); it++)
		vec->push_back(it->first);
	return vec;
}

vector<string>* SoundAdminPortaudio::getFileDescriptions(){
	return loader->getFileDescriptions();
}

//manejo portaudio

int portaudioCallback(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags, void *userData ){
	SoundAdminPortaudio* data = (SoundAdminPortaudio*) userData;
	(void) inputBuffer;
	(void) statusFlags;
	(void) timeInfo;
	return data->soundAdminCallback(outputBuffer, framesPerBuffer);
}

int SoundAdminPortaudio::soundAdminCallback(void *outputBuffer, unsigned long int framesPerBuffer){
	float ** output = (float **) outputBuffer;
	for(int frames = 0; frames < (int)framesPerBuffer; frames++){
		for(int chan = 0; chan < numspeakers; chan++){
			output[chan][frames] = 0.0;
		}
	}
	
	int numPlaying = 0;
	
	float *buffer = new float[framesPerBuffer];
	//destrucción fuera del stack frame (para evitar memory leaks)
	auto_ptr<float> destr(buffer);
	Vec3f listPos = listener->getPosition();
	map<string, Source*>::iterator it;
	for(it = sources.begin(); it != sources.end(); it++){
		Source *src = it->second;
		if(src->isPlaying() && src->getFileName() != ""){
			int pos = src->getAdvance();
			int read = loader->readFromFile(buffer, src->getFileName(), &pos, framesPerBuffer, src->isLooped());
			float *posS = src->getPosition();
			Vec3f posSrc;
			posSrc[0] = posS[0];
			posSrc[1] = posS[1];
			posSrc[2] = posS[2];
			float atten = Vbap::getDistanceAttenuation(posSrc, listPos);
			//Salida de una fuente ambiente
			//Las fuentes que se encuentran demasiado cerca del listener se reproducen como
			//fuentes ambiente
			if(src->isAmbient()){
				for(int chan = 0; chan < numspeakers; chan++)
					for(int frames = 0; frames < read; frames++)
						output[chan][frames] += buffer[frames]*src->getGain();
			}
			else{
				if(distribution == DISTRIBUTED){
					pair<Vec3f, int*>* res = 
						panning_algorithm->selectTriangle(posS[0], posS[1], posS[2]);
					int *chans = res->second;
					Vec3f gain = res->first;
					for(int frames = 0; frames < read; frames++){
						output[chans[0]][frames] += buffer[frames]*gain[0]*atten;
						output[chans[1]][frames] += buffer[frames]*gain[1]*atten;
						output[chans[2]][frames] += buffer[frames]*gain[2]*atten;
					}
				}
				else if(distribution == STEREO){
					Vec2f gain = Vbap::getStereoGain(posS[0], posS[1], posS[2]);
					for(int frames = 0; frames < read; frames++){
						output[0][frames] += buffer[frames]*gain[0]*atten;
						output[1][frames] += buffer[frames]*gain[1]*atten;
					}
				}
				else{//Mono
					for(int frames = 0; frames < read; frames++){
						for(int chan = 0; chan < numspeakers; chan++)
							output[chan][frames] += buffer[frames]*atten;
					}
				}
			}
			src->setAdvance(pos);
			numPlaying++;
			if(pos == 0 && !src->isLooped())
				src->stopSource();
		}
	}
	
	if(numPlaying > 0){
		float scale = (numPlaying < 10 ? (float) 1.0f/10 : (float) 1.0f/(numPlaying*2));
		//	cout << scale << endl;
		//clipping y escalamiento
		for(int frames = 0; frames < (int)framesPerBuffer; frames++){
			for(int chan = 0; chan < numspeakers; chan++){
				//Multiplicamos por un factor de escalamiento para evitar clipping
				output[chan][frames] *= scale;
				//clipping
				if(output[chan][frames] > 1.0)
					output[chan][frames] = 1.0;
				else if(output[chan][frames] < -1.0)
					output[chan][frames] = -1.0;
			}
		}
	}
//	delete [] buffer;
	return paContinue;
}

//métodos privados

//void SoundAdminPortaudio::workWithSinusoidalSources(float** output, int framesPerBuffer){
//	Vec3f listPos = listener->getPosition();
//	map<string, Source*>::iterator it;
//	if(distribution == DISTRIBUTED){
//		for(int frames = 0; frames < (int)framesPerBuffer; frames++){
//			for(it = sources.begin(); it != sources.end(); it++){
//				Source *src = (*it).second;
//				if(src->isPlaying()){
//					float value = sine[(int)src->getPitch()];
//					if(src->isAmbient()){
//						for(int chan = 0; chan < numspeakers; chan++)
//							output[chan][frames] += value*src->getGain(); 
//					}
//					else{
//						float *pos = src->getPosition();
//						pair<Vec3f, int*>* res = 
//							panning_algorithm->selectTriangle(pos[0], pos[1], pos[2]);
//						Vec3f posSrc;
//						posSrc[0] = pos[0];
//						posSrc[1] = pos[1];
//						posSrc[2] = pos[2];
//						float atten = Vbap::getDistanceAttenuation(posSrc, listPos);
//						int *chans = res->second;
//						Vec3f gain = res->first;
//						output[chans[0]][frames] += value*gain[0]*atten;
//						output[chans[1]][frames] += value*gain[1]*atten;
//						output[chans[2]][frames] += value*gain[2]*atten;
//					}
//					int val = src->getPhase() + src->getAdvance();
//					int adv = (val >= TABLE_SIZE ? val - TABLE_SIZE: val);
//					src->setPitch((float)adv);
//					//cout << adv << endl;
//				}
//			}
//		}
//	}
//	else if(distribution == STEREO){
//		for(int frames = 0; frames < (int)framesPerBuffer; frames++){
//			for(it = sources.begin(); it != sources.end(); it++){
//				Source *src = (*it).second;
//				if(src->isPlaying()){
//					float value = sine[(int)src->getPitch()];
//					if(src->isAmbient()){
//						for(int chan = 0; chan < numspeakers; chan++)
//							output[chan][frames] += value*src->getGain(); 
//					}
//					else{
//						float *pos = src->getPosition();
//						Vec3f posSrc;
//						posSrc[0] = pos[0];
//						posSrc[1] = pos[1];
//						posSrc[2] = pos[2];
//						float atten = Vbap::getDistanceAttenuation(posSrc, listPos);
//						Vec2f gain = Vbap::getStereoGain(pos[0], pos[1], pos[2]); 
//						output[0][frames] += value*gain[0]*atten;
//						output[1][frames] += value*gain[1]*atten;
//					}
//					int val = src->getPhase() + src->getAdvance();
//					int adv = (val >= TABLE_SIZE ? val - TABLE_SIZE: val);
//					src->setPitch((float)adv);
//				}
//			}
//		}
//	}
//	else{
//		for(int frames = 0; frames < (int)framesPerBuffer; frames++){
//			for(int chan = 0; chan < numspeakers; chan++){
//				float sum = 0.0f;
//				for(it = sources.begin(); it != sources.end(); it++){
//					Source *src = ((*it).second);
//					if(src->isPlaying()){
//						float *pos = src->getPosition();
//						Vec3f posSrc;
//						posSrc[0] = pos[0];
//						posSrc[1] = pos[1];
//						posSrc[2] = pos[2];
//						float atten = Vbap::getDistanceAttenuation(posSrc, listPos);
//						sum += sine[(int)src->getPitch()]*atten;//*src->getGain();
//						//cout << "source: " << src->getId() << " " << src->getPhase() << " " << sine[src->getPhase()] << endl;
//						int val = src->getPhase() + src->getAdvance();
//						int adv = (val >= TABLE_SIZE ? val - TABLE_SIZE: val); 
//						//cout << adv << endl;
//						src->setPitch((float)adv);
//					}
//				}
//				output[chan][frames] = sum;
//			}
//		}
//	}
//}

vector<int>* SoundAdminPortaudio::availableDevices(int numChannels, int samplerate){
	vector<int>* vec = new vector<int>();
	int numdev = Pa_GetDeviceCount();
	for(int i = 0; i < numdev; i++){
		const PaDeviceInfo *deviceInfo;
		deviceInfo = Pa_GetDeviceInfo(i);
		if(deviceInfo->maxOutputChannels >= numChannels){
			
			vector<int>* supported = getSupportedSampleRates(i);
			while(!supported->empty()){
				int sr = supported->back(); 
				if(sr == samplerate)
					vec->push_back(i);
				supported->pop_back();
			}
			delete supported;
		}
	}
	return vec;
}

vector<int>* SoundAdminPortaudio::getSupportedSampleRates(int deviceNumber){
	int numDefaultRates = 7;
	int defaultRates[] = {8000,11025,16000,22050,44100,48000,96000};
	vector<int> *supported = new vector<int>();
	PaStreamParameters params;
	params.device = deviceNumber;
	params.channelCount = Pa_GetDeviceInfo(deviceNumber)->maxOutputChannels;
	params.sampleFormat = paFloat32 | paNonInterleaved;
	params.suggestedLatency = 100;
	params.hostApiSpecificStreamInfo = NULL;
	for (int i = 0; i < numDefaultRates; i++)
	{
		if (Pa_IsFormatSupported(NULL, &params, defaultRates[i]) == 0)
			supported->push_back(defaultRates[i]);
	}
	return supported;
}

//void SoundAdminPortaudio::initializeSineTable(){
//	for(int i=0; i<TABLE_SIZE; i++ )
//		sine[i] = (float) gmtl::Math::sin(((double)i/(double)TABLE_SIZE) * M_PI * 2. );
//}
