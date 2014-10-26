<CsoundSynthesizer>;
 
  <CsOptions>
	csound -d -odevaudio -+rtaudio=alsa 
  </CsOptions>
 
  <CsInstruments>
	sr     = 44100           ; Sample rate.
	kr     = 4410            ; Control signal rate.
	ksmps  = 10              ; Samples pr. control signal.
	nchnls = 2               ; Number of output channels.

	instr 1
		kamp = 30000
		; If you don't know the frequency of your audio file,
		; set both the kcps and ibas parameters equal to 1.
		kcps = 1
		ifn = 1
		ibas = 1
	
		; Variables de canales desde las cuales se obtienen los valores
		Sname 	chnget 	"filename"; prueba del software bus
		kdeg 	chnget 	"degree"; valor en grados para la ubicación de la señal
		kdist	chnget	"distance"; Distancia para atenaución
		krev	chnget	"reverb"; valor de reverberación
		
		krec = 0
		; obtenemos la señal de archivo
		sndload	"alive.wav"
		asignal	loscilx kamp, kcps, ifn;, ibas; oscilador para el archivo
		
		a1 a2	locsig asignal, kdeg, kdist, krev
		a1 a2	locsend	
		outs a1, a2         ; Output.

	endin
  </CsInstruments>
 
  <CsScore>
	i 1 0 6		         ; 6 segundos del instrumento
	e
  </CsScore>
 
</CsoundSynthesizer>