/*********************************************************************/ 
	Implementación de un Sistema de Sonido Envolvente
	           para Ambientes Tipo CAVE
/*********************************************************************/

Proyecto de Grado.
Desarrollado por John Jairo Palma Robayo (joh-palm@uniandes.edu.co)
Código: 200312089

Universidad de Los Andes
Departamento de Ingeniería de Sistemas y Computación
Grupo IMAGINE
Segundo semestre de 2007.

/*********************************************************************/
				README
/*********************************************************************/

El documento siguiente describe los contenidos de este CD, los cuales
abarcan el proceso de desarrollo de un sistema de audio espacializado
que implica el manejo y ubicación de múltiples fuentes sonoras en el
espacio, así como una salida física correspondiente a su ubicación.

La posterior implementación del sistema generó dos herramientas, 
apoyadas cada una en una tecnología diferente: OpenAL(www.openal.org) 
y PortAudio (www.portaudio.com). El desarrollo se encuentra cubierto,
por la Lesser General Public License (LGPL), de la cual se encuentra
una copia adjunta.(http://www.gnu.org/copyleft/lesser.html).

Si desea más información puede escribir a joh-palm@uniandes.edu.co, o
a tripulante@gmail.com.

/*********************************************************************/
				CONTENIDOS
/*********************************************************************/

/Documentos: Contiene el documento final del Proyecto de Grado, el cual
explica los motivos para el desarrollo del sistema, las investigaciones
previas, el diseño y su posterior implementación, así como diagramas 
detallados de las implementaciones (hechos con Umbrello).

/Implementaciones: Contiene los desarrollos parciales y finales del 
sistema. /OpenALTest contiene la implementación bajo OpenAL y 
/Uniandes3DAudio alberga el desarrollo hecho bajo PortAudio. Dentro de
/doc/html en ambos proyectos se encuentra documentción generada 
mediante Doxygen (www.doxygen.org) y pruebas que incluyen interacción
con un tracker usando VRPN.

/demos_pruebas: Contiene cada una de las pruebas hechas para el 
desarrollo del sistema, abarcando desde la toma de decisiones respecto
a herramientas como la creación de librerías auxiliares. También fueron
documentadas con Doxygen y una descripción se encuentra a continuación:

	/CSoundTest: Pruebas usando CSoundAPI para manipular una 
		instancia de CSound y cambiar los datos de una fuente.
	/libsndfileTest: Librería para cargar archivos a buffers de
		OpenAL usando libsndfile (en prueba).
	/LoadOggFilesOpenALTest: Librería que permite cargar un 
		archivo .ogg y convertirlo a un buffer OpenAL.
	/MixFileProject: Prueba de salida mezclada usando libsndfile
		y PortAudio
	/PortAudioTest: Mezcla y alteración en tiempo real de 
		múltiples ondas sinusoidales.
	/PortAudioTest2: Prueba del manejo de múltiples streams en
		PortAudio.
	/VBAP: Librería que implementa el algoritmo VBAP (Vector
		Base Amplitude Panning) para espacializar fuentes
		sonoras.
