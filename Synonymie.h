/* 	$Log: Synonymie.h,v $
/* 	Revision 1.2  2006/11/23 14:04:01  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.2  2001/05/01 17:25:07  thierry
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
#include "VarSem.h"
#include "Communication.h"
#include "Graphe.h"
#include "SynoTerm_Gen.h"

#if !defined(SYNONYMIE_H)
#define SYNONYMIE_H



/********************************/

#define REGLE1          0x0001  /* lien infere par la regle 1 */
#define REGLE2          0x0002  /* lien infere par la regle 2 */
#define REGLE3          0x0003  /* lien infere par la regle 3 */

#define INTERDIT        0x00F0 /* Lien interdit */
#define DICOSYN         0x0010 /* Dictionnaire de synonymes */
#define THESAURUS       0x0020 /* Thesaurus */
#define PROP_EXPERT     0x0030 /* Lien propose par l'expert */
#define LIEN_EXTRAIT    0x0040 /* Lien extrait a l'aide des regles inverses */

#define DIVERSITE_SHANNON1       0  /* Diversite de Shannon pour N1 et N */
#define DIVERSITE_SHANNON2       1  /* Diversite de Shannon pour N2 et Adj */

#endif


extern FILE *FichResSyn;
extern Noeud *NoeudGen;
extern int CompteurTete,CompteurExp;


/***********************/



int Rule_1(FILE *FichResSyn, int E1, int E2, int, int flux_ecriture);

int Rule_2(FILE *FichResSyn, int T1, int T2, int , int flux_ecriture);

int Rule_3T(FILE *FichResSyn, int T1, int T2, int, int flux_ecriture);

int Rule_3E(FILE *FichResSyn, int E1, int E2, int, int flux_ecriture);

int Regle1(FILE *FichResSyn, char *entree, char *syn, int, int );

int Regle2(FILE *FichResSyn,char *entree, char *syn, int, int );

int InferenceCTSynE1(int, int, char *, int, int, int);

int Nouv_Infer(char *terme1, char *terme2, int);

int ExtractLiensInit(int NomFichSyn, int Fichct, char *FichResult, int info, int nbgnmax, int sock, int flux_ecriture);

int Regle1Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture);

int Regle2Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture);

int Regle3Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture);

