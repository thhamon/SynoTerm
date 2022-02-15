/* 	$Log: Fichier.c,v $
/* 	Revision 1.3  2006/11/23 14:04:00  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.2  2006/05/19 11:01:13  ht
/* 	ajout d'évènement, correction de bug dans le makefile
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.6  2003/08/01 10:12:06  thierry
 * 	Ajout d'une valeur de retour a FermerF
 *
 * 	Revision 1.5  2001/08/05 08:20:47  thierry
 * 	Ajout de l'indication de l'inclusion des bibliotheques
 *
 * 	Revision 1.4  2001/07/28 13:54:33  thierry
 * 	Ajout de commentaires oublies
 *
 * 	Revision 1.3  2001/07/28 12:06:12  thierry
 * 	*** empty log message ***
 *
 * 	Revision 1.2  2001/05/01 19:40:25  thierry
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

/**********************************************************************
 *          Inclusion des bibliotheques declarees localement          *
 **********************************************************************/

#include "Communication.h"
#include "Fichier.h"

/**********************************************************************
 *                                                                    *
 * Gestion des fichiers dans SynoTerm                                 *
 *                                                                    *
 **********************************************************************/

/************************************************************************
 *                                                                      *
 * Contenu : Table contenant les fichiers ouverts dans SynoTerm         *
 *                                                                      *
 ************************************************************************/

FichierMem liste_fichier[NBFICHIER];

/************************************************************************
 *                                                                      *
 * Contenu : Nombre de fichiers presents dans la table liste_fichier    *
 *                                                                      *
 ************************************************************************/

int nbre_fichier = 0;

/************************************************************************
 *                                                                      *
 * Contenu : Table contenant un historique des fichiers ouverts         *
 *                                                                      *
 ************************************************************************/

int Historique[10];

/************************************************************************
 *                                                                      *
 * Contenu : Index de l'historique des fichiers ouverts                 *
 *                                                                      *
 ************************************************************************/

int Hcur;


/************************************************************************
 *                                                                      *
 * Objectif : Charge un dictionnaire au format TEI (Non Implemente)     *
 *                                                                      *
 * Argument : Nom du dictionnnaire                                      *
 *                                                                      *
 ************************************************************************/

int charge_Dico_TEI(char *nom_dico)
{
  fprintf(stderr,"Non implemente\n");
}

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation dess informations concernant un type de      *
 *            fichiers ouverts dans SynoTerm et contenus la table des   *
 *            fichiers                                                  *
 *            Les messages et les informations sont envoyees sur la     * 
 *            sortie standard                                           *
 *                                                                      *
 * Argument : Type de fichiers ouverts dans SynoTerm                    *
 *                                                                      *
 ************************************************************************/

int info_table(int flux_ecrit, int type)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Index de la table des fichiers ouverts */
  char *msg;

  printlog("Dans info_table\n");

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Verification si des fichiers sont ouverts 
     Si non, envoie du message 'NonFichiers'
  */
  if (nbre_fichier==0){
/*     print_com_msg("Begin",flux_ecrit,OUI); */
    print_com_msg("NonFichiers",flux_ecrit,OUI);
    print_com_msg("End",flux_ecrit,OUI);
/*     printlog("Pas de table en memoire"); */
    return(1);
  }

  /* Envoie des informations concernant les fichiers correspondant
     au type de fichier demande
   */
  /* Envoi du message 'Begin' indiquant le debut de l'envoi des
     informations sur les fichiers
  */
  print_com_msg("Begin",flux_ecrit,OUI);

  i=0;
/*   printlog("Affichage des informations sur les tables en memoire\n"); */
  while(i<NBFICHIER){
    /* On regarde si le fichier est bien ouvert dans SynoTerm
       et  si on veut tous les fichiers ouverts ou seulement ceux d'un 
       certain type 
     */
    if (((type==ALL)||(liste_fichier[i].type==type))&&(liste_fichier[i].nb_champs!=-1)) {
      printlog("trouve : %s\n", liste_fichier[i].nom);
      print_com_msg((liste_fichier[i].info_fichier)->fichname,flux_ecrit,OUI);
      print_com_msg((liste_fichier[i].info_fichier)->dirname,flux_ecrit, NON);
      print_com_msg("/",flux_ecrit,NON);
      print_com_msg((liste_fichier[i].info_fichier)->fichname,flux_ecrit,OUI);
    }
    /* On passe a l'enregistrement suivant
     */
    i++;
  }
  /* Envoi du message 'End' indiquant la fin de l'envoi des
     informations sur les fichiers
  */
  print_com_msg("End",flux_ecrit,OUI);
  return(1);
}

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de l'index du fichier dont le nom est fourni *
 *            en argument                                               *
 *                                                                      *
 * Argument : Nom du fichier dont on veut l'index                       *
 *                                                                      *
 * Retour : index du fichier                                            *
 *                                                                      *
 ************************************************************************/

int get_table(char *NomFich)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Index de la table des fichiers ouverts */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/
    
  /* Verification si des fichiers sont ouverts 
     Si non, rien
  */
  if (nbre_fichier==0){
    writelogS("Pas de table en memoire");
    return(-1);
  }
  
  /* Recherche du fichier 'NomFich' dans la table
   */
  i=0;
  while((i<NBFICHIER)&&((liste_fichier[i].nom == 0)||(strcmp(NomFich,liste_fichier[i].nom)!=0))) i++;

  /* Si on a trouve le fichier, on envoie l'index
   */
  if (i<NBFICHIER) return(i);
  else return (-1);
}

/************************************************************************
 *                                                                      *
 * Objectif : Recherche du premier fichier de la table des fichiers     *
 *            ouverts dont le type est 'typeFich'                       *
 *            Remarque : interet ?                                      *
 *                                                                      *
 * Argument : Type du fichier a rechercher                              *
 *                                                                      *
 * Retour : index du fichier                                            *
 *                                                                      *
 ************************************************************************/

int get_table_type(int typeFich)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Index de la table des fichiers ouverts */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Verification si des fichiers sont ouverts 
     Si non, rien
  */
  if (nbre_fichier==0){
    writelogS("Pas de table en memoire");
    return(-1);
  }

  /* Recherche du fichier de type typeFich dans la table
   */
  i=0;
  while((i<NBFICHIER)&&((liste_fichier[i].type == 0)||(liste_fichier[i].type != typeFich))) i++;
  /* Si on a trouve le fichier, on envoie l'index
   */
  if (i<NBFICHIER) return(i);
  else return (-1);
}

/************************************************************************
 *                                                                      *
 * Objectif : Suppression d'un fichier de la table des fichiers         *
 *            La recherche s'effectue soit a partir de l'index du       *
 *            fichier dans la table, soit a partir du nom du fichier    *
 *                                                                      *
 * Argument : 1. Index du fichier a supprimer de la table               *
 *                                                                      *
 *            2. Nom du fichier a supprimer de la table                 *
 *                                                                      *
 *                                                                      *
 ************************************************************************/

int FermerF(int numf,char *nomf)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Index de la table des fichiers ouverts */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Cas : Suppression a partir du nom du fichier
   */
  if (nomf!=NULL) {
    /* On recherche l'index du fichier
     */
    i=0;
    while((i<NBFICHIER)&&(strcmp(liste_fichier[i].nom,nomf)!=0)){
      i++;
    }
    /* On met les champs 'type', nombre de champs et nom a 'zero'
     */
    liste_fichier[i].type=-1;
    liste_fichier[i].nb_champs=-1;
    liste_fichier[i].nom=NULL;
  }
  else { /* Cas : Suppression a partir de l'index du fichier 
          */
    if (numf<NBFICHIER){
      /* On met les champs 'type', nombre de champs et nom a 'zero'
       */
      liste_fichier[numf].type=-1;
      liste_fichier[numf].nb_champs=-1;
      liste_fichier[numf].nom=NULL;
    }
  }
  return(0);
}

/************************************************************************
 *                                                                      *
 * Objectif : Retourne le code correspond a un type de fichier          *
 *                                                                      *
 * Argument : Le nom du type dont on veut le code                       *
 *                                                                      *
 * Retour : Code correspond au type de fichier                          *
 *                                                                      *
 ************************************************************************/

int getCodeFichier(char *str) 
{
  /* Cas corpus 
   */
  if (strcmp(str,"corpus")==0) return(CORPUS);
  /* Cas dictionnaire de synonymes 
   */
  if (strcmp(str,"syn")==0) return(SYN);
  /* Cas nouveau fichier cree (fichier resultat)
   */
  if (strcmp(str,"new")==0) return(NEW);

  /* listde de termes */
  if (strcmp(str,"liste de termes")==0) return(TERMLIST);
}

/************************************************************************
 *                                                                      *
 * Objectif : Retourne le type du fichier correspondant au code         *
 *                                                                      *
 * Argument : 1. Code dont on veut le type                              *
 *                                                                      *
 *            2. Type du fichier en retour                              *
 *                                                                      *
 * Retour : Type du fichier correspondant au code                       *
 *                                                                      *
 ************************************************************************/

int getTypeFichier(int code, char *str) 
{
  /* Cas corpus 
   */
  if (code==CORPUS) strcpy(str,"corpus");
  /* Cas dictionnaire de synonymes 
   */
  if (code==SYN) strcmp(str,"syn");
  /* Cas nouveau fichier cree (fichier resultat)
   */
  if (code==NEW) strcmp(str,"new");
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'une valeur a un champ dans le fichier d'entete    *
 *            d'un fichier                                              *
 *                                                                      *
 * Argument : 1. Flux sur le fichier d'entete                           *
 *                                                                      *
 *            2. Nom du champs auquel on veut ajouter une valeur        *
 *                                                                      *
 *            3. Valeur a ajouter                                       *
 *                                                                      *
 ************************************************************************/

int AjouterChampsDsEntete(FILE *FichierEnt, char *champs, char *val)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *tmp; /* Contient le nom du champs courant (ou au moins une partie) */
  char c; /* Contient le caractere lu a la position courante */
  int len; /* Longueur du champs */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /*  Initialisation des variables et allocation memoire
   */
  len = strlen(champs);
  tmp=(char *)malloc(sizeof(char) * (len+1));

  /* On se place au debut du fichier d'entete
   */
  rewind(FichierEnt);
  /* Parcours du fichier a la recherche du champ
   */
  while(!feof(FichierEnt)) {
    /* On lit le nom du champs courant ...
     */
    fread(tmp,len, 1, FichierEnt);
    tmp[len]=0;
    /* ... et on se place a la fin de la ligne
     */
    do {
      fread(&c,1,1,FichierEnt);
    } while((!feof(FichierEnt))&&(c != '\n'));
    /* On a trouve le champs qu'on cherche
     */
    if (strcmp(tmp,champs) == 0) {
      /* On revient un caractere e arriere
       */
      fseek(FichierEnt,-1L,SEEK_CUR);
      /* On ajoute la valeur
       */
      fprintf(FichierEnt,";%s\n",val);
      return(0);
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Lecture du fichier pour recuperer :                       *
 *                    1. Le nom du repertoire (dirname)                 *
 *                    2. Le nom du fichier (basename)                   *
 *                    3. Le nom du fichier sans l'extension             *
 *                    4. L'extension du fichier                         * 
 *                                                                      *
 * Argument : 1. Le fichier a analyser                                  *
 *            2. Une structure contenant les informations sur le        *
 *               fichier                                                *
 *                                                                      *
 * Retour :  Etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 29/08/2003                           *
 *                                                                      *
 ************************************************************************/


int Analyse_Nom_Fichier(char *nom_fichier, struct Donnees_Nom_Fichier *donnees_nom_fichier)
{
  char *pointeur_tmp;

  /* Recuperation du nom du repertoire */
  if ((pointeur_tmp = (char *)strrchr(nom_fichier, SEP_REP)) != NULL) {
    if ((donnees_nom_fichier->dirname = (char *)malloc(sizeof(char) * (strlen(nom_fichier) - strlen(pointeur_tmp)  + 1))) == NULL) {
          fprintf(stderr,"Erreur d'allocation memoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
    }
    strncpy(donnees_nom_fichier->dirname, nom_fichier, strlen(nom_fichier) - strlen(pointeur_tmp));
    donnees_nom_fichier->dirname[pointeur_tmp - nom_fichier] = 0;
    pointeur_tmp++; /* Pour la suite (recuperation du nom du fichier) */
  } else {
    if ((donnees_nom_fichier->dirname = (char *)malloc(sizeof(char) * NOM_MIN)) == NULL) {
          fprintf(stderr,"Erreur d'allocation memoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
    }
    strcpy(donnees_nom_fichier->dirname,REP_CUR);
    pointeur_tmp = nom_fichier; /* Pour la suite (recuperation du nom du fichier) */
  }

  /* Recuperation du nom du fichier */

  if ((donnees_nom_fichier->basename = (char *)malloc(sizeof(char) * (strlen(pointeur_tmp) + 1))) == NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  } else {
    strcpy(donnees_nom_fichier->basename, pointeur_tmp);
  }
  
  /* Recuperation du nom du fichier sans l'extension */

  if ((pointeur_tmp = (char *)strrchr(donnees_nom_fichier->basename, SEP_EXT)) != NULL) {
    if ((donnees_nom_fichier->fichname = (char *)malloc(sizeof(char) * (strlen(donnees_nom_fichier->basename) - strlen(pointeur_tmp) + 1))) == NULL) {
          fprintf(stderr,"Erreur d'allocation memoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
    }
    strncpy(donnees_nom_fichier->fichname, donnees_nom_fichier->basename, strlen(donnees_nom_fichier->basename) - strlen(pointeur_tmp));
    donnees_nom_fichier->fichname[strlen(donnees_nom_fichier->basename) - strlen(pointeur_tmp)] = 0;
    pointeur_tmp++; /* Pour la suite (recuperation de l'extension du fichier) */
  } else {
    /* S'il n'y a pas d'extension, on considere que c'est le nom du fichier */
    if ((donnees_nom_fichier->fichname = (char *)malloc(sizeof(char) * strlen(donnees_nom_fichier->basename))) == NULL) {
          fprintf(stderr,"Erreur d'allocation memoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
    }
    strcpy(donnees_nom_fichier->fichname,donnees_nom_fichier->basename);
    pointeur_tmp = NULL; /* Pour la suite (recuperation de l'extension du fichier) */
  }

  /* Recuperation de l'extension du fichier */

  if (pointeur_tmp != NULL) {
    if ((donnees_nom_fichier->extname = (char *)malloc(sizeof(char) * (strlen(pointeur_tmp) + 1))) == NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    } else {
      strcpy(donnees_nom_fichier->extname, pointeur_tmp);
    }
  } else {
    pointeur_tmp = NULL;
  }

  return(EXEC_OK);
}


/************************************************************************
 *                                                                      *
 * Objectif : Recherche si un fichier existe précédemment dans          *
 *            un intervalle de l'historique                             *
 *                                                                      *
 * Argument : 1. numero du fichier a rechercher                         *
 *                                                                      *
 *            2. Borne supérieure de l'intervalle de recherche          *
 *                                                                      *
 * Retour :  indication si le fichier existe ou non                     *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

int existe_prec_Historique(int numfichier, int bornesup) 
{
  int i;

  i=0;
  while((i<bornesup) && (Historique[i] != numfichier)) i++;
  
  
  if (i==bornesup) {
    printlog("Fichier %s non enregistre\n",liste_fichier[numfichier].nom);
    return(EXISTE_PAS);
  } else {
    printlog("Fichier %s deja enregistre\n",liste_fichier[numfichier].nom);
    return(EXISTE);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Indique si le fichier passé en argument est du type       *
 *            attendu                                                   *
 *                                                                      *
 * Argument : 1. Numero du fichier                                      *
 *                                                                      *
 *            2. Type du fichier                                        *
 *                                                                      *
 * Retour :  indication si le fichier est au bon type                   *
 *                                                                      *
 * Date de derniere modification : 24/07/2006                           *
 *                                                                      *
 ************************************************************************/

int is_type(int id_fichier,int type) {

  if (liste_fichier[id_fichier].type==type) {
    return(TRUE);
  } else {
    return(FALSE);
  }
}
