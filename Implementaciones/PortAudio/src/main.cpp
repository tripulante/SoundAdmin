#include "soundadmin/SoundAdminPortaudio.h"
#include <string>
#include <memory>

using namespace std;

//SoundAdminPortaudio* admin;

//int main(int argc, const char *argv[]){
//	
//	if(argc != 2){
//		cout << "No hay suficientes argumentos" << endl;
//	}
//	
//	try{
//		string file(argv[1]);
//		admin = SoundAdminPortaudio::getSoundAdminInstance();
//		admin->setConfiguration(SoundAdminPortaudio::SR_44100_KHZ, 5, "../data/test_data2.txt", SoundAdminPortaudio::DISTRIBUTED);
//		if(!admin->addFile("testFile", file)){
//			throw string("No se pudo cargar el archivo...");
//		}
//		admin->createSource("testsrc", "testFile", false);
//		admin->createSource("testsrc2", "testFile", true);
//		admin->playSource("testsrc");
//		float x, y, z;
//		x = y = z = 0.0f;
//		admin->updateSource("testsrc", x, y, z);
//		char c;
//		while(c != 'o'){
//			cin >> c;
//			switch(c){
//			case 'a': x--; break;
//			case 'd': x++; break;
//			case 'w': y++; break;
//			case 's': y--; break;
//			case 'r': z++; break;
//			case 'f': z--; break;
//			case 't': x = y = z = 0.0; break;
//			case 'y': 
//				if(admin->isPlaying("testsrc"))
//					admin->stopSource("testsrc");
//				else
//					admin->playSource("testsrc");
//				break;
//			case 'b': 
//				if(!admin->isPlaying("testsrc2"))
//					admin->playSource("testsrc2");
//				else
//					admin->stopSource("testsrc2");
//				break;
//			default: break;
//			}
//			admin->updateSource("testsrc", x, y, z);
//		}
//	}
//	catch(string s){
//		cout << s << endl;
//	}
//	return 0;
//}
