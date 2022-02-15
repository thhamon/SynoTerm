#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/* #include "Synonymie.h" */

#if !defined(SYNOTERM_GEN_H)
#define SYNOTERM_GEN_H

/*************************************************************************/

#define TRUE 1
#define FALSE 0

/* Definitions pour la gestion des erreurs */

#define ERR_INIT -1
#define ERR_LECTURE -2
#define ERR_ALLOCATION -3
#define ERR_COMMANDE -4
#define ERR_FICHIER_NOEXIST -5
#define ERR_RENVOI_STD -1
#define ERR_TRAITMT -6
#define ERR_TIME -7
#define ERR_ECRITURE -8
#define ERR_OUVERTURE -9
#define ERR_TABLE -10
#define ERR_FORK -11
#define ERR_FICHIER -12
#define ERR_CHARGDB -1
#define ERR_COPIE -13
#define ERR_PAS_TROUVE -1
#define ERR_GET_ID -14
#define ERR_RECHERCHE -15
#define ERR_GET_LEM_CT -16
#define ERR_ID_VAL_INCONNU -17
#define ERR_CHG_VAL -18


/* Lorsque tout s'est bien passe mais pas de modification */
#define EXEC_NO 1

/* Lorsque tout s'est bien passe */
#define EXEC_OK 0

/**********************************************************************/

/************************************************************************/
/* Fichier temporaire contenant les validations courantes */

#define FICHIERVALIDATION "Validation-sauve.log"

/**********************************************************************/

/* Existence ou non d'un information */

#define EXISTE          1
#define EXISTE_PAS      0

/**********************************************************************/

#define BUF_SIZE 10000
#define BUF_MOT_SZ      1024
#define BUF_SIZE_COM      1024*1024
#define CAT_SIZE 4

#endif

extern int CG; /* Indique si le graphe des candidats termes est construit ou non */

extern struct DonneesSyntxq Lst_Termes; /* Graphe (Liste des termes) */
extern int typoC;                       /* Indication de la typagraphie du corpus (pauvre ou non */
extern int typoD;                       /* Indication de la typagraphie du dictionnaire (pauvre ou non */

