/* 	$Log: SynoTermM.c,v $
/* 	Revision 1.4  2006/11/30 17:28:51  ht
/* 	corrections
/* 	intégration partielle de la possiblité d'ajouter les réuslts extérieurs tels que
/* 	ceux de Faster (a continuer)
/* 	
/* 	Revision 1.3  2006/11/23 14:04:01  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.2  2006/05/19 11:01:13  ht
/* 	ajout d'évènement, correction de bug dans le makefile
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.5  2003/04/30 16:00:05  ht
 * 	Modification de la lecture des informations iso et typo
 *
 * 	Revision 1.4  2002/09/15 21:48:10  thierry
 * 	1. Augmentation de la taille des variables contenant les noms des
 * 	fichiers. Elle est fixé arbitrairement a 3000 afin de prendre en
 * 	compte le passage du chemin complet dans le nom du fichier.
 *
 * 	2. Modification de l'enregistrement des resultats.  le nom du fichier
 * 	devant contenir les resultats est passe sans l'extension
 *
 * 	Revision 1.3  2001/08/05 08:04:33  thierry
 * 	Commentaires des fonctions
 *
 * 	Revision 1.2  2001/05/01 19:43:22  thierry
 * 	*** empty log message ***
 *	 */

#include<error.h>
/**********************************************************************
 *          Inclusion des bibliotheques declarees localement          *
 **********************************************************************/

#include "SynoTerm_Gen.h"
#include "SynoTermM.h"

/************************************************************************
 *                                                                      *
 * Contenu : Graphe des candidats termes                                *
 *                                                                      *
 ************************************************************************/

struct DonneesSyntxq Lst_Termes;

/************************************************************************
 *                                                                      *
 * Contenu : Indication de la typographie du corpus                     *
 *                                                                      *
 ************************************************************************/

int typoC = 0;

/************************************************************************
 *                                                                      *
 * Contenu : Indication de la typographie du dictionnaire               *
 *                                                                      *
 ************************************************************************/

int typoD = 0;

/************************************************************************
 *                                                                      *
 * Contenu : Mode d'utilisation de SynoTerm                             *
 *                                                                      *
 ************************************************************************/

int Mode_SynoTerm = 0;


/************************************************************************
 *                                                                      *
 * Objectif : Fonction principale                                       *
 *                                                                      *
 * Arguments : 1. Nombre d'agument du programme en ligne de commande    *
 *                                                                      *
 *            2. Liste de argument du programme en ligne de commande    * 
 *               (Fonction de SynoTerm, Options de SynoTerm,            *
 *                Fichiers de donnees, etc.)                            *
 *                                                                      *
 * Retour :                                                             *
 *                                                                      *
 * Date de derniere modification : 30/12/2003                           *
 *                                                                      *
 ************************************************************************/

main(int argc, char **argv) {

  int vers_fils[2];                   /* descripteurs du pipe "pere->fils" */
  int vers_pere[2];                   /* descripteurs du pipe "fils->pere" */
  int pid;                            /* Contient le pid du processus fils cree par fork */
  int options = 0;                    /* Contient les options passees en argument */
  struct Commande *commande;

  init_SynoTerm();

  /************************************************************************/
  /* Affichage d'informations sur SynoTerm, au lancement de l'application
   */
  print_Info_SynoTerm();

  /************************************************************************/

  /********************************************************************
                        Determination du fichier de log 
   ********************************************************************/
  Generation_fichier_log();
  
  /************************************************************************/
  
  /* Le graphe des candidats termes n'est pas encore contruit 
   */
  CG=0;

  /********************************************************************
                            Gestion des options
  ********************************************************************/

  if ((commande = Gestion_options(argc, argv,&options)) < (struct Commande *)NULL) {
    exit((int) commande);
  }


/* 	  case 'E' :  */
/* 	    /\* Extraction des relations initiales */
/* 	     *\/ */
/* 	    fonction = FONC_EXTRACTIONLINIT; */
/* 	    break; */
/* 	  case 'n' : */
/* 	    /\* Calcul de la diversite de Shannon sur les candidats termes */
/* 	       fonction = FONC_DIVERSITE_SHANNON ; */
/* 	       break; */
/* 	       default : */
/* 	       /\* Fonction inconnue. Probleme. Donc aucune fonction ... */
/* 	    *\/ */
/* 	    fonction = 0; */
/* 	  } */
  /********************************************************************
                            Fin de la gestion des options
  *********************************************************************/


  if (!(options & NOINTERFACE)) {
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
      lancementInterface("", vers_fils, vers_pere);
      break;
    default:
      /* On est dans le pere 
       */
      InitialisationConnexion(vers_fils, vers_pere);

      Commandes(STDIN_FILENO, STDOUT_FILENO);
    }
  } else {
    /* 
       SynoTerm est lance sans son interface
    */
    Commandes_Select(STDIN_FILENO, STDOUT_FILENO, commande);
  }
}

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


int Affiche_Journal_De_Bord()
{
/*       if (options & DISPLAYLOG) { */
/* 	if ((pid2 = fork ()) == 0 ) { */
/* 	/\* Affichage du journal des evenements si necessaire */
/* 	 *\/ */
/* 	writelogS("Lancement de l'affichage des log"); */
/* 	sprintf(cmd,"xterm -geometry -0-25 -e tail -f %s",nomlog); */
/* 	system(cmd); */
/*       } */

}

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

int lancementInterface(char *arg, int *vers_fils, int *vers_pere) 
{
  char *cmd;    /* Contient la ligne de commande */

  /* Redirection de stdin 
   */
  close ( 0 );			
  dup ( vers_fils[0] );
  /* Redirection de stdout
   */
  close ( 1 );			
  dup ( vers_pere[1] );
  
  /* Fermeture de pipes    
   */
  close ( vers_fils[0] );
  close ( vers_pere[1] );
  /* Creation de la ligne de commande
   */
  if ((cmd = (char *)malloc(sizeof(char)*(strlen(NOM_SYNOTERM_PERL) + strlen(arg) + 2 )))==NULL) {
      fprintf(stderr,"Memory allocation error\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
  sprintf(cmd, "%s %s",NOM_SYNOTERM_PERL, arg);

  fprintf(stderr,"-> Running the user interface %s\n",cmd);
  /* Execution de la ligne de commande
     Lancement de l'interface de validation
   */
  system( cmd); 

}

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

int InitialisationConnexion(int *vers_fils, int *vers_pere)
{
  /* Redirection de stdin 
   */
  close ( 0 );	
  dup ( vers_pere[0] );
  /* Redirection de stdout  
   */
  close ( 1 );	
  dup ( vers_fils[1] );
  
  /* Pas de sortie bufferisee 
   */
  setbuf ( stdout, NULL );        

  /* fermeture de pipes 
   */
  close ( vers_fils[1] );
  close ( vers_pere[0] );
}

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
 *             4. Information sur le fichier chargé                     *
 *                                                                      *
 ************************************************************************/

/* XXX A REVOIR */

int Charger_desamb(char *NomFich, int typo, int iso, struct Donnees_Nom_Fichier *info_fichier) 
{
  char *tmpbase;        /* Nom du fichier (base, sans l'extension) */
  char *ExtBase;        /* Extension du fichier */
  char tmp[3000];
  char cmd[3000];
  FILE *Ftmp;

  /* Enregistrement des actions dans le journal des evenements
   */
  printlog("Chargement d'un fichier .desamb");

  /* Recuperation de l'extension et de la base du nom du fichier */
  recup_nom_ext(NomFich,&tmpbase,&ExtBase);
  /* Creation du nom du fichier contenant le fichier au format de
     SynoTerm
   */
  strcpy(tmp,tmpbase);
  strcat(tmp,".db");

  /* Transformation du fichier au format de SynoTerm
   */
  if (Charge_desamb(NomFich,tmp, typo, iso) != EXEC_OK) {
      fprintf(stderr,"Erreur lors du chargement du fichier %s/%s\n", info_fichier->dirname,info_fichier->fichname);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_LECTURE);
  }

  /* Chargement du fichier dans la table des fichiers ouverts dans SynoTerm
   */
  if (charger_db(tmp, tmpbase,info_fichier) != EXEC_OK) {
      fprintf(stderr,"Erreur lors du chargement (db) du fichier %s/%s\n", info_fichier->dirname,info_fichier->fichname);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_LECTURE);
  }
  printlog("Fin Chargement d'un fichier .desamb");
}

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

int Charger_dicosyn(char *NomFich, int typo, int iso) 
{
  char *NomBase;     /* Nom du fichier (base, sans l'extension) */
  char *ExtBase;     /* Extension du fichier */

  /* Ecriture dans le journal des evenements
   */
  printlog("%d",typo);
  printlog("%d",iso);
  printlog("Chargement du dictionnaire INALF7");
  /* Recuperation de l'extension et de la base du nom du fichier
   */
  recup_nom_ext(NomFich,&NomBase,&ExtBase);
  /* Traitement du fichier avec la fonction ecrit en flex
   */
  charge_Dico_INaLF7(NomFich,NomBase, typo, iso);

  printlog("Fin Chargement du dictionnaire INALF7");
}

/************************************************************************
 *                                                                      *
 * Objectif : Fonction de modification de la validation des relations   *
 *            inferees                                                  *
 *                                                                      *
 * Argument : Aucun                                                     *
 *                                                                      *
 ************************************************************************/

/* int ModifValidation() */
/* { */
/*     char tmp2[100];    /\* Resultat de la validation sous forme d'une chaine */
/* 		          de caracteres */
/* 		        *\/ */
/*     char typel[10];    /\* Type de la relation *\/ */
/*     FILE *Ftmp;        /\* Flux sur le fichier de sauvegarde de la validation */
/* 		          effectuee */
/* 		        *\/ */
/*     int nouvval;       /\* Valeur de la validation *\/ */
/*     int idTI1;         /\* Identifiant du premier terme *\/ */
/*     int idTI2;         /\* identifiant du second terme *\/ */

/*     printlog("Dans ModifValidation"); */

/*     /\* Ouverture du fichier de sauvegarde de la validation effectuee */
/*      *\/ */
/*     Ftmp = fopen("Validation.svgrd","a+"); */
/*     /\* Lecture du resultat de la validation E [-1,0,1] */
/*      *\/ */
/*     if (fgets(tmp2,5,stdin)!=NULL) { */
/*       /\* Transformation de la valeur en entier *\/ */
/*       nouvval=atoi(tmp2); */
/*       printlog("%s",nouvval); */
/*     } else { */
/*       /\* Probleme ... */
/*        *\/ */
/*       fprintf(stderr,"Erreur de lecture de la validation du lien"); */
/*     } */
  
/*     /\* Lecture du type de lien  */
/*      *\/ */
/*     if (fgets(typel,10,stdin)==NULL) { */
/*       /\* Probleme ... */
/*        *\/ */
/*       fprintf(stderr,"Erreur de lecture du type du lien"); */
/*     } */
/*     else { */
/*       /\* On termine la recuperation du type de la relation */
/*        *\/ */
/*       typel[strlen(typel)-1] = 0;  */
/*       printlog(typel); */
/*     } */
/*     /\* Lecture de l'identifiant du premier terme  */
/*      *\/ */
/*     if (fgets(tmp2,100,stdin)!=NULL) { */
/*       /\* Passage de l'identifiant en entier *\/ */
/*       idTI1=atoi(tmp2); */
/*     } else { */
/*       /\* Probleme ... */
/*        *\/ */
/*       fprintf(stderr,"Erreur de lecture de l'identifiant du terme 1"); */
/*     } */
    
/*     /\* Lecture de l'identifiant du deuxieme terme  */
/*      *\/ */
/*     if (fgets(tmp2,100,stdin)!=NULL) { */
/*       /\* Passage de l'identifiant en entier *\/ */
/*       idTI2=atoi(tmp2); */
/*     } else { */
/*       /\* Probleme ... */
/*        *\/ */
/*       fprintf(stderr,"Erreur de lecture de l'identifiant du terme 2"); */
/*     } */
/*     /\* Sauvegarde de la validation dans ... le fichier de sauvegarde */
/*      *\/ */
/*     fprintf(Ftmp,"Validation\n%d\n%s\n%d\n%d\n",nouvval,typel,idTI1,idTI2); */
/*     /\* Fermeture du fichier de sauvegarde */
/*      *\/ */
/*     fclose(Ftmp); */
/*     /\* Modification des informations de validation sur le lien  */
/*      *\/ */
/*     Modif_Val(nouvval, typel, idTI1, idTI2); */
/* } */




/* /\************************************************************************ */
/*  *                                                                      * */
/*  * Objectif : Liberation de la mémoire occupée par la structure         * */
/*  *            contenant un élément de la commande                       * */
/*  *                                                                      * */
/*  * Argument : 1. un élément de la commande                              * */
/*  *                                                                      * */
/*  * Retour : l'etat de l'execution de la fonction                        * */
/*  *                                                                      * */
/*  * Date de derniere modification : 29/09/2003                           * */
/*  *                                                                      * */
/*  ************************************************************************\/ */


/* int free_Elt_Commande(Elt_commande *elt_comm) { */
/*   free(elt_comm); */
/*   return(EXEC_OK); */
/* } */

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


int print_Info_SynoTerm(void) {

  int desc_fich_version;
  int i;
  char version[LONG_VERSION];
  
  if ((desc_fich_version=open(FICHIER_VERSION,O_RDONLY))==-1) {
    fprintf(stderr,"Erreur d'ouverture du fichier %s\n",FICHIER_VERSION);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_OUVERTURE);
  }
  i = 0;
  while((read(desc_fich_version, version+i,sizeof(char)) != 0) && (version[i] != '\n')){i++;};
  version[i] = 0;
  
  fprintf(stderr,"SynoTerm %s\n",version);
  fprintf(stderr,"Copyright (C) 1999 Thierry Hamon (LIPN)\n");
  fprintf(stderr,"thierry.hamon@lipn.univ-paris13.fr\n");
  return(EXEC_OK);
}

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

struct Commande *Gestion_options(int argc, char **argv, int *options) {
  char c;
  int option_index = 0;
  int index_commande = 0;
  struct Commande *commande;
  char *ofile = NULL; /* Contient le nom du fichier de sortie */
  char *typo;
  char *codage;

  /* 
     Liste des fonctions et options en format long 
  */
  static struct option long_options[] = {
    {"corpus", 1, 0, 'C'},
    {"dictionnaire", 1, 0, 'D'},
    {"filtrage", 1, 0, 'F'},
    {"inferenceSyn", 1, 0, 'I'},
    {"inferenceSyn2", 1, 0, 'A'},
    {"sortie", 1, 0, 'S'},
    {"validation", 1, 0, 'V'},
    {"extractionLien_init", 1, 0, 'E'},
    {"Lien_inferes", 1, 0, 'L'},
    {"log", 0, 0, 0},
    {"latin1", 0, 0, 'i'},
    {"iso8859-1", 0, 0, 'i'},
    {"help", 0, 0, 'h'},
    {"devel", 1, 0, 0},
    {0, 0, 0, 0}
  };

  *options = 0;                    /* Contient les options passees en argument */

  if ((commande = (struct Commande *)malloc(sizeof(struct Commande)))==NULL) {
    fprintf(stderr,"Memory allocation error\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return((struct Commande *)ERR_ALLOCATION);
  }
  commande->Nb_elt_commande=0;

  if ((typo = (char *)malloc(sizeof(char) * SZ_TYPO))==NULL) {
    fprintf(stderr,"Memory allocation error\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return((struct Commande *)ERR_ALLOCATION);
  }
  strcpy(typo,"0");
  if ((codage = (char *)malloc(sizeof(char) * SZ_CODAGE))==NULL) {
    fprintf(stderr,"Memory allocation error\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return((struct Commande *)ERR_ALLOCATION);
  }
  strcpy(codage,"0");

  if (argc == 1) {
    printlog("Pas d'argument\n");
    fprintf(stderr, "Running SynoTerm without parameter (user interface mode)\n");
  } 
  else {
    while (optind < argc) {
      if ((c = getopt_long (argc, argv, ":C:D:F:I:A:S:V:E:L:o:c:d:m:iht?",long_options, &option_index)) != -1) {
	switch (c) {
/* 	case 0: */
/* 	  printlog("option %s\n", long_options[option_index].name); */
/* 	  fprintf (stderr, "option %s", long_options[option_index].name); */
/* 	  if (optarg) { */
/* 	    printlog(" with arg %s \n", optarg); */
/* 	    fprintf (stderr," with arg %s", optarg); */
/* 	  } */
/* 	  fprintf(stderr,"\n"); */
/* 	  break; */

	case 'C':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    printlog("option chargement du corpus `%s'\n", optarg);
	    fprintf(stderr, "option chargement du corpus `%s'\n", optarg);
	    Cree_commande(commande,  NB_ELT_LOAD_CORPUS, NB_ELT_LOAD_CORPUS, "Load_desamb", optarg, typo, codage);
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'D':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    printlog("option chargement du dictionnaire`%s'\n", optarg);
	    fprintf(stderr, "option chargement du dictionnaire `%s'\n", optarg);
	    Cree_commande(commande, NB_ELT_LOAD_DICTIONNAIRE, NB_ELT_LOAD_DICTIONNAIRE, "Load_syn", optarg, typo, codage);
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'c':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    printlog("Elimination des doublons croisés dans le dictionnaire %s\n",optarg);
	    fprintf(stderr,"Elimination des doublons croisés dans le dictionnaire %s\n",optarg);
	    Cree_commande(commande, NB_ELT_ELIM_DOUBLONS_CR, NB_ELT_ELIM_DOUBLONS_CR, "SynElimDoublC", optarg, "1");
	    Charge_Fichier_option(commande->elt_commande[1]);
	  }  else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'd':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    printlog("Elimination des doublons dans le dictionnaire %s\n",optarg);
	    fprintf(stderr,"Elimination des doublons dans le dictionnaire %s\n",optarg);
	    Cree_commande(commande, NB_ELT_ELIM_DOUBLONS, NB_ELT_ELIM_DOUBLONS, "SynElimDoubl", optarg, "1");
	    Charge_Fichier_option(commande->elt_commande[1]);
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'F':
	  if ((commande->Nb_elt_commande == 0) && (optind< argc) && (argv[optind][0] != '-')) {
	    *options |= NOINTERFACE;
	    printlog("Filtrage des liens du dictionnaire  %s sur le corpus %s\n",optarg,argv[optind]);
	    fprintf(stderr,"Filtrage des liens du dictionnaire  %s sur le corpus %s\n",optarg,argv[optind]);
	    Cree_commande(commande, NB_ELT_FILTRAGE, NB_ELT_FILTRAGE, "FiltrageCTSyn", optarg, argv[optind]);
	    Charge_Fichier_option(commande->elt_commande[1]);
	    Charge_Fichier_option(commande->elt_commande[2]);
	    optind++;
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'I':
	  if ((commande->Nb_elt_commande == 0) && (optind< argc) && (argv[optind][0] != '-')) {
	    *options |= NOINTERFACE;
	    printlog("Inference a partir des liens du dictionnaire %s sur le corpus %s\n",optarg,argv[optind]);
	    fprintf(stderr,"Inference a partir des liens du dictionnaire %s sur le corpus %s\n",optarg,argv[optind]);
	    if (ofile != NULL) {
	      Cree_commande(commande, NB_ELT_INFERENCE, NB_ELT_INFERENCE, "InferenceCTSyn", optarg, argv[optind], ofile);
	    } else {
	      Cree_commande(commande, NB_ELT_INFERENCE, NB_ELT_INFERENCE - 1, "InferenceCTSyn", optarg, argv[optind]);
	    }
	    Charge_Fichier_option(commande->elt_commande[1]);
	    Charge_Fichier_option(commande->elt_commande[2]);
	    optind++;
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'A':
	  if ((commande->Nb_elt_commande == 0) && (optind< argc) && (argv[optind][0] != '-')) {
	    *options |= NOINTERFACE;
	    fprintf(stderr,"Inference a partir d'une ressource supplementaire (%s) sur le fichier `%s'\n", argv[optind], optarg);
	    printlog("Inference a partir d'une ressource supplementaire (%s) sur le fichier `%s'\n", argv[optind], optarg);
	    Cree_commande(commande, NB_ELT_INFERENCE_SUPPL, NB_ELT_INFERENCE_SUPPL, "InferenceCTSyn_Suppl", optarg, argv[optind], ofile);
	    optind++;
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'S':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    fprintf(stderr,"Production des fichiers de sortie a partir du fichier `%s'\n", optarg);
	    printlog("Production des fichiers de sortie a partir du fichier `%s'\n", optarg);
	    Cree_commande(commande, NB_ELT_SORTIE, NB_ELT_SORTIE - 1, "Sortie", optarg, ofile);

	    if ((commande->elt_commande[NB_ELT_SORTIE - 1] = (char *)malloc(sizeof(char)*(SZ_LIGNE_CMD)))==NULL) {
	      fprintf(stderr,"Memory allocation error\n");
	      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	      return((struct Commande *)ERR_ALLOCATION);
	    }
	    sprintf(commande->elt_commande[NB_ELT_SORTIE - 1], " %c ", OPTION_SORTIE);
	    while((c=getopt(argc,argv,":cflFx")) !=-1) {
	      switch (c) {
	      case 'c':
		fprintf(stderr,"Sortie classes\n");
		printlog("Sortie classes\n");
		strcat(commande->elt_commande[NB_ELT_SORTIE - 1], OPTION_SORTIE_CLASSE);
		break;
	      case 'f':
		fprintf(stderr,"Sortie familles\n");
		printlog("Sortie familles\n");
		strcat(commande->elt_commande[NB_ELT_SORTIE - 1], OPTION_SORTIE_FAMILLE);
		break;
	      case 'l':
		fprintf(stderr,"Sortie lemmes\n");
		printlog("Sortie lemmes\n");
		strcat(commande->elt_commande[NB_ELT_SORTIE - 1], OPTION_SORTIE_LEMME);
		break;
	      case 'F':
		fprintf(stderr,"Sortie formes fléchies\n");
		printlog("Sortie formes fléchies\n");
		strcat(commande->elt_commande[NB_ELT_SORTIE - 1], OPTION_SORTIE_FORME_FLECHIE);
		break;
	      case 'x':
		fprintf(stderr,"Sortie XML\n");
		printlog("Sortie XML\n");
		strcat(commande->elt_commande[NB_ELT_SORTIE - 1], OPTION_SORTIE_XML);
		break;
	      default:
		printf ("?? getopt returned character code 0%o (%c) ??\n", c,c);
		exit(-1);
	      }
	    }
	    fprintf(stderr, "ligne de commande %s\n", commande->elt_commande[NB_ELT_SORTIE - 1]);
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	case 'V':
	  if (commande->Nb_elt_commande == 0) {
	    *options |= NOINTERFACE;
	    fprintf(stderr,"Validation du fichier %s a partir du fichier resultat `%s'\n", optarg, argv[optind]);
	    printlog("Validation du fichier %s a partir du fichier resultat `%s'\n", optarg, argv[optind]);
	    Cree_commande(commande, NB_ELT_VALIDATION_FICHIER, NB_ELT_VALIDATION_FICHIER - 1, "Validation_Fichier", optarg, argv[optind]);
	    optind++;
	    if ((commande->elt_commande[NB_ELT_VALIDATION_FICHIER - 1] = (char *)malloc(sizeof(char)*(SZ_LIGNE_CMD)))==NULL) {
	      fprintf(stderr,"Memory allocation error\n");
	      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	      return((struct Commande *)ERR_ALLOCATION);
	    }
	    sprintf(commande->elt_commande[NB_ELT_VALIDATION_FICHIER - 1], " %c ", OPTION_VALIDATION_FICHIER);
	  } else {
	    help_SynoTerm(argc, argv);
	  }
	  break;
	  /*       case 'E': */
	  /* 	printf ("option E with value `%s %s'\n", optarg,argv[optind++]); */
	  /* 	break; */
	  /*       case 'L': */
	  /* 	printf ("option L with value `%s'\n", optarg); */
	  /* 	break; */
	case 'o':
	  if ((ofile = (char *)malloc(sizeof(char)*(strlen(optarg)+1)))==NULL) {
	    fprintf(stderr,"Memory allocation error\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return((struct Commande *)ERR_ALLOCATION);
	  }
	  sprintf(ofile, optarg);
	  if (commande->Nb_elt_commande != 0) {
	    if (strcmp(commande->elt_commande[0],"InferenceCTSyn") == 0) {
	      printlog("Specification d'un fichier de sortie : %s\n", optarg);
	      fprintf(stderr, "Specification d'un fichier de sortie : %s", optarg);
	      Ajout_Elt_Commande(commande, INDEX_OFILE_INFERENCE, ofile);
	    } 
	    if (strcmp(commande->elt_commande[0],"Sortie") == 0) {
	      printlog("Specification d'un fichier de sortie : %s\n", optarg);
	      fprintf(stderr, "Specification d'un fichier de sortie : %s", optarg);
	      Ajout_Elt_Commande(commande, INDEX_OFILE_SORTIE, ofile);
	    } 
	  }
	  *options |= SORTIEFICHIER ;
	  break;
/* 	case 'l': */
/* 	  printlog("Affichage du journal de bord\n"); */
/* 	  fprintf(stderr,"Affichage du journal de bord\n"); */
/* 	  *options |= DISPLAYLOG; */
/* 	  break; */
	case 'i':
	  sprintf(codage, "1");
	  printlog("Codage ISO-latin-1\n");
	  fprintf(stderr,"Codage ISO-latin-1\n");
	  if (commande->Nb_elt_commande != 0) {
	    if ((strcmp(commande->elt_commande[0],"Load_Syn") == 0) || (strcmp(commande->elt_commande[0],"Load_desamb") == 0)) {
	      printlog("Specification d'un fichier de sortie : %s", optarg);
	      fprintf(stderr, "Specification d'un fichier de sortie : %s", optarg);
	      Ajout_Elt_Commande(commande, INDEX_CODAGE, "1");
	    } 
	  }
	  *options |= ISO;
	  break;
	case 't':
	  printlog("Typographie pauvre\n");
	  fprintf(stderr,"Typographie pauvre\n");
	  sprintf(typo, "1");
	  if (commande->Nb_elt_commande != 0) {
	    if ((strcmp(commande->elt_commande[0],"Load_Syn") == 0) || (strcmp(commande->elt_commande[0],"Load_desamb") == 0)) {
	      printlog("Specification d'un fichier de sortie : %s", optarg);
	      fprintf(stderr, "Specification d'un fichier de sortie : %s", optarg);
	      Ajout_Elt_Commande(commande, INDEX_TYPO, "1");
	    } 
	  }
	  *options |= FTYPOPAUVRE;
	  break;
	case '?':
	  fprintf (stderr,"?? getopt returned character code 0%o (%c) ??\n", c,c);
	case 'h':
	  fprintf (stderr,"option help\n");
	  help_SynoTerm(argc, argv);
	  break;
	default:
	  printf ("?? getopt returned character code 0%o (%c) ??\n", c, c);
	  exit(-1);
	}
      }

      /* 	if ((argc == 3)&&(strcmp(argv[1],"--devel") == 0)) { */
      /* 	  fprintf(stderr, "Passage en mode developpement\n"); */
      /* 	  fprintf(stderr, "ATTENTION : certaines fonctions sont experimentales\n"); */
      /* 	  fprintf(stderr, "            Elles peuvent avoir un comportement instable\n"); */
      /* 	  fprintf(stderr, "            ou ne sont pas encore deboggees\n\n"); */
      /* 	  *options |= DEVEL; */
      /* 	  type_devel = atoi(argv[2]); */
	  
	  
      /* 	  /\* Traitements des fonctions *\/ */
      /* 	  switch (c) { */
      /* 	  case 'e' : */
      /* 	    /\* On passe des classes de synonymes sous forme d'un corpus */
      /* 	     *\/ */
      /* 	    fonction = FONC_CLASSES2CORPUS; */
      /* 	    break; */
      /* 	  case 'E' :  */
      /* 	    /\* Extraction des relations initiales */
      /* 	     *\/ */
      /* 	    fonction = FONC_EXTRACTIONLINIT; */
      /* 	    break; */
      /* 	  case 'n' : */
      /* 	    /\* Calcul de la diversite de Shannon sur les candidats termes */
      /* 	       fonction = FONC_DIVERSITE_SHANNON ; */
      /* 	       break; */
      /* 	       default : */
      /* 	       /\* Fonction inconnue. Probleme. Donc aucune fonction ... */
      /* 	    *\/ */
      /* 	    fonction = 0; */
      /* 	  } */
    }
    return(commande);
  }
}

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

int Generation_fichier_log(void)
{
  int i;

  /********************************************************************
                        Determination du fichier de log
   ********************************************************************/
  i = 1;
  do {
    sprintf(nomlog,"%s%d%c%s",BASE_LOG,i,SEP_EXT,EXT_LOG);
    i++;
    /* On a cree le nom de fichier potentiel,
       on verifie maintenant s'il existe
    */
  }
  while(!access(nomlog,F_OK));

  printlog("Fichier du journal de bord cree\n");

  /***********************************/
  /* Pour information, on affiche le fichier de log sur la sortie en erreur
   */
  fprintf(stderr,"-> fichier de log : %s\n",nomlog);
  return(EXEC_OK);
}

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

int init_SynoTerm(void) 
{

}

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


int help_SynoTerm(int argc, char **argv) 
{
  /* Fonction d'affichage de l'aide
     pas du tout a jour
     A revoir
  */
  fprintf(stderr,"Syntaxe : %s Liste_des_machines\n",argv[0]);
  fprintf(stderr,"Syntaxe : %s\n\t\t localhost par defaut\n",argv[0]);
  exit(-1);
}


/************************************************************************
 *                                                                      *
 * Objectif : Fonction d'utilisation    de SynoTerm                     *
 *                                                                      *
 * Argument : Rien                                                      *
 *                                                                      *
 * Retour : Etat de l'execution de la fonction                          *
 *                                                                      *
 * Date de derniere modification :  26/12/2003                          *
 *                                                                      *
 ************************************************************************/

int setMode_SynoTerm(int mode)
{
  Mode_SynoTerm = mode;
}

