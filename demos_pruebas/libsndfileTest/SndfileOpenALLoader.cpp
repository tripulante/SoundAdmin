#include "SndfileOpenALLoader.h"

SndfileOpenALLoader::SndfileOpenALLoader()
{
	handle = NULL;
}

SndfileOpenALLoader::~SndfileOpenALLoader()
{
	delete handle;
}

void SndfileOpenALLoader::loadFile(string file) throw (string){

	handle = new SndfileHandle(file.c_str(), SFM_READ);
	if(handle->refCount()== 0){
		throw string("Error al cargar el archivo");// + string(handle->strError());
	}
	else{
		printFileInfo();
	}
}

bool SndfileOpenALLoader::loadBufferFromFile(ALuint buffer) throw (string){

	//buffer para guardar los datos
	int totalSize = handle->frames()*handle->channels();
	cout << "tamaño a leer" << totalSize << endl;
	double *pcm = new double[totalSize];
	int size = 0;
	int result;
	bool fileEnded = false;
	while(size < totalSize && !fileEnded){
		//result = handle->readf(pcm, BUFFER_SIZE);
		result = handle->readRaw(pcm + size, BUFFER_SIZE);
		cout << "Leído: " << result << endl;
		if(result < 0){
			throw string("Error en la carga del archivo");
		}
		else if(result == 0){
			cout << "fin!" << endl;
			fileEnded = true;
		}
		else{
			size += result;
			cout << "Tamaño " << size << endl;
		}
	}
	cout << "Resultado: " << size << endl;
	if(size == 0){ // no leyó nada
		return false;
	}
	/*leemos los datos*/
	alBufferData(buffer, format, pcm, size, handle->samplerate());
	errorOpenAL();
	delete pcm;
	return true;

}

void SndfileOpenALLoader::errorOpenAL() throw (string){
	if(alGetError() != AL_NO_ERROR){
		throw string("Error en OpenAL: ") + string(alutGetErrorString(alGetError()));
	}	
}

void SndfileOpenALLoader::printFileInfo(){
	if(handle){
		cout << "canales: " << handle->channels() << endl;
		cout << "Formato: " << handle->format() << endl;
		cout << "Formato como String: " << getFormatAsString(handle->format()); 
		cout << "Frames: " << handle->frames() << endl;
		cout << "Sample rate: " << handle->samplerate() << endl;
		
	}
}

string SndfileOpenALLoader::getFormatAsString(int format){
	string str;
	bool mono = (handle->channels() > 1 ? true : false);
	bool usableAL = false;
	bool bitrateEight = false;//true son 8 bits, false son 16
	switch(format & SF_FORMAT_TYPEMASK){
		case SF_FORMAT_AIFF: str = "AIFF";
			break;
		case SF_FORMAT_ALAW: str = "ALAW";
			break;
		case SF_FORMAT_WAVEX: str = "WAVEX";
			break;
		case SF_FORMAT_AU: str = "AU";
			break;
		case SF_FORMAT_FLAC: str = "flac";
			break;
		case SF_FORMAT_WAV: str = "WAV";
			break;
		default: 
			throw string("formato desconocido\n");
	}
	switch(format & SF_FORMAT_SUBMASK){
	
		case SF_FORMAT_DWVW_12: str += " 12 bits DWVW\n";
			break;
		case SF_FORMAT_DWVW_16: str += " 16 bits DWVW\n";
			break;
		case SF_FORMAT_DWVW_24: str += " 24 bits DWVW\n";
			break;
		case SF_FORMAT_DPCM_16:	str += " 16 Bits DPCM\n";
			usableAL = true;
			break;
		case SF_FORMAT_DOUBLE: str += " 32 bit double\n";
			usableAL = true;
			break;
		case SF_FORMAT_FLOAT: str += " float\n";
			usableAL = true;
			break;
		case SF_FORMAT_DPCM_8: str += " 8 Bits DPCM\n";
			usableAL = true;
			bitrateEight = true;
			break;
		case SF_FORMAT_PCM_16: str += " 16 bits PCM\n";
			usableAL = true;
			break;
		case SF_FORMAT_PCM_24: str += " 24 bits PCM\n";
			break;
		case SF_FORMAT_PCM_S8: str += " 8 bits PCM con signo\n";
			usableAL = true;
			bitrateEight = true;
			break;
		case SF_FORMAT_PCM_U8: str += " 8 bits PCM sin signo\n";
			bitrateEight = true;
			break;
		default: str += " bitrate desconocido\n";
			break;
	}
	if(!usableAL)
		throw string("No se puede utilizar el archivo con OpenAL");
	/*cambiamos el formato del buffer*/
	if(bitrateEight && mono)//8 bit mono
		this->format = AL_FORMAT_MONO8;
	else if (bitrateEight && !mono)//8 bit stereo
		this->format = AL_FORMAT_STEREO8;
	else if(!bitrateEight && mono)//16 bit mono
		this->format = AL_FORMAT_MONO16;
	else
		this->format = AL_FORMAT_STEREO16;
	return str;
}

