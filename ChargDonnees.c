/* 	$Log: ChargDonnees.c,v $
/* 	Revision 1.2  2006/11/23 14:04:00  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.4  2001/07/27 12:35:17  thierry
 * 	Fin de l'ajout de commentaires (une fonction oubliee)
 *
 * 	Revision 1.3  2001/07/27 12:23:14  thierry
 * 	Ajout des commentaires
 *
 * 	Revision 1.2  2001/05/01 19:40:03  thierry
 * 	*** empty log message ***
 *	 */

/************************************************************************
 *                                                                      *
 * Ensemble des fonctions du l'operation Operations->Inference          *
 *                                                                      *
 ************************************************************************/

#include "ChargDonnees.h"
#include "Fichier.h"
#include "VarSem.h"

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

FILE *DicoWDbl;

/************************************************************************
 *                                                                      *
 * Objectif : Suppression des doublons dans le fichier des relations    *
 *                                                                      *
 * Argument : Identifiant du fichier a traiter dans la table des fi-    *
 *            chiers                                                    *
 *                                                                      *
 ************************************************************************/

int deldoublon_db(int Fich1)
{
  /************************************
  * Declaration des variables locales *
  ************************************/

  char cmd[3000];                  /* contient la commande a executer au 
                                      niveau systeme 
                                    */
  char fichold[1000];              /* nom de l'ancien fichier */

  /**********************************************
  * Fin de la declaration des variables locales *
  ***********************************************/


  /******************************************************************************/
  /* Sauvegarde du fichier des relations dans un fichier avec l'extension 'old' */
  /******************************************************************************/

  strcpy(fichold,liste_fichier[Fich1].nom);
  strcat(fichold,".db.old");

  sprintf(cmd,"cp -f %s.db %s",liste_fichier[Fich1].nom,fichold);
  system(cmd);

  /*******************************************************************/
  /* Appel du script perl TriDoublons.pl pour supprimer les doublons */
  /*******************************************************************/

  sprintf(cmd,"TriDoublons.pl %s %s.db",fichold,liste_fichier[Fich1].nom);
  system(cmd);
    
  /***********************************************************************/
  /* Ajout du champs 'Prod' dans l'entree Fich1 de la table des fichiers */
  /***********************************************************************/

  liste_fichier[Fich1].lst_champs = (char **)realloc(liste_fichier[Fich1].lst_champs,sizeof(char *)*(liste_fichier[Fich1].nb_champs+1));
  liste_fichier[Fich1].lst_champs[liste_fichier[Fich1].nb_champs] = (char *)malloc(sizeof(char)*5);
  strcpy(liste_fichier[Fich1].lst_champs[liste_fichier[Fich1].nb_champs],"Prod");
  liste_fichier[Fich1].nb_champs++;

  /***************************************************/
  /* Ajout du champs 'Prod' dans le fichier d'entete */
  /***************************************************/

  strcpy(fichold,liste_fichier[Fich1].nom);
  strcat(fichold,".db.ent");

  printlog("Passe (2)\n");

    
  DicoWDbl = fopen(fichold,"r+");

  AjouterChampsDsEntete(DicoWDbl, "Champs", "Prod");
  fclose(DicoWDbl);
}

/************************************************************************
 *                                                                      *
 * Objectif : Suppression des doublons croises dans le fichier des      *
 *            relations                                                 *
 *                                                                      *
 * Argument : Identifiant du fichier a traiter dans la table des fi-    *
 *            chiers                                                    *
 *                                                                      *
 ************************************************************************/

int deldoublonCr_db(int Fich1)
{
  /************************************
  * Declaration des variables locales *
  ************************************/
  char cmd[3000];                  /* contient la commande a executer au 
                                      niveau systeme 
                                    */
  char fichold[1000];              /* Flux sur le fichier des relations 
                                      Remarque : Peut-etre qu'il serait bien 
                                      renommer cette variable pour que le nom
                                      soit plus general
		 		    */

  /**********************************************
  * Fin de la declaration des variables locales *
  ***********************************************/

  /******************************************************************************/
  /* Sauvegarde du fichier des relations dans un fichier avec l'extension 'old' */
  /******************************************************************************/

  strcpy(fichold,liste_fichier[Fich1].nom);
  strcat(fichold,".old");


  
  sprintf(cmd,"cp -f %s.db %s",liste_fichier[Fich1].nom,fichold);
  system(cmd);
   
  /*************************************************************************/
  /* Appel du script perl DelDblsCr.pl pour supprimer les doublons croises */
  /*************************************************************************/

  sprintf(cmd,"DelDblsCr.pl < %s > %s.db",fichold,liste_fichier[Fich1].nom);
  system(cmd);
        
  /*************************************************************************/
  /* Ajout du champs 'ProdCr' dans l'entree Fich1 de la table des fichiers */
  /*************************************************************************/

  liste_fichier[Fich1].lst_champs = (char **)realloc(liste_fichier[Fich1].lst_champs,sizeof(char *)*(liste_fichier[Fich1].nb_champs+1));
  liste_fichier[Fich1].lst_champs[liste_fichier[Fich1].nb_champs] = (char *)malloc(sizeof(char)*7);
  strcpy(liste_fichier[Fich1].lst_champs[liste_fichier[Fich1].nb_champs],"ProdCr");
  liste_fichier[Fich1].nb_champs++;

  /*****************************************************/
  /* Ajout du champs 'ProdCr' dans le fichier d'entete */
  /*****************************************************/

  strcpy(fichold,liste_fichier[Fich1].nom);
  strcat(fichold,".db.ent");
    
  DicoWDbl = fopen(fichold,"r+");

  AjouterChampsDsEntete(DicoWDbl, "Champs", "ProdCr");

  fclose(DicoWDbl);
}

/************************************************************************
 * Objectif : Non definie                                               *
 *                                                                      *
 * Argument :                                                           *
 ************************************************************************/

int ElimCTDoublons(int Fich1)
{
    
}

/************************************************************************
 * Objectif : Filtrage de relations telles que les deux mots lies       *
 *            soient dans la liste des candidats termes                 *
 *                                                                      *
 * Arguments : 1. Identifiant du fichier des relations dans la table    *
 *                des fichiers                                          *
 *             2. Identifiant du fichier des candidats termes dans la   *
 *                table des fichiers                                    *
 ************************************************************************/

int Filtrage(int Fichsyn, int Fichct)
{
  char cmd[3000];                  /* contient la commande a executer au 
                                      niveau systeme 
				    */
   
  /************************************************************************/
  /* Appel du script perl Filtrage.pl pour supprimer les doublons croises */
  /************************************************************************/

  sprintf(cmd,"Filtrage.pl %s %s.db %d %d",liste_fichier[Fichct].nom, /*liste_fichier[Fichct].nb_fichier,*/ liste_fichier[Fichsyn].nom, liste_fichier[Fichct].typo, liste_fichier[Fichsyn].typo);
  /* Execution de la commande */
  system(cmd);
}
