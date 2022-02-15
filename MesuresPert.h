/* 	$Log: MesuresPert.h,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.3  2001/08/03 11:44:32  thierry
 * 	Ajout des commentaires
 *
 * 	Revision 1.2  2001/05/01 17:22:42  thierry
 * 	*** empty log message ***
 *	 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* Inclusion de la bibliotheque 'Synonymie.h' declaree localement */
#include "Synonymie.h"


#if !defined(PERTINENCE_H)
#define PERTINENCE_H

/***********************************************************************
 *        Definition des macros pour les codes de categorie            *
 ***********************************************************************/

#define CAT_NOM         0         /* Categorie Nom            */
#define CAT_ADJ         1         /* Categorie Adjectif       */
#define CAT_CT          2         /* Categorie Candidat Terme */

/***********************************************************************/

#define MAX_ELT_DIV 600       /* Nombre maximal d'element dans la table */

/**********************************************************************
 *               Table contenant les diversites de Shannon            *
 **********************************************************************/

struct Classif_Div_S {
  int Div_N1[MAX_ELT_DIV];    /* N1 du patron (N1 (de) N2) */
  int Div_N2[MAX_ELT_DIV];    /* N2 du patron (N1 (de) N2) */
  int Div_Adj[MAX_ELT_DIV];   /* Adj du patron (N Adj) */
  int Div_N[MAX_ELT_DIV];     /* N du patron (N Adj) */
};

#endif

/**********************************************************************
 *                Declaration des variables globales                  *
 **********************************************************************/

/************************************************************************
 *                                                                      *
 * Contenu : Table contenant les candidats termes classes en fonction   *
 *           de la diversite de Shannon                                 *
 *                                                                      *
 ************************************************************************/

extern struct Classif_Div_S Tab_Classif_DS;

/**********************************************************************
 *                      Declaration des fonctions                     *
 **********************************************************************/

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement de la diversite de Shannon dans un fichier *
 *                                                                      *
 * Argument : Nom du fichier                                            *
 *                                                                      *
 ************************************************************************/

int Affiche_Diversite(char *NomFichier);

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la diversite de Shannon pour un terme simple    *
 *                                                                      *
 * Argument : 1. Index du terme simples                                 *
 *                                                                      *
 *            2. Indication s'il s'agit d'un candidat terme en position *
 *               tete ou expansion                                      *
 *                                                                      *
 *            3. Indication du calcul d'une diversite normalisee        *
 *                                                                      *
 ************************************************************************/

int Calcul_Diversite_TS(int indT,int pos, int norm);

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la diversite de Shannon                         *
 *                                                                      *
 * Argument : Indication du calcul d'une diversite normalisee           *
 *                                                                      *
 ************************************************************************/

int Calcul_Diversite(int norm);

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de n*log(n)                                        *
 *                                                                      *
 * Argument : valeur de n                                               *
 *                                                                      *
 ************************************************************************/

double nlogn(double n);

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement  dans un fichier, des candidats termes     *
 *            classes suivant la diversite de Shannon                   *
 *                                                                      *
 * Argument : Nom du fichier                                            *
 *                                                                      *
 ************************************************************************/

int Affiche_Classif_Div_Shannon(char *NomFichier);

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de la categorie grammaticale a partir d'un   *
 *            lemme                                                     *
 *            Remarque : a placer dans un autre fichier                 *
 *                                                                      *
 * Argument : le lemme                                                  *
 *                                                                      *
 * Retour : Code de la categorie                                        *
 *                                                                      *
 ************************************************************************/

int Get_CatGram(char *lemme);

/************************************************************************
 *                                                                      *
 * Objectif : Classement des candidats termes en fonction de la valeur  * 
 *            de la diversite de Shannon, de la categorie grammaticale  * 
 *            et la position dans le candidat terme.                    *
 *                                                                      *
 ************************************************************************/

int Classif_Div_Shannon();
