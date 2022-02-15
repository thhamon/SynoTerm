/* 	$Log: config.h,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.5  2003/10/27 15:07:57  ht
 * 	Ajout de la macro pour la sortie XML
 *
 * 	Revision 1.4  2003/09/04 07:23:35  ht
 * 	*** empty log message ***
 *
 * 	Revision 1.3  2001/08/05 08:19:11  thierry
 * 	Commentaires des macros
 * 	Entete de declaration des fonctions
 *
 * 	Revision 1.2  2001/05/01 17:29:40  thierry
 * 	*** empty log message ***
 *	 */

#include <sys/wait.h>

#if !defined(CONFIG_H)
#define CONFIG_H


/************************************************************************
 *           Definitions des macros correspondant aux options           *
 ************************************************************************/


#define SOCK               2           /* Utilisation des sockets (Obsolete) */
#define NOINTERFACE        4           /* Pas d'utilisation/lancement de l'interface
					  de validation 
				        */
#define DISPLAYLOG         8           /* Affichage du journal des evenements dans 
					  un terminal a part
				        */
#define ISO                16          /* Indication du codage des caracteres du fichier
					  avec la norme ISO-8859-1
				        */ 
#define FTYPOPAUVRE        32          /* Indication de la redaction du fichier en typographie
					  pauvre
				        */
#define ELIMINDBLS         64          /* Elimination des doublons dans le dictionnaire de 
					  relations 
				        */
#define ELIMINDBLSCR       128         /* Elimination des doublons croises dans le dictionnaire
					  de relations 
				        */
#define AFFICHECLASSES     256         /* Utilisation d'un regroupement en classe lors de la sortie
					  des relations inferes dans un fichier
				        */
#define AFFICHEFAMILLES    512         /* Utilisation d'un regroupement en famille lors de la sortie
					  des relations inferes dans un fichier
				        */
#define AFFICHELISTE       1024        /* Sortie des relations dans un fichier, presentation sous forme
					  d'une liste plate 
				        */
#define SORTIEFICHIER      2048        /* Indique qu'un fichier de sortie a ete precise */
#define AFFICHEFAMILLESFF  4096        /* Utilisation d'un regroupement en famille lors de la sortie
					  des relations inferes dans un fichier. Les candidats termes
					  sont presentes sous forme flechie
				        */

#define AFFICHEXML         8192        /* Sortie des relations dans un fichier, presentation sous forme
					  d'une liste plate 
				        */

#define DEVEL               16834       /* Passage en mode developpement */

/************************************************************************
 *           Definitions des macros correspondant aux fonctions         *
 ************************************************************************/

#define FONC_CHARGER_CORPUS       1    /* Chargement d'un corpus au format de SynoTerm */
#define FONC_CHARGER_DICO         2    /* Chargement d'un dictionnaire au format de SynoTerm */
#define FONC_FILTRAGE             3    /* Filtrage des relations proposees dans le dictionnaire */
#define FONC_INFERENCE            4    /* Inference des relations */
#define FONC_SORTIE               5    /* Realisation d'une sortie des relations inferees, dans 
					  un fichier
				        */
#define FONC_VALIDATION           6    /* Validation des relations inferees a partir d'information 
					  existantes 
				        */
#define HELP                      7    /* Affichage de l'aide  */
#define FONC_INFERENCE2           8    /* Inference des relations a partir d'un second dictionnaire */
#define FONC_CLASSES2CORPUS       9    /* Passage de classes sous forme d'une corpus pour l'extraction 
					  des relations initiales 
				        */
#define FONC_EXTRACTIONLINIT      10   /* Extraction des relations initiales */
#define FONC_DIVERSITE_SHANNON    11   /* Calcul de la diversite de Shannon pour les candidats termes */

#endif

