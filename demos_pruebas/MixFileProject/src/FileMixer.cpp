#include "FileMixer.h"

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
int fileCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData );

int fileCallback(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags, void *userData ){
	FileMixer* data = (FileMixer*) userData;
	(void) inputBuffer;
	(void) statusFlags;
	(void) timeInfo;
	return data->mixFunc(outputBuffer, framesPerBuffer);
}


FileMixer::FileMixer(int sampleRate) throw (string)
{
	srand(time(NULL));
	startPortaudio();
	numchannels = 2;
	const PaDeviceInfo *deviceInfo;
	int numdev = Pa_GetDeviceCount();
// 	cout << numdev << endl;
	vector<int> avail;
	for(int i = 0; i < numdev; i++){
		deviceInfo = Pa_GetDeviceInfo(i);

		if(deviceInfo->maxOutputChannels > 0){
// 			cout << "Código Dispositivo: " << i << endl;
// 			cout << "Nombre Dispositivo: " << deviceInfo->name << endl;
// 			cout << "Número canales salida : " << deviceInfo->maxOutputChannels << endl;
// 			cout << "Sample rate: " << deviceInfo->defaultSampleRate << endl;
			vector<int>* supported = getSampleRates(i);
			unsigned int j = 0;
			bool out = false;
			while(j < supported->size() && !out){
				if(supported->at(j) == sampleRate){
					avail.push_back(i);
					out = true;
				}
				j++;
			}
			delete supported;
		}
	}
	
	if(avail.size() > 0){
		PaStreamParameters params;
		int device = avail.front();
		params.device = device;
		deviceInfo = Pa_GetDeviceInfo(params.device);
		params.channelCount = 2;//deviceInfo->maxOutputChannels;
		params.hostApiSpecificStreamInfo = NULL;
		params.suggestedLatency = deviceInfo->defaultLowOutputLatency;
		params.sampleFormat = paFloat32;
		params.sampleFormat |= paNonInterleaved;
		PaError err = Pa_OpenStream(&stream, NULL, &params, sampleRate,
			paFramesPerBufferUnspecified, paClipOff | paDitherOff, fileCallback, this);
		if(err != paNoError){
			string msg("Error al abrir el stream: ");
			string end( Pa_GetErrorText(err));
			throw string(msg + end);
		}
	}
	else
		throw string("No hay dispositivos que cumplan con ese sample Rate");
}

FileMixer::~FileMixer()
{
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
}

void FileMixer::startPortaudio() throw (string){
	PaError err;
	err = Pa_Initialize();
	if(err != paNoError){
		string str("Error al incializar PortAudio ");
		string end(Pa_GetErrorText(err));
		throw string(str + end);
	}
}

int FileMixer::mixFunc(void *outputBuffer, unsigned long framesPerBuffer){
	float ** output = (float**) outputBuffer;
	float *out = new float[framesPerBuffer];

	for(unsigned int frames = 0; frames < framesPerBuffer; frames++)
		for(int chan = 0; chan < numchannels; chan++)
			output[chan][frames] = 0.0f;
	float scaleFactor = 2.0f;
	map<string, Source*>::iterator it;
	for(it = sources.begin(); it != sources.end(); it++){
		Source *src = (*it).second;
		int read = src->readFromFile(out, framesPerBuffer);
		if(read > 0)
			for(int frames = 0; frames < read; frames++){
				for(int chan = 0; chan < numchannels; chan++){
// 					for(int k = 0; k < read; k++)
					output[chan][frames] += out[frames]*src->getGain();
					output[chan][frames] *= scaleFactor;
				}
			}
	}
	//Clipping
	for(unsigned int frames = 0; frames < framesPerBuffer; frames++)
		for(int chan = 0; chan < numchannels; chan++){
			output[chan][frames] = (output[chan][frames] > 1.0 ? 1.0 : output[chan][frames]);
			output[chan][frames] = (output[chan][frames] < -1.0 ? -1.0 : output[chan][frames]);
//			cout << output[chan][frames] << endl;
		}

	return paContinue;
}

vector<int>* FileMixer::getSampleRates(int deviceNumber){
	int numDefaultRates = 7;
	int defaultRates[] = {8000,11025,16000,22050,44100,48000,96000};
	vector<int> *supported = new vector<int>();
	if(Pa_GetDeviceInfo(deviceNumber)->maxOutputChannels> 0){
		PaStreamParameters params;
		params.device = deviceNumber;
		params.channelCount = 2;
		params.sampleFormat = paFloat32 | paNonInterleaved;
		params.suggestedLatency = 100;
		params.hostApiSpecificStreamInfo = NULL;
		for (int i = 0; i < numDefaultRates; i++)
		{
			if (Pa_IsFormatSupported(NULL, &params, defaultRates[i]) == 0)
				supported->push_back(defaultRates[i]);
		}
	}
	return supported;
}

void FileMixer::createSource(string id, const char* fileRoute){
	if(sources.count(id) == 0){
		SndfileHandle *snd = new SndfileHandle(fileRoute, SFM_READ, 0, 0, 0);
		if(snd->refCount() > 0){
			Source *src = new Source(id);
			src->setFile(snd);
			int r = rand()%5+1;
			src->setGain((float)r/10);
			sources[id] = src;
			
			cout << "Fuente Creada: " << id << endl;
		}
	}
}

void FileMixer::startPlaying() throw (string){
	PaError err;
	err = Pa_StartStream(stream);
	if(err != paNoError){
		string str("Error al iniciar reproducción PortAudio ");
		string end(Pa_GetErrorText(err));
		throw string(str + end);
	}
}

void FileMixer::changeRandomGain(){
	int r = rand()%10 + 1;
	int s = rand()%sources.size();
	map<string, Source*>::iterator it;
	int i = 0;
	for(it = sources.begin(); it != sources.end() && i != s; it++, i++);
	float f = (float)1/r;
	Source *src = (*it).second;
	src->setGain(f);
	cout << "Ganancia cambiada: (" << src->getId() << "," << src->getGain() << ")" << endl;
}
