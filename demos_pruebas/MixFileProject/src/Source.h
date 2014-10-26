#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>
#include <sndfile.hh>

using namespace std;
/**
 * @class Source
 * @brief Definición de una fuente estándar que carga de un archivo
 * @author John Jairo Palma Robayo
 * @file Source.h 
 * */
class Source{
	
protected:
	/**
	 * Posición en el espacio de la fuente
	 * pos[0] = x, pos[1] = y; pos[2] = z;
	 * */
	float pos[3];
	
	/**
	 * Indica si la fuente está actualmente en reproducción
	 * */
	bool playing;
	
	/**
	 * Identificador de la fuente
	 * */
	string id;
	
	/**
	 * Infica si la fuente es ambiente o no
	 * */
	bool ambient;
	
	/**
	 * Fase de la tabla con la cual trabaja esta fuente.
	 * A cada fuente se asigna una fase de una onda sinosoidal.
	 * */
	int phase;

	/**
	* Ganancia de reproducción de la fuente
	*/
	float gain;
	
	/**
	 * Posición de la fuente dentro del archivo
	 * */
	int adv;

	/**
	 * Archivo al que está asociada la fuente
	 * */
	SndfileHandle *file;
	
	/**
	 * Imprime en pantalla la información del archivo asociado a la fuente, si existe
	 * */
	void printFileInfo(){
		if(file){
			cout << "Canales: " << file->channels() << endl;
			cout << "Formato: " << file->format() << endl;
			cout << "Formato como String: " << getFormatAsString(file->format()); 
			cout << "Frames: " << file->frames() << endl;
			cout << "Sample rate: " << file->samplerate() << endl;
			
		}
	}

	/**
	 * toma el formato de un archivo libsndfile y lo devuelve como una cadena legible
	 * @param format Formato a consultar
	 * @return cadena legible con la información del formato 
	 * */
	string getFormatAsString(int format){
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
		return str;
	}
	
public:
	
	/**
	 * Constructor de la clase. La fuente empieza en el origen
	 * y no se está reproduciendo
	 * @param nuID Nueva Id que identifica a la fuente
	 * */
	Source(string nuID){
		id = nuID;
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = false;
		ambient = false;
		gain = 0.0;
		phase = adv = 0;
		file = NULL;
	}
	
	/**
	 * Destructor de la fuente
	 * */
	virtual ~Source(){
		id = "";
		pos[0] = pos[1] = pos[2] = 0.0;
		playing = false;
		ambient = false;
		gain = 0.0;
		phase = adv = 0;
	}
	
	/**
	 * Actualiza la posición de la fuente
	 * @param x Nueva posición en x de la fuente
	 * @param y Nueva posición en y de la fuente
	 * @param z Nueva posición en z de la fuente
	 * */
	void update(float x, float y, float z){
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}
	
	/**
	 * @return Arreglo con la posición de la fuente
	 * */
	float* getPosition(){
		return pos;
	}
	
	/**
	 * Pone la fuente a sonar o la detiene
	 * */
	void togglePlaying(){
		playing = !playing;
	}
	
	/**
	 * @return true si la fuente se está reproduciendo, false si no.
	 * */
	bool isPlaying(){
		return playing;
	}
	
	/**
	 * Cambia el ID de la fuente
	 * */
	void setId(string nuID){
		id = nuID;
	}
	
	/**
	 * @return Id de la fuente
	 * */
	string getId(){
		return id;
	}
	
	/**
	 * @return booleano que indica si la fuente es ambiente
	 * */
	bool isAmbient(){
		return ambient;
	}
	
	/**
	 * Cambia la fuente a ambiente/no ambiente 
	 * @param amb Indica si la fiente es ambiente
	 * */
	void setAmbient(bool amb){
		ambient = amb;
	}

	/**
	* Cambia la ganancia de la fuente
	* @param nGain Nuevo valor de ganancia (entre 0 y 1)
	*/
	void setGain(float nGain){
		gain = nGain;
	}

	/**
	* @return Ganancia de la fuente
	*/
	float getGain(){
		return gain;
	}
	
	/**
	* @param nuPh nueva fase de la fuente
	*/
	void setPhase(int nuPh){
		phase = nuPh;
	}

	/**
	* @return valor de la fase del sistema
	*/
	int getPhase(){
		return phase;
	}
	
	/**
	 * Cambia la posición de la fuente dentro del archivo
	 * @param advance Nueva posición de la fuente dento del archivo
	 * */
	void setAdvance(int advance){
		adv = advance;
	}
	
	/**
	 * @return Posición de la fuente con respecto al archivo
	 * */
	int getAdvance(){
		return adv;
	}

	/**
	 * Cambia el archivo asociado a la fuente
	 * @param handle Nuevo archivo a asociarse a la fuente
	 * */
	void setFile(SndfileHandle *handle){
		file = handle;
		printFileInfo();
	}

	/**
	 * Lee datos del archivo y los coloca en el buffer de salida
	 * @param output Buffer de salida
	 * @param framesToRead Cantidad de datos a leerse del archivo
	 * @return Cantidad de datos leídos efectivamente.  
	 * */
	int readFromFile(float *output, int framesToRead){
		
		int read = 0;
		int left = framesToRead;
//		int items = file->read(output, framesToRead);
		int format = file->format();
		bool bit16 = false;
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
				file->seek(adv, SEEK_SET);

				if(left > (file->frames() - adv)){
					left = file->frames() - adv;
					adv = 0;
				}
				else{
					read = left;
					adv += read;
				}
				file->readf((short*)salida, read);
				left -= read;
			}	
			for(int ch = 0; ch < file->channels(); ch++)
				for(int i = 0; i < read; i++)
					output[i] = (((short*)salida)[file->channels()*i+ch]+0.5)/float(1<<15);
//			cout << "Convirtiendo 16 bit" << endl;
			delete[] salida;
		}
//		else if(bit8){
//			char *salida = new char[framesToRead*file->channels()];
//			while(left > 0){
//				file->seek(adv, SEEK_SET);
//
//				if(left > (file->frames() - adv)){
//					left = file->frames() - adv;
//					adv = 0;
//				}
//				else{
//					read = left;
//					adv += read;
//				}
//				file->readRaw(salida, read);
//				left -= read;
//			}	
//			for(int ch = 0; ch < file->channels(); ch++)
//				for(int i = 0; i < read; i++)
//					output[i] = (salida[file->channels()*i+ch]+0.5)/float(1<<7);
//			delete []salida;
//		}
		else{
			char *salida = new char[framesToRead*file->channels()*sizeof(float)];
			while(left > 0){
				file->seek(adv, SEEK_SET);

				if(left > (file->frames() - adv)){
					left = file->frames() - adv;
					adv = 0;
					cout << "Restart" << endl;
				}
				else{
					read = left;
					adv += read;
				}
				file->readf((float*)salida, read);
				left -= read;
			}
			for(int ch = 0; ch < file->channels(); ch++)
				for(int i = 0; i < read; i++)
					output[i] = ((float*)salida)[i*file->channels() + ch];
//			cout << "Convirtiendo 24 bit" << endl;
			delete []salida;
		}
//		while(left > 0){
//			file->seek(adv, SEEK_SET);
//	
//			if(left > (file->frames() - adv)){
//				left = file->frames() - adv;
//				adv = 0;
//			}
//			else{
//				read = left;
//				adv += read;
//			}
//			file->read(output, read);
//			left -= read;
//		}
// 		cout << "Leído por la fuente " << id << ": " << read << endl;
// 		cout << "Posición: " << adv << endl;
		return read;
//		return items;
	}
};

#endif /*SOURCE_H_*/
