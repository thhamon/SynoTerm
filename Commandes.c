#include "Commandes.h"


/************************************************************************
 *                                                                      *
 * Objectif : Inialisation des parametres de configuration du programme *
 *                                                                      *
 * Arguments : 1. flux de lecture                                       *
 *                                                                      *
 *             2. flux d'ecriture                                       *
 *                                                                      *
 *             3. la structure qui contiendra les commandes             *
 *                                                                      *
 * Retour : Entier indiquant le resultat de l'appel                     *
 *                                                                      *
 * Date de derniere modification : 31/07/2003                           *
 *                                                                      *
 ************************************************************************/


int Initialisation_Commandes(int flux_lect, int flux_ecrit, struct Commande *commande) {

  /*************************************
   * Declaration des variables locales *
   *************************************/
  int i;

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

   /* On envoie tout sur la sortie standard
     */
    commande->Nb_elt_commande = 0;

    /* Index de l'historique
     */
    Hcur=0;
    /* Indicateur de la construction du graphe
     */
    CG=0;
    /* Fermeture de tous les fichiers ouverts
     */
    i=0;
    while((i<NBFICHIER) && (!FermerF(i,NULL))) i++;
    
    /* Sortie */
    if (i<NBFICHIER) {
      fprintf(stderr,"Erreur interne: fermeture de tous les fichiers impossibles");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_INIT);
    }
    else {
      return(EXEC_OK);
    }
}

/************************************************************************
 *                                                                      *
 * Objectif : Lecture d'un element d'une commande                       *
 *            (Protocole de communication)                              *
 *                                                                      *
 * Argument : 1. flux de lecture                                        *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Lecture_Elt_Commande(int flux_lect, Elt_commande *elt_comm) {
  int taille_elt_comm; /* taille de l'element a lire */
  int ret_lect; /* nombre d'octets lus, lors de la lecture courante */
  int ret; /* nombre total d'octets lus */
  char buf[NB_DIGIT_INT_COM+1];

  /* Lecture d'un entier sur 10 caracteres, puisque Perl n'envoie que des caracteres */
  if (read(flux_lect, buf, sizeof(char)*NB_DIGIT_INT_COM)==-1) {
    fprintf(stderr,"Erreur de lecture de la taille sur le flux de communication\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_LECTURE);
  } else {
    buf[NB_DIGIT_INT_COM]=0;
    taille_elt_comm = atoi(buf);
     printlog("Taille de la chaine a lire : %d\n", taille_elt_comm);
  }
  if (taille_elt_comm) {
    if ((*elt_comm = (char *)malloc(sizeof(char)*(taille_elt_comm+1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    ret_lect=0;
    ret = 0;
    do {
      if ((ret_lect=read(flux_lect, (*elt_comm)+ret, sizeof(char) * (taille_elt_comm - ret))) == -1) {
	fprintf(stderr,"Erreur de lecture de la chaine sur le flux de communication\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_LECTURE);
      }
      ret += ret_lect;
    } while (ret <taille_elt_comm);
    (*elt_comm)[taille_elt_comm]=0;
    printlog("elt_comm = %s\n", *elt_comm);
  } else {
    *elt_comm = NULL;
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Lecture complete d'une commade                            *
 *            (Protocole de communication)                              *
 *                                                                      *
 * Argument : 1. flux de lecture                                        *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Lecture_Commande(int flux_lect, struct Commande *commande) {
  int i;
  char buf[NB_DIGIT_INT_COM+1];

  /* Lecture d'un entier sur 10 caracteres, puisque Perl n'envoie que des caracteres */

  printlog("Dans Lecture_Commande\n");

  if (read(flux_lect, buf, sizeof(char)*NB_DIGIT_INT_COM)==-1) {
    fprintf(stderr,"Erreur de lecture du nombre d'element sur le flux de communication\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_LECTURE);
  } else {
    buf[NB_DIGIT_INT_COM]=0;
    commande->Nb_elt_commande = atoi(buf);
    printlog("Nombre d'element : %d\n", commande->Nb_elt_commande);

    if ((commande->elt_commande = (Elt_commande *)malloc(sizeof(Elt_commande)*commande->Nb_elt_commande))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
  }
  for(i=0; i<commande->Nb_elt_commande; i++) {
    if (Lecture_Elt_Commande(flux_lect, &(commande->elt_commande[i])) != EXEC_OK) {
      fprintf(stderr,"Erreur de lecture d'un element de commande\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_LECTURE);
    }
  }

  printlog("Fin Lecture_Commande \n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de conversion d'une commande (chaine de caracte- *
 *            -re) en un entier (pour la suite du traitement)           *
 *                                                                      *
 * Argument : 1. element de la commande a convertir                     *
 *                                                                      *
 * Retour : L'entier correspondant a l'element de la commande           *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Convert_Commande(Elt_commande commande) {
/*   fprintf(stderr, "commande : %s\n", commande); */
  if (strcmp(commande,"Load_db")==0) {              /* VERIFIER */
    return (L0AD_DB);
  }
  if (strcmp(commande,"Load_desamb")==0) {          /* VERIFIER */
    return (LOAD_DESAMB);
  }
  if (strcmp(commande,"Load_cls2corpus")==0) {
    return (LOAD_CLS2CORPUS);
  }
  if (strcmp(commande,"Load_syn")==0) {             /* VERIFIER */
    return (LOAD_SYN);
  }
  if (strcmp(commande,"Load_dic")==0) {
    return (LOAD_DIC);
  }
  if (strcmp(commande,"Infoall")==0) {              /* VERIFIER */
    return (INFO_ALL);
  }
  if (strcmp(commande,"Infosyn")==0) {              /* VERIFIER */
    return (INFO_SYN);
  }
  if (strcmp(commande,"Infodesamb")==0) {           /* VERIFIER */
    return (INFO_DESAMB);
  }
  if (strcmp(commande,"Infonew")==0) {              /* VERIFIER */
    return (INFO_NEW);
  }
  if (strcmp(commande,"Infotermlist")==0) {              /* VERIFIER */
    return (INFO_TERMLIST);
  }
  if (strcmp(commande,"SynElimDoubl")==0) {         /* VERIFIER */
    return (SYN_ELIMDOUBL);
  }
  if (strcmp(commande,"SynElimDoublC")==0) {        /* VERIFIER */
    return (SYN_ELIMDOUBLCR);
  }
  if (strcmp(commande,"FiltrageCTSyn")==0) {        /* VERIFIER */
    return (SYN_FILTRAGE);
  }
  if (strcmp(commande,"InferenceCTSyn")==0) {       /* VERIFIER */
    return (SYN_INFERENCE);
  }
  if (strcmp(commande,"Quit")==0) {                 /* VERIFIER */
    return (QUIT);
  }
  if (strcmp(commande,"QuitNoEnr")==0) {            /* VERIFIER */
    return (QUIT_NOENR);
  }
  if (strcmp(commande,"EnregistrementMat")==0) {    /* VERIFIER */
    return (SAVE_MAT);
  }
  if (strcmp(commande,"ChargerMat")==0) {           /* VERIFIER */
    return (LOAD_MAT);
  }
  if (strcmp(commande,"Validation")==0) {           /* VERIFIER */
    return (VALIDATION);
  }
  if (strcmp(commande,"ExtractLiensInit")==0) {
    return (SYN_EXTRAC_RELINIT);
  }
  if (strcmp(commande,"Sortie")==0) {
    return (SORTIE);
  }
  if (strcmp(commande,"Validation_Fichier")==0) {
    return (VALIDATION_FICHIER);
  }
  if (strcmp(commande,"InferenceCTSyn_Suppl")==0) {
    return (SYN_INFERENCE_SUPPL);
  }
  if (strcmp(commande,"Load_term_list")==0) {
    return (LOAD_TERM_LIST);
  }
  else {
    fprintf(stderr,"Erreur : commande inconnue : %s\n", commande);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return (ERR_COMMANDE);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier au format db (format SynoTerm)    *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/08/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_db(struct Commande *commande)
{
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a charger
   */
  char *tmpbase;
  struct Donnees_Nom_Fichier *donnees_nom_fichier;  /* Ensemble des informations sur le nom d'un fichier */

  /* Verfication de l'extension */
  tmpbase = commande->elt_commande[1] + (strlen(commande->elt_commande[1]) - strlen(EXT_FICHIER_ST_CT));
  if (strcmp(tmpbase,EXT_FICHIER_ST_CT) != 0) {
    fprintf(stderr,"Erreur dans l'extension du fichier %s (format incorrect)\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER);
  }

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }

  if (Analyse_Nom_Fichier(commande->elt_commande[1], donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }
  if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier->dirname) + strlen(donnees_nom_fichier->fichname) + 2)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(tmpbase,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname);

  if (charger_db(commande->elt_commande[1], tmpbase, donnees_nom_fichier) == -1) {
    fprintf(stderr,"Erreur lors du chargement du fichier %s\n",commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_CHARGDB);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier au format desamb (sorte LEXTER)   *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 01/10/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_desamb(struct Commande *commande)
{
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a charger
	  champ 3 : le fichier est en typographie pauvre (les lettres sont uniquement des majuscules) 1, sinon 0
	  champ 4 : les caracteres sont codés sur 8bits (1) sinon SGML (0)
   */
  struct Donnees_Nom_Fichier *donnees_nom_fichier; /* Ensemble des informations sur le nom d'un fichier */
  int typographie_pauvre; /* champ 3 */
  int codage_8bits; /* champ 4 */

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }

  if (Analyse_Nom_Fichier(commande->elt_commande[1], donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }

  typographie_pauvre = atoi(commande->elt_commande[2]);
  codage_8bits = atoi(commande->elt_commande[3]);
  
  if (Charger_desamb(commande->elt_commande[1], typographie_pauvre, codage_8bits, donnees_nom_fichier) == -1) {
    fprintf(stderr,"Erreur lors du chargement du fichier %s\n",commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_CHARGDB);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier de classe de termes apres         *
 *            convertion au format db (SynoTerm)                        *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_cls2corpus(struct Commande *commande)
{
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a charger
	  champ 3 : le fichier est en typographie pauvre (les lettres sont uniquement des majuscules) 1, sinon 0
	  champ 4 : les caracteres sont codés sur 8bits (1) sinon SGML (0)
   */
  char *tmpbase;
  struct Donnees_Nom_Fichier *donnees_nom_fichier; /* Ensemble des informations sur le nom d'un fichier */
  int typographie_pauvre; /* champ 3 */
  int codage_8bits; /* champ 4 */

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }

  if (Analyse_Nom_Fichier(commande->elt_commande[1], donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }

  typographie_pauvre = atoi(commande->elt_commande[2]);
  codage_8bits = atoi(commande->elt_commande[3]);
  

  if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier->dirname) + strlen(donnees_nom_fichier->fichname) + 2)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(tmpbase,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname);

  if (Classes2Corpus(tmpbase, typographie_pauvre, codage_8bits) == -1) {
    fprintf(stderr,"Erreur lors du chargement du fichier %s\n",commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_CHARGDB);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier contenant un dictionnaire de      *
 *            synonymes au format INaLF                                 *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_syn(struct Commande *commande)
{
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a charger
	  champ 3 : le fichier est en typographie pauvre (les lettres sont uniquement des majuscules) 1, sinon 0
	  champ 4 : les caracteres sont codés sur 8bits (1) sinon SGML (0)
   */
  char *tmpbase;
  struct Donnees_Nom_Fichier *donnees_nom_fichier; /* Ensemble des informations sur le nom d'un fichier */
  int typographie_pauvre; /* champ 3 */
  int codage_8bits; /* champ 4 */

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  if (Analyse_Nom_Fichier(commande->elt_commande[1], donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }

  if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier->dirname) + strlen(donnees_nom_fichier->fichname) + 2)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(tmpbase,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname);

  typographie_pauvre = atoi(commande->elt_commande[2]);
  codage_8bits = atoi(commande->elt_commande[3]);

  if (charge_Dico_INaLF7(commande->elt_commande[1], tmpbase, typographie_pauvre, codage_8bits, donnees_nom_fichier) == -1) {
    fprintf(stderr,"Erreur lors du chargement du fichier %s\n",commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_CHARGDB);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier contenant un dictionnaire         *
 *            (fonction non implementée)                                *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 06/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_dic(struct Commande *commande)
{
  fprintf(stderr,"Fonction non implémentée. Fichier de données %s\n",commande->elt_commande[1]);
  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
 
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Appel de la fonction d'elimination des doublons           *
 *                                                                      *
 * Argument :  1. la commande                                           *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 09/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ElimDoubl(struct Commande *commande) {

  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a traiter
  */
  int indFichier;

  printlog("Debut elimination des doublons\n");

  if ((indFichier = get_table(commande->elt_commande[1])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  if (deldoublon_db(indFichier) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors de la suppression des doublons dans le fichier %s\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  printlog("Fin elimination des doublons\n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Appel de la fonction d'elimination des doublons croises   *
 *                                                                      *
 * Argument :  1. la commande                                           *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 09/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ElimDoubl_Cr(struct Commande *commande) {
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier a traiter
	  champ 3 : (conditionnel, pour le traitement en ligne de commande) indication si chargement du fichier
  */
  int indFichier;

  printlog("Debut elimination des doublons croises\n");
  if ((indFichier = get_table(commande->elt_commande[1])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  printlog("indFichier %d \n", indFichier);
  if (deldoublonCr_db(indFichier) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors de la suppression des doublons croisés dans le fichier %s\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  printlog("Fin elimination des doublons croises\n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Filtrage de la liste de synonymes au regard de la liste   *
 *            de candidats termes                                       *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 15/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_Filtrage(struct Commande *commande) {
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier contenant les synonymes
	  champ 3 : le nom du fichier contenant les candidats termes
  */

  int indFichierLstSyn;
  int indFichierLstCT;

  if ((indFichierLstSyn = get_table(commande->elt_commande[1])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  if ((indFichierLstCT = get_table(commande->elt_commande[2])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  if (Filtrage(indFichierLstSyn, indFichierLstCT) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors du filtrage : %s et %s\n", commande->elt_commande[1], commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Inference des relations de synonymes                      *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 31/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_Inference(struct Commande *commande, int flux_ecriture) {
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier contenant les synonymes
	  champ 3 : le nom du fichier contenant les candidats termes
	  champ 4 : le nom du fichier temporaire qui contiendra les resultats
  */
  int indFichierLstSyn;
  int indFichierLstCT;
  int indFichierLstCT_IC; /* corpus pour l'indexation contrôlée - gestion de l'historique */
  int indFichierLstLT; /* liste des termes pour l'indexation contrôlée */ 
  int indFichierResTmp;
  char *pofile;

  printlog("Dans Commande_Syn_Inference (nb elt = %d)\n",commande->Nb_elt_commande);
  printlog("Dans Commande_Syn_Inference (%s - %s)\n", commande->elt_commande[1], commande->elt_commande[2]);

  /* Suppresion de l'extension si necessaire */

  if (commande->elt_commande[3] != NULL) {
    pofile = commande->elt_commande[3] + strlen(commande->elt_commande[3]) - strlen(EXT_GNM) - sizeof(char);
    if ((pofile[0] == SEP_EXT) && (strcmp(pofile+1,EXT_GNM) == 0)) {
      pofile[0] = 0;
    }
  } else {
    if ((commande->elt_commande[3] = (char *)malloc(sizeof(char)*(strlen(commande->elt_commande[2]) + strlen(EXT_FICHIER_ST_TMP) + 1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    sprintf(commande->elt_commande[3], "%s%s", commande->elt_commande[2],EXT_FICHIER_ST_TMP);
  }


  fprintf(stderr, "Results in %s\n", commande->elt_commande[3]);

  if ((indFichierLstSyn = get_table(commande->elt_commande[1])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }


  if ((indFichierLstCT = get_table(commande->elt_commande[2])) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  if (commande->Nb_elt_commande == 4) {
    printlog("Inference indexation libre\n");
    Historique[Hcur++] = indFichierLstSyn;
    Historique[Hcur++] = indFichierLstCT;
  } else {
    if (commande->Nb_elt_commande == 6) {

      if ((indFichierLstLT = get_table(commande->elt_commande[5])) == ERR_RENVOI_STD) {
	fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[2]);
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_FICHIER_NOEXIST);
      }
      Historique[Hcur++] = indFichierLstLT;

      Historique[Hcur++] = indFichierLstSyn;

      if ((indFichierLstCT_IC = get_table(commande->elt_commande[4])) == ERR_RENVOI_STD) {
	fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[2]);
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_FICHIER_NOEXIST);
      }
      Historique[Hcur++] = indFichierLstCT_IC;

    printlog("Inference indexation controlée (%d , %d)\n",indFichierLstCT_IC,indFichierLstLT);
    }
  }

   print_com_msg(ENVOI_FICHIER_RESULTAT,flux_ecriture,OUI);
   print_com_msg(liste_fichier[indFichierLstCT].nom,flux_ecriture,OUI);
   print_com_msg(SEP_EXT_STRING, flux_ecriture, OUI);
   print_com_msg(EXT_GNM, flux_ecriture, OUI);

  printlog("Debut inference\n");

  if (InferenceCTSynE1(indFichierLstSyn, indFichierLstCT, commande->elt_commande[3], DICOSYN, liste_fichier[indFichierLstCT].nbrgnmax, flux_ecriture) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors de l'inference : %s et %s\n", commande->elt_commande[1], commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  printlog("Fin Commande_Syn_Inference\n");
  printlog("Enregistrement des liens inferes\n");
  if (EnregLiensInf(commande->elt_commande[3]) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors du filtrage : %s et %s\n", commande->elt_commande[1], commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  printlog("Fin de l'enregistrement des liens inferes\n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Terminaison du programme avec enregistrement des          *
 *            resultats                                                 *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 22/09/2004                           *
 *                                                                      *
 ************************************************************************/

int Commande_Quit(struct Commande *commande) {
  /* Dans commande : 
          champ 1 : la commande
  */

  if (CG) {
    if (Commande_Save_Mat(NULL) != EXEC_OK) {
      return(ERR_TRAITMT);
    }
  }
  if (Commande_Quit_NoEnr(NULL) != EXEC_OK) {
    return(ERR_TRAITMT);

  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Terminaison du programme sans enregistrement des          *
 *            resultats                                                 *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat d'execution de la commande                           *
 *                                                                      *
 * Date de derniere modification : 22/09/2004                           *
 *                                                                      *
 ************************************************************************/

int Commande_Quit_NoEnr(struct Commande *commande) {
  /* Dans commande : 
          champ 1 : la commande
  */
  
  if (CG) {
    DestructionMat();
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement de la matrice. Sauvegarde des liens        *
 *            inferes                                                   *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 22/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Save_Mat(struct Commande *commande) {
  /* Dans commande :
     champ 1 : la commande
  */

  char *FichRes;
  int indFichierLstCT;
  int i = 0;

  do {
    indFichierLstCT = Historique[Hcur-i];
    i++;
  } while((!is_type(indFichierLstCT, CORPUS))&&(!is_type(indFichierLstCT, NEW)));

  /* XXX Revoir cette partie, ce n'est pas tres propre */
  if (liste_fichier[indFichierLstCT].type==NEW) {/* XXX C'est bof mais on laise comme ca pour l'instant */
    if ((FichRes = (char *)malloc(sizeof(char)*(strlen(liste_fichier[indFichierLstCT].nom) + 1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    sprintf(FichRes,"%s",liste_fichier[indFichierLstCT].nom, SEP_EXT, EXT_GNM);
  } else {
    if ((FichRes = (char *)malloc(sizeof(char)*(sizeof(char) + strlen(EXT_RES) + strlen(liste_fichier[indFichierLstCT].nom)+1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    sprintf(FichRes,"%s%c%s",liste_fichier[indFichierLstCT].nom,SEP_EXT, EXT_RES, SEP_EXT, EXT_GNM);
  }
  if (EnregLiensInf(FichRes) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors du filtrage : %s et %s\n", commande->elt_commande[1], commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement de la matrice. Sauvegarde des liens inferes    *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 02/01/2004                           *
 *                                                                      *
 ************************************************************************/

int Commande_Load_Mat(struct Commande *commande, int flux_ecriture) {
  /* Dans commande :
     champ 1 : la commande
     champ 2 : le fichier des resultats
  */

  int indFichierLstCT;
  struct Donnees_Nom_Fichier *donnees_nom_fichier;
  char *tmpbase; /* Nom temporaire */

  /****************************************************
   *                                                  *
   * Chargement du fichier résultat avec son contexte *
   *                                                  *
   ****************************************************/

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
 if (Analyse_Nom_Fichier(commande->elt_commande[1], donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }
  if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier->dirname) + strlen(donnees_nom_fichier->fichname) + 2)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(tmpbase,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname);
/*   fprintf(stderr, "tmpbase = %s\n", tmpbase); */

  if (charger_db(commande->elt_commande[1], tmpbase, donnees_nom_fichier) == -1) {
    fprintf(stderr,"Erreur lors du chargement du fichier %s\n",commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_CHARGDB);
  }

  if ((indFichierLstCT=ChargLiensInf(flux_ecriture)) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors du chargement de la matrice \n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Validation d'un lien                                      *
 *                                                                      *
 * Argument : 1. la commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Validation(struct Commande *commande) {
  /* Dans commande :
     champ 1 : la commande
     champ 2 : la nouvelle valeur
     champ 3 : le type du lien
     champ 4 : l'identifiant du premier terme
     champ 5 : l'identifiant du second terme
  */

  FILE *indFichierTmp;
  int ValeurValidation;
  int idTerme1;
  int idTerme2;
  int typeL;
  
/*   fprintf(stderr, "Commande : %s\n", commande->elt_commande[0]); */
/*   fprintf(stderr, "Nouvelle valeur : %s\n", commande->elt_commande[1]); */
/*   fprintf(stderr, "Type de lien : %s\n", commande->elt_commande[2]); */
/*   fprintf(stderr, "Identifiant du terme 1 : %s\n", commande->elt_commande[3]); */
/*   fprintf(stderr, "Identifiant du terme 2 : %s\n", commande->elt_commande[4]); */
/*   fprintf(stderr, "Type de lien (int) : %s\n", commande->elt_commande[5]); */


  if ((indFichierTmp = fopen(FICHIERVALIDATION, "a+")) == NULL) {
    fprintf(stderr,"Erreur lors de l'ouverture du fichier temporaire contenant les validations\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
/*     return(ERR_FICHIER_NOEXIST); */
  }

  ValeurValidation = atoi(commande->elt_commande[1]);

  idTerme1 = atoi(commande->elt_commande[3]);
  idTerme2 = atoi(commande->elt_commande[4]);
  typeL = atoi(commande->elt_commande[5]);

  AjoutTypeLien(commande->elt_commande[2], typeL);

  fprintf(indFichierTmp, "Validation\n%d\n%s\n%d\n%d\n%d\n", ValeurValidation, commande->elt_commande[2], idTerme1, idTerme2, typeL);
  fclose(indFichierTmp);

  if (Modif_Val(ValeurValidation, commande->elt_commande[2], idTerme1, idTerme2, typeL) == ERR_RENVOI_STD) {
    fprintf(stderr,"Erreur lors de la modification du type du lien entre le terme %s et le terme %s\n", commande->elt_commande[3], commande->elt_commande[4]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
  }
  return(EXEC_OK);
}


/************************************************************************
 *                                                                      *
 * Objectif : Extraction des liens initiaux a partir d'une ressource    *
 *            existante
 *                                                                      *
 * Argument : 1. Le flux de lecture                                     *
 *            2. Le flux d'ecriture                                     *
 *            3. La commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Syn_ExtractLiensInitiaux(int flux_lect, int flux_ecrit,struct Commande *commande) {
  /* Dans commande : 
     champ 1 : la commande
     champ 2 : le nom du fichier contenant les liens entre termes complexes
     champ 3 : le nom du fichier faisant office de corpus
     champ 4 : le nom du fichier contenant les liens extraits
  */

  int indFichierLstSynC;
  int indFichierLstCT;
  int indFichierLiensExtraits;


/*   fprintf(stderr, "--> %s\n", commande->elt_commande[1]); */
/*   fprintf(stderr, "--> %s\n", commande->elt_commande[2]); */

   if ((indFichierLstSynC = get_table(commande->elt_commande[1]))== ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

   if ((indFichierLstCT = get_table(commande->elt_commande[2]))== ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[2]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

   Historique[Hcur++] = indFichierLstSynC;
   Historique[Hcur++] = indFichierLstCT;
   /* Envoi du nom du fichier resultat au script Perl (l'interface)
    */
   print_com_msg(ENVOI_FICHIER_RESULTAT, flux_ecrit, OUI);
   print_com_msg(liste_fichier[indFichierLstCT].nom, flux_ecrit, NON);
   print_com_msg(SEP_EXT_STRING, flux_ecrit, OUI);
   print_com_msg(EXT_GNM, flux_ecrit, OUI);

   /* Extraction des liens initiaux
    */
   if (ExtractLiensInit(indFichierLstSynC, indFichierLstCT, commande->elt_commande[3], LIEN_EXTRAIT, liste_fichier[indFichierLstCT].nbrgnmax,-1, flux_ecrit)) {
    fprintf(stderr,"Erreur lors de l'extraction de liens initiaux: %s, %s et %s\n", commande->elt_commande[1], commande->elt_commande[2], commande->elt_commande[3]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);     
   }
   return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Impression des resultats dans differents formats          *
 *                                                                      *
 * Argument : 1. Le flux de lecture                                     *
 *            2. Le flux d'ecriture                                     *
 *            3. La commande                                            *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Sortie(int flux_lect, int flux_ecrit,struct Commande *commande)
{
  /*
     champ 1 : La commande
     champ 2 : le nom du fichier a lire (contient les resultats d'une inference
     champ 3 : le nom du fichier de sortie
     champ 4 : Le debut de la ligne de commande (contient deja un espace a la fin)
   */

  int vers_fils[2];                   /* descripteurs du pipe "pere->fils" */
  int vers_pere[2];                   /* descripteurs du pipe "fils->pere" */
  int pid;
  struct Commande commande_Load_Mat;
  char *ligne_commande;

  if ((ligne_commande = (char *)malloc(sizeof(char) * (strlen(commande->elt_commande[NB_ELT_SORTIE - 1]) + strlen(commande->elt_commande[1]) + 1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(ligne_commande, "%s%s", commande->elt_commande[NB_ELT_SORTIE - 1], commande->elt_commande[1]);

  /********************************************************************
           Creation des tubes pour la communication avec l'interface
  ********************************************************************/
  /* Tube vers le fils (l'interface en Perl/Tk) 
   */
  pipe ( vers_fils );
  /* Tube vers le pere 
   */
  pipe ( vers_pere );
  /**********************************************************************
       Fin de la creation des tubes pour la communication avec l'interface
  ***********************************************************************/
  switch(pid = fork ()) {
  case -1:
    /*
      Probleme de creation de processus
    */
    fprintf ( stderr,"Couldn't fork process\n");
    exit(ERR_FORK);
  case 0: 
    /* On est dans le fils   
       On lance l'interface
    */
    lancementInterface(ligne_commande, vers_fils, vers_pere);
      break;
  default:
    /* On est dans le pere 
     */
    InitialisationConnexion(vers_fils, vers_pere);

    Cree_commande(&commande_Load_Mat, NB_ELT_LOAD_DB, NB_ELT_LOAD_DB, "Load_Mat",commande->elt_commande[1]);
    Commande_Load_Mat(&commande_Load_Mat, flux_ecrit);
    Free_commande(&commande_Load_Mat);
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Validation des resultats à partir d'un fichier            *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_Validation_Fichier(struct Commande *commande, int flux_ecriture)
{
  /*
     champ 1 : la commande
     champ 2 : le nom du fichier contenant les resultats d'une precedente inference
     champ 3 : le nom du fichier contenant les relations validees
     champ 4 : Le debut de la ligne de commande (contient deja un espace a la fin)
  */
  int vers_fils[2];                   /* descripteurs du pipe "pere->fils" */
  int vers_pere[2];                   /* descripteurs du pipe "fils->pere" */
  int pid;
  struct Commande commande_suppl;
  char *ligne_commande;
  char *pofile;

  if ((ligne_commande = (char *)malloc(sizeof(char) * (strlen(commande->elt_commande[3]) + strlen(commande->elt_commande[2]) + 1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(ligne_commande, "%s%s", commande->elt_commande[3], commande->elt_commande[2]);
  fprintf(stderr, "ligne de commande : %s%s\n", commande->elt_commande[3], commande->elt_commande[2]);

  /********************************************************************
           Creation des tubes pour la communication avec l'interface
  ********************************************************************/
  /* Tube vers le fils (l'interface en Perl/Tk) 
   */
  pipe ( vers_fils );
  /* Tube vers le pere 
   */
  pipe ( vers_pere );
  /**********************************************************************
       Fin de la creation des tubes pour la communication avec l'interface
  ***********************************************************************/
  switch(pid = fork ()) {
  case -1:
    /*
      Probleme de creation de processus
    */
    fprintf ( stderr,"Couldn't fork process\n");
    exit(ERR_FORK);
  case 0: 
    /* On est dans le fils   
       On lance l'interface
    */
    lancementInterface(ligne_commande, vers_fils, vers_pere);
      break;
  default:
    /* On est dans le pere 
     */
    InitialisationConnexion(vers_fils, vers_pere);

    Cree_commande(&commande_suppl, NB_ELT_LOAD_DB, NB_ELT_LOAD_DB, "Load_Mat",commande->elt_commande[1]);
    Commande_Load_Mat(&commande_suppl, flux_ecriture);

    /**********************************************************************
     Recuperation des messages envoyes par le script Perl
     **********************************************************************/

    do {
      Free_commande(&commande_suppl);
      Lecture_Commande(STDIN_FILENO, &commande_suppl);
    } while(strcmp(commande_suppl.elt_commande[0], "DebutVal") != 0);
    Free_commande(&commande_suppl);
    while ((Lecture_Commande(STDIN_FILENO, &commande_suppl) == EXEC_OK) && (strcmp(commande_suppl.elt_commande[0], "FinVal")!=0) ) {
      Commande_Validation(&commande_suppl);
    }
    printlog("Enregistrement des liens inferes\n");
    pofile = commande->elt_commande[1] + strlen(commande->elt_commande[1]) - strlen(EXT_GNM) - sizeof(char);
    if ((pofile[0] == SEP_EXT) && (strcmp(pofile+1,EXT_GNM) == 0)) {
      pofile[0] = 0;
    }
    if (EnregLiensInf(commande->elt_commande[1]) == ERR_RENVOI_STD) {
      fprintf(stderr,"Erreur lors de l'enregistrement des liens : %s et %s\n", commande->elt_commande[1], commande->elt_commande[2]);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_TRAITMT);
    }
    printlog("Fin de l'enregistrement des liens inferes\n");
    
    DestructionMat();

    /**********************************************************************
     Fin de la recuperation des messages envoyes par le script Perl
     **********************************************************************/

  }
  return(EXEC_OK);

}

/************************************************************************
 *                                                                      *
 * Objectif : Inference a partir d'une ressource supplementaire         *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Commande_InferenceCTSyn_Suppl(struct Commande *commande, int flux_ecrit)
{
  /*
     champ 1 : la commande
     champ 2 : le nom du fichier contenant les synonymes
     champ 3 : le nom du fichier contenant les resultats d'une precedente inference
     champ 4 : le nom du fichier temporaire qui contiendra les resultats
  */

  struct Commande commande_suppl;
  int indFichierLstCT;
  int indFichierLstSyn;
  char *fichier_resultat; /* XXX Temporaire pour des raisons de compatibilites sur EnregLiensInf */

  fprintf(stderr, "Results in %s\n", commande->elt_commande[3]);

  /*     InitialisationConnexion(vers_fils, vers_pere); */ /* A decommenter lorsque les modifications sur ChargLienInf sera faite */

  Cree_commande(&commande_suppl, NB_ELT_LOAD_DB, NB_ELT_LOAD_DB, "Load_Mat",commande->elt_commande[2]);
  Commande_Load_Mat(&commande_suppl, flux_ecrit);
  Free_commande(&commande_suppl);
  Charge_Fichier_option(commande->elt_commande[1]);

  if ((indFichierLstCT = get_table_type(CORPUS)) == ERR_RENVOI_STD) {
    fprintf(stderr,"Fichier %s non charge\n", commande->elt_commande[1]);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_FICHIER_NOEXIST);
  }

  if (commande->elt_commande[3] != NULL) {
    if ((fichier_resultat = (char *)malloc(sizeof(char) * (strlen(commande->elt_commande[3]) + 1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    strcpy(fichier_resultat,commande->elt_commande[3]);
    fichier_resultat[strlen(fichier_resultat) - strlen(EXT_GNM) - sizeof(char)] = 0;
  } else {
    if ((fichier_resultat = (char *)malloc(sizeof(char) * (strlen(commande->elt_commande[2]) + 1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    strcpy(fichier_resultat,commande->elt_commande[2]);
    fprintf(stderr, "Results in %s\n", fichier_resultat);
/*     fichier_resultat[strlen(fichier_resultat) - strlen(EXT_GNM) - sizeof(char)] = 0; */
  }
  Cree_commande(&commande_suppl, NB_ELT_INFERENCE, NB_ELT_INFERENCE, "InferenceCTSyn", commande->elt_commande[1], liste_fichier[indFichierLstCT].nom, fichier_resultat);
  Commande_Syn_Inference(&commande_suppl, flux_ecrit);
  DestructionMat();
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Classes2Corpus()
{
/* 	      case FONC_CLASSES2CORPUS : */
/* 		/\* Passage des classes de synonymes sous forme d'un corpus  */
/* 		   au format de SynoTerm */
/* 		 *\/ */
/* 		writelogI((options & FTYPOPAUVRE) == FTYPOPAUVRE); */
/* 		writelogI((options & ISO) == ISO); */
/* 		writelogS("Chargement des classes au format du corpus"); */
/* 		/\* On recupere la base et l'extension du fichier */
/* 		 *\/ */
/* 		recup_nom_ext(fichierE1,&NomBase,&ExtBase); */
/* 		/\* on va effectuer la transformation dans l'appel de fonction */
/* 		 *\/ */
/* 		Classes2Corpus(NomBase, (options & FTYPOPAUVRE) == FTYPOPAUVRE, (options & ISO) == ISO); */
/* 		writelogS("Fin Chargement des classes au format du corpus"); */
/* 		break; */

}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Extration_Liens_initiaux()
{
/* 	      case FONC_EXTRACTIONLINIT : */
/* 		/\* Extraction des relations initiales */
/* 		 *\/ */
/* 		writelogS("Extraction de liens initiaux"); */
/* 		/\* On recupere la base et l'extension du fichier 1 */
/* 		 *\/ */
/* 		recup_nom_ext(fichierE1,&NomBase,&ExtBase); */
/* 		writelogS(NomBase); */
/* 		/\* Chargement du fichier 1 dans la table des fichiers ouverts */
/* 		 *\/ */
/* 		charger_db(fichierE1,NomBase,NULL); */
/* 		/\* Recuperation de l'identifiant du fichier 1 dans la table */
/* 		 *\/ */
/* 		Fich1 = get_table(NomBase); */
/* 		writelogS(fichierE2); */
/* 		/\* On recupere la base et l'extension du fichier 2 */
/* 		 *\/ */
/* 		recup_nom_ext(fichierE2,&NomBase,&ExtBase); */
/* 		/\* Chargement du fichier 1 dans la table des fichiers ouverts */
/* 		 *\/ */
/* 		charger_db(fichierE2,NomBase,NULL); */
/* 		writelogS(NomBase); */
/* 		writelogI(Fich1); */
		
/* 		if (Fich1!=-1) { */
/* 		  /\* On verifie que le fichier 1 est bien dans la table */
/* 		   *\/ */
/* 		  /\* */
/* 		   * Voir remarque ci-dessus */
/* 		   *\/ */
/* 		  /\* Recuperation de l'identifiant du fichier 1 dans la table */
/* 		   *\/ */
/* 		  Fich2 = get_table(NomBase); */
/* 		  writelogI(Fich2); */
/* 		  if (Fich2!=-1){ */
/* 		    /\* On verifie que le fichier 2 est bien dans la table */
/* 		     *\/ */
/* 		    /\* On rajoute les fichiers 1 et 2 (enfin leur identifiant)  */
/* 		       dans l'historique  */
/* 		     *\/ */
/* 		    Historique[Hcur++] = Fich1; */
/* 		    Historique[Hcur++] = Fich2; */
/* 		    if (options & SORTIEFICHIER) { */
/* 		      /\* Un fichier de sortie a ete precise */
/* 		         On commence par envoyer toutes les informations */
/* 			 et les relations sont inferees dans le puits  */
/* 			 sans fond (Monsieur Dave Null) */
/* 		      *\/ */
/* 		      FLUXCOM = fopen("/dev/null","a+"); */
/* 		      /\* Extraction des relations initiales */
/* 		       *\/ */
/* 		      ExtractLiensInit(Fich1, Fich2, NomFich, LIEN_EXTRAIT, liste_fichier[Fich2].nbrgnmax,-1); */
/* 		      /\* On detruit la matrice */
/* 		       *\/ */
/* 		      DestructionMat(); */
/* 		    } else { */
/* 		      /\* Pas de fichier precise, on envoie tout sur la sortie  */
/* 			 standard et dans un fichier de meme nom que le corpus  */
/* 			 avec l'extension res.db */
/* 		      *\/ */
/* 		      writelogS("FLUXCOM = stdout"); */
/* 		      FLUXCOM = stdout; */
/* 		      strcpy(NomFich,fichierE2); */
/* 		      /\* Extraction des relations initiales */
/* 		       *\/ */
/* 		      ExtractLiensInit(Fich1, Fich2, NomFich, LIEN_EXTRAIT, liste_fichier[Fich2].nbrgnmax,-1); */
/* 		      /\* On detruit la matrice */
/* 		       *\/ */
/* 		      DestructionMat(); */
/* 		    } */
/* 		  } */
/* 		} */
/* 		writelogS("Fin Extraction des liens initiaux"); */
/* 		break; */

}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification :   /  /                               *
 *                                                                      *
 ************************************************************************/

int Commandes_Diversite_Shannon()
{
/* 	      case FONC_DIVERSITE_SHANNON : */
/* 		/\* Calcul de la diversite de Shannon pour chaque terme */
/* 		 *\/ */
/* 		writelogS("Chargement d'un fichier resultat en memoire"); */
/* 		/\* Chargement des fichiers deja inferes  */
/* 		 *\/ */
/* 		if (options & SORTIEFICHIER) { */
/* 		  /\* Un fichier de sortie a ete precise */
/* 		     On va donc rediriger tous les informations  */
/* 		     et resultats dedans */
/* 		  *\/ */
/* 		  /\* On commence par envoyer ce qui avait ete infere precedemment */
/* 		     dans le puits sans fond (Monsieur Dave Null) */
/* 		   *\/ */
/* 		  FLUXCOM = fopen("/dev/null","a+"); */
/* 		  /\* On charge les liens qui ont ete infere precedemment */
/* 		   *\/ */
/* 		  ChargLiensInf_anc(fichierE1,-1,NULL); */
/* 		  writelogS("Fin du chargement d'un fichier resultat en memoire"); */
/* 		  writelogS("Diversite de Shannon"); */
/* 		  /\* Calcul de la diversite de Shannon */
/* 		   *\/ */
/* 		  Calcul_Diversite(0); */
/* 		  /\* Affichage de la diversite de Shannon dans le fichier 'test_diversite.txt' */
/* 		   *\/ */
/* 		  Affiche_Diversite("test_diversite.txt"); */
/* 		  writelogS("Classif_Div_Shannon"); */
/* 		  /\* Classement des termes en fonction de la diversite de Shannon */
/* 		   *\/ */
/* 		  Classif_Div_Shannon(); */
/* 		  writelogS("Affiche_Classif_Div_Shannon"); */
/* 		  /\* Affichage des termes classes selon la diversite */
/* 		     de Shannon dans le fichier 'test_diversite.txt'  */
/* 		   *\/ */
/* 		  Affiche_Classif_Div_Shannon("test_diversite.txt"); */
/* 		  /\* On detruit la matrice */
/* 		   *\/ */
/* 		  DestructionMat(); */
/* 		} else { */
/* 		  /\* Pas de fichier precise, on envoie tout sur la sortie  */
/* 		     standard et dans un fichier de meme nom que le corpus  */
/* 		     avec l'extension res.db */
/* 		   *\/ */
/* 		  FLUXCOM = stdout; */
/* 		  strcpy(NomFich,fichierE1); */
/* 		  /\* On charge les liens qui ont ete infere precedemment */
/* 		   *\/ */
/* 		  ChargLiensInf_anc(fichierE1,-1,NULL); */
/* 		  writelogS("Diversite de Shannon"); */
/* 		  /\* Calcul de la diversite de Shannon */
/* 		   *\/ */
/* 		  Calcul_Diversite(0); */
/* 		  /\* Affichage de la diversite de Shannon dans le fichier 'test_diversite.txt' */
/* 		   *\/ */
/* 		  Affiche_Diversite("test_diversite.txt"); */
/* 		  writelogS("Classif_Div_Shannon"); */
/* 		  /\* Classement des termes en fonction de la diversite de Shannon */
/* 		   *\/ */
/* 		  Classif_Div_Shannon(); */
/* 		  writelogS("Affiche_Classif_Div_Shannon"); */
/* 		  /\* Affichage des termes classes selon la diversite */
/* 		     de Shannon dans le fichier 'test_diversite.txt'  */
/* 		   *\/ */
/* 		  Affiche_Classif_Div_Shannon("test_diversite.txt"); */
/* 		  /\* On detruit la matrice */
/* 		   *\/ */
/* 		  DestructionMat(); */
/* 		} */
/* 		break; */

}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'une liste de termes validés ou attestés      *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le flux d'ecriture                                     *
 *                                                                      *
 * Retour : l'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 19/05/2006                           *
 *                                                                      *
 ************************************************************************/


int Commande_load_term_list(struct Commande *commande, int flux_ecrit) {
  /* Dans commande : 
          champ 1 : la commande
	  champ 2 : le nom du fichier à charger
  */
  printlog("Liste de termes %s\n", commande->elt_commande[1]);

} 

/************************************************************************
 *                                                                      *
 * Objectif :  Ajout d'un element a une commande a un indice            *
 *             Permet aussi des modifications de valeurs                *
 *                                                                      *
 * Argument :  1. La commande                                           *
 *                                                                      *
 *             2. L'indice de l'element                                 *
 *                                                                      *
 *             3. La valeur de l'element                                *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 * Date de derniere modification : 27/12/2003                           *
 *                                                                      *
 ************************************************************************/


int Ajout_Elt_Commande(struct Commande *commande, int ind, char *val_elt)
{
  if (commande->elt_commande[ind] != NULL) {
    free(commande->elt_commande[ind]);
  }
  if ((commande->elt_commande[ind] = (char *)malloc(sizeof(char)*(strlen(val_elt)+1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
  }
  sprintf(commande->elt_commande[ind],val_elt);
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Libèration de la mémoire allouée à une commande           *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 * Retour : L'etat de l'execution de la commande                        *
 *                                                                      *
 * Date de derniere modification : 31/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Free_commande(struct Commande *commande)
{
  int i;

  for(i=0;i< commande->Nb_elt_commande;i++) {
    if (commande->elt_commande[i] != NULL) {
      free(commande->elt_commande[i]);
    }
  }
  if (commande->Nb_elt_commande != 0) {
    free(commande->elt_commande);
  }
  commande->Nb_elt_commande = 0;
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Chargement d'un fichier avec modification du nom pour la  *
 *            normalisation                                             *
 *                                                                      *
 * Argument : 1. Nom du fichier a charger                               *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Charge_Fichier_option(char *fichier)
{
  struct Commande commande_suppl; 
  struct Donnees_Nom_Fichier *donnees_nom_fichier; /* Ensemble des informations sur le nom d'un fichier */

  Cree_commande(&commande_suppl, NB_ELT_LOAD_DB, NB_ELT_LOAD_DB, "Load_db", fichier);
  Commande_Load_db(&commande_suppl);

  Free_commande(&commande_suppl);

  if ((donnees_nom_fichier = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  if (Analyse_Nom_Fichier(fichier, donnees_nom_fichier) != EXEC_OK) {
    fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }
  sprintf(fichier,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname);
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif :  Creation d'une commande a partir des arguments           *
 *                                                                      *
 * Argument : 1. La commande                                            *
 *                                                                      *
 *            2. Le nombre d'elements theorique de la commande          *
 *                                                                      *
 *            3. Le nombre d'elements effectivement placés              *
 *                                                                      *
 *            4. La liste des elements a placer (arguments variables)   *
 *                                                                      *
 * Retour :  L'etat de l'execution de la fonction                       *
 *                                                                      *
 * Date de derniere modification : 27/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Cree_commande(struct Commande *commande, int nb_elt, int nb_val, ...) 
{
  va_list p_arg;
  int i;
  char *s;

  va_start  (p_arg, nb_val);
  

  commande->Nb_elt_commande = nb_elt;
  if ((commande->elt_commande = (Elt_commande *)malloc(sizeof(Elt_commande)*commande->Nb_elt_commande))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }

  for(i=0;i< nb_val;i++) {
    s = va_arg (p_arg, char *);
    if (s != NULL) {
      if ((commande->elt_commande[i] = (char *)malloc(sizeof(char)*(strlen(s)+1)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      sprintf(commande->elt_commande[i],s);
    } else {
      commande->elt_commande[i] = NULL;
    }

  }
  for(i=nb_val;i<nb_elt;i++) {
    commande->elt_commande[i]=NULL;
  }

  va_end (p_arg);
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Traitement des commandes en mode interface                *
 *            **** Nouvelle version, en cours de developpement ****     *
 *                                                                      *
 * Arguments : 1. Flux de lecture                                       *
 *                                                                      *
 *             2. Flux d'ecriture                                       *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 29/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commandes(int flux_lect, int flux_ecrit  ) 
{

  struct Commande commande; /* commande lue sur le flux de communication */

  #ifdef DEBUG
    fprintf(stderr, "\n**** fonction Commandes developpement \n\n");
  #endif


  Initialisation_Commandes(flux_lect, flux_ecrit, &commande);

  printlog("Fin Initialisation Commandes\n");

  
  commande.Nb_elt_commande = 0;
  do {
    Free_commande(&commande);

    Lecture_Commande(flux_lect, &commande);

    Commandes_Select(flux_lect, flux_ecrit, &commande);

  } while((Convert_Commande(commande.elt_commande[0]) != QUIT)&&(Convert_Commande(commande.elt_commande[0]) != QUIT_NOENR));
    
}

/************************************************************************
 *                                                                      *
 * Objectif : Selection  des commandes                                  *
 *            **** Nouvelle version, en cours de developpement ****     *
 *                                                                      *
 * Arguments : 1. Flux de lecture                                       *
 *                                                                      *
 *             2. Flux d'ecriture                                       *
 *                                                                      *
 *             3. Commande                                              *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 29/09/2003                           *
 *                                                                      *
 ************************************************************************/

int Commandes_Select(int flux_lect, int flux_ecrit, struct Commande *commande) 
{
  printlog("Dans Commandes_Select\n");
  switch (Convert_Commande(commande->elt_commande[0])) {
  case L0AD_DB: Commande_Load_db(commande);                          /* VERIFIER */
    break;
  case LOAD_DESAMB: Commande_Load_desamb(commande);                  /* VERIFIER */
    break;
  case LOAD_CLS2CORPUS: Commande_Load_cls2corpus(commande);
    break;
  case LOAD_SYN: Commande_Load_syn(commande);                        /* VERIFIER */
    break;
  case LOAD_DIC: Commande_Load_dic(commande);                        /* VERIFIER */
    break;
  case INFO_ALL: info_table(flux_ecrit,ALL);                          /* VERIFIER */
    break;
  case INFO_SYN: info_table(flux_ecrit,SYN);                          /* VERIFIER */
    break;
  case INFO_DESAMB: info_table(flux_ecrit,CORPUS);                    /* VERIFIER */
    break;
  case INFO_TERMLIST: info_table(flux_ecrit,TERMLIST);                /* VERIFIER */
    break;
  case INFO_NEW: info_table(flux_ecrit,NEW);                          /* VERIFIER */
    break;
  case SYN_ELIMDOUBL: Commande_Syn_ElimDoubl(commande);              /* VERIFIER */
    break;
  case SYN_ELIMDOUBLCR: Commande_Syn_ElimDoubl_Cr(commande);         /* VERIFIER */
    break;
  case SYN_FILTRAGE: Commande_Syn_Filtrage(commande);                /* VERIFIER */
    break;
  case SYN_INFERENCE: Commande_Syn_Inference(commande, flux_ecrit);  /* VERIFIER */
    break;
  case QUIT: Commande_Quit(commande);                                /* VERIFIER */
    break;
  case QUIT_NOENR: Commande_Quit_NoEnr(commande);                    /* VERIFIER */
    break;
  case SAVE_MAT: Commande_Save_Mat(commande);                        /* VERIFIER */
    break;
  case LOAD_MAT: Commande_Load_Mat(commande, flux_ecrit);            /* VERIFIER */
    break;
  case VALIDATION: Commande_Validation(commande);                    /* VERIFIER */
    break;
  case SYN_EXTRAC_RELINIT: Commande_Syn_ExtractLiensInitiaux(flux_lect, flux_ecrit,commande);
    break;
  case SORTIE: Commande_Sortie(flux_lect, flux_ecrit,commande);      /* VERIFIER */
    break;
  case VALIDATION_FICHIER: Commande_Validation_Fichier(commande, flux_ecrit);    
    break;
  case SYN_INFERENCE_SUPPL: Commande_InferenceCTSyn_Suppl(commande, flux_ecrit); 
    break;
  case LOAD_TERM_LIST: Commande_load_term_list(commande, flux_ecrit); 
    break;
  case ERR_COMMANDE: 
  default: 
    fprintf(stderr, "Erreur dans le protocole de communication: Commande inconnue\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    exit(ERR_COMMANDE);
  }
  printlog("Fin Commandes_Select\n");
  return(EXEC_OK);
}

