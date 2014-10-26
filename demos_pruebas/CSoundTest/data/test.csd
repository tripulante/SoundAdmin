<CsoundSynthesizer>;
 
  <CsOptions>
	csound -d -odevaudio -+rtaudio=alsa -L stdin
  </CsOptions>
 
  <CsInstruments>
	sr     = 44100           ; Sample rate.
	kr     = 4410            ; Control signal rate.
	ksmps  = 10              ; Samples pr. control signal.
	nchnls = 2               ; Number of output channels.

	instr 1
		kamp = 5000
		; If you don't know the frequency of your audio file,
		; set both the kcps and ibas parameters equal to 1.
		kcps = 1
		ifn = 2
		ibas = 1
	
		; Variables de canales desde las cuales se obtienen los valores
		kval 	chnget 	"testOut"; prueba del software bus
		kdeg 	chnget 	"degree"; valor en grados para la ubicación de la señal
		kdist	chnget	"distance"; Distancia para atenaución
		krev	chnget	"reverb"; valor de reverberación
		
		krec = 0
;		printf	"Hello World Csound!", 5,0
		; obtenemos la señal de la tabla
		asignal	loscil kamp, kcps, ifn, ibas	; Simple oscillator.
		;loop
		;aout, krec	sndloop	asignal, 1, 60, 0.5
		; usamos locsig para distribuir la señal en stereo
		
		a1 a2	locsig asignal, kdeg, kdist, krev
		a1 a2	locsend	
			
			outs a1, a2         ; Output.
	endin
  </CsInstruments>
 
  <CsScore>
;	|#.Tabla|tiempo	|S.Tabla|Rutina	|Archivo|	Skip?	Canales	Datos
;	f 2	0	0	1	"03.wav"	0	0	0	; Leer los datos desde el header, y todos los canales
;	f1 0 8192 10 1           ; Table containing a sine wave.
;	i 1 0 60		         ; 6 segundos del instrumento
;	e
  </CsScore>
 
</CsoundSynthesizer>
