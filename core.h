/**
 * Funzioni che riguardano il porcile e il gamelogic in se'
 *
 * \file core.h
 * \author Davide Gessa
 */
#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FAME				4		///< Unita' di tempo per aumentare di un unita' la fame dei maiali
#define BISOGNI				5		///< Stessa cosa di FAME ma per i bisogni fisiologici
#define NASCITA				150		///< Unita' di tempo dopo il quale nascono random maiali
#define VITA				500		///< Unita' di tempo dopo il quale muore un maiale
#define MAX_PESO			200		///< Peso massimo dopo il quale diminuisce la salute del maiale
#define MAX_LETAME			300		///< Kg di letame dopo il quale i maiali perdono salute
#define MAX_MAIALI			10		///< Numero massimo di maiali nella fattoria
#define MAX_UNIT_SECONDS	10		///< Secondi massimi per unita' di tempo
#define DEFAULT_SEC4UNIT	2		///< Secondi per unita' di tempo di default
#define START_MAIALI		3		///< Numero maiali all'inizio del gioco
#define START_CIBO			START_MAIALI * 50		///< Kili di cibo all'inizio del gioco
#define START_MONETE		START_MAIALI * 50		///< Numero monete all'inizio del gioco

/** Valori di default dei nuovi nati */
#define START_SALUTE		100
#define START_BISOGNI		50
#define START_FAME			50
#define START_PESO			50

#define DEFAULT_FILENAME	"porcile.sav"	///< Nome di default per il file di salvataggio

#define PREZZO_CIBO			1				///< Kg di cibo per una moneta
#define PREZZO_LETAME		1				///< Prezzo per eliminare un kg di letame

#define VERSION				"1.0"					///< Versione del gioco
#define TITLE				"SirbonVille " VERSION	///< Titolo del gioco

/** Struttura che contiene le informazioni di un maiale */
typedef struct
{
	int			eta;			///< Eta'
	int			peso;			///< Peso
	int			salute;			///< Tasso di salute
	unsigned	fame;			///< Stato di fame
	unsigned	bisogni;		///< Bisogno fisologici
} maiale_t;




/** Struttura che contiene le informazioni del porcile */
typedef struct
{
	unsigned letame;				///< Kg di letame nel procile
	unsigned monete;				///< Quantita' di monete disponibili
	unsigned cibo;					///< Kg di cibo disponibile
	unsigned tempo;					///< Unita' di tempo della giocata

	unsigned n_maiali;				///< Numero di maiali		(posso farne a meno)
	maiale_t maiali[MAX_MAIALI];	///< Array dei maiali della fattori

	
	unsigned	sec4unit;			///< Secondi per unita' di tempo
} Porcile;


/** Dichiarazioni delle funzioni */
int					sv_save					(Porcile *, char *);
int					sv_load					(Porcile *, char *);
void				sv_new					(Porcile *, unsigned);
void				sv_recompact			(Porcile *);
int					sv_check_end			(Porcile *);
int					sv_update				(Porcile *);
void				sv_new_default_maial	(Porcile *, int);

int					sv_buy_food				(Porcile *, int);
int					sv_clear				(Porcile *, int);
int					sv_sell					(Porcile *, int);
int					sv_feed					(Porcile *, int, int);
int					sv_feed_all				(Porcile *, int);

#endif
