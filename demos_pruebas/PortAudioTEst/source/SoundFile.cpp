#include "SoundFile.h"

/**
* Función Estática para portaudio
*
*/
int fileCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData );

int fileCallback(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags, void *userData ){
	SoundFile* data = (SoundFile*) userData;
	(void) inputBuffer;
	(void) statusFlags;
	(void) timeInfo;
	return data->readFile(outputBuffer, framesPerBuffer);
}

SoundFile::SoundFile(const char* filename, int numchan)
{
	sndfile = new SndfileHandle(filename, SFM_READ, 0, 0, 0);
	position = 0;
	numchannels = numchan;
	initializeSineTable();
	srand(time(NULL));
	selectedChannel= 0;
}

SoundFile::~SoundFile()
{
	sources.clear();
	delete sndfile;
}

void SoundFile::initializeSineTable(){
	for(int i=0; i<TABLE_SIZE; i++ )
	{
		sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
		//cout << "Seno No. " << i << " : " << sine[i] << endl;
	}
	
}

vector<int>* SoundFile::getSampleRates(int deviceNumber){
	int numDefaultRates = 7;
	int defaultRates[] = {8000,11025,16000,22050,44100,48000,96000};
	vector<int> *supported = new vector<int>();
	if(Pa_GetDeviceInfo(deviceNumber)->maxOutputChannels>= numchannels){
		PaStreamParameters params;
		params.device = deviceNumber;
		params.channelCount = 2;
		params.sampleFormat = paFloat32;
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

void SoundFile::printDeviceInfo(){
	
	const PaDeviceInfo *deviceInfo;
	int numdev = Pa_GetDeviceCount();
	cout << "Device Count " << numdev << endl << endl;
	for(int i = 0; i < numdev; i++){
		deviceInfo = Pa_GetDeviceInfo(i);
		if(deviceInfo->maxOutputChannels >= numchannels){
			cout << "Código Dispositivo: " << i << endl;
			cout << "Nombre Dispositivo: " << deviceInfo->name << endl;
			cout << "Número canales salida : " << deviceInfo->maxOutputChannels << endl;
			cout << "Sample rate: " << deviceInfo->defaultSampleRate << endl;
			vector<int>* supported = getSampleRates(i);
			cout << "Sample Rate Soportadas: " << endl;
			while(!supported->empty()){
				cout << supported->back() << " ; ";
				supported->pop_back();
			}
			cout << endl;
			cout << "API: " << deviceInfo->hostApi << endl;
			cout << "------------------------------" << endl << endl;
			delete supported;
		}
	}
	
}

void SoundFile::printApiInfo(){	
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

bool SoundFile::startPortaudio(){
	PaError err;
	err = Pa_Initialize();
	if(err != paNoError){
		cout << "Error al incializar PortAudio " << Pa_GetErrorText(err) << endl;
		return false;
	}
	return true;
}

bool SoundFile::addSource(string id){
	if(sources.find(id) != sources.end())
		return false;
	else{
		Source* src = new Source(id);
		//src->setPhase(rand()%TABLE_SIZE);
		src->setAdvance((rand()%100+1));
		src->setGain(0.5f);
		sources[id] = src;
		return true;
	}
}

bool SoundFile::initialize(int device){
	PaError err;
	PaStreamParameters params; // parámetros de uso
	const PaDeviceInfo *deviceInfo; //información del device disponible
	if(device >= Pa_GetDeviceCount() || Pa_GetDeviceInfo(device)->maxOutputChannels == 0)
	{
		cout << "Ha escogido un dispositivo inválido" << endl;
		return false;
	}
	vector<int>* sup = getSampleRates(device);
	params.device = device;
	deviceInfo = Pa_GetDeviceInfo(params.device);
	params.channelCount = deviceInfo->maxOutputChannels;
	params.hostApiSpecificStreamInfo = NULL;
	params.suggestedLatency = deviceInfo->defaultLowOutputLatency;
	params.sampleFormat = paFloat32;
	params.sampleFormat |= paNonInterleaved;
	/*información de debug*/
	cout << endl << deviceInfo->name << endl << endl;
	err = Pa_OpenStream(&stream, NULL, &params, sup->front(),
		paFramesPerBufferUnspecified, paClipOff, fileCallback, this);

	if(err != paNoError){
		cout << "Error al abrir el stream: " << Pa_GetErrorText(err) << endl;
		return false;
	}
	delete sup;
	return true;
}

bool SoundFile::startPlaying(){
	
	PaError err = Pa_StartStream(stream);
	if(err != paNoError){
		cout << "Error al inciar el stream: " << Pa_GetErrorText(err) << endl;
		return false;
	}
	return true;
}

bool SoundFile::stopPlaying(){
	PaError err = Pa_StopStream(stream);
	if(err != paNoError){
		cout << "Error al detener el stream: " << Pa_GetErrorText(err) << endl;
		return false;
	}

	err = Pa_CloseStream(stream);
	if(err != paNoError){
		cout << "Error al cerrar el stream: " << Pa_GetErrorText(err) << endl;
		return false;
	}
	return true;
}

void SoundFile::finishPortaudio(){
	Pa_Terminate();
}

int SoundFile::readFile(void *outputBuffer, unsigned long framesPerBuffer){
// 	int thisSize = framesPerBuffer;
// 	int thisRead;
// 	float *out = (float*) outputBuffer;
// 	float *cursor = new float[thisSize]; /* set the output cursor to the beginning */
// 	for(int i = 0; i < framesPerBuffer*2; i++)
// 		out[i] = 0.0;
// 	//cout << position << endl;
// 	while (thisSize > 0)
// 	{
// 		/* seek to our current file position */
// 		sndfile->seek(position, SEEK_SET);
// 
// 		/* are we going to read past the end of the file?*/
// 		if (thisSize > (sndfile->frames() - position))
// 		{
// 			/*if we are, only read to the end of the file*/
// 			thisRead = sndfile->frames() - position;
// 			/* and then loop to the beginning of the file */
// 			position = 0;
// 		}
// 		else
// 		{
// 			/* otherwise, we'll just fill up the rest of the output buffer */
// 			thisRead = thisSize;
// 			/* and increment the file position */
// 			position += thisRead;
// 		}
// 
// /* since our output format and channel interleaving is the same as
// 		sf_readf_int's requirements */
// 		/* we'll just read straight into the output buffer */
// 		sndfile->read(cursor, thisRead);
// 		/* increment the output cursor*/
// 		for(int i = 0; i < thisRead; i++){
// 			*(out++) = cursor[i];
// 			*(out++) = cursor[i]*0.5f;
// 		}
// 		/* decrement the number of samples left to process */
// 		thisSize -= thisRead;
// 	}
	
// 	float* salida = (float*) outputBuffer;
// 	float* buffer = new float[framesPerBuffer];
// 	sf_count_t items = 0;
// 	items = sndfile->readf(buffer, framesPerBuffer);
// 	cout << "?" << endl;
// 	for(unsigned int i = 0; i < framesPerBuffer; i++){
// 		
// 		*(salida++) = buffer[i];
// 		*(salida++) = buffer[i]*0.5f;
// 		
// 	}
	
//	float* salida = (float*) outputBuffer;
//	float* tempBuffer = new float[framesPerBuffer*sources.size()];
//	map<string, Source*>::iterator it;
//	for(int i = 0; i < (int)framesPerBuffer*sources.size(); i++){
//		salida[i] = tempBuffer[i] =0.0;
//	}
//	for(int i = 0; i < (int)framesPerBuffer*sources.size(); i++){
//		for(it = sources.begin(); it != sources.end(); it++){
//			Source * src = ((*it).second);
//			tempBuffer[i] += src->getPhase();
//		}	
//	}
//	
//	for(unsigned int i=0; i<sources.size(); i++ )
//	{
//		for(it = sources.begin(); it != sources.end(); it++){
//			Source * src = ((*it).second);
//			salida[i] += src->getGain()*tempBuffer[i];
//			salida[i+1] += src->getGain()*tempBuffer[i];
//		}
//		
//	}
//	for(int i = 0; i < (int)framesPerBuffer*sources.size(); i++)
//	{
//		float f = salida[i];
//		if (f > 1.0)
//			salida[i] = 1.0;
//		else if (f < -1.0)
//			salida[i] = -1.0;
//	}
	
	float** output = (float**) outputBuffer;
	//TODO tener en cuenta para manejar escalamiento sin clipping
	//float scaleFactor = 0.9;//(sources.size() > 2 ? (float)1/5:(float)1/2);
	//inicializar
	for(int frames = 0; frames < (int)framesPerBuffer; frames++){
		for(int chan = 0; chan < numchannels; chan++)
			output[chan][frames] = 0.0;
	}
	map<string, Source*>::iterator it;
#if SPK_TEST == 0
	for(int frames = 0; frames < (int)framesPerBuffer; frames++){
		float sum = 0.0f;
		for(it = sources.begin(); it != sources.end(); it++){
			Source *src = ((*it).second);
			sum += sine[src->getPhase()]*src->getGain();
			int val = src->getPhase() + src->getAdvance();
			int adv = (val >= TABLE_SIZE ? val - TABLE_SIZE: val); 
			//cout << adv << endl;
			src->setPhase(adv);
		}
		output[selectedChannel][frames] = sum;
	}
	
#else
	for(int frames = 0; frames < (int)framesPerBuffer; frames++){
		for(int chan = 0; chan < numchannels; chan++){
			float sum = 0.0f;
			for(it = sources.begin(); it != sources.end(); it++){
				Source *src = ((*it).second);
				sum += sine[src->getPhase()]*src->getGain();
				//cout << "source: " << src->getId() << " " << src->getPhase() << " " << sine[src->getPhase()] << endl;
				int val = src->getPhase() + src->getAdvance();
				int adv = (val >= TABLE_SIZE ? val - TABLE_SIZE: val); 
				//cout << adv << endl;
				src->setPhase(adv);
				
			}
// 			TODO Revisar: Factor de escalamiento para evitar clipping
// 			output[chan][frames] = (sum*scaleFactor);
			output[chan][frames] = sum;
		}
	}
	
#endif
// Manejo de clipping
	for(int frames = 0; frames < (int)framesPerBuffer; frames++){
		for(int chan = 0; chan < numchannels; chan++){
			output[chan][frames] = (output[chan][frames] > 1.0 ? 1.0f: output[chan][frames]);
			output[chan][frames] = (output[chan][frames] < -1.0 ? -1.0f: output[chan][frames]);
// 			cout << output[chan][frames] << endl;
		}						
	}
			
	return paContinue;

}

void SoundFile::changeRandomPhase(){
	
	int s = rand()%sources.size();
	int r = rand()%10+1;
	map<string, Source*>::iterator it;
	int i = 0;
	for(it = sources.begin(); it != sources.end() && i != s; it++, i++);
	Source *src = ((*it).second);
	src->setAdvance(r);
}

void SoundFile::changeRandomGain(){
	int r = rand()%10 + 1;
	int s = rand()%sources.size();
	map<string, Source*>::iterator it;
	int i = 0;
	for(it = sources.begin(); it != sources.end() && i != s; it++, i++);
	float f = (float)1/r;
	Source *src = (*it).second;
	src->setGain(f);
}

void SoundFile::changeChannel(){
	selectedChannel++;
	selectedChannel = (selectedChannel >= numchannels ? 0 : selectedChannel);
	cout << "Canal Seleccionado: " << selectedChannel << endl;
}
