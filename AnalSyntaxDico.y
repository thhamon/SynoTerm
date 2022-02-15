/*	$Log: AnalSyntaxDico.y,v $
/*	Revision 1.2  2007/02/28 11:33:34  ht
/*	quelques modifs
/*	
/*	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/*	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/*	
 *	Revision 1.4  2003/08/05 11:04:02  thierry
 *	Correction d'un Warning
 *
 *	Revision 1.3  2001/05/01 19:10:00  thierry
 *	Correction des commentaires de log
 *
 *	Revision 1.2  2001/05/01 19:06:10  thierry
 *	*** empty log message ***
 */	
%{
	/*
	 * 
	 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "VarSem.h"
#include "ChargDonnees.h"
#include "Fichier.h"

FILE *DicoSynFlux;
FILE *FicDicoSyn;
FILE *DSF;
FILE *DicoSu;

%}

%token VEDETTE SYNONYME ID FL

%%

entrees : entree | entrees entree
	;

entree: VEDETTE ':' l_sens FL
	;

l_sens: ID l_synonymes l_sens
	| ID l_synonymes
	| l_synonymes 
	;

l_synonymes: SYNONYME ',' l_synonymes
	| SYNONYME
	;


%%

DicoSynerror(char *s)
{
	fprintf(stderr,"--> Erreur d'analyse (%s)\n", s);
	printf("Fermeture du Dictionnaire de Synonymes\n");
	fclose(FicDicoSyn);
	
	printf("Fermeture de la base du Dictionnaire de Synonymes\n");
	fclose(DicoSynFlux);
	printf("Base DicoSyn.txt Fermee\n");
	exit(-1);
}


int charge_Dico_INaLF7(char *nom_dico,char *nom, int typo, int iso, struct Donnees_Nom_Fichier *info_fichier)
{
	/* Chargement d'un dictionnaire au format original du dico 7 fourni par l'INaLF*/
    char *nom_dicodb,*nom_dicodbent,*nom_dicotxt,*nom_dicoutxt,cmd[3000];
    FILE *FileEnt;
  
    if (iso){
	sprintf(cmd,"ISO8859toSGML.pl %s",nom_dico);
        system(cmd);
    }
    
    nom_dicotxt = (char *)malloc(sizeof(char)*(strlen(nom)+5));
    strcpy(nom_dicotxt,nom);
    strcat(nom_dicotxt,".txt");
    DicoSynFlux = fopen(nom_dicotxt,"wt"); 
  
  
    if ((FicDicoSyn = fopen(nom_dico,"r"))==NULL){
      fprintf(stderr,"Ouverture du fichier %s impossible\n", nom_dico);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_TRAITMT);     
    }

    DicoSynin = FicDicoSyn;

    /*fprintf(stderr,"dicoParse\n");*/
    while(!feof(DicoSynin)) { DicoSynparse(); }

    fclose(FicDicoSyn);
    fclose(DicoSynFlux);

  		/* Chargement en Memoire */
  
    if ((nom_dicodb = (char *)malloc(sizeof(char)*(strlen(nom_dico)+4)))==NULL){
	fprintf(stderr,"Erreur d'allocation memoire");
	writelogS("Erreur d'allocation memoire");
	exit(-1);     
    }
    strcpy(nom_dicodb,nom);
    strcat(nom_dicodb,".db");
  
    strcpy(cmd, "cp ");
    strcat(cmd, nom_dicotxt);
    strcat(cmd," ");
    strcat(cmd, nom_dicodb);
    system(cmd);
  
  /* Creation du fichier d'entete */
  
  if ((nom_dicodbent = (char *)malloc(sizeof(char)*(strlen(nom_dicodb)+5)))==NULL){
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
  }
  strcpy(nom_dicodbent,nom_dicodb);
  strcat(nom_dicodbent,".ent");
  
  if ((FileEnt = fopen(nom_dicodbent,"w"))==NULL){
      fprintf(stderr,"Ouverture du fichier d'entete %s impossible\n", nom_dicodbent);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_TRAITMT);     
  }
  fprintf(FileEnt,"Type = syn\n");
  fprintf(FileEnt,"Typographie = %d\n",typo);
  fprintf(FileEnt,"Champs = Term1;Term2\n");

  fclose(FileEnt);
  charger_db(nom_dicodb, nom, info_fichier);
  
  return(EXEC_OK);
}
