Dipendenze:
	Per compilare il programma, e' necessario avere le seguenti 
	librerie:
		gtk >= 2.6
		glib
		pkg-config

Compilazione:
	Non ho utilizzato aclocal/autoconf/automake perche' non ho idea
	di come funzioni su windows; ho realizzato un semplice makefile
	che utilizza pkg-config per ottenere i path delle librerie gtk.
	Per compilare basta digitare:

	$ make
	$ make link


	Per compilare da linux un binario per windows tramite toolchain
	i486_mingw32 c'e' il makefile 'Makefile.toolchain_win' (naturalmente
	e' necessario anche installare le librerie standard e gtk per mingw32):

	$ make -f Makefile.toolchain_win
	$ make -f Makefile.toolchain_win link

	Per compilare da windows per windows, si puo' installare codeblocks
	ed il pacchetto di gtk per windows, dopodiche' si crea un progetto
	gtk (e' presente di default nei tipi di progetto di cb), si aggiungono
	i file del progetto e si compila tramite l'interfaccia. Se c'e'
	qualche problema con gdk-pixbuf, basta aggiungerlo manualmente alle
	librerie del progetto.


Avvio:
	$ ./sirbonville [salvataggio]

Windows:
	Il binario per windows e' contenuto nella cartella 'win' insieme
	alle dll di gtk, dovrebbe funzionare correttamente su xp/vista/7.


Valori di gioco:
	Li ho un po' modificati per rendere la giocabilita' piu' piacevole.
	La fame aumenta un po' piu' rapidamente, in modo che sia probabile
	che muoiano di fame, per i bisogni stessa cosa.
	I maiali si riproducono ogni 150 unita' di tempo, cosi' ogni maiale fa
	in tempo a fare tre cucciolate.
	I maiali d'inizio sono 3.

	Il file allegato, porcil.sav, contiene un salvataggio di prova.

	Ho modificato il numero di kg di mangime per una moneta, altrimenti
	il giocatore poteva spendere tutti i soldi in cibo, nutrire un solo
	maiale, rivenderlo, e guadagnare 10 volte circa in piu' dei soldi 
	iniziali. Ora due kg di cibo costano una moneta, in modo che il 
	guadagno sia nettamente minore.

Documentazione:
	Ho generato la documentazione in html (cartella doc) delle varie 
	funzioni del progetto, tramite doxygen.
