#include "AudioFileLoader.h"

AudioFileLoader::AudioFileLoader()
{
}

AudioFileLoader::~AudioFileLoader()
{
	destroyAllFiles();
}

void AudioFileLoader::loadFile(string desc, string file) throw (string){
	if(desc == "")
		throw string("La descripción es inválida");
	if(!(libsndfiles.count(desc) > 0 || oggFiles.count(desc) > 0)){
		int ext = file.find_last_of(".");
		if(ext != (int)string::npos)
		{
			string extension = file.substr(ext + 1);
			if(extension == "OGG" || extension == "ogg"){
				FILE *f = fopen(file.c_str(), "rb");
				OggVorbis_File *ovfile = new OggVorbis_File();
				if(f != NULL){
					int res = ov_open(f, ovfile, NULL, 0); 
					if( res != 0){
						fclose(f);
						throw string( "Error al abrir el archivo " + file);
					}
					oggFiles[desc] = ovfile;
				}
				else
				{
					throw string("Error al abrir el archivo Ogg");
				}
			}
			//agregamos archivos distintos a ogg (soportados por libsndfile)
			else
			{
				SndfileHandle *snd = new SndfileHandle(file.c_str(), SFM_READ, 0, 0, 0);
				if(snd->refCount() > 0){
					libsndfiles[desc] = snd;
				}
				else{
					throw string("Error al abrir el archivo ") + file;
				}
			}
		}
		else
			throw string("No hay extensión! Archivo inválido ") + file;
	}
	else
		throw string("ya existe un archivo con esa descripción: ") + desc;
}

bool AudioFileLoader::deleteFile(string desc){
	//segunda verificación (por si acaso)
	if(libsndfiles.count(desc) > 0){
		libsndfiles.erase(desc);
		return (libsndfiles.count(desc) == 0);
	}
	else if(oggFiles.count(desc) > 0){
		OggVorbis_File *f = oggFiles[desc];
		int ret = ov_clear(f);
		if(ret != 0)
			return false;
		oggFiles.erase(desc);
		return (oggFiles.count(desc) == 0);
	}
	else
		return true;
}

void AudioFileLoader::destroyAllFiles() throw (string){
	map<string, OggVorbis_File*>::iterator it;
	for(it = oggFiles.begin(); it != oggFiles.end(); it++){
		OggVorbis_File *f = it->second;
		ov_clear(f);
	}
	libsndfiles.clear();
	oggFiles.clear();
}

int AudioFileLoader::readFromFile(float *buffer, string file, int *pos, int framesToRead, bool loop){
	if(libsndfiles.count(file) > 0)
		return readFromPcm(libsndfiles[file], buffer, pos, framesToRead, loop);
	else if(oggFiles.count(file) > 0)
		return readFromOgg(oggFiles[file], buffer, pos, framesToRead, loop);
	else
		return 0;
}

int AudioFileLoader::readFromOgg(OggVorbis_File *file, float* buffer, int* pos, 
		int framesToRead, bool loop){
	//TODO No implementado!
	return 0;
}

int AudioFileLoader::readFromPcm(SndfileHandle *file, float* buffer, int* pos, 
		int framesToRead, bool loop){
	int read = 0;
	int left = framesToRead;
	int format = file->format();
	bool bit16 = false;
	bool end = false;
	switch(format & SF_FORMAT_SUBMASK){
		case SF_FORMAT_PCM_16: 
			bit16 = true;
			break;
		case SF_FORMAT_PCM_24:
			bit16 = false;
			break;
		default: break;
	}
	if(bit16){
		char *salida = new char[framesToRead*file->channels()*sizeof(short)];
		
		while(left > 0){
			file->seek(*pos, SEEK_SET);

			if(left > (file->frames() - *pos)){
				left = file->frames() - *pos;
				*pos = 0;
//				if(loop)
					end = true;
			}
			else{
				read = left;
				*pos += read;
			}
			file->readf((short*)salida, read);
			left -= read;
		}	
		for(int ch = 0; ch < file->channels(); ch++)
			for(int i = 0; i < read; i++)
				buffer[i] = (((short*)salida)[file->channels()*i+ch]+0.5)/float(1<<15);
		//			cout << "Convirtiendo 16 bit" << endl;
		delete[] salida;
	}
	else{
		char *salida = new char[framesToRead*file->channels()*sizeof(float)];
		while(left > 0 && !end){
			file->seek(*pos, SEEK_SET);

			if(left > (file->frames() - *pos)){
				left = file->frames() - *pos;
				*pos = 0;
//				if(!loop)
					end = true;
			}
			else{
				read = left;
				*pos += read;
			}
			file->readf((float*)salida, read);
			left -= read;
		}
		for(int ch = 0; ch < file->channels(); ch++)
			for(int i = 0; i < read; i++)
				buffer[i] = ((float*)salida)[i*file->channels() + ch];
		delete []salida;
	}
	if(!loop && end)
		*pos = 0;
	return read;
}

void AudioFileLoader::printOggFileInfo(OggVorbis_File file){
	vorbis_info *info = ov_info(&file, -1);
	if(info){
		cout << "Canales: " << info->channels << endl;
		cout << "Bitrate Nominal: " << info->bitrate_nominal << endl;
		cout << "Codec: " << info->codec_setup << endl;
		cout << "Sample Rate: "<< info->rate << endl;
		cout << "Versión: " << info->version << endl;
		cout << "Total de muestras Raw: " << ov_raw_total(&file, -1) << endl;
		cout << "Total de muestras PCM: " << ov_pcm_total(&file, -1) << endl;
	}
}

void AudioFileLoader::printSndfileInfo(SndfileHandle handle){
	cout << "Canales: " << handle.channels() << endl;
	int format = handle.format(); 
	string str;
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
			str = "formato desconocido\n";
	}
	switch(format & SF_FORMAT_SUBMASK){
		case SF_FORMAT_DWVW_12: str += " 12 bits DWVW\n";
		break;
		case SF_FORMAT_DWVW_16: str += " 16 bits DWVW\n";
		break;
		case SF_FORMAT_DWVW_24: str += " 24 bits DWVW\n";
		break;
		case SF_FORMAT_DPCM_16:	str += " 16 Bits DPCM\n";
		break;
		case SF_FORMAT_DOUBLE: str += " 32 bit double\n";
		break;
		case SF_FORMAT_FLOAT: str += " float\n";
		break;
		case SF_FORMAT_DPCM_8: str += " 8 Bits DPCM\n";
		break;
		case SF_FORMAT_PCM_16: str += " 16 bits PCM\n";
		break;
		case SF_FORMAT_PCM_24: str += " 24 bits PCM\n";
		break;
		case SF_FORMAT_PCM_S8: str += " 8 bits PCM con signo\n";
		break;
		case SF_FORMAT_PCM_U8: str += " 8 bits PCM sin signo\n";
		break;
		default: str += " bitrate desconocido\n";
		break;
	}
	cout << "Formato como String: " << str; 
	cout << "Frames: " << handle.frames() << endl;
	cout << "Sample rate: " << handle.samplerate() << endl;
}

bool AudioFileLoader::fileExists(string desc){
	return (libsndfiles.count(desc) > 0 || oggFiles.count(desc) > 0);
}

vector<string>* AudioFileLoader::getFileDescriptions(){
	vector<string>* vec = new vector<string>();
	map<string, SndfileHandle*>::iterator it;
	for(it = libsndfiles.begin(); it != libsndfiles.end(); it++){
		vec->push_back(it->first);
	}
	map<string, OggVorbis_File*>::iterator itogg;
	for(itogg = oggFiles.begin(); itogg != oggFiles.end(); itogg++)
		vec->push_back(itogg->first);
	return vec;
}
