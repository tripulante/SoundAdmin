#include "SoundLoaderAL.h"

SoundLoaderAL::SoundLoaderAL()
{
}

SoundLoaderAL::~SoundLoaderAL()
{
	if(!files.empty())
		destroyAllFiles();
}

ALuint SoundLoaderAL::createBufferFromWav(string wavfile){
	ALuint buffer = alutCreateBufferFromFile((const char*)wavfile.c_str());
	if(buffer == AL_NONE){
		ALenum en = alutGetError();
		string str;
		switch(en){
		case ALUT_ERROR_INVALID_OPERATION: str = "Alut sin incializar";break;
		case ALUT_ERROR_NO_CURRENT_CONTEXT: str ="Alut sin contexto"; break;
		case ALUT_ERROR_BUFFER_DATA: str = "Error al pasar datos al buffer"; break;
		default: str = "";break;
		}
		throw str + string("Error al cargar el archivo: ") + string(wavfile + " ") + 
		string(alutGetErrorString(en));
		//intentamos cargar en caso que alut no lo soporte
		//return createBufferFromOtherExtension(wavfile);
	}
	else
		return buffer;
		
}

ALuint SoundLoaderAL::createBufferFromOgg(string oggfile){
	ALuint buffer = AL_NONE;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer))
		return AL_NONE;
	else{
		OggLoader *loader = new OggLoader();
		loader->loadFile(oggfile);
		loader->loadFileOntoBuffer(buffer);
		delete loader;
		return buffer;
	}
}

ALuint SoundLoaderAL::createBufferFromOtherExtension(string file){
	return AL_NONE;
}

void SoundLoaderAL::loadFile(string description, string file) throw (string){
	if(description.length() == 0 || file.length() == 0){
		throw string("Error al cargar el archivo: \n") + string("No tiene descripción o nombre de archivo válido");
	}
	size_t ext = file.find_last_of(".");
	if(ext != string::npos){
		ALuint buffer = AL_NONE;
		string extension = file.substr(ext + 1);
		if(extension == "WAV" || extension == "wav")
			buffer = createBufferFromWav(file);
		else if(extension == "OGG" || extension == "ogg")
			buffer = createBufferFromOgg(file);
		else
			buffer = createBufferFromOtherExtension(file);
		if(buffer == AL_NONE)
			throw string("No se pudo cargar el archivo");
		else{
			SourceFileAL* alFile = new SourceFileAL(description, buffer);
			files.push_back(alFile);
		}
	}
	else{
		throw string("Archivo incorrecto!: No tiene extensión");
	}
}

/*void SoundLoaderAL::loadFiles(string file) throw (string){
	ifstream files(file.c_str());
	string line;
	char_separator<char> func(";");
	if(files.is_open()){
		while(!files.eof()){
			getline(files, line);
			cout << line << endl;
			/*definimos el separador de lneas*/
			/*TODO terminar lo siguiente
			 * 1. Divisin en tokens de la lnea
			 * 2. Revisar sintxis de la misma
			 * 3. 	if (linea correcta)
			 * 			3.1 Cargar el archivo usando funciones
			 * 			3.2 Agregar el archivo a la lista junto con la descripcin
			 * 		else
			 * 			3.1.1 Descartar y enviar mensaje de error para debug 
			 * */
		/*}
		files.close();
	}
	else{
		throw SoundException("");
	}
}*/

void SoundLoaderAL::destroyAllFiles() throw (string){
	files.clear();
}

SourceFileAL* SoundLoaderAL::findFile(string desc){
	list<SourceFileAL*>::iterator it;
	for(it = files.begin(); it != files.end(); it++){
		SourceFileAL* f = *it;
		if(f->getDescription() == desc){
			return f;
		}
	}
	return NULL;
}

list<string>* SoundLoaderAL::getFileDescriptions(){
	list<string>* lista = new list<string>();
	list<SourceFileAL*>::iterator it;
	for(it = files.begin(); it != files.end(); it++){
		SourceFileAL* f = *it;
		lista->push_front(f->getDescription());
	}
	return lista;
}
