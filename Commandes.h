#include <stdarg.h>

#include "SynoTerm_Gen.h"
#include "Fichier.h"
#include "Synonymie.h"

#if !defined(COMMANDES_H)
#define COMMANDES_H

/*************************************************************************/

/* Liste des macros correspondant aux commandes */

#define L0AD_DB 1
#define LOAD_DESAMB 2
#define LOAD_CLS2CORPUS 3
#define LOAD_SYN 4
#define LOAD_DIC 5
#define INFO_ALL 6
#define INFO_SYN 7
#define INFO_DESAMB 8
#define INFO_NEW 9
#define SYN_ELIMDOUBL 10
#define SYN_ELIMDOUBLCR 11
#define SYN_FILTRAGE 12
#define SYN_INFERENCE 13
#define QUIT 14
#define QUIT_NOENR 15
#define SAVE_MAT 16
#define LOAD_MAT 17
#define VALIDATION 18
#define SYN_EXTRAC_RELINIT 19
#define SORTIE 20
#define VALIDATION_FICHIER 21
#define SYN_INFERENCE_SUPPL 22
#define LOAD_TERM_LIST 23
#define INFO_TERMLIST 24

/**********************************************************************/

#define SZ_TYPO 2
#define SZ_CODAGE 2
#define SZ_LIGNE_CMD 25

/**********************************************************************/

#define INDEX_TYPO 2
#define INDEX_CODAGE 3
#define INDEX_OFILE_INFERENCE 3
#define INDEX_OFILE_SORTIE 1

/**********************************************************************/

#define NB_ELT_LOAD_DB 2
#define NB_ELT_LOAD_CORPUS 4
#define NB_ELT_LOAD_DICTIONNAIRE 4
#define NB_ELT_ELIM_DOUBLONS_CR 2
#define NB_ELT_ELIM_DOUBLONS 2
#define NB_ELT_FILTRAGE 3
#define NB_ELT_INFERENCE 4
#define NB_ELT_SORTIE 4
#define NB_ELT_VALIDATION_FICHIER 4
#define NB_ELT_INFERENCE_SUPPL 4


/**********************************************************************/

#define NB_DIGIT_INT_COM 10

/**********************************************************************/

typedef char * Elt_commande; /* Type de l'element constituant une commande */

/* Structure definissant une commande */
struct Commande {
  int Nb_elt_commande;
  Elt_commande *elt_commande;
};

#endif

/************************************************************************
 *                                                                      *
 * Objectif : Inialisation des parametres de configuration du programme *
 *                                                                      *
 * Arguments : 1. flux de lecture                                       *
 *                                                                      *
 *             2. flux d'ecriture                                       *
 *                                                                      *
 *             3. la structure qui contiendra les commandes             *
 *                                                                      *
 * Retour : Entier indiquant le resultat de l'appel                     *
 *                                                                      *
 * Date de derniere modification : 31/07/2003                           *
 *                                                                      *
 ************************************************************************/

int Initialisation_Commandes(int flux_lect, int flux_ecrit, struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Lecture d'un element d'une commande                       *
 *            (Protocole de communication)                              *
 *                                                                      *
 * Argument : 1. flux de lecture                                        *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Lecture_Elt_Commande(int flux_lect, Elt_commande *elt_comm);

/************************************************************************
 *                                                                      *
 * Objectif : Lecture complete d'une commade                            *
 *            (Protocole de communication)                              *
 *                                                                      *
 * Argument : 1. flux de lecture                                        *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Lecture_Commande(int flux_lect, struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de conversion d'une commande (chaine de caracte- *
 *            -re) en un entier (pour la suite du traitement)           *
 *                                                                      *
 * Argument : 1. element de la commande a convertir                     *
 *                                                                      *
 * Retour : L'entier correspondant a l'element de la commande           *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Convert_Commande(Elt_commande commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier au format db (format SynoTerm)    *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_db(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier au format desamb (sorte LEXTER)   *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 01/10/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_desamb(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier de classe de termes apres         *
 *            convertion au format db (SynoTerm)                        *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_cls2corpus(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier contenant un dictionnaire de      *
 *            synonymes au format INaLF                                 *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_syn(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier contenant un dictionnaire         *
 *            (fonction non implementée)                                *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_dic(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Appel de la fonction d'elimination des doublons           *
 *                                                                      *
 * Argument :  1. la commande                                           *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 09/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ElimDoubl(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Appel de la fonction d'elimination des doublons croises   *
 *                                                                      *
 * Argument :  1. la commande                                           *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 09/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ElimDoubl_Cr(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Filtrage de la liste de synonymes au regard de la liste   *
 *            de candidats termes                                       *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 15/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_Filtrage(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Inference des relations de synonymes                      *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 31/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_Inference(struct Commande *commande, int flux_ecriture);

/************************************************************************
 *                                                                      *
 * Objectif : Terminaison du programme avec enregistrement des          *
 *            resultats                                                 *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 19/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Quit(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Terminaison du programme sans enregistrement des          *
 *            resultats                                                 *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 15/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Quit_NoEnr(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement de la matrice. Sauvegarde des liens        *
 *            inferes                                                   *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 22/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Save_Mat(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement de la matrice. Sauvegarde des liens inferes    *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 02/01/2004                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_Mat(struct Commande *commande, int flux_ecriture);

/************************************************************************
 *                                                                      *
 * Objectif : Validation d'un lien                                      *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Validation(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Extraction des liens initiaux a partir d'une ressource    *
 *            existante
 *                                                                      *
 * Argument : 1. Le flux de lecture                                     *
 *            2. Le flux d'ecriture                                     *
 *            3. La commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ExtractLiensInitiaux(int flux_lect, int flux_ecrit,struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Impression des resultats dans differents formats          *
 *                                                                      *
 * Argument : 1. Le flux de lecture                                     *
 *            2. Le flux d'ecriture                                     *
 *            3. La commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Sortie(int flux_lect, int flux_ecrit,struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Validation des resultats à partir d'un fichier            *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Validation_Fichier(struct Commande *commande, int flux_ecrit);

/************************************************************************
 *                                                                      *
 * Objectif : Inference a partir d'une ressource supplementaire         *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_InferenceCTSyn_Suppl(struct Commande *commande, int flux_ecrit);

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Classes2Corpus();

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Extration_Liens_initiaux();

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Diversite_Shannon();

/************************************************************************
 *                                                                      *
 * Objectif :  Ajout d'un element a une commande a un indice            *
 *             Permet aussi des modifications de valeurs                *
 *                                                                      *
 * Argument :  1. La commande                                           *
 *                                                                      *
 *             2. L'indice de l'element                                 *
 *                                                                      *
 *             3. La valeur de l'element                                *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 27/12/2003                           *
 *                                                                      *
 ************************************************************************/


int Ajout_Elt_Commande(struct Commande *commande, int ind, char *val_elt);

/************************************************************************
 *                                                                      *
 * Objectif : Libèration de la mémoire allouée à une commande           *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 31/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Free_commande(struct Commande *commande);

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier avec modification du nom pour la  *
 *            normalisation                                             *
 *                                                                      *
 * Argument : 1. Nom du fichier a charger                               *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Charge_Fichier_option(char *fichier);

/************************************************************************
 *                                                                      *
 * Objectif :  Creation d'une commande a partir des arguments           *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le nombre d'elements theorique de la commande          *
 *                                                                      *
 *            3. Le nombre d'elements effectivement placés              *
 *                                                                      *
 *            4. La liste des elements a placer (arguments variables)   *
 *                                                                      *
 * Retour :  L'etat de l'execution de la fonction                       *
 *                                                                      *
 * Date de derniere modification : 27/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Cree_commande(struct Commande *commande, int nb_elt, int nb_val, ...);

/************************************************************************
 *                                                                      *
 * Objectif : Traitement des commandes en mode interface                *
 *            **** Nouvelle version, en cours de developpement ****     *
 *                                                                      *
 * Arguments : 1. Flux de lecture                                       *
 *                                                                      *
 *             2. Flux d'ecriture                                       *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 29/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commandes(int flux_lect, int flux_ecrit  );

/************************************************************************
 *                                                                      *
 * Objectif : Selection  des commandes                                  *
 *            **** Nouvelle version, en cours de developpement ****     *
 *                                                                      *
 * Arguments : 1. Flux de lecture                                       *
 *                                                                      *
 *             2. Flux d'ecriture                                       *
 *                                                                      *
 *             3. Commande                                              *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 29/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commandes_Select(int flux_lect, int flux_ecrit, struct Commande *commande);


/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'une liste de termes validés ou attestés      *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 19/05/2006                           *
 *                                                                      *
 ************************************************************************/


int Commande_load_term_list(struct Commande *commande, int flux_ecrit);
