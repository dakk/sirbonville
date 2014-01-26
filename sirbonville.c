/*
	Copyright 2014 Davide Gessa

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
/**
 * Programma principale, crea ed avvia la gui
 * 
 * \file sirbonville.c
 * \author Davide Gessa
 */
#include "core.h"
#include "gui.h"
#include <stdio.h>
#include <time.h>

/** Funzione principale */
int main(int argc, char **argv)
{
	Porcile porcile;

	/* Inizializzo il generatore di numeri random */
	srand(time(NULL));

	/* Visualizzo un messaggio in console */
	printf("%s\n", TITLE);

	/* Controlla se ci sono paramentri, nel caso il parametro sia un nomefile 
	 * di salvataggio carichiamo quella partita */
	if(argc == 2)
	{
		if(!sv_load(&porcile, argv[1]))
		{
			printf("Impossibile caricare lo stato del gioco dal file %s.\n", argv[1]);
		}
		else
			printf("Stato del gioco caricato dal file %s\n", argv[1]);
	}


	/* Crea la gui */
	gtk_init (&argc, &argv);
	
	if(!gui_create(&porcile))
	{
		printf("Errore durante il caricamento della gui. Uscita");
		return 0;
	}

	/* Mainloop della gui */
	gtk_main();	

	return 0;
}
