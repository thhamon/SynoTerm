/* 	$Log: VarSem.h,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.6  2001/08/05 08:04:13  thierry
 * 	Entetes des fonctions definies dans SynoTermM.c
 * 	Commentaire de la macro CONSTRUIT
 *
 * 	Revision 1.5  2001/07/18 21:41:48  thierry
 * 	Prise en compte de difference entre les systemes SunOS et Linux
 * 	au niveau des inclusions de bibliotheques sys/ddi.h et sys/filio.h
 *
 * 	Revision 1.4  2001/07/16 23:23:17  thierry
 * 	*** empty log message ***
 *
 * 	Revision 1.3  2001/05/20 16:57:37  thierry
 * 	.
 *
 * 	Revision 1.2  2001/05/01 17:27:17  thierry
 * 	*** empty log message ***
 *	 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifdef SunOS
#include <sys/ddi.h>
#include <sys/filio.h>
#endif

#include <stdarg.h>

#include "SynoTerm_Gen.h"

#if !defined(VARSEM_H)
#define VARSEM_H



/**********************************************************************/

#define LIMIT_SIZE 1000
#define PORT 7777

#define CONSTRUIT	1         /* Indicarion de la construction du
				     graphe des candidats termes
				   */

#define TAILLE_FICHIER_LOG 32

/************************************************************************/

#define OPTION_SORTIE 's'
#define OPTION_SORTIE_CLASSE "C "
#define OPTION_SORTIE_FAMILLE "F "
#define OPTION_SORTIE_LEMME "L "
#define OPTION_SORTIE_XML "X "
#define OPTION_SORTIE_FORME_FLECHIE "f "
#define OPTION_VALIDATION_FICHIER 'V'

/************************************************************************/


/************************************************************************/
/* Fichier temporaire contenant les validations courantes */

#define FICHIERVALIDATION "Validation-sauve.log"

/*************************************************************************/

/* Communication entre le moteur C et l'interface Perl */


/*************************************************************************/

struct LstMachines {
     int rep;
     char *IDMachine;
};

/************************************************************************/


#endif

extern char nomlog[TAILLE_FICHIER_LOG]; /* Nom du fichier du journal de bord */
extern type_devel; /* indique le type de developpement -- a utiliser uniquement avec l'option DEVEL */

int writelogS(char *strmessage);
int writelogI(int entmessage);
int writelogD(double entmessage);

int recup_nom_ext(char *nom_ext, char **nom, char **ext);

int ecrit_sock(int sock, char *msg);
char *lit_sock(int sock);

int lit_champ(FILE *FluxF, char *str, char sep);

int EnvoiOrdre(int nbsock, int *lst_sock, char *ordre);

/************************************************************************
 *                                                                      *
 * Objectif : Lancement du script Perl de gestion de l'interface        *
 *                                                                      *
 * Argument : 1. Argument du script Perl                                *
 *                                                                      *
 *            2. Identifiant du tube vers le processus fils             *
 *                                                                      *
 *            3. Identifiant du tube vers le processus pere             *
 *                                                                      *
 ************************************************************************/

int lancementInterface(char *arg, int *vers_fils, int *vers_pere);

/************************************************************************
 *                                                                      *
 * Objectif : Fonction d'initialisation de la connexion en les          *
 *            processus pere et fils                                    *
 *                                                                      *
 * Arguments : 1. Identifiant du tube vers le processus fils            *
 *                                                                      *
 *             2. Identifiant du tube vers le processus pere            *
 *                                                                      *
 ************************************************************************/

int InitialisationConnexion(int *vers_fils, int *vers_pere);

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de modification de la validation des relations   *
 *            inferees                                                  *
 *                                                                      *
 * Argument : Aucun                                                     *
 *                                                                      *
 ************************************************************************/

int ModifValidation();




/************************************************************************
 *                                                                      *
 * Objectif : Ecriture d'un message dans un fichier de log              *
 *                                                                      *
 * Argument : 1. le formatage (type printf)                             *
 *            2, ... les arguments                                      *
 *                                                                      *
 * Retour : l'etat d'execution de la fonction                           *
 *                                                                      *
 ************************************************************************/

int printlog(char *strformat, ...);


/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la longueur maximale de deux chaînes de         *
 *            caractères                                                *
 *                                                                      *
 * Argument : 1. La première chaine de caractères                       *
 *            2. La deuxieme chaine de caractères                       *
 *                                                                      *
 * Retour :  La longueur maximale                                       *
 *                                                                      *
 * Date de derniere modification : 15/12/2003                           *
 *                                                                      *
 ************************************************************************/

int max_len(char *str1, char *str2);

/************************************************************************
 *                                                                      *
 * Objectif :  Determination du nom du fichier de journal de bord       *
 *                                                                      *
 * Argument : Rien                                                      *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Generation_fichier_log(void);

/************************************************************************
 *                                                                      *
 * Objectif : Fonction d'initialisation de SynoTerm                     *
 *                                                                      *
 * Argument : Rien                                                      *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification :  26/12/2003                          *
 *                                                                      *
 ************************************************************************/

int init_SynoTerm(void);

/************************************************************************
 *                                                                      *
 * Objectif :  Afficher l'aide de SynoTerm                              *
 *                                                                      *
 * Argument : Rien                                                      *
 *                                                                      *
 * Retour : Rien. Mais termine le programme avec la valeur -1           *
 *                                                                      *
 * Date de derniere modification : 26/12/2003                           *
 *                                                                      *
 ************************************************************************/


int help_SynoTerm(int, char **);

