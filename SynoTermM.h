#include <getopt.h>
#include "Commandes.h"
#include "Synonymie.h"
#include "VarSem.h"
#include "Fichier.h"
#include "ChargDonnees.h"
#include "config.h"
#include "MesuresPert.h"


#if !defined(SYNOTERMM_H)
#define SYNOTERMM_H

/**********************************************************************/
/* Macro contenant le nom du fichier de la version courante de SynoTerm */

#define FICHIER_VERSION "SynoTerm.version"
#define NOM_SYNOTERM_EXEC "SynoTerm"
#define NOM_SYNOTERM_PERL "SynoTerm.pl"

#define LONG_VERSION 20

#endif

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de chargement d'un fichier au format desamd      *
 *                                                                      *
 * Arguments : 1. Nom du fichier a charger                              *
 *                                                                      *
 *             2. Indicateur de la typographie du corpus                *
 *                                                                      *
 *             3. Indicateur du codage des caracteres du corpus         *
 *                                                                      *
 ************************************************************************/

int Charger_desamb();

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de chargement d'un dictionnaire de synonymes     *
 *                                                                      *
 * Arguments : 1. Nom du fichier a charger                              *
 *                                                                      *
 *             2. Indicateur de la typographie du corpus                *
 *                                                                      *
 *             3. Indicateur du codage des caracteres du corpus         *
 *                                                                      *
 ************************************************************************/

int Charger_dicosyn(char *NomFich, int typo, int iso);

/************************************************************************
 *                                                                      *
 * Objectif : récupération des options en ligne de commandes            *
 *                                                                      *
 * Argument : 1. Taille du tableau des arguments (argc)                 *
 *                                                                      *
 *            2. Tableau des arguments (argv)                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

struct Commande *Gestion_options(int argc, char **argv, int *options);

/************************************************************************
 *                                                                      *
 * Objectif : Gestion de l'affichage du journal de bord                 *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/


int Affiche_Journal_De_Bord();

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
 * Objectif : Affichage des informations sur SynoTerm                   *
 *                                                                      *
 * Argument : Rien                                                      *
 *                                                                      *
 * Retour : Etat de l'exécution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 08/12/2003                           *
 *                                                                      *
 ************************************************************************/


int print_Info_SynoTerm(void);

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


int help_SynoTerm(int argc, char **argv);

