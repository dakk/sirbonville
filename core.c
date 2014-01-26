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
 * Funzioni che riguardano il porcile e il gamelogic in se'
 *
 * \file core.c
 * \author Davide Gessa
 */
#include "core.h"

/**
 * Aggiorna le stats dei maiali e del porcile 
 *
 * \param p puntatore al porcile
 * \return numero di maiali morti in questa iterazione
 */
int sv_update(Porcile *p)
{
	int morti = 0;
	int i;
	int nmai = p->n_maiali;


	/* Aggiorno il tempo totale di gioco */
	p->tempo++;



	/* Aggiorniamo le statistiche dei maiali, in ordine, come scritto nelle specifiche */
	for(i = 0; i < nmai; i++)
	{
		/* Fame */
		if((p->tempo % FAME) == 0)
			p->maiali[i].fame = (p->maiali[i].fame + 1)  % 100;
		




		/* Bisogni */
		if((p->tempo % BISOGNI) == 0)
			p->maiali[i].bisogni++;




		/* Salute */
		/* Quando la fame raggiunge il massimo, l'animale perde due punti salute ogni
		 * unita' di tempo */
		if(p->maiali[i].fame == 100)
			p->maiali[i].salute -= 2;
			
		if(p->maiali[i].peso > MAX_PESO)
			p->maiali[i].salute -= 1;

		/* Sotraggo alla salute il danno inflitto dal letame in eccedenza, se presente */
		if(p->letame > MAX_LETAME)
			p->maiali[i].salute -= 1; 





		/* Peso */
		/* Quando la fame raggiunge i 60, l'animale perde un chilo ogni unita' di tempo*/
		if(p->maiali[i].fame >= 60)
			p->maiali[i].peso -= 1;




		/* Letame */
		if(p->maiali[i].bisogni == 100)
		{
			p->letame += ((p->maiali[i].peso / 10));
			p->maiali[i].bisogni = 0;
		}




		/* Aggiorno l'eta' */
		p->maiali[i].eta += 1;

		/* Se e' vecchio, muore quando la salute e' sotto i 60, se e' giovane, muore se e' minore o uguale
		 * a 30; inoltre muore anche se supera il tempo massimo di vita o se il peso raggiunge lo zero */
		if(	((p->maiali[i].eta < 2 * VITA / 3) && (p->maiali[i].salute <= 30)) || 
			((p->maiali[i].eta >= 2 * VITA / 3) && (p->maiali[i].salute < 60)) ||
			(p->maiali[i].eta >= VITA) || (p->maiali[i].peso <= 0))
		{
			p->maiali[i].eta = -1;
			p->n_maiali--;
			morti++;
		}
	}

	sv_recompact(p);



	/* Se e' il giorno giusto genera nuovi maialini random*/
	if((p->tempo % NASCITA) == 0)
	{
		int nascite = (rand() % (p->n_maiali / 2)) + 1;

		if(nascite > 0)
		{
			for(i = 0; (i < nascite) && ((i + p->n_maiali) < MAX_MAIALI); i++)
				sv_new_default_maial(p, i + p->n_maiali);

			p->n_maiali += i;

			/* I maiali in piu' vengono venduti a 50 monete ciascuno */
			p->monete += (nascite - i) * 50;

			sv_recompact(p);
		}
	}

	/* Restituisco il numero di maiali morti in questa iterazione */
	return morti;
}



/**
 * Ricompatta i maiali ancora vivi dell'array
 *
 * \param p puntatore al porcile da ricompattare
 */
void sv_recompact(Porcile *p)
{
	int i, k;
	int n_mai = 0;

	
	/* Per ogni posizione dell'array del maiale, se il maiale
	 * i e' morto, scorre la restante parte dell'array, se alla
	 * posizione j c'e' un maiale vivo, sposta il maiale da j a i
	 * e continua l'analisi dell'array. */
	for(i = 0; i < MAX_MAIALI; i++)
	{
		if(p->maiali[i].eta < 0)
		{
			for(k = i + 1; k < MAX_MAIALI; k++)
			{
				if(p->maiali[k].eta >= 0)
				{
					p->maiali[i] = p->maiali[k];
					p->maiali[k].eta = -1;

					k = MAX_MAIALI;
				}
			}
		}
	}
}



/**
 * Inizializza il porcile con i valori di default per una nuova partita a sirbonville
 *
 * \param p puntatore al porcile
 * \param sec4unit secondi per unita' di tempo
 */
void sv_new(Porcile *p, unsigned sec4unit)
{
	int i;

	p->n_maiali = START_MAIALI;
	p->monete = START_MONETE;
	p->cibo = START_CIBO; 
	p->letame = 0;
	p->n_maiali = START_MAIALI;
	p->tempo = 0;
	p->sec4unit = sec4unit;

	for(i = 0; i < MAX_MAIALI; i++)
	{
		if(i > START_MAIALI) 
			p->maiali[i].eta = -1;
		else
			sv_new_default_maial(p, i);
	}
}



/**
 * Inizializza un maiale con i valori di nascita 
 *
 * \param p puntatore al maiale
 * \param i numero del maiale
 */
void sv_new_default_maial(Porcile *p, int i)
{
	if(i < MAX_MAIALI)
	{
		p->maiali[i].eta = 0;
		p->maiali[i].salute = START_SALUTE;
		p->maiali[i].bisogni = START_BISOGNI;
		p->maiali[i].fame = START_FAME;
		p->maiali[i].peso = START_PESO;
	}
}

/**
 * Salva lo stato del porcile in un file
 *
 * \param p puntatore al porcile da salvare
 * \param fn nome/path del file in cui salvare
 * \return 1 se il salvataggio e' andato a buon fine
 */
int sv_save(Porcile *p, char *fn)
{
	FILE *f;
	
	/* Apre il file in lettura di bytes */
	f = fopen(fn, "wb");

	/* Se il file non e' stato aperto corretamente, termina la funzione con un esito negativo */
	if(f == NULL)
		return 0;

	/* Scrive nel file precedentemente aperto, una quantita' di bytes uguale alla dimensione
	 * della struttura porcile, prendendo i dati dalla struttura puntata da p, dopodiche'
	 * chiude il file */
	fwrite(p, sizeof(Porcile), 1, f);
	fclose(f);

	return 1;
}




/**
 * Carica lo stato del porcile da un file
 *
 * \param p puntatore al porcile nel quale vorremmo mettere i dati caricati dal file
 * \param fn nome/path del file in cui caricare
 * \return 1 se il caricamento e' andato a buon fine
 */
int sv_load(Porcile *p, char *fn)
{
	FILE *f;
	
	/* Apre il file in lettura di bytes */
	f = fopen(fn, "rb");

	/* Se il file non e' stato aperto corretamente, termina la funzione con un esito negativo */
	if(f == NULL)
		return 0;

	/* Legge dal file precedentemente aperto, una quantita' di bytes uguale alla dimensione
	 * della struttura porcile, ed imagazzina i dati nella struttura puntata da p, dopodiche'
	 * chiude il file */
	fread(p, sizeof(Porcile), 1, f);
	fclose(f);

	return 1;
}




/**
 * Nutre un maiale
 *
 * \param p puntatore al porcile
 * \param quant quantita' di cibo da dare al maiale
 * \param who maiale da nutrire
 *
 * \return kg di cibo effettivamente distribuito
 */
int sv_feed(Porcile *p, int quant, int who)
{
	unsigned peso = 0;

	if((who > p->n_maiali) || (quant == 0) || (p->cibo == 0))
		return 0;

	if(quant > p->cibo)
		quant = p->cibo;

	if(quant > p->maiali[who].fame)
	{
		peso = quant - p->maiali[who].fame;
		quant = p->maiali[who].fame;
	}


	p->maiali[who].fame -= quant;
	p->maiali[who].peso += peso;
	p->cibo -= (quant + peso);

	return (quant + peso);
}



/**
 * Nutre tutti i maiali del porcile con una quantita' ugualle
 *
 * \param p puntatore al porcile
 * \param quant quantita' di cibo per ogni maiale
 *
 * \return kg di cibo distribuiti
 */
int sv_feed_all(Porcile *p, int quant)
{
	int i; 
	int kg = 0;

	for(i = 0; i < p->n_maiali; i++)
		kg += sv_feed(p, quant, i);

	return kg;
}



/**
 * Vende un maiale
 *
 * \param p puntatore al porcile
 * \param who indice del maiale
 *
 * \return soldi guadagnati
 */
int sv_sell(Porcile *p, int who)
{
	int money;

	if(who > p->n_maiali)
		return 0;

	p->maiali[who].eta = -1;
	money = p->maiali[who].salute * p->maiali[who].peso / 100;
	p->monete += money;

	/* Aggiorno il numero di maiali e ricompatto il porcile */
	p->n_maiali--;
	sv_recompact(p);

	return money;		
}



/**
 * Elimina tot kg di letame dal porcile
 *
 * \param p puntatore al porcile
 * \param quant quantita' di letame da eliminare
 *
 * \return monete spese
 */
int sv_clear(Porcile *p, int quant)
{
	if(quant > p->letame)
		quant = p->letame;
	if((quant * PREZZO_LETAME) > p->monete)
		quant = p->monete / PREZZO_LETAME;

	p->letame -= quant;
	p->monete -= quant * PREZZO_LETAME;

	return (quant * PREZZO_LETAME);
}



/**
 * Compra del cibo
 *
 * \param p puntatore al porcile
 * \param quant monete da spendere
 * \return monete spese
 */
int sv_buy_food(Porcile *p, int quant)
{
	quant = quant > p->monete ? p->monete : quant;
	p->monete -= quant;
	p->cibo += quant * PREZZO_CIBO;

	return quant;
}
