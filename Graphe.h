#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "SynoTerm_Gen.h"
#include "Fichier.h"
#include "stringSGML.h"
#include "Communication.h"

#if !defined(GRAPHE_H)
#define GRAPHE_H

/**********************************************************************/

/* Definition des positions des CTs */

#define TETE		0
#define EXPANSION	1

/* Type de lien mixte entre une tête et une expansion */
#define TE		0
#define ET		1


/**********************************************************************/

/* Definition des differents champs contenant les informations liees 
   au CT */

#define CATEG		0
#define LEMME		1
#define IDGNM		2
#define IDPHRASE	3
#define COORD           4

/**********************************************************************/

/* Informations sur la decomposition et les dépendances entre CT */

#define COMP		0
#define DEP		1

/**********************************************************************/

/* Macro pour le codage et le decodage des indices des tetes et expansions */

#define CodeIndExp(x)      ((x + 1) * -1)
#define CodeIndTete(x)        (x + 1)
#define DecodeIndExp(x)    ((x * -1) -1)
#define DecodeIndTete(x)   (x - 1)

/**********************************************************************/

/* Pour la séelection d'intervalle */

#define PARTIE_SUP      0
#define PARTIE_INF      1

#define TAILLE_CATEGORIE 3

/**********************************************************************/

#define NONINF          0x0000  /* lien fourni par une ressource */

#define INFERENCE       0x000F 
#define ORIGINE         0x00F0 /* Origine du lien infere */
#define TYPELIEN        0x0F00 /* Type du lien */

/* A SUPPRIMER (doit être défini uniquement dans Synonymie.h */
#define DICOSYN         0x0010 /* Dictionnaire de synonymes */
#define LIEN_EXTRAIT    0x0040 /* Lien extrait a l'aide des regles inverses */


/**********************************************************************/

/* Taille maximum de l'identifiant de validatité d'une relation */

#define TAILLE_MAX_VAL 6

/**********************************************************************/

#define INITIALISATION -1

/**********************************************************************/

#define NON_VAL         0x0000 /* Lien non valid\'e */
#define UNVAL           0x0F00 /* Lien invalide */

/* Identifiant sous forme d'une chaine de caractère */
#define NON_VAL_CHAR    "NV" /* Lien non valid\'e */
#define UNVAL_CHAR      "UNVAL" /* Lien invalide */



/* Code à supprimer */
#define VAL_SYN         0x0100 /* Lien de synonymie */
#define VAL_HYPO        0x0200 /* Lien d'hyponymie */
#define VAL_MERO        0x0300 /* Lien de meronymie */
#define VAL_HYPER       0x0400 /* Lien d'hyperonymie */
#define VAL_HOLO        0x0500 /* Lien d'holonymie */
#define VAL_ANTO        0x0600 /* Lien d'holonymie */
#define VAL_VA          0x0700 /* Lien d'holonymie */

#define NB_TYPE_LIEN_INIT 7

/* Identifiant sous forme d'une chaine de caractère */
#define VAL_SYN_CHAR         "SYN" /* Lien de synonymie */
#define VAL_HYPO_CHAR        "HYPO" /* Lien d'hyponymie */
#define VAL_MERO_CHAR        "MERO" /* Lien de meronymie */
#define VAL_HYPER_CHAR       "HYPER" /* Lien d'hyperonymie */
#define VAL_HOLO_CHAR        "HOLO" /* Lien d'holonymie */
#define VAL_ANTO_CHAR        "ANTO" /* Lien d'holonymie */
#define VAL_VA_CHAR          "VA" /* Lien d'holonymie */


/**********************************************************************/

/* Reference vers l'autre CT synonyme */
#define REF		0

/* Type de relation (Inference ou origine) */
#define VAL		1

/* information about the production of the relation */

#define PRODUCTIVITE    2

#define NO_INCRPROD     0
#define INCRPROD_ONESTEP 1

/**********************************************************************/

#define SEP_INFOLEXTER "$"

/**********************************************************************/

/* A laisser ? */
#define NBMESPERT               2  /* Nombre de mesures de pertinence */

/**********************************************************************/

struct noeud {	/* Structure pour un noeud */
	char *Trait[5]; /* Liste des traits attaches au noeud : 
			      0 : Categorie
			      1 : Lemme
			      2 : Identifiant du groupe nominal maximal
			      3 : Identifiant de la phrase
			      4 : Coordination entre la tete et l'expansion
			 */
	int NbTraits; /* Nombre ds traits dans 
			 Liste des traits attache au noeud */
	int nbfils; /* nombre de fils (normalement 0 ou 2) */
	struct noeud *fils[2]; /* liste des fils 
			 Candidats Termes Lexter :
			 	0 = Tete
				1 = Expansion
		       */
	struct noeud *Pere;
	};

typedef struct noeud Noeud;

/**********************************************************************/

		/* Matrice */

struct GrphTerms {
  char *lemme;
  int *decomp[2]; 	/*
			 * decomp[0][i] : Tete
			 * decomp[1][i] : Exp
			 */
  int nbdecomp;
  int *compdep[2]; 		/*
				 * comp[0][i] : CT
				 * comp[1][i] : 
				 * 	exp(si lst_tetes)/tete(si lst_exps)
				 */
  int nbcompdep;
  int *syn[3];
  int nbsyn;
  char *donneesLexter; 	/* A décomposer en deux ? IDgnm / idPhr ? */
  int Prod;		/* A Laisser ? oui ! oui ! (20/10/1999)*/
  double Mesures_Pertinence[NBMESPERT]; /* A transformer sous forme de poids ??? */
};

/**********************************************************************/

struct DonneesSyntxq {
  struct GrphTerms *lst_tetes;
  struct GrphTerms *lst_exps;
  int nb_exps;
  int nb_tetes;
};

struct TableauTypeLien {
  char **TypesLien;
  int size;
};

#endif

/************************************************************************
 *  Déclaration des variables                                           *
 ************************************************************************/

extern FILE *ConstrEntetein;            /* Flux de lecture de l'entête */
extern int typeEntete;                  /* Définition du type d'entête à lire */
extern int Compteur;                    /* Nombre d'élément lus -- définition globale 
                                           nécessaire pour la lecture avec Flex 
                                         */
extern FILE *ConstrMatricein;           /* Flux de lecture du fichier des groupes 
                                           nominaux maximaux 
                                         */
 
extern struct TableauTypeLien tab_typeLien; /* Tableau des types des liens */

/**********************************************************************/

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un nouveau noeud (en feuille au candidat terme    *
 *            courant                                                   *
 *                                                                      *
 * Argument : Le pere du noeud cree                                     *
 *                                                                      *
 * Retour : Le pointeur sur le nouveau noeud                            *
 *          NULL si problème                                            *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

Noeud *ConstrCT(Noeud *Pere);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un trait au noeud courant                         *
 *                                                                      *
 * Arguments : 1. Le noeud auquel ajoute le trait                       *
 *                                                                      *
 *             2. Le trait a ajouter                                    *
 *                                                                      *
 *             3. Le type du trait a ajouter                            *
 *                                                                      *
 * Retour : - EXEC_OK si tout s'est bien passé                          *
 *                                                                      *
 *          - ERR_ALLOCATION en  cas de problème d'allocation           *
 *                                                                      *
 *          - ERR__COPIE en cas de problème de copie                    *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Ajout_Trait_CT(Noeud *N,char *trait, int type);

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de l'identifiant des groupes nominaux        *
 *            maximaux et des phrases dans lequel se trouve le candidat * 
 *            terme                                                     *
 *                                                                      *
 * Argument : le noeud correspondant au candidat terme                  *
 *                                                                      *
 * Retour : les identifiants des groupes nominaux maximaux et des       * 
 *          phrases dans un chaine de caracteres                        *
 *                                                                      *
 *          NULL en cas d'erreur                                        *
 *                                                                      *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

char *get_ID(Noeud *CT);

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de l'ensemble des informations (lemme, categorie,  *
 *            identifiants, decomposition) sur un candidat terme        *
 *            Balisage des informations dans le style SGML              *
 *                                                                      *
 * Argument : Le pointeur sur le noeud dont on veut recuperer les       *
 *            informations                                              *
 *                                                                      *
 * Retour : Les informations a recuperer (dans une chaine de carateres) *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/


char *get_str_CT(Noeud *CT);

/************************************************************************
 *                                                                      *
 * Objectif : Calcul du lemme du candidat terme                         *
 *                                                                      *
 * Argument : Le noeud courant                                          *
 *                                                                      *
 * Retour : Le lemme à recuperer                                        *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

char *get_lem_CT(Noeud *CT);

/************************************************************************
 *                                                                      *
 * Objectif : Construction du graphe des candidats termes               *
 *                                                                      *
 * Argument : 1. Identifiant dans la table des fichier ouverts dans     *
 *               SynoTerm du fichier contenant les candidats termes     *
 *                                                                      *
 *            2. Nombre de groupes nominaux maximaux                    *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int ConstrMatrice(int Fichct, int nbgnmax);

/************************************************************************
 *                                                                      *
 * Objectif : Liberation de la memoire reservee pour un candidat terme  *
 *            represente dans un arbre (preliminaire a la construction  *
 *            d'un graphe)                                              *
 *                                                                      *
 * Argument : Pointeur sur le noeud correspondant au candidat terme     *
 *            dont on veut liberer la memoire                           *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Free_CT(Noeud *CT);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche du lemme dans la liste des candidats termes     *
 *            en position tete                                          *
 *                                                                      *
 * Argument : Lemme recherche                                           *
 *                                                                      *
 * Retour : indice du lemme recherche (>= 0)                            *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_lemmeT(char *lemme);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche du lemme dans la liste des candidats termes     *
 *            en position expansion                                     *
 *                                                                      *
 * Argument : lemme recherche                                           *
 *                                                                      *
 * Retour : indice du lemme recherche ( >= 0 )                          *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_lemmeE(char *lemme);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche de deux indices dans les tableaux contenant les *
 *            les tetes et les expansions (indique si une relation de   *
 *            decomposition existe deja)                                *
 *                                                                      *
 * Arguments : 1. Le candidat terme (dans le graphe) ou la recherche    *
 *               sera effectuee                                         *
 *                                                                      *
 *             2. L'indice de la tete a rechercher ( >= 0 )             *
 *                                                                      *
 *             3. L'indice de l'expansion a rechercher ( >= 0 )         *
 *                                                                      *
 * Retour : indice ( >= 0 ) de la relation de decomposition si elle     *
 *          existe, -1 sinon                                            *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_ind_decomp(struct GrphTerms *lst_comp,int ind0,int ind1);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche de deux indices dans les tableaux contenant les *
 *            les relations de composition et de dependance (indique si * 
 *            une relation de composition et de dependance existe deja) *
 *                                                                      *
 * Arguments : 1. Le candidat terme (dans le graphe) ou la recherche    *
 *               sera effectuee                                         *
 *                                                                      *
 *             2. L'indice de la tete a rechercher ( >= 0 )             *
 *                                                                      *
 *             3. L'indice de l'expansion a rechercher ( >= 0 )         *
 *                                                                      *
 * Retour : indice ( >= 0 ) de la relation de composition et de         *
 *           dependance si elle existe, -1 sinon                                       *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_ind_compdep(struct GrphTerms *lst_decomp,int ind0,int ind1);

/************************************************************************
 *                                                                      *
 * Objectif :  Test de l'existence d'un lemme dans la matrice           *
 *                                                                      *
 * Argument : 1. lemme de la relation de synonymie                      *
 *                                                                      *
 *            2. lemme dans la matrice                                  *
 *                                                                      *
 * Retour :  VRAI si le lemme est dans la matrice                       *
 *           FALSE sinon                                                *
 *                                                                      *
 * Date de derniere modification : 12/10/2003                           *
 *                                                                      *
 ************************************************************************/

int exist_lemme_matrice(char *lemme_syn, char *lemme_mat);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un candidat terme dans le graphe                  *
 *                                                                      *
 * Arguments : 1. Candidat terme represente sous forme d'un arbre       *
 *                                                                      *
 *             2. Position (tete (0) ou expansion (1)) du candidat      *
 *                terme a ajouter                                       *
 *                                                                      *
 * Retour : l'indice du candidat terme ( >= 0 )                         *
 *          -1 en cas d'erreur                                          *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Ajout_CTMax_Matrice(Noeud *CT,int pos);

/************************************************************************
 *                                                                      *
 * Objectif :  Identification de l'intervalle où se trouve un lemme     *
 *             dans la matrice                                          *
 *                                                                      *
 * Argument : 1. lemme de la relation de synonymie                      *
 *                                                                      *
 *            2. lemme dans la matrice                                  *
 *                                                                      *
 * Retour :  PARTIE_INF si le lemme est dans la partie inférieure de    *
 *           l'intervalle                                               *
 *           PARTIE_SUP si c'est dans la partie supérieure              *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int select_intervalle_lemme_matrice(char *lemme_syn, char *lemme_mat);


/************************************************************************
 *                                                                      *
 * Objectif :  Recherche un lemme dans un tableau donne en argument     *
 *             entre deux bornes                                        *
 *                                                                      *
 * Argument : 1. Le tableau où est effectué la recherche                *
 *                                                                      *
 *            2. le lemme recherché                                     *
 *                                                                      *
 *            3. la catégorie                                           *
 *                                                                      *
 *            4. la borne inférieure                                    *
 *                                                                      *
 *            5. la borne supérieure                                    *
 *                                                                      *
 * Retour : l'indice du lemme dans le tableau, (decode) s'il existe     *
 *                                                                      *
 *          la borne supérieure + 1 s'il n'existe pas                   *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int recherche_dans_tab(struct GrphTerms *tab, char *lemme, char *categ, int inddebut, int indfin);


/************************************************************************
 *                                                                      *
 * Objectif :  Recherche un lemme dans un tableau donne en argument     *
 *             entre deux bornes                                        *
 *                                                                      *
 * Argument : 1. Le tableau où est effectué la recherche                *
 *                                                                      *
 *            2. le lemme recherché                                     *
 *                                                                      *
 *            3. la catégorie                                           *
 *                                                                      *
 *            4. la borne inférieure                                    *
 *                                                                      *
 *            5. la borne supérieure                                    *
 *                                                                      *
 * Retour : l'indice du lemme dans le tableau, (decode) s'il existe     *
 *                                                                      *
 *          la borne supérieure + 1 s'il n'existe pas                   *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int recherche_dans_tab(struct GrphTerms *tab, char *lemme, char *categ, int inddebut, int indfin);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche dicotomique d'un lemme dans un intervalle d'un  *
 *            des deux tableaux des candidats termes. On prend en       *
 *            compte egalement de la categorie (si indiquee)            *
 *                                                                      *
 * Arguments : 1. Lemme a rechercher                                    *
 *                                                                      *
 *             2. Categorie du lemme                                    *
 *                                                                      *
 *             3. Tableau ou sera effectuee la recherche (TETE ou       *
 *                EXPANSION)                                            *
 *                                                                      *
 *             4. Borne inferieure de l'intervalle                      *
 *                                                                      *
 *             5. Borne superieure de l'intervalle                      *
 *                                                                      *
 * Retour : l'indice du lemme (decode)                                  *
 *                                                                      *
 * Date de derniere modification : 01/12/2003                           *
 *                                                                      *
 ************************************************************************/

int recherche_entete_LemS(char *lemme_orig, char *categ, int type,int inddebut,int indfin);


/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation associe a un candidat terme          *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             2. Indice du terme 2 (code)                              *
 *                                                                      *
 *             3. Tableau des termes                                    *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 *             5. Increment step for the production (0 or 1)            *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteLien(int Terme1, int Terme2, struct GrphTerms *Tab, int info, int incrProd, int flux_ecriture);


/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Arguments : 1. Tableau des termes                                    *
 *                                                                      *
 *             2. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             3. Indice du terme 2 (code)                              *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 ************************************************************************/

int AjoutRef(struct GrphTerms * Tab, int Terme1, int Terme2, int info);


/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Arguments : 1. Tableau des termes 1                                  *
 *                                                                      *
 *             2. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             3. Tableau des termes 2                                  *
 *                                                                      *
 *             4. Indice du terme 2 (code)                              *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Date de derniere modification : 06/10/2003                           *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 ************************************************************************/

int AjoutLien(struct GrphTerms * Tab1, int Terme1, int CodeIndTerme1, struct GrphTerms * Tab2, int Terme2, int CodeIndTerme2, int info);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre un candidat terme en position tete  *
 *            et un candidat terme en position expansion                *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Indication si le premier candidat terme est en        *
 *                position expansion et le second en position tete      *
 *                ou si le premier candidat terme est en tete et le     *
 *                second est en position expansion                      *
 *                                                                      *
 *             4. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute (1) ou non (0)           *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienTE(int Terme1, int Terme2, int type,int info, int, int);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre deux candidats termes en position   *
 *            expansion                                                 *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute (1) ou non (0)           *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienE(int E1, int E2, int info, int, int);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre deux candidats termes en position   *
 *            tete                                                      *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute ou non                   *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienT(int T1, int T2, int info, int, int);


/************************************************************************
 *                                                                      *
 * Objectif : Construction du graphe des candidats termes               *
 *                                                                      *
 * Arguments : 1. Numero du fichier du corpus dans la table des         *
 *                fichiers                                              *
 *                                                                      *
 *             2. Nombre de groupes nominaux maximaux                   *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int ConstructGraphe(int Fichct,int nbgnmax);


/************************************************************************
 *                                                                      *
 * Objectif : Destruction de la matrice (graphe des candidats termes)   *
 *                                                                      *
 * Argument : Aucun                                                     *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int DestructionMat();

/************************************************************************
 *                                                                      *
 * Objectif : l'identifiant de la relation donné sous forme d'un entier *
 * est transformé en une chaine de caractères                           *
 *                                                                      *
 * Argument : le code de l'identifiant de la relation sous forme d'une  *
 *            chaine de caractères                                      *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *          Code de l'identification de la relation passé par adresse   *
 *                                                                      *
 * Note : Fonction à réécrire pour la généraliser à n'importe quel type *
 *        de la relation (à l'aide d'un tableau)                        *
 *                                                                      *
 * Date de derniere modification : 20/07/2004                           *
 *                                                                      *
 ************************************************************************/

int valint2valch(int val, char *chval);

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument : 1. Flux d'enregistrement                                  *
 *                                                                      *
 *            2. Premier noeud du graphe                                *
 *                                                                      *
 *            3. Identifiant correspondant au noeud passé en second     *
 *               argument                                               *
 *                                                                      *
 *            4. Deuxième noeud du graphe                               *
 *                                                                      *
 *            5. Identifiant correspondant au noeud passé en quatrième  *
 *               argument                                               *
 *                                                                      *
 *            6. Validité du lien                                       *
 *                                                                      *
 * Retour : Etat de l'exécution de la fonction                          *
 *                                                                      *
 * Note : Revoir l'allocation de ch (et l'affichage)                    *
 *                                                                      *
 * Date de derniere modification : 20/07/2004                           *
 *                                                                      *
 ************************************************************************/

int EnregistrementLien(FILE *FluxEnr, struct GrphTerms * Elmt1, int id1, struct GrphTerms * Elmt2, int id2, int val);

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement des resultats de l'inference               *
 *                                                                      *
 * Argument : 1. Nom du fihier contenant les resultats                  *
 *                                                                      *
 * Retour : Etat de l'exécution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

int EnregLiensInf(char *Result);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement des liens inféré                               *
 *                                                                      *
 * Argument : 1. Informations utiles sur le nom du fichier              *
 *                                                                      *
 * Retour : L'etat de l'execution de la function                        *
 *                                                                      *
 * Note : Revoir la fonction pour la généraliser, ne doit pas regarder  *
 * l'origine des données avec des identifiants explicites mais prendre  *
 * ce qu'il y a dans le fichier en entrée                               *
 *                                                                      *
 * Date de derniere modification : 02/01/2004                           *
 *                                                                      *
 ************************************************************************/

int ChargLiensInf(int flux_ecriture);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien dans le graphe à partir des lemmes des    *
 *            deux termes (entree et syn)                               *
 *                                                                      *
 * Argument : 1. lemme du premier terme ou mot                          *
 *                                                                      *
 *            2. lemme du deuxième terme ou mot                         *
 *                                                                      *
 *            3. Information sur le lien ajouté                         *
 *                                                                      *
 * Retour :  Etat de l'exécution de la fonction                         *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int AjoutLien_ch(char *entree, char *syn, int info);

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien dans le graphe à partir des indices des   *
 *            lemmes des deux termes (entree et syn)                    *
 *                                                                      *
 * Argument : 1. indice du lemme du premier terme ou mot (code)         *
 *                                                                      *
 *            2. indice du lemme du deuxième terme ou mot               *
 *                                                                      *
 *            3. Information sur le lien ajouté                         *
 *                                                                      *
 * Retour :  Etat de l'exécution de la fonction                         *
 *                                                                      *
 * Date de modification : 18/11/2003                                    *
 *                                                                      *
 ************************************************************************/

int AjoutLien_int(int indentree, int indsyn, int info);

/************************************************************************
 *                                                                      *
 * Objectif : Recherche de l'identifiant de l'autre terme dans la liste *
 *            des relations du terme courant                            *
 *                                                                      *
 * Argument : 1. Tableau des références correspondant aux relations     *
 *                                                                      *
 *            2. Taille du tableau                                      *
 *                                                                      *
 *            3. Référence recherchée                                   *
 *                                                                      *
 *            4. Tableau de validité des relations                      *
 *                                                                      *
 *            5. information sur la relation                            *
 *                                                                      *
 * Retour : l'indice du terme en relation si trouvé                     *
 *          ERR_PAS_TROUVE sinon                                        *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int IndexLien(int *TabRef, int TailleTabRef, int Ref, int *TabVal, int info);

/************************************************************************
 *                                                                      *
 * Objectif : Changement de la valeur d'information associée à la       *
 *            relation                                                  *
 *                                                                      *
 * Argument : 1. Information courante associée à la relation            *
 *                                                                      *
 *            2. Type de momdification (INITIALISATION ou ajout)        *
 *                                                                      *
 *            3. Type d'information sur la validité de la relation      *
 *                                                                      *
 * Retour : Etat de l'exécution de la relation                          *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int Chg_Val(int *infolien, int val, int type);

/************************************************************************
 *                                                                      *
 * Objectif : Modification de la valeur de validité d'une relation      *
 *                                                                      *
 * Argument : 1. Nouvelle valeur                                        *
 *                                                                      *
 *            2. Type de relation                                       *
 *                                                                      *
 *            3. Identifiant du premier terme                           *
 *                                                                      *
 *            4. Identifiant du deuxième terme                          *
 *                                                                      *
 * Retour : Etat de l'exécution de la relation                          *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int Modif_Val(int nval, char *typel, int IdTI1,int IdTI2, int);

/* indique si une l'identifiant correspondant à un identifiant de termlist ou non */

int is_TermList(char *id_corpus, char *nom_LT, int *nb_id_c, int *nb_id_lt);

/*  */

int Valide_Pour_AjoutLien(char *id_corpus1, char *id_corpus2, char *nom_LT);

int AjoutTypeLien(char *typeL, int);

int initTableauTypeLien(void);


/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation EXPANSION associe a un candidat      *
 *            terme                                                     *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Tableau des Indices du terme 1                        *
 *                                                                      *
 *             2. Taille du tableau 1                                   *
 *                                                                      *
 *             3. Tableau des Indice du terme 2                         *
 *                                                                      *
 *             4. Taille du tableau 2                                   *
 *                                                                      *
 *             5. Tableau des termes                                    *
 *                                                                      *
 *             6. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteUnLienParmiLesRefE(int *RefTerm1, int taille1, int *RefTerm2, int taille2, struct GrphTerms *Tab, int info);


/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation TETE associe a un candidat terme     *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Tableau des Indices du terme 1                        *
 *                                                                      *
 *             2. Taille du tableau 1                                   *
 *                                                                      *
 *             3. Tableau des Indice du terme 2                         *
 *                                                                      *
 *             4. Taille du tableau 2                                   *
 *                                                                      *
 *             5. Tableau des termes                                    *
 *                                                                      *
 *             6. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteUnLienParmiLesRefT(int *RefTerm1, int taille1, int *RefTerm2, int taille2, struct GrphTerms *Tab, int info);

