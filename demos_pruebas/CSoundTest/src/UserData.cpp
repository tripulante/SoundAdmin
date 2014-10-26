#include "UserData.h"

UserData::UserData(Csound *cs, int res)
{
	csound = cs;
	result = res;
	performing = false;
	degrees = distance = reverb = 0.0;
}

UserData::~UserData()
{
	if (csound)
		delete csound;	
}

void UserData::togglePerform(){
	performing = !performing;
}

bool UserData::isPerforming(){
	return performing;
}

int UserData::getResult(){
	return result;
}

Csound* UserData::getCsound(){
	return csound;
}

void UserData::destroyCsound(){
	if(csound)
		delete csound;
}

void UserData::setInteractionData(double degrees, double reverb, double distance){
	this->degrees = degrees;
	this->reverb = reverb;
	this->distance = distance;
}

double UserData::getDegrees(){
	return degrees;
}

double UserData::getDistance(){
	return distance;
}

double UserData::getReverb(){
	return reverb;
}

MYFLT* UserData::getPval(){
	return pvalues;
}

void UserData::setPval(MYFLT* val){
	pvalues = val;
}
