#include "SourceFileAL.h"

SourceFileAL::SourceFileAL(string desc, ALuint buf)
{
	buffer = buf;
	description = desc;
}

SourceFileAL::~SourceFileAL()
{
	description = "";
	alDeleteBuffers(1, &buffer);
	ALenum err = alGetError();
	if(err != AL_NO_ERROR){
		cout << "Error al eliminar el buffer: " << alutGetErrorString(err) << endl;
	}
}

void SourceFileAL::setBuffer(ALuint buf){
	buffer = buf;
}

ALuint SourceFileAL::getBuffer(){
	return buffer;
}

void SourceFileAL::setDescription(string nuDesc){
	description = nuDesc;
}

string SourceFileAL::getDescription(){
	return description;
}
