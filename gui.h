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
 * Contiene le funzioni che gestiscono la gui e gli eventi
 *
 * \file gui.h
 * \author Davide Gessa
 */
#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <malloc.h>
#include "core.h"


#define ICON_FILE	"data/icon"			///< Path delle icone dei maiali
#define LETAM_FILE	"data/sterc_icon"	///< Path dell'icona del letame
#define ICON_FRMT	".png"				///< Formato delle icone
#define ICON_NUM	8					///< Numero icone dei maiali

/** 
 * Livello di bisogni, dopo il quale compare l'icona a
 * fianco al maiale
 * */
#define LETAME_ICON_LIMIT	96


/* Id dei tasti */
#define BUTTON_OK_ID		1
#define BUTTON_CLOSE_ID		2




/** Contiene i dati relativi ad una sessione del programma ed 
 *    hai widget che vengono modificati nel mainloop; allocando
 *    una struttura di questo tipo, e passando il puntatore 
 *    alle callback, le callback potranno utilizzare i widget
 *    modificabili della gui, e potranno interagire col porcile */
typedef struct gui_session_p
{
	unsigned sec2update;		///< Secondi dall'ultimo update
	unsigned started;			///< Indica se il gioco e' iniziato
	GtkWidget *status_bar;		///< Status bar per le varie info di gioco
	GtkWidget *main_window;		///< Widget della finestra principale
	GtkListStore *maial_list;	///< ListStore contenente la lista di visualizzazione dei maiali
	GdkPixbuf *icons[ICON_NUM];	///< Icone del maiale
	GdkPixbuf *icon_letame;		///< Icona che compare quando un maiale sta' per raggiungere i bisogni con valore 100
	Porcile *porcil;			///< Puntatore al porcile

	GtkFileFilter *file_filter;	///< Standard file filter for save/load

	char savefile[1024];		///< File di salvataggio
} GuiSession;



/* Funzioni di comune utilita' nel gioco */
void gui_show_message_box(GuiSession *, GtkMessageType, char *);
int gui_entry_dialog(GuiSession *, char *, char *, char *, int, int);
int gui_create(Porcile *);
int gui_ask_for_exit(GuiSession *, char *);


/* Callbacks delle azioni del programma */
void gui_load_cb(GtkWidget *, GuiSession *);
void gui_save_cb(GtkWidget *, GuiSession *);
void gui_save_as_cb(GtkWidget *, GuiSession *);
void gui_new_cb(GtkWidget *, GuiSession *);
void gui_about_cb(GtkWidget *, GuiSession *);
void gui_close_cb(GtkWidget *, GuiSession *);
int gui_update_cb(GuiSession *);


/* Callbacks delle azioni di gioco */
void gui_buy_food_cb(GtkWidget *, GuiSession *);
void gui_clear_cb(GtkWidget *, GuiSession *);
void gui_feed_pig_cb(GtkWidget *, GuiSession *);
void gui_feed_pigs_cb(GtkWidget *, GuiSession *);
void gui_sell_pig_cb(GtkWidget *, GuiSession *);



#endif
