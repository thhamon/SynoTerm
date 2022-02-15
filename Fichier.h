/* 	$Log: Fichier.h,v $
/* 	Revision 1.2  2006/11/23 14:04:00  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.4  2001/07/28 13:54:22  thierry
 * 	*** empty log message ***
 *
 * 	Revision 1.3  2001/07/28 12:06:43  thierry
 * 	Ajout de commentaires
 * 	Il reste une fonction (charge_db) a commenter
 *
 * 	Revision 1.2  2001/05/01 17:21:24  thierry
 * 	*** empty log message ***
 *	 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "SynoTerm_Gen.h"
#include "VarSem.h"

#if !defined(FICHIER_H)
#define FICHIER_H

/****************************************************/
/* Nombre d'entree dans la table des fichiers       */
/****************************************************/

#define NBFICHIER 	10

#define EXT_FICHIER_ST_TMP ".tmp"
#define EXT_FICHIER_ST_CT  ".db"
#define EXT_FICHIER_ST_TETES ".tetes"
#define EXT_FICHIER_ST_EXPANSIONS ".exps"
#define EXT_FICHIER_ST_ENTETE ".ent"
#define EXT_FICHIER_ST_RESULTATS ".res"
/* #define EXT_FICHIER_ST_ "" */


#define NOM_MIN 2
#define SEP_REP '/'
#define SEP_EXT '.'
#define SEP_EXT_STRING "."
#define REP_CUR "."
#define SEP_LIGNE '\n'
#define EXT_GNM "db"
#define EXT_RES "res"
#define EXT_LOG "log"
#define BASE_LOG "log/SynoTermM"


/**********************************************/
/* Nombre d'entree dans la table des fichiers */
/**********************************************/

#define NBFICHIER 	10

/*********************************************/
/* Macro pour les codes des types de fichier */
/**********************************************/

#define ALL 		0 /* Tous les types de fichier */
#define CORPUS 		1 /* Type des fichiers contenant un corpus */
#define SYN 		2 /* Type des fichiers contenant un dictionnaire de synonymes */
#define NEW 		3 /* Type des fichiers nouvellement crees */
#define TERMLIST        4 /* Type des fichiers conteant une liste de termes */

#define MOCTETS		1048576L /* Un MegaOctet */
#define KOCTETS		1024L /* Un KiloOctect */

/****************************************************************
 * Definition de la structure contenant les informations sur un *
 * fichier                                                      *
 ****************************************************************/

typedef struct {
  char *nom;                /* Nom du fichier  */
  int type;                 /* Type du fichier */
  int typo;                 /* Fichier en typpographie pauvre ou non */
  int nb_champs;            /* Nombre de champs dans les enregistrements du fichier */
  char **lst_champs;        /* Liste des noms des champs */
  int nb_fichiers_origines; /* Nombre de fichiers utilises a l'origine pour generer 
                               le fichier */
  char **fichiers_origines; /* Nom des fichiers utilises a l'origine pour generer 
                               le fichier */
  
  struct Donnees_Nom_Fichier *info_fichier; /* info sur le fichier */

  int nbrgnmax;             /* Nombre de groupes nominaux maximaux dans le fichier
                               (utilise dans les fichiers de type corpus */
  int nbrct;                /* Nombre d'occurences de candidats termes dans le fichier
                               (utilise dans les fichiers de type corpus */
  int nbrctdiff;            /* Nombre de candidats termes (sans doublons) dans le 
                               fichier (utilise dans les fichiers de type corpus */
  int fichierval;           /* Indication si le fichier a ete totalement valide ou non 
                               (utilise dans les fichiers resultats (new) */
  int nbtetes;              /* Nombre de tetes dans le fichier (utilise dans les 
                               fichiers de type corpus */
  int nbexps;               /* Nombre d'expansion dans le fichier (utilise dans les 
                               fichiers de type corpus */
} FichierMem;

/* Structure contenant les informations utiles sur le nom du fichier */

struct Donnees_Nom_Fichier {
  char *dirname;  /* Le nom du repertoire (dirname)  */
  char *basename; /* Le nom du fichier (basename) */
  char *fichname; /* Le nom du fichier sans l'extension */
  char *extname;  /* L'extension du fichier */
};

#endif


/********************************************************************** 
 *                Declaration des variables globales                  * 
 **********************************************************************/



/************************************************************************
 *                                                                      *
 * Contenu : Table contenant un historique des fichiers ouverts         *
 *                                                                      *
 ************************************************************************/

extern int Historique[10];

/************************************************************************
 *                                                                      *
 * Contenu : Index de l'historique des fichiers ouverts                 *
 *                                                                      *
 ************************************************************************/

extern int Hcur;

/************************************************************************
 *                                                                      *
 * Contenu : Table contenant les fichiers ouverts dans SynoTerm         *
 *                                                                      *
 ************************************************************************/

extern FichierMem liste_fichier[NBFICHIER];

/************************************************************************
 *                                                                      *
 * Contenu : Nombre de fichiers presents dans la table liste_fichier    *
 *                                                                      *
 ************************************************************************/

extern int nbre_fichier;

/********************************************************************** 
 *                      Declaration des fonctions                     * 
 **********************************************************************/

int charger_db(char *NomFich, char *NomFichbase, struct Donnees_Nom_Fichier*);

/************************************************************************
 *                                                                      *
 * Objectif : Charge un dictionnaire au format TEI (Non Implemente)     *
 *                                                                      *
 * Argument : Nom du dictionnnaire                                      *
 *                                                                      *
 ************************************************************************/

int charge_Dico_TEI(char *nom_dico);

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation dess informations concernant un type de      *
 *            fichiers ouverts dans SynoTerm et contenus la table des   *
 *            fichiers                                                  *
 *            Les messages et les informations sont envoyees sur la     * 
 *            sortie standard                                           *
 *                                                                      *
 * Argument : Type de fichiers ouverts dans SynoTerm                    *
 *                                                                      *
 ************************************************************************/

int info_table(int, int);

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de l'index du fichier dont le nom est fourni *
 *            en argument                                               *
 *                                                                      *
 * Argument : Nom du fichier dont on veut l'index                       *
 *                                                                      *
 * Retour : index du fichier                                            *
 *                                                                      *
 ************************************************************************/

int get_table(char *NomFich);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche du premier fichier de la table des fichiers     *
 *            ouverts dont le type est 'typeFich'                       *
 *            Remarque : interet ?                                      *
 *                                                                      *
 * Argument : Type du fichier a rechercher                              *
 *                                                                      *
 * Retour : index du fichier                                            *
 *                                                                      *
 ************************************************************************/

int get_table_type(int typeFich);

/************************************************************************
 *                                                                      *
 * Objectif : Suppression d'un fichier de la table des fichiers         *
 *            La recherche s'effectue soit a partir de l'index du       *
 *            fichier dans la table, soit a partir du nom du fichier    *
 *                                                                      *
 * Argument : 1. Index du fichier a supprimer de la table               *
 *                                                                      *
 *            2. Nom du fichier a supprimer de la table                 *
 *                                                                      *
 *                                                                      *
 ************************************************************************/

int FermerF(int numf,char *nomf);

/************************************************************************
 *                                                                      *
 * Objectif : Retourne le code correspond a un type de fichier          *
 *                                                                      *
 * Argument : Le nom du type dont on veut le code                       *
 *                                                                      *
 * Retour : Code correspond au type de fichier                          *
 *                                                                      *
 ************************************************************************/

int getCodeFichier(char *str);

/************************************************************************
 *                                                                      *
 * Objectif : Retourne le type du fichier correspondant au code         *
 *                                                                      *
 * Argument : 1. Code dont on veut le type                              *
 *                                                                      *
 *            2. Type du fichier en retour                              *
 *                                                                      *
 * Retour : Type du fichier correspondant au code                       *
 *                                                                      *
 ************************************************************************/

int getTypeFichier(int code, char *str);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'une valeur a un champ dans le fichier d'entete    *
 *            d'un fichier                                              *
 *                                                                      *
 * Argument : 1. Flux sur le fichier d'entete                           *
 *                                                                      *
 *            2. Nom du champs auquel on veut ajouter une valeur        *
 *                                                                      *
 *            3. Valeur a ajouter                                       *
 *                                                                      *
 ************************************************************************/

int AjouterChampsDsEntete(FILE *FichierEnt, char *champs, char *val);

/************************************************************************
 *                                                                      *
 * Objectif : Lecture du fichier pour recuperer :                       *
 *                    1. Le nom du repertoire (dirname)                 *
 *                    2. Le nom du fichier (basename)                   *
 *                    3. Le nom du fichier sans l'extension             *
 *                    4. L'extension du fichier                         * 
 *                                                                      *
 * Argument : 1. Le fichier a analyser                                  *
 *            2. Une structure contenant les informations sur le        *
 *               fichier                                                *
 *                                                                      *
 * Retour :  Etat de l'execution de la fonction                         *
 *                                                                      *
 ************************************************************************/


int Analyse_Nom_Fichier(char *nom_fichier, struct Donnees_Nom_Fichier *donnees_nom_fichier);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche si un fichier existe précédemment dans          *
 *            un intervalle de l'historique                             *
 *                                                                      *
 * Argument : 1. numero du fichier a rechercher                         *
 *                                                                      *
 *            2. Borne supérieure de l'intervalle de recherche          *
 *                                                                      *
 * Retour :  indication si le fichier existe ou non                     *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

int existe_prec_Historique(int numfichier, int bornesup);


/************************************************************************
 *                                                                      *
 * Objectif : Indique si le fichier passé en argument est du type       *
 *            attendu                                                   *
 *                                                                      *
 * Argument : 1. Numero du fichier                                      *
 *                                                                      *
 *            2. Type du fichier                                        *
 *                                                                      *
 * Retour :  indication si le fichier est au bon type                   *
 *                                                                      *
 * Date de derniere modification : 24/07/2006                           *
 *                                                                      *
 ************************************************************************/

int is_type(int ,int );

