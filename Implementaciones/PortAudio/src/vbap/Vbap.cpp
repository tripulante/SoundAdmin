#include "Vbap.h"

bool sortFunction(speakDist a, speakDist b){
	return (a < b);
}

Vbap::Vbap(int numspeak) {
	numspeakers = numspeak;
	distances = new float*[numspeakers];
	for (int i = 0; i < numspeakers; i++)
		distances[i] = new float[numspeakers];
	for (int i = 0; i < numspeakers; i++)
		for (int j = 0; j < numspeakers; j++)
			distances[i][j] = 0.0f;
	//agregamos 1 más para hacer manejo de error
	totalSpeakers = new Speaker*[numspeakers+1];
	for (int i = 0; i <= numspeakers; i++)
		totalSpeakers[i] = NULL;
}

Vbap::~Vbap() {
	for (int i = 0; i < numspeakers; i++)
		delete [] distances[i];
	delete [] distances;
	for (int i = 0; i <= numspeakers; i++)
		if (totalSpeakers[i])
			delete totalSpeakers[i];
	delete [] totalSpeakers;
	activeTriangles.clear();
	distBetweenSpeakers.clear();
}

void Vbap::printDistanceMatrix() {
	for (int i = 0; i < numspeakers; i++) {
		for (int j = 0; j < numspeakers; j++) {
			cout << distances[i][j] << "\t";
		}
		cout << endl;
	}
}

bool Vbap::createSpeakers(const char* configFilePath) {
	ifstream file(configFilePath);
	string line;
	if (file.is_open()) {
		int i = 0;
		while (!file.eof() && i < numspeakers+1) {
			getline(file, line);
			//			cout << line << endl;
			stringstream sstream(stringstream::in | stringstream::out);
			sstream << line;
			float az, elev;
			sstream >> az;
			sstream >> elev;
			Speaker *speak = new Speaker(az, elev, i);
			totalSpeakers[i] = speak;
			i++;
		}
		//		cout << i << endl;
		if (i < (numspeakers-1) || totalSpeakers[numspeakers] != NULL) {
			cout << "Número de speakers diferente a los cargados" << endl;
			for (int j = 0; j < i && j < numspeakers && totalSpeakers[j]
					!= NULL; j++)
				delete totalSpeakers[j];
			return false;
		}
//		for(int j = 0; j < numspeakers; j++)
//			cout << totalSpeakers[j]->toString();
		return true;
	} else {
		cout << "El archivo no pudo ser cargado. Ingrese la ruta correctamente"
				<< endl;
		return false;
	}

}

void Vbap::createAllTriangles() {
	for (int i = 0; i < numspeakers; i++)
		for (int j = i+1; j < numspeakers; j++)
			for (int k = j+1; k < numspeakers; k++) {
				Speaker *a = totalSpeakers[i];
				Speaker *b = totalSpeakers[j];
				Speaker *c = totalSpeakers[k];
				SpeakerTriangle *trian = new SpeakerTriangle(a, b, c);
				float vol = SpeakerTriangle::triangleVolume(*trian);
				//				cout << vol << endl;

				if (vol > MIN_VOL_PER_SIDE) {

					distances[i][j] = distances[j][i]
							= abs(SpeakerTriangle::angleBetween(a->getVector(),
									b->getVector()));
					distances[i][k] = distances[k][i]
							= abs(SpeakerTriangle::angleBetween(a->getVector(),
									c->getVector()));
					distances[k][j] = distances[j][k]
							= abs(SpeakerTriangle::angleBetween(b->getVector(),
									c->getVector()));
					//					distances[i][i] = distances[j][j] = distances[k][k] = 0.0;
					activeTriangles.push_back(trian);

				} else { //borramos la memoria creada
					delete trian;
				}
			}
	for(int i = 0; i < numspeakers; i++)
		for(int j = 0; j < numspeakers; j++){
			if(distances[i][j] > 0.0f && i != j){
				speakDist distanceij;
				distanceij.i = i;
				distanceij.j = j;
				distanceij.distance = distances[i][j];
				if(find(distBetweenSpeakers.begin(), distBetweenSpeakers.end(), distanceij) 
						== distBetweenSpeakers.end()){
					distBetweenSpeakers.push_back(distanceij);
				}
			}
		}
	
	sort(distBetweenSpeakers.begin(), distBetweenSpeakers.end(), sortFunction);
}

void Vbap::removeTriangles() {

	vector<SpeakerTriangle*>::iterator it1;
	vector<speakDist>::iterator tempIt, tempIt2;
	
	for (tempIt = distBetweenSpeakers.begin(); tempIt != distBetweenSpeakers.end(); tempIt++){
		if(distances[(*tempIt).i][(*tempIt).j] > 0.0f)
			for (tempIt2 = tempIt; tempIt2 != distBetweenSpeakers.end(); tempIt2++){
				if (tempIt != tempIt2){
					intersection(*tempIt, *tempIt2);
				}
			}
	}
	
	int borrados = 0;
	for (it1 = activeTriangles.begin(); it1 != activeTriangles.end(); it1++) {
		if (insideTriplet(*it1) || hasCrossedSide(*it1)) {
			it1 = activeTriangles.erase(it1);
			it1--;
			borrados++;
		}
	}

}

//Métodos privados
bool Vbap::insideTriplet(SpeakerTriangle* a) {
	Matrix33f mat = a->getMatrix();
	bool result = false;
	for (int i = 0; i < numspeakers /*&& !result*/; i++) {
		if (!a->isTriangleVertex(i)) {
			Vec3f res, speak;
			speak = totalSpeakers[i]->getVector();
			res = mat*speak;
//			normalize(res);

			if (res[0] >= -0.001f && res[1] >= -0.001f && res[2] >= -0.001f) {
				result = true;
			}
		}
	}

	return result;
}

bool Vbap::hasCrossedSide(SpeakerTriangle* b) {
	Speaker **speakers = b->getSpeakers();
	int l1 = speakers[0]->getSpeakerNumber();
	int l2 = speakers[1]->getSpeakerNumber();
	int l3 = speakers[2]->getSpeakerNumber();
	if (distances[l1][l2] == 0.0f || distances[l1][l3] == 0.0f
			|| distances[l2][l3] == 0.0f) {
		return true;
	}
	return false;
}

void Vbap::intersection(speakDist a, speakDist b) {
	//Verificamos que no se crucen dos parlantes iguales
	if(!(a == b)) {
//		cout << "Intersectando: <(" << a.i << "," << a.j << ")";
//		cout << "("<< b.i << "," << b.j << ")>"<< endl; 
		Vec3f c, lmln, lilj;
		cross(lilj, totalSpeakers[a.i]->getVector(), totalSpeakers[a.j]->getVector());
		cross(lmln, totalSpeakers[b.i]->getVector(), totalSpeakers[b.j]->getVector());
		cross(c, lilj, lmln);
		Vec3f negC = -c;
		//Calculamos los valores para el primer par de speakers
		float angcA1 = SpeakerTriangle::angleBetween(c, totalSpeakers[a.i]->getVector());
		float angcA2 = SpeakerTriangle::angleBetween(c, totalSpeakers[a.j]->getVector());
		float sumij = angcA1+ angcA2;
		float angNegA1 = SpeakerTriangle::angleBetween(negC, totalSpeakers[a.i]->getVector());
		float angNegA2 = SpeakerTriangle::angleBetween(negC, totalSpeakers[a.j]->getVector());
		float sumNegij = angNegA1 + angNegA2;
		//Calculamos los valores para el segundo par de speakers
		float angcB1 = SpeakerTriangle::angleBetween(c, totalSpeakers[b.i]->getVector());
		float angcB2 = SpeakerTriangle::angleBetween(c, totalSpeakers[b.j]->getVector());
		float summn = angcB1 + angcB2;
		float angNegB1 = SpeakerTriangle::angleBetween(negC, totalSpeakers[b.i]->getVector());
		float angNegB2 = SpeakerTriangle::angleBetween(negC, totalSpeakers[b.j]->getVector());
		float sumNegmn = angNegB1 + angNegB2;
		//Calculamos las distancias entre los ángulos
		float angij = SpeakerTriangle::angleBetween(totalSpeakers[a.i]->getVector(), totalSpeakers[a.j]->getVector());
		float angmn = SpeakerTriangle::angleBetween(totalSpeakers[b.i]->getVector(), totalSpeakers[b.j]->getVector());

//		omitimos valores cerca a los puntos de cruce
		if(abs(angcA1) <= 0.01 || abs(angcA2) <= 0.01 || abs(angcB1) <= 0.01 ||
				abs(angcB2) <= 0.01 || abs(angNegA1) <= 0.01 || abs(angNegA2) <= 0.01 ||
				abs(angNegB1) <= 0.01 || abs(angNegB2) <= 0.01) 
			return;

		float totalij = abs((angij - sumij));
		float totalmn = abs((angmn - summn));
		float totalnegij = abs((angij - sumNegij));
		float totalnegmn = abs((angmn - sumNegmn)); 
		if((totalij <= 0.01 && totalmn <= 0.01) ||
				( totalnegij <= 0.01 && totalnegmn <= 0.01)) {
			//esto indica que se intersectan las líneas de los triángulos
			//eliminamos las conexiones entre los speakers intersectados
			distances[b.i][b.j] = distances[b.j][b.i] = 0.0f;
		}
	}
}

pair<Vec3f, int*>* Vbap::selectTriangle(float x, float y, float z){
	Vec3f gain, source;
	source[0] = x;
	source[1] = y;
	source[2] = z;
	normalize(source);
	int *speakers = new int[3];
	speakers[0] = speakers[1] = speakers[2] = 0; 
//	int counter = 0;
	bool salir = false;
	for(unsigned int i = 0; i < activeTriangles.size() && !salir; i++){
		SpeakerTriangle *s = activeTriangles.at(i);
		Vec3f temp;
		Matrix33f mat = s->getMatrix();
		xform(temp, mat, source);
		
		if(temp[0] >= -0.001f && temp[1] >= -0.001f && temp[2] >= -0.001f){
			Speaker **speak = s->getSpeakers();
			speakers[0] = speak[0]->getSpeakerNumber();
			speakers[1] = speak[1]->getSpeakerNumber();
			speakers[2] = speak[2]->getSpeakerNumber();
			gain = temp;
			normalize(gain);
			salir = true;
		}
	}
//	cout << "Se contaron " << counter << " Triángulos a los que pertenecía " << source << endl;
	pair<Vec3f, int*> *gain_spk = new pair<Vec3f, int*>(gain, speakers);
	return gain_spk;
}

float Vbap::getDistanceAttenuation(Vec3f pos, Vec3f listener){
	//TODO Revisar
	Vec3f dist = pos - listener;
	
	//Esto indicaría que la fuente se encuentra muy cerca del listener
	if(lengthSquared(dist) < 1.0){
		return 1.0;
	}
	else{
		float atten = 1/lengthSquared(dist);
		if(atten <= 0.001)
			atten = 0.001;
		return atten;
	}
}

Vec2f Vbap::getStereoGain(float x, float y, float z){
	/*
	 * TODO revisar el algoritmo para buscar una mejor manera de distribuir el 
	 * audio espacialmente en 2D (tal como en las consolas)
	 * Éste sólamente asigna un valor entre 0 y 1 para izquierda y derecha
	 * */ 
	Vec2f gain;
	if(x > 0){
		//derecha
		gain[1] = 1.0f;
		gain[0] = 0.0f;
	}
	else if(x == 0){
		gain[0] = gain[1] = 1.0f;
	}
	else{
		gain[0] = 1.0f;
		gain[1] = 0.0f;
	}
	return gain;
}
