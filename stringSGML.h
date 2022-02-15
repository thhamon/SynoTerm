/* 	$Log: stringSGML.h,v $
/* 	Revision 1.2  2007/08/01 08:48:17  ht
/* 	modification of the Makefile
/* 	bug fixes
/* 	addition of the functionality allowing to load WordNet from WordNet file
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.2  2001/05/01 17:30:48  thierry
 * 	*** empty log message ***
 *	 */

#include <stdio.h>
#include<string.h>

#define TYPOPAUVRE	1 /* Typo Pauvre */
#define CATEG_GRAM	2 /* Categorie grammaticale*/
#define CARNONDEF	4 /* caratere non defini */


int strcmp_sgml(char *ch1, char *ch2);
int strncmp_sgml(char *ch1, char *ch2, int n);

int strcmpi_sgml(char *ch1, char *ch2);
int strncmpi_sgml(char *ch1, char *ch2,int n);

int strcmp_sgml_carndef(char *ch1, char *ch2);
int strncmp_sgml_carndef(char *ch1, char *ch2, int n);
int strcmpi_sgml_carndef(char *ch1, char *ch2);
int strncmpi_sgml_carndef(char *ch1, char *ch2,int n);

int strcmp_carndef(char *ch1, char *ch2);
int strncmp_carndef(char *ch1, char *ch2, int n);
int strcmpi_carndef(char *ch1, char *ch2);
int strncmpi_carndef(char *ch1, char *ch2,int n);
