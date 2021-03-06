/**
 * \file mydata.c
 * \brief Mydata structure avec les objets principaux
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Mydata structure avec les objets principaux
 *
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "mydata.h"

#define MYDATA_MAGIC 0x46EA7E05

// Cette fonction permet de tester si le data que l’on a recuperé dans
// une callback contient bien my ; sinon, cela veut dire que :
//  - soit on a oublié de transmettre my dans g_signal_connect,
//  - soit on s’est trompé dans le nombre de paramètres de la callback.

/**
 * \fn Mydata *get_mydata (gpointer data)
 * \brief Fonction de récupération de l'objet Mydata à partir d'un gpointeur
 *
 * \param self Un Mydata passé sous forme de gpointer
 * \return Mydata * si aucune erreur, NULL sinon.
 */
Mydata *get_mydata (gpointer data){
    if (data == NULL) {
        fprintf (stderr, "get_mydata: NULL data\n"); return NULL;
    }
    if (((Mydata *)data)->magic != MYDATA_MAGIC) {
        fprintf (stderr, "get_mydata: bad magic number\n"); return NULL;
    }
    return data;
}

/**
 * \fn void init_mydata(Mydata *my)
 * \brief Fonction d'initialisation de l'objet Mydata
 *
 * \param self Un objet Mydata
 * \return void
 */
void init_mydata(Mydata *my){
    my->title = "Aho Corasick";
    my->win_width = 1280;
    my->win_height = 920;
    my->magic = MYDATA_MAGIC;
    my->current_folder = NULL;
    my->p_text_view = NULL;
    my->contents = NULL;
	my->p_text_view_scroll = NULL;
	my->search_bar = NULL;
	my->entry = NULL;
}

