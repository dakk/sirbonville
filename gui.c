/**
 * Contiene le funzioni che gestiscono la gui e gli eventi
 *
 * \file gui.c
 * \author Davide Gessa
 */
#include "gui.h"



/**
 * Inizializza la gui
 *
 * \param porc puntatore al porcile
 * \return 1 se l'inizializzazione e' andata a buon fine
 */
int gui_create(Porcile *porc)
{
	GtkWidget *main_box;
	GtkWidget *menu_bar;
	GtkWidget *menu_item;
	GtkWidget *menu_sub;
	GtkWidget *status_bar;
	GtkWidget *menu_file;
	GtkWidget *menu_help;
	GtkWidget *list_view;
	GtkWidget *scrolled_bar;
	GtkWidget *button_box;
	GtkWidget *button;
	GtkWidget *menu_porc;
	GtkTreeViewColumn *column;
	GuiSession *session;


	/* Controlliamo che porc sia un puntatore valido */
	if(porc == NULL)
		return 0;

	/* Alloco la memoria per la struttura di sessione */
	session = (GuiSession *)  malloc(sizeof(GuiSession));

	/* Controllo se e' stata allocata la memoria per session */
	if(session == NULL)
		return 0;

	/* Azzero il file di salvataggio e imposto il porcile nella sessione */
	session->savefile[0] = 0;
	session->porcil = porc;
	session->started = 0;
	
	/* Creo la finestra principale */
	session->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(session->main_window), TITLE);
	gtk_window_resize(GTK_WINDOW(session->main_window), 600, 400);

	g_signal_connect(G_OBJECT(session->main_window), "destroy", G_CALLBACK(gui_close_cb), session);


	/* Creo la box principale per il packing dei widget principali */
	main_box = gtk_vbox_new(0, 0);
	gtk_container_add(GTK_CONTAINER(session->main_window), main_box);


	/* Semplice menu' */
	menu_bar = gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(main_box),menu_bar, FALSE, FALSE,0);

	/* File menu */
    menu_file = gtk_menu_item_new_with_label("File");
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), menu_file);

	menu_sub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file), menu_sub);

	menu_item = gtk_menu_item_new_with_label ("Apri");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_load_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Nuovo");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_new_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_separator_menu_item_new();
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Salva");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_save_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Salva come..");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_save_as_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_separator_menu_item_new();
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Esci");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_close_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	
	/* Porcile menu */
    menu_porc = gtk_menu_item_new_with_label("Porcile");
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), menu_porc);

	menu_sub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_porc), menu_sub);

	menu_item = gtk_menu_item_new_with_label ("Compra cibo");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_buy_food_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Ciba maiale");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_feed_pig_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_menu_item_new_with_label ("Ciba maiali");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_feed_pigs_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_separator_menu_item_new();
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);
	
	menu_item = gtk_menu_item_new_with_label ("Pulisci");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_clear_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);

	menu_item = gtk_separator_menu_item_new();
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);
	
	menu_item = gtk_menu_item_new_with_label ("Vendi un maiale");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_sell_pig_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);



	/* Menu help */
    menu_help = gtk_menu_item_new_with_label("Aiuto");
	gtk_menu_item_set_right_justified(GTK_MENU_ITEM(menu_help), TRUE);
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), menu_help);

	menu_sub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_help), menu_sub);

	menu_item = gtk_menu_item_new_with_label ("About");
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(gui_about_cb), session);
	gtk_menu_append(GTK_MENU(menu_sub), menu_item);



	/* Button box con i vari tasti per le azioni di gioco */
	button_box = gtk_hbox_new(0, 0);
	gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
	
	button = gtk_button_new_with_label("Compra cibo");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_buy_food_cb), session);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	button = gtk_button_new_with_label("Ciba un maiale");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_feed_pig_cb), session);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	button = gtk_button_new_with_label("Ciba maiali");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_feed_pigs_cb), session);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	button = gtk_button_new_with_label("Pulisci");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_clear_cb), session);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	button = gtk_button_new_with_label("Vendi un maiale");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_sell_pig_cb), session);
	gtk_container_add(GTK_CONTAINER(button_box), button);



	/* Creo la liststore per imagazzinare le informazioni dei maialini; creo
	 * inoltre una treeview per la visualizzazione usando la liststore come modello, ed aggiungo
	 * ad essa le colonne */
	session->maial_list = gtk_list_store_new(8, GDK_TYPE_PIXBUF, GDK_TYPE_PIXBUF, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, 
												G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(session->maial_list));

	column = gtk_tree_view_column_new_with_attributes (" ", gtk_cell_renderer_pixbuf_new (), "pixbuf", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes (" ", gtk_cell_renderer_pixbuf_new (), "pixbuf", 1, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Id", gtk_cell_renderer_text_new (), "text", 2, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Eta'", gtk_cell_renderer_text_new (), "text", 3, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Peso", gtk_cell_renderer_text_new (), "text", 4, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Salute", gtk_cell_renderer_text_new (), "text", 5, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Fame", gtk_cell_renderer_text_new (), "text", 6, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);

	column = gtk_tree_view_column_new_with_attributes ("Bisogni", gtk_cell_renderer_text_new (), "text", 7, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);


	 
	/* Aggiungo la treeview in una scrolled bar */
	scrolled_bar = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_bar), list_view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_bar), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start(GTK_BOX(main_box), scrolled_bar, TRUE, TRUE, 0); 



	/* Status bar */
	session->status_bar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(main_box), session->status_bar, FALSE, FALSE, 0);
 

	/* Creo il filtro per il widget file_chooser */
	session->file_filter = gtk_file_filter_new();
	gtk_file_filter_set_name(GTK_FILE_FILTER(session->file_filter), "Sirbonville Save (.sav)"); 
	gtk_file_filter_add_pattern(GTK_FILE_FILTER(session->file_filter), "*.sav");


	/* Carica le icone disponibili dinamicamente; e' possibile aggiungere altre
	 * icone semplicemenete modificando ICON_NUM e aggiungendo un immagine con il
	 * nome adatto ed il formato ICON_FRMT */
	int i;
	char path[512];

	for(i = 0; i < ICON_NUM; i++)
	{
		sprintf(path, "%s%d%s", ICON_FILE, i+1, ICON_FRMT);
		session->icons[i] = gdk_pixbuf_new_from_file(path, NULL);
	}

	session->icon_letame = gdk_pixbuf_new_from_file(LETAM_FILE ICON_FRMT, NULL);

	/* Visualizzo tutti i widget della finestra */
	gtk_widget_show_all(session->main_window);


	/* Chiamo una volta la funzione di update per inizializzare la list_view, e poi impost
	 * una timeout callback richiamata secondo */
	gui_update_cb(session);
	g_timeout_add(1000, (GSourceFunc) gui_update_cb, session);
	session->sec2update = 0;


	return 1;
}


/** 
 * Funzione richiamata ogni secondo, aggiorna la listview e la statusbar; se i secondi passati equivalgono a quelli
 * di un unita' di tempo, aggiorno il porcile 
 *
 * \param ses puntatore alla sessione
 */
int gui_update_cb(GuiSession *ses)
{
	int i;
	int icon_num;
	GtkTreeIter it;
	maiale_t tmp_pig;
	char stat_text[1024];
	GdkPixbuf *letam;
	int morti = 0;


	/* Pulisco la liststore */
	gtk_list_store_clear(GTK_LIST_STORE(ses->maial_list));

	/* Se la partita non e' stata creata, la creo */
	if(!ses->started)
	{
		gui_new_cb(ses->main_window, ses);
	}

	/* Se i secondi passati sono uguali a quelli di un unita' di tempo della giocata, aggiorno le informazioni 
	 * del porcile e memorizzo il numero di maialini morti */
	if((++(ses->sec2update)) == ses->porcil->sec4unit)
	{
		ses->sec2update = 0;
		morti = sv_update(ses->porcil);
	}


	/* Inserisco i maiali nella liststore */
	for(i = 0; i < ses->porcil->n_maiali; i++)
	{
		/* Inizializzo una var d'appoggio con il maiale corrente */
		tmp_pig = ses->porcil->maiali[i];

		/* Calcolo dinamicamente l'immagine da abbinare al maiale corrente */
		icon_num = tmp_pig.eta / (VITA / ICON_NUM);

		/* Se i bisogni di un maiale superano i 90, visualizzo un icona che segnala che sta' per aumentare il letame
		 * del porcile */
		if(ses->porcil->maiali[i].bisogni > LETAME_ICON_LIMIT)
			letam = ses->icon_letame;
		else
			letam = NULL;

		/* Aggiunge il maiale alla list store; dopo l'append, la variabile it
		 * contiene un riferimento alla nuova riga inserita; dopodiche' la 
		 * modifichiamo */
		gtk_list_store_append (ses->maial_list, &it);
		gtk_list_store_set (ses->maial_list, &it,
						  0, ses->icons[icon_num],
						  1, letam,
                          2, i + 1,
                          3, tmp_pig.eta,
                          4, tmp_pig.peso,
						  5, tmp_pig.salute,
						  6, tmp_pig.fame,
						  7, tmp_pig.bisogni,
                          -1);
	} 

	/* Aggiorno la statusbar con le informazioni globali del porcile */
	sprintf(stat_text, "Monete: %d;  Letame: %d;  Cibo: %d;  Maiali: %d;  Tempo di gioco: %d",
						ses->porcil->monete, ses->porcil->letame, ses->porcil->cibo, 
						ses->porcil->n_maiali, ses->porcil->tempo);
	gtk_statusbar_push(GTK_STATUSBAR(ses->status_bar), 0, stat_text);


	/* Se il numero di maiali e' uguale a zero, mostriamo un messaggio di avviso e creiamo una nuova partita */
	if(ses->porcil->n_maiali == 0)
	{
		sprintf(stat_text, "Non hai piu' maiali, la partita e' finita'!\nHai giocato per %d unita'"
						   "di tempo, totalizzando %d monete.\n", ses->porcil->tempo, ses->porcil->monete);

		gui_show_message_box(ses, GTK_MESSAGE_INFO, stat_text);
		gui_new_cb(ses->main_window, ses);
		gui_update_cb(ses);	
	}

	/* Altrimenti, se son morti alcuni maiali, visualizzo un messaggio */
	else if(morti != 0)
	{
		if(morti == 1)
			sprintf(stat_text, "E' morto un maiale");
		else
			sprintf(stat_text, "Sono morti %d maiali.", morti);

		gui_show_message_box(ses, GTK_MESSAGE_WARNING, stat_text);
	}

	return 1;
}


/** 
 * Carica una partita salvata in precedenza 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_load_cb(GtkWidget* w, GuiSession *ses)
{
	GtkWidget *d;
	int ret;
	
	/* Creo un file_chooser_dialog */
	d = gtk_file_chooser_dialog_new("Carica una partita salvata", 
									GTK_WINDOW(ses->main_window),
									GTK_FILE_CHOOSER_ACTION_OPEN, 
									GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
									GTK_STOCK_OPEN,   GTK_RESPONSE_OK, NULL);

	/* Imposto il filtro per i tipi di file da riconoscere */
	/* Commentato per misteriosi segfault su windows */
	//gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(d), GTK_FILE_FILTER(ses->file_filter));

	/* Avvio la dialog_box e salvo il risultato */
	ret = gtk_dialog_run(GTK_DIALOG(d));

	/* Se il tasto premuto nel dialog ha generato il risultato OK, eseguo il caricamento */
	if(ret == GTK_RESPONSE_OK)
	{ 
		strcpy(ses->savefile, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(d)));
	
		if(!sv_load(ses->porcil, ses->savefile))
		{
			/* Errore nel caricamento del file, visualizzo un message box */
			gui_show_message_box(ses, GTK_MESSAGE_WARNING, 
									"Errore nel caricamento della partita!\n"
									"Verra creata una partita vuota.\n");
			gui_new_cb(w, ses);
		}
		else
		{
			char title[256];

			sprintf(title, "%s - %s", TITLE, ses->savefile);
			gtk_window_set_title(GTK_WINDOW(ses->main_window), title);
			ses->sec2update = 0;
			ses->started = 1;
		}
	}

	gtk_widget_destroy(d);
}




/** 
 * Salva con nome della partita
 *  
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_save_as_cb(GtkWidget* w, GuiSession *ses)
{
	GtkWidget *d;
	int ret;
	char title[512];
	
	d = gtk_file_chooser_dialog_new("Salva come...", 
									GTK_WINDOW(ses->main_window),
									GTK_FILE_CHOOSER_ACTION_SAVE, 
									GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
									GTK_STOCK_SAVE,   GTK_RESPONSE_OK, NULL);

	/* Imposto il filtro per i tipi di file da riconoscere */
	/* Commentato per misteriosi segfault su windows */
	//gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(d), GTK_FILE_FILTER(ses->file_filter));


	/* Avvio la dialog box */
	ret = gtk_dialog_run(GTK_DIALOG(d));


	/* Se viene premuto il tasto ok, aggiorno il nome del file, ed imposto il titolo */
	if(ret == GTK_RESPONSE_OK)
	{ 
		strcpy(ses->savefile, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(d)));	

		gui_save_cb(w, ses);
		
		sprintf(title, "%s - %s", TITLE, ses->savefile);
		gtk_window_set_title(GTK_WINDOW(ses->main_window), title);
	}

	gtk_widget_destroy(d);
}




/** 
 * Salvataggio di una partita 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_save_cb(GtkWidget* w, GuiSession *ses)
{
	/* Se nella sessione non e' ancora memorizzato nessun nomefile, 
	 * richiamo la save as */
	if(ses->savefile[0] == 0)
		gui_save_as_cb(w, ses);



	/* Altrimenti effettuo il salvataggio e visualizzo un opportuno messaggio
	 * a seconda dell'esito del salvataggio */
	else
	{
		if(sv_save(ses->porcil, ses->savefile))
			gui_show_message_box(ses, GTK_MESSAGE_INFO, "Partita salvata");
		else
			gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Errore nel salvare la partita");	
	}
}




/** 
 * Nuova partita 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_new_cb(GtkWidget* w, GuiSession *ses)
{
	unsigned how;

	/* Se un altra partita e' in corso, chiede se vuole salvare */
	if((ses->started) && (gui_ask_for_exit(ses, "Vuoi salvare prima la partita corrente?")))
		gui_save_cb(w, ses);		


	/* Imposto il titolo di default */
	gtk_window_set_title(GTK_WINDOW(ses->main_window), TITLE);

	/* Richiede la velocita' da utilizzare per il gioco */
	how = gui_entry_dialog(ses, "Secondi per unita' di tempo", "Immetti il numero di secondi"
							" da attribuire ad\nun unita' di tempo:", "Nuova partita", 1, MAX_UNIT_SECONDS);
	
	/* Se non e' stato inserito nessun numero, ma c'era gia' un altra partita in corso,
	 * termino la funzione e torno alla partita */
	if((how == -1) && ses->started)
		return;

	/* Se non e' stato inserito nessun numero, e non c'era nessuna partita in corso,
	 * imposto un valore di default */
	if(how == -1)
		how = DEFAULT_SEC4UNIT;
	
	/* Creo la nuova partita */
	sv_new(ses->porcil, how);
	ses->savefile[0] = 0;
	ses->sec2update = 0;
	ses->started = 1;
}




/** 
 * Callback per l'opzione del menu che visualizza il creatore del software
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_about_cb(GtkWidget* w, GuiSession *ses)
{
	gui_show_message_box(ses, GTK_MESSAGE_INFO, "Progetto di fine corso PR1\n"
												"© Davide Gessa 2011, all pigs reserved\ndagide91@gmail.com");
}




/** 
 * Callback di chiusura del programma 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_close_cb(GtkWidget* w, GuiSession *ses)
{
	/* Chiede se vogliamo uscire seriamente */
	if(gui_ask_for_exit(ses, "Vuoi salvare prima di uscire?"))
		gui_save_cb(w, ses);

	/* Chiude la gui */
	gtk_main_quit();

	/* Dealloca la sessione */
	free(ses);
}




/** 
 * Semplice funzione che visualizza una message box 
 *	
 * \param ses puntatore alla sessione
 * \param type tipo di messaggio
 * \param text, testo della message box
 */
void gui_show_message_box(GuiSession *ses, GtkMessageType type, char *text)
{
	GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(ses->main_window),
					GTK_DIALOG_DESTROY_WITH_PARENT, type, GTK_BUTTONS_OK, text);

	gtk_dialog_run(GTK_DIALOG(d));
	gtk_widget_destroy(d);
}



/**
 * Visualizza una dialog box con due bottoni, per domande yes/no
 *
 * \param ses puntatore alla sessione
 * \param text stringa da visualizzare
 *
 * \return 1 se ha risposto si
 */
int gui_ask_for_exit(GuiSession *ses, char *text)
{
	GtkWidget *d;
	int value = 0;
	GtkWidget *content_area;
		
	d = gtk_dialog_new();

	gtk_dialog_add_button(GTK_DIALOG(d), "No", BUTTON_CLOSE_ID);
	gtk_dialog_add_button(GTK_DIALOG(d), "Si", BUTTON_OK_ID);
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(d));

	gtk_container_add(GTK_CONTAINER(content_area), gtk_label_new(text));

	gtk_widget_show_all(d);

	if(gtk_dialog_run(GTK_DIALOG(d)) == BUTTON_OK_ID)
		value = 1;

	gtk_widget_destroy(d);

	return value;
}




/** 
 * Dialog box per l'inserimento di un numero tramite entry 
 *
 * \param ses puntatore alla sessione
 * \param title titolo della finestra
 * \param text testo di descrizione
 * \param button_text testo del tasto di conferma
 * \param min numero minimo inseribile
 * \param max numero massimo inseribile
 *
 * \return valore inserito
 */
int gui_entry_dialog(GuiSession *ses, char *title, char *text, char *button_text, int min, int max)
{
	GtkWidget *d;
	int value = -1;
	GtkWidget *content_area;
	GtkWidget *entry;
		
	d = gtk_dialog_new();

	gtk_dialog_add_button(GTK_DIALOG(d), "Chiudi", BUTTON_CLOSE_ID);
	gtk_dialog_add_button(GTK_DIALOG(d), button_text, BUTTON_OK_ID);
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(d));
	entry = gtk_spin_button_new_with_range(min, max, 1);

	gtk_container_add(GTK_CONTAINER(content_area), gtk_label_new(text));
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(d);

	if(gtk_dialog_run(GTK_DIALOG(d)) == BUTTON_OK_ID)
		value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	
	gtk_widget_destroy(d);

	return value;
}



/** 
 * Compra kg di cibo 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_buy_food_cb(GtkWidget *w, GuiSession *ses)
{
	char info_txt[256];
	int how;

	/* Se non abbiamo abbastanza monete, termina la funzione */
	if(ses->porcil->monete / PREZZO_CIBO == 0)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Non hai monete a sufficienza!");
		return;
	}

	sprintf(info_txt, "Quante decine di kg di cibo vuoi comprare (%d kg, 1 moneta)? (1 .. %d)", PREZZO_CIBO, ses->porcil->monete);
	how = gui_entry_dialog(ses, "Quantita'", info_txt, "Compra", 1, ses->porcil->monete);

	if(how == -1)
		return;

	how = sv_buy_food(ses->porcil, how);

	sprintf(info_txt, "Sono state spese %d monete!", how);
	gui_show_message_box(ses, GTK_MESSAGE_INFO, info_txt);
}



/** 
 * Puliamo il porcile 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_clear_cb(GtkWidget *w, GuiSession *ses)
{
	char info_txt[256];
	int how;

	/* Se non c'e' letame o se non abbiamo abbastanza soldi, termina la funzione */
	if(ses->porcil->letame == 0)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Non e' presente letame nel porcile!");
		return;
	}
	if(ses->porcil->monete < PREZZO_LETAME)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Non hai monete a sufficienza!");
		return;
	}


	sprintf(info_txt, "Quanti kg di letame vuoi eliminare? (1 .. %d)", ses->porcil->letame);
	how = gui_entry_dialog(ses, "Quantita'", info_txt, "Pulisci", 1, ses->porcil->letame);

	if(how == -1)
		return;

	how = sv_clear(ses->porcil, how);

	sprintf(info_txt, "Sono state spese %d monete!", how);
	gui_show_message_box(ses, GTK_MESSAGE_INFO, info_txt);	
}



/** 
 * Nutriamo un maialino 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_feed_pig_cb(GtkWidget *w, GuiSession *ses)
{
	char info_txt[256];
	int how, who;

	if(ses->porcil->cibo == 0)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Non hai cibo!");
		return;
	}


	sprintf(info_txt, "Quale maiale vuoi nutrire? (1 .. %d)", ses->porcil->n_maiali);
	who = gui_entry_dialog(ses, "Selezione maiale", info_txt, "Seleziona", 1, ses->porcil->n_maiali);

	if(who == -1)
		return;

	sprintf(info_txt, "Quanti kg di cibo vuoi dare? (1 .. %d)", ses->porcil->cibo);
	how = gui_entry_dialog(ses, "Quantita' di cibo", info_txt, "Ciba", 1, ses->porcil->cibo);

	if(how == -1)
		return;

	how = sv_feed(ses->porcil, how, who - 1);

	sprintf(info_txt, "Operazione effetuata!");
	gui_show_message_box(ses, GTK_MESSAGE_INFO, info_txt);	
}



/** 
 * Nutriamo tutti i maialini equamente 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_feed_pigs_cb(GtkWidget *w, GuiSession *ses)
{
	char info_txt[256];
	int how;		

	/* Se il cibo totale e' minore del numero dei maiali, non c'e' abbastanza cibo per
	 * cibare equamente tutti i maiali */
	if(ses->porcil->cibo < ses->porcil->n_maiali)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Non hai abbastanza cibo!");
		return;
	}

	sprintf(info_txt, "Quanti kg di cibo vuoi dare a ciascun maiale? (1 .. %d)", 
			ses->porcil->cibo / ses->porcil->n_maiali);
	how = gui_entry_dialog(ses, "Quantita' di cibo", info_txt, "Ciba", 1, ses->porcil->cibo / ses->porcil->n_maiali);

	if(how == -1)
		return;

	how = sv_feed_all(ses->porcil, how);

	sprintf(info_txt, "Operazione effetuata!");
	gui_show_message_box(ses, GTK_MESSAGE_INFO, info_txt);	
}



/** 
 * Vendiamo un maialino 
 *
 * \param w widget del chiamante
 * \param ses puntatore alla sessione
 */
void gui_sell_pig_cb(GtkWidget *w, GuiSession *ses)
{
	char info_txt[256];
	int who;

	/* Se c'e' solo un maiale, non lo posso vendere */
	if(ses->porcil->n_maiali == 1)
	{
		gui_show_message_box(ses, GTK_MESSAGE_WARNING, "Hai solo un maiale, se lo vendi hai perso!");
		return;
	}

	sprintf(info_txt, "Quale maiale vuoi vendere? (1 .. %d)", ses->porcil->n_maiali);
	who = gui_entry_dialog(ses, "Selezione maiale", info_txt, "Seleziona", 1, ses->porcil->n_maiali);

	if(who < 1)
		return;

	who = sv_sell(ses->porcil, (who - 1));

	sprintf(info_txt, "Maiale venduto!\nHai guadagnato %d monete.", who);
	gui_show_message_box(ses, GTK_MESSAGE_INFO, info_txt);	
}


