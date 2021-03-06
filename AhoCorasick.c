/* 
 * File:   AhoCorasick.c
 * Author: Lucas,Gaëtan
 *
 * Created on 28 septembre 2017, 19:07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define NBCARAC	26
#define FIRSTCARAC	97

char * ajouteprefix(int size,char * mot){
	char *prefix=calloc(size,sizeof(char));
	if(prefix==NULL){
		perror("malloc");
		exit(1);
	}
	memcpy(prefix, mot, size);
	return prefix;
}

char ** genere_prefix(char ** mots,int start,int end,int * nb_etats){			//argv,2,argc, un pointeur sur un entier
	int nbprefixmax=0,sizeprefixmax=0;
	for (int nummots=start; nummots < end; ++nummots){
		int taillemot=strlen(mots[nummots]);
		nbprefixmax+=taillemot;
		if (sizeprefixmax < taillemot){
			sizeprefixmax = taillemot;
		}
	}
	printf("sizeprefixmax = %d , nbprefixmax = %d \n",sizeprefixmax,nbprefixmax);		// affichage première partie 
	char ** prefix = malloc(sizeof(char *)*nbprefixmax);
	if(prefix==NULL){
		perror("malloc");
		exit(1);
	}
	int isfirst;
	int etats=0;
	for (int sizeprefix = 1; sizeprefix <= sizeprefixmax; ++sizeprefix){		// genere les prefix taille par taille
		isfirst=1;
		for (int nummots = start; nummots < end; ++nummots){	//parcour tout les mots
			if(sizeprefix <= strlen(mots[nummots])){
				if(isfirst){													//premier mot de cette taile
					isfirst=0;
					prefix[etats]=ajouteprefix(sizeprefix,mots[nummots]);
					etats++;
				}
				else {
					int tmp=strncmp(mots[nummots],prefix[etats-1],sizeprefix);	//comparaison des deux string jusqu'a sizeprefix
					if (tmp > 0){												
						prefix[etats]=ajouteprefix(sizeprefix,mots[nummots]);
						etats++;
					}
					else if(tmp < 0){											//tmp est censer etre positif vu qu'on a trié avant
						perror("tris ascii");
					}
				}
			}
		}
	}
	*nb_etats=etats;
	return prefix;
}

int ** tableau_commande(char ** prefix,int nb_etats){
	int ** commande=malloc(sizeof(int *)*nb_etats+1);
	if(commande==NULL){
		perror("malloc");
		exit(1);
	}
	for (int i = 0; i < nb_etats+1; ++i){
		commande[i]=calloc(NBCARAC,sizeof(int));
		if(commande[i]==NULL){
			perror("calloc");
			exit(1);
		}
	}
	int sizeprefix=1,firstprefix=-1,nextfirstprefix=0;
	for (int numprefix = 0; numprefix < nb_etats; ++numprefix){
		if(strlen(prefix[numprefix])==sizeprefix){
			if(sizeprefix==1){
				commande[0][prefix[numprefix][0]-FIRSTCARAC]=numprefix+1;			// met le numérot de l'etat correspondant au prefix dans la case de la lette/0 (prefix[numprefix][sizeprefix-1]-'a')
			}
			else{
				int preprefix=-1;
				for (int i = firstprefix; preprefix==-1 && i < nextfirstprefix; ++i){
					if(strncmp(prefix[numprefix],prefix[i],sizeprefix-1) == 0){	//comparaison des deux string jusqu'a sizeprefix-1
						preprefix=i;											// on trouve le préfix du préfix
					}
				}
				if (preprefix!=-1){
					commande[preprefix+1][prefix[numprefix][sizeprefix-1]-FIRSTCARAC]=numprefix+1;	// met le numérot de l'etat correspondant au prefix dans la case de la lette/preprefix (prefix[numprefix][sizeprefix-1]-'a')
				}
				else{
					printf("%s %s %s \n",prefix[numprefix],prefix[firstprefix],prefix[nextfirstprefix]);			//perror si on a pas trouver le prefix du prefix
					perror("commande");
				}
			}
		}
		else{													//on change et on recommence car prefix de taille supérieur
			sizeprefix++;
			firstprefix=nextfirstprefix;
			nextfirstprefix=numprefix;
			numprefix--;												// est-ce moche ?
		}
	}
	return commande;
}

void show_tableau_commande(int ** commande, int nb_etats){
	printf("\ntableau commande :\n");
	for (int j = 0; j < NBCARAC; ++j){
		printf("\t%c",j+FIRSTCARAC );
	}
	for (int i = 0; i < nb_etats+1; ++i){
		printf("%d :\t", i);
		for (int j = 0; j < NBCARAC; ++j){
			printf("%d\t", commande[i][j]);
		}
		printf("\n");
	}
}

int * tableau_erreur(char ** prefix,int nb_etats){
	int * tab_erreur=calloc(nb_etats,sizeof(int));
	for (int numprefix = 0; numprefix < nb_etats; ++numprefix){
		if(strlen(prefix[numprefix])>1){											// pour tout les prefix de plus d'un lettre
			int sufix=0;
			int diffsize=strlen(prefix[numprefix])-strlen(prefix[sufix]);
			while(diffsize>0){														// on regarde parmis tout les prefix plus petit qu'eux
				if(strcmp(prefix[numprefix]+diffsize,prefix[sufix]) == 0 ){			// si l'un d'entre eux resemble a leur sufix et on prend le plus grand d'entre eux
					tab_erreur[numprefix]=sufix+1;
				}
				++sufix;
				diffsize=strlen(prefix[numprefix])-strlen(prefix[sufix]);
			}
		}
	}
	return tab_erreur;
}

void show_tableau_erreur(int * erreur,int nb_etats){
	printf("\ntableau erreur :\n");
	for (int i = 0; i < nb_etats; ++i){
		printf("%d\t: %d\n",i,erreur[i]);
	}
}


void fuuuuusion(int ** commande,int * erreur,int nb_etats){
	for (int etat = 0; etat < nb_etats; ++etat){
		if(erreur[etat]>0){					// pour tout les etats qui on un retour en cas d'erreur
			for (int i = 0; i < NBCARAC; ++i){
				if(commande[etat+1][i]==0 && commande[erreur[etat]][i]!=0){
					commande[etat+1][i]=commande[erreur[etat]][i];			// ajout des transition non 0 de l'erreur[etat]  sur les transition 0 de l'etat
				}
			}
		}
	}
}
