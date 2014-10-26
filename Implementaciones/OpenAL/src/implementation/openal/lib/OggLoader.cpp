#include "OggLoader.h"

OggLoader::OggLoader()
{
}

OggLoader::~OggLoader()
{
	ov_clear(&file);
	if(info)
		delete info;
}

void OggLoader::loadFile(string path) throw (string){
	
	FILE *f = fopen(path.c_str(), "rb");
	if(f != NULL){
		int res = ov_open(f, &file, NULL, 0); 
		if( res == 0){
			info = ov_info(&file, -1);
			printFileInfo();
			
		    if(info->channels == 1)
		        format = AL_FORMAT_MONO16;
		    else
		        format = AL_FORMAT_STEREO16;
		}
		else{
			fclose(f);
			throw string( "Error al abrir el archivo " + path);
		}
	}
	else
	{
		throw string("Error al abrir el archivo Ogg");
	}
}

void OggLoader::printFileInfo(){
	if(info){
		cout << "Canales: " << info->channels << endl;
		cout << "Bitrate inferior: " << info->bitrate_lower << endl;
		cout << "Bitrate Nominal: " << info->bitrate_nominal << endl;
		cout << "Bitrate Superior: " << info->bitrate_upper << endl;
		cout << "Bitrate Window: " << info->bitrate_window << endl;
		cout << "Codec: " << info->codec_setup << endl;
		cout << "Sample Rate: "<< info->rate << endl;
		cout << "Versión: " << info->version << endl;
		cout << "Total de muestras Raw: " << ov_raw_total(&file, -1) << endl;
		cout << "Total de muestras PCM: " << ov_pcm_total(&file, -1) << endl;
	}
}

bool OggLoader::loadFileOntoBuffer(ALuint buffer) throw (string){

	//buffer para guardar los datos
	int totalsize = ov_pcm_total(&file, -1)*2*info->channels;
	//cout << "Total a leer: " << totalsize << endl;
	char *pcm = new char[totalsize];
	//cout << "Pudo crear el arreglo?" << endl;
	int size = 0;
	int section;
	int result;
	bool end = false;
	while(size < totalsize && !end){
		result = ov_read(&file, pcm + size, totalsize - size, 0, 2, 1, &section);
		//cout << " BUFFER_SIZE - size " << totalsize - size << endl;
		if(result < 0){
			end = true;
			throw string("Error en la carga del archivo");
		}
		else{
			if(result == 0)
				end = true;
			size += result;
		}
	}
	if(size == 0){ // no leyó nada
		cout << "No leyó nada!" << endl;
		return false;
	}
	/*leemos los datos*/
	alBufferData(buffer, format, pcm, size, info->rate);
	error();
	//borramos el arreglo temporal
	delete pcm;
	return true;
}

void OggLoader::error() throw (string){
	ALenum error = alGetError(); 
	if(error != AL_NO_ERROR){
		throw string("Error en OpenAL: ") + string(alutGetErrorString(alGetError()));
	}
	error = alutGetError();
	if( error != ALUT_ERROR_NO_ERROR)
		throw string("Error en ALUT: ") + string(alutGetErrorString(alGetError()));
}

