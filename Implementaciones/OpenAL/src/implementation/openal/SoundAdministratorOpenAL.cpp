#include "SoundAdministratorOpenAL.h"

SoundAdministratorOpenAL::SoundAdministratorOpenAL()
{
	loader = new SoundLoaderAL();
	context = NULL;
	device = NULL;
	initializeAdministrator();
}

SoundAdministratorOpenAL::~SoundAdministratorOpenAL()
{
	closeAdministrator();
}

void SoundAdministratorOpenAL::initializeAdministrator() throw (string){
	if(device == NULL && context == NULL){
		device = alcOpenDevice(NULL);
		if(!device){
			throw string("No se pudo abrir el dispositivo");
		}
		if(!alutInitWithoutContext(NULL, NULL)){
			throw string ("Error al inicializar ALUT: ") + string(alutGetErrorString(alutGetError())); 
		}
		context = alcCreateContext(device, NULL);
		if(!context){
			string str =  "No se pudo crear un contexto AL";
			string str2 = alGetString(alcGetError(device));
			throw (str + str2);
		}
		alcMakeContextCurrent(context);
	}
}

void SoundAdministratorOpenAL::closeAdministrator() throw (string) {
	stopAllSources();
	destroyAllSources();
	cleanFiles();
	delete loader;
	alutExit();
	if(context)
		alcDestroyContext(context);
	if(device)
		alcCloseDevice(device);
	context = NULL;
	device = NULL;
}

//  void SoundAdminstratorOpenAL::setConfiguration(SampleRate sr, BitRate br, int numParlantes)  ;

//Creación, asignación y destrucción de fuentes

bool SoundAdministratorOpenAL::createSource(string id, string fileDesc, bool amb) throw (string){
	if(getSourceActive(id))
		//throw SoundException();
		return false;//la fuente está activa
	SourceFileAL *file = loader->findFile(fileDesc); 
	if(!file)
		return false;//el archivo no existe
	
	/*creamos la fuente*/
	Source *src = new Source(id);
	SourceStructAL *strct = new SourceStructAL(file, src);
	/*verificamos y guardamos en la lista*/
	strct->setAsAmbient(amb);
	active.push_back(strct);
	return true;
}

//reproducción de fuentes

void SoundAdministratorOpenAL::destroySource(string id) throw (string){
	
	list<SourceStructAL*>::iterator it;
	bool elim = false;
	for(it = active.begin(); it != active.end() && !elim; it++){
		SourceStructAL *s = *it;
		if(s->getSource()->getId() == id){
			active.erase(it);
			elim = true;
		}	
	}
}

void SoundAdministratorOpenAL::playSource(string id, bool loop){
	SourceStructAL *src = getSourceActive(id);
	if(src)
		src->playSource(loop);
}

void SoundAdministratorOpenAL::pauseSource(string id){
	SourceStructAL *src = getSourceActive(id);
		if(src)
			src->pauseSource();
}

void SoundAdministratorOpenAL::stopSource(string id){
	SourceStructAL *src = getSourceActive(id);
		if(src)
			src->stopSource();
}

void SoundAdministratorOpenAL::detachSource(string id){
	
	SourceStructAL *src = getSourceActive(id);
	if(src){
		src->setFile(NULL);
	}
}

bool SoundAdministratorOpenAL::attachSource(string id, string desc){
	SourceFileAL *file  = loader->findFile(desc);
	if(file){
		SourceStructAL* src = getSourceActive(id);
		if(src){
			src->setFile(file);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void SoundAdministratorOpenAL::playAllSources(){
	list<SourceStructAL*>::iterator it;
	/*iniciamos las fuentes activas*/
	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		s->playSource(true);
	}
}

void SoundAdministratorOpenAL::pauseAllSources(){
	list<SourceStructAL*>::iterator it;
	/*detenemos las fuentes activas*/
	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		s->pauseSource();
	}
}

void SoundAdministratorOpenAL::stopAllSources(){
	list<SourceStructAL*>::iterator it;
	/*detenemos las fuentes activas*/
	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		s->stopSource();
	}
}

list<string>* SoundAdministratorOpenAL::getSourceIds(){
	list<string>* ids = new list<string>();
	list<SourceStructAL*>::iterator it;
	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		ids->push_front(s->getSource()->getId());
	}
	return ids;
}

list<string>* SoundAdministratorOpenAL::getActiveSources(){
	list<string>* ids = new list<string>();
	list<SourceStructAL*>::iterator it;
	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		if(s->getSource()->isPlaying())
			ids->push_front(s->getSource()->getId());
	}
	return ids;
}

void SoundAdministratorOpenAL::destroyAllSources( ){

	while(!active.empty()){
		cout << "Eliminado fuentes"<< endl;
		active.pop_front();
	}
	
}

bool SoundAdministratorOpenAL::existsSource(string id){
	/*busca entre las fuentes ambiente*/
	if(getAmbientSource(id))
		return true;
	/*busca entre las fuentes activas*/
	else if(getSourceActive(id))
		return true;
	else
		return false;
}

//Carga de archivos

list<string>* SoundAdministratorOpenAL::getFileDescriptions(){
	
	return loader->getFileDescriptions();
}

/*void SoundAdministratorOpenAL::loadFiles(string dataFile) throw (string){

	loader->loadFiles(dataFile);
}*/

void SoundAdministratorOpenAL::loadFile(string description, string file) throw (string){
	loader->loadFile(description, file);
}

void SoundAdministratorOpenAL::cleanFiles(){
	loader->destroyAllFiles();
}

bool SoundAdministratorOpenAL::fileExists(string desc){
	
	return (loader->findFile(desc) == NULL) ? false : true;
}

//movimiento de fuentes 

void SoundAdministratorOpenAL::updateSource(string id, float x, float y, float z){

	SourceStructAL *strct = getSourceActive(id);
	if(strct){
		strct->updateSource(x,y,z);
	}
}

//sonido ambiente, relacionado con el listener (escucha)

void SoundAdministratorOpenAL::useSourceAsAmbient(string id, bool ambient) throw (string){
	SourceStructAL *strct = getSourceActive(id);
	if(strct){
		strct->setAsAmbient(ambient);
	}
}

bool SoundAdministratorOpenAL::isAmbient(string id){
	SourceStructAL *strct = getSourceActive(id);
	if(strct){
		return strct->isAmbient();
	}
	return false;
}

void SoundAdministratorOpenAL::updateListener(float x, float y, float z){
	/**
	 * TODO Esto es en cuanto a OpenAL... y PortAudio?
	 * TODO Cómo cambiamos la orientación del listener?
	 * */
	float arr[] = {x, y, z};
	alListenerfv(AL_POSITION, arr);
	alGetError();
}


void SoundAdministratorOpenAL::cleanAmbientSources(){
	list<SourceStructAL*>::iterator it;

	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		if(s->isAmbient()){
			s->setAsAmbient(false);
			s->stopSource();
		}
	}
}

//Métodos privados

SourceStructAL* SoundAdministratorOpenAL::getAmbientSource(string id){
	list<SourceStructAL*>::iterator it;

	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		if(s->getSource()->getId() == id && s->isAmbient())
			return s;
	}
	return NULL;
}

SourceStructAL* SoundAdministratorOpenAL::getSourceActive(string id){
	list<SourceStructAL*>::iterator it;

	for(it = active.begin(); it != active.end(); it++){
		SourceStructAL *s = *it;
		if(s->getSource()->getId() == id)
			return s;
	}
	return NULL;
}
