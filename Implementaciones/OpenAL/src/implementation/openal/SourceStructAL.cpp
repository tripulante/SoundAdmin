#include "SourceStructAL.h"

SourceStructAL::SourceStructAL(SourceFileAL *f, Source *s) throw (string)
{
	file = f;
	src = s;
	alGenSources(1, &source);
	ALenum err = alGetError(); 
	if(err != AL_NO_ERROR){
		throw (string("Error al crear la fuente: ") + string(alutGetErrorString(err)));
	}
	alSourcei(source, AL_BUFFER, f->getBuffer());
} 

SourceStructAL::~SourceStructAL()
{
	if(src->isPlaying()){
		src->togglePlaying();
		alSourceStop(source);
	}
	alDeleteSources(1, &source);
	file = NULL;
	delete src;
	cout << "Destructor de la clase Struct llamado!" << endl;
}

void SourceStructAL::updateSource(float x, float y, float z){
	src->update(x,y,z);
	if(!isAmbient()){
		alSourcefv(source, AL_POSITION, src->getPosition());
		
		if(alGetError() != AL_NO_ERROR){
			cout << "Error al posicionar a la escucha" << endl;
		}
	}
}

void SourceStructAL::setFile(SourceFileAL *f){
	alSourceStop(source);
	if(f == NULL){
		
		alSourcei(source, AL_BUFFER, NULL);
	}
	else{
		file = f;
		alSourcei(source, AL_BUFFER, f->getBuffer());
	}
		
}

void SourceStructAL::setSource(Source* s){
	src = s;
}

Source* SourceStructAL::getSource(){
	return src;
}

SourceFileAL* SourceStructAL::getFile(){
	return file;
}

void SourceStructAL::playSource(bool loop){
	
	if(loop)
		alSourcei(source, AL_LOOPING, AL_TRUE);
	else
		alSourcei(source, AL_LOOPING, AL_FALSE);
	if(!src->isPlaying()){
		src->togglePlaying();
		alSourcePlay(source);
		if(alGetError() != AL_NO_ERROR){
			cout << "error al reproducir la fuente" << endl; 
		}
	}
}

void SourceStructAL::stopSource(){
	if(src->isPlaying()){
		src->togglePlaying();
		alSourceStop(source);
	}
}

void SourceStructAL::pauseSource(){
	if(src->isPlaying()){
		src->togglePlaying();
		alSourcePause(source);
	}
}

void SourceStructAL::setAsAmbient(bool ambient){
	src->setAmbient(ambient);
	/*Colocamos la fuente como ambiente: No tenemos en cuenta el movimiento*/
	if(isAmbient()){
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
		float pos[] = {0.0,0.0, -1.0 };
		alSourcefv(source, AL_POSITION, pos);
	}
	else{
		alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcefv(source, AL_POSITION, src->getPosition());
	}
}
bool SourceStructAL::isAmbient(){
	return src->isAmbient();
}
