/* 	$Log: ChargDonnees.h,v $
/* 	Revision 1.2  2007/08/01 08:48:17  ht
/* 	modification of the Makefile
/* 	bug fixes
/* 	addition of the functionality allowing to load WordNet from WordNet file
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.5  2001/08/03 12:10:49  thierry
 * 	Ajout des commentaires pour la fonction Charge_desamb
 *
 * 	Revision 1.4  2001/07/28 11:42:13  thierry
 * 	Ajout d'une meta commentaire
 * 	(une macro (NBFICHIER) a supprimer)
 *
 * 	Revision 1.3  2001/07/27 12:34:58  thierry
 * 	Ajout des commentaires
 * 	Reste a faire des commentaires sur certaines variables
 * 	declarees en externe
 *
 * 	Revision 1.2  2001/05/01 17:19:36  thierry
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
#include<string.h>

#if !defined(CHARGDONNEES_H)
#define CHARGDONNEES_H

#endif

/**********************************************************************/
/*                Declaration des variables globales                  */
/**********************************************************************/


extern FILE *DicoSynFlux;

extern FILE *DicoSynin;

/************************************************************************
 *                                                                      *
 * Contenu : flux sur les fichiers ouverts dans les fonctions           *
 *                                                                      *
 * Remarque : cette variable est utilisee pour toutes sortes de fichier *
 *            Il faudrait peut-etre dupliquer ou la renommer ou l'inclu *
 *            -re dans les fonctions. Il n'est probablement pas neces-  *
 *            -saire qu'elle soit declaree globalement                  *
 *                                                                      *
 ************************************************************************/

extern FILE *DicoWDbl;

extern FILE *dbSynin;

/**********************************************************************/
/*                      Declaration des fonctions                     */
/**********************************************************************/

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de chargement d'un fichier au format desamd      *
 *            (Declaree dans ChargDesamb.fl)                            *
 *                                                                      *
 * Arguments : 1. Nom du fichier a charger (au format desamb)           *
 *                                                                      *
 *             2. Nom du fichier resultat (au format de SynoTerm)       *
 *                                                                      *
 *             3. Indicateur de la typographie du corpus                *
 *                                                                      *
 *             4. Indicateur du codage des caracteres du corpus         *
 *                                                                      *
 ************************************************************************/

int Charge_desamb(char *nom_desamb, char *nom_db, int typo, int iso);

/************************************************************************
 *                                                                      *
 * Objectif : Suppression des doublons dans le fichier des relations    *
 *                                                                      *
 * Argument : Identifiant du fichier a traiter dans la table des fi-    *
 *            chiers                                                    *
 *                                                                      *
 ************************************************************************/

int deldoublon_db(int Fich1);

/************************************************************************
 *                                                                      *
 * Objectif : Suppression des doublons croises dans le fichier des      *
 *            relations                                                 *
 *                                                                      *
 * Argument : Identifiant du fichier a traiter dans la table des fi-    *
 *            chiers                                                    *
 *                                                                      *
 ************************************************************************/

int deldoublonCr_db(int Fich1);

/************************************************************************
 * Objectif : Non definie                                               *
 *                                                                      *
 * Argument :                                                           *
 ************************************************************************/

int ElimCTDoublons(int Fich1);

/************************************************************************
 * Objectif : Filtrage de relations telles que les deux mots lies       *
 *            soient dans la liste des candidats termes                 *
 *                                                                      *
 * Arguments : 1. Identifiant du fichier des relations dans la table    *
 *                des fichiers                                          *
 *             2. Identifiant du fichier des candidats termes dans la   *
 *                table des fichiers                                    *
 ************************************************************************/

int Filtrage(int Fichsyn, int Fichct);

