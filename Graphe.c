#include "Graphe.h"

struct TableauTypeLien tab_typeLien;

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un nouveau noeud (en feuille au candidat terme    *
 *            courant                                                   *
 *                                                                      *
 * Argument : Le pere du noeud cree                                     *
 *                                                                      *
 * Retour : - Le pointeur sur le nouveau noeud                          *
 *                                                                      *
 *          - NULL si problème                                          *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

Noeud *ConstrCT(Noeud *Pere)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  Noeud *NConstr;           /* Nouveau noeud cree*/

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Allocation de la place memoire pour le nouveau noeud */
  if ((NConstr=(Noeud *)malloc(sizeof(Noeud))) == NULL) {
    fprintf(stderr,"Erreur d'allocation mémoire du noeud\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(NULL);
  }
  /* Le pere du noeud est ... le pere */
  NConstr->Pere=Pere;
  /* Initialisation */
  /* Nombre de fils : 0 */
  NConstr->nbfils=0;
  /* Le fils tete pointe sur rien */
  NConstr->fils[TETE]=NULL;
  /* Le fils expansion pointe sur rien */
  NConstr->fils[EXPANSION]=NULL;
  /* Pas encore de trait */
  NConstr->NbTraits=0;
  if (Pere!=NULL) {
    /* Si le pere existe, on ajoute le nouveau noeud dans la 
       liste des noeuds fils
       Et on incremente le nombre de fils du pere
     */
    Pere->fils[Pere->nbfils]=NConstr;
    Pere->nbfils++;
  }
  /* On renvoie le pointeur sur le nouveau noeud */
  return(NConstr);
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un trait au noeud courant                         *
 *                                                                      *
 * Arguments : 1. Le noeud auquel ajoute le trait                       *
 *                                                                      *
 *             2. Le trait a ajouter                                    *
 *                                                                      *
 *             3. Le type du trait a ajouter                            *
 *                                                                      *
 * Retour : - EXEC_OK si tout s'est bien passé                          *
 *                                                                      *
 *          - ERR_ALLOCATION en  cas de problème d'allocation           *
 *                                                                      *
 *          - ERR__COPIE en cas de problème de copie                    *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Ajout_Trait_CT(Noeud *N,char *trait, int type)
{
  /* Allocation de memoire pour le nouveau trait */
  if ((N->Trait[type]=(char *)malloc(sizeof(char)*(strlen(trait)+1))) == NULL) {
    fprintf(stderr,"Erreur d'allocation mémoire du trait\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  /* Copie du trait a sa place */
  if ((strcpy(N->Trait[type],trait)) == NULL) {
    fprintf(stderr,"Erreur de copie du trait\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_COPIE);

  }
  return(EXEC_OK);
}


/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de l'identifiant des groupes nominaux        *
 *            maximaux et des phrases dans lequel se trouve le candidat * 
 *            terme                                                     *
 *                                                                      *
 * Argument : le noeud correspondant au candidat terme                  *
 *                                                                      *
 * Retour : les identifiants des groupes nominaux maximaux et des       * 
 *          phrases dans un chaine de caracteres                        *
 *                                                                      *
 *          NULL en cas d'erreur                                        *
 *                                                                      *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/

char *get_ID(Noeud *CT)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *idtmp; /* Variable contenant les identifiants des groupes nominaux 
		  maximaux et des phrases
	        */
   
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  if (CT->Pere != NULL){
    /* Il ne s'agit pas de la racine de l'arbre, 
       on va voir plus haut (le pere)
     */
    if ((idtmp=get_ID(CT->Pere)) == NULL) {
      return(NULL);
    }
  }
  else {
    /* On est a la racine de l'arbre
     */
    /* Allocation de memoire pour les identifiants des groupes nominaux maximaux
       et des identifiants des phrases
     */
    if ((idtmp = (char *)malloc((strlen(CT->Trait[IDGNM])+strlen(CT->Trait[IDPHRASE])+2)*sizeof(char))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire de l'identifiant\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(NULL);
    }
    /* On construit l'identifiant
     */
    sprintf(idtmp,"%s:%s",CT->Trait[IDGNM],CT->Trait[IDPHRASE]);
  }
  /* Envoi de l'identifiant
   */
  return(idtmp);
}

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de l'ensemble des informations (lemme, categorie,  *
 *            identifiants, decomposition) sur un candidat terme        *
 *            Balisage des informations dans le style SGML              *
 *                                                                      *
 * Argument : Le pointeur sur le noeud dont on veut recuperer les       *
 *            informations                                              *
 *                                                                      *
 * Retour : Les informations a recuperer (dans une chaine de carateres) *
 *                                                                      *
 * Date de derniere modification : 06/07/2004                           *
 *                                                                      *
 ************************************************************************/


char *get_str_CT(Noeud *CT)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *strtmp;    /* Ensemble des informations */
  char *strtmp1;   /* Informations concernant la tete (fils 0) */
  char *strtmp2;   /* Informations concernant la tete (fils 1) */
  char *idtmp;     /* identifiant des groupes nominaux maximaux et des
		      phrases dans lequel apparait le candidat terme
		    */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  if (CT->nbfils==0) {
    /* On est arrive a une feuille
       Allocation de la place memoire
    */
    if ((strtmp=(char *)malloc(sizeof(char)*(strlen(CT->Trait[CATEG]) + strlen(CT->Trait[LEMME]) + strlen(CT->Trait[IDGNM])  +  35))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(NULL);
    }
    /* Copie des informations 
     */
    sprintf(strtmp,"<LM>%s</LM><C>%s</C><G>%s</G>",CT->Trait[1], CT->Trait[0], CT->Trait[2]);
  }
  else {
    /* Le candidat terme a une tete et une expansion
     */
    /* On recupere les informations sur la tete
     */
    if ((strtmp1=get_str_CT(CT->fils[TETE])) == NULL) {
      return(NULL);
    }
    /* On recupere les informations sur l'expansion
     */
    if ((strtmp2=get_str_CT(CT->fils[EXPANSION])) == NULL) {
      return(NULL);
    }

    /* Recuperation des identifiants des groupes nominaux et des phrases
     */
    if ((idtmp=get_ID(CT)) == NULL) {
      return(NULL);
    }

    /* Allocation de la place memoire
     */
    if ((strtmp=(char *)malloc(sizeof(char)*(strlen(strtmp1) + strlen(strtmp2) + strlen(CT->Trait[CATEG]) + strlen(idtmp) + 45))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(NULL);
    }
    /* Copie des informations
     */
    sprintf(strtmp,"\n\t<T>%s</T><CD>%s</CD><E>%s</E><ID>%s</ID>", strtmp1, CT->Trait[CATEG], strtmp2, idtmp);
    /* Liberation memoire des variables contenant les informations sur 
       les fils du candidat terme
     */
    free(strtmp1);
    free(strtmp2);
  }
  /* Renvoi des informations sur le candidat terme
   */
  return(strtmp);
}

/************************************************************************
 *                                                                      *
 * Objectif : Calcul du lemme du candidat terme                         *
 *                                                                      *
 * Argument : Le noeud courant                                          *
 *                                                                      *
 * Retour : Le lemme à recuperer                                        *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

char *get_lem_CT(Noeud *CT)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *strtmp;    /* Lemme du candidat terme courant */
  char *strtmp1;   /* Lemme de la tete du candidat terme */
  char *strtmp2;   /* Lemme de l'expansion du candidat terme */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   printlog("PAsse get lem CT %d\n",CT->nbfils); */

  if (CT->nbfils==0) {
    /* On est arrive a une feuille
       Allocation de la place memoire
    */
/*   printlog("Passe (1)\n"); */
    if ((strtmp=(char *)malloc(sizeof(char)*(strlen(CT->Trait[LEMME]) + strlen(CT->Trait[CATEG])+3))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(NULL);
    }
/*   printlog("Passe\n"); */
    /* Copie du lemme et de la categorie
     */
    sprintf(strtmp,"%s__%s",CT->Trait[LEMME], CT->Trait[CATEG]);
  }
  else {
/*   printlog("Passe (2)\n"); */
    /* Le candidat terme a une tete et une expansion
     */
    /* On recupere le lemme de la tete et de l'expansion
     */
    if ((strtmp1=get_lem_CT(CT->fils[0])) == NULL) {
      return(NULL);
    }
    if ((strtmp2=get_lem_CT(CT->fils[1])) == NULL) {
      return(NULL);
    }
    /* Allocation de la place memoire
     */
    if ((strtmp=(char *)malloc(sizeof(char)*(strlen(strtmp1) + strlen(strtmp2) + 2))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(NULL);
    }
    /* Copie du lemme de la tete et de l'expansion
     */
    sprintf(strtmp,"%s=%s",strtmp1,strtmp2);
    /* Liberation memoire des variables contenant le lemme  
       des fils du candidat terme
     */
    free(strtmp1);
    free(strtmp2);
  }
  /* Renvoi du lemme du candidat terme
   */
  return(strtmp);
}


/************************************************************************
 *                                                                      *
 * Objectif : Construction du graphe des candidats termes               *
 *                                                                      *
 * Argument : 1. Identifiant dans la table des fichier ouverts dans     *
 *               SynoTerm du fichier contenant les candidats termes     *
 *                                                                      *
 *            2. Nombre de groupes nominaux maximaux                    *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int ConstrMatrice(int Fichct, int nbgnmax)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *NomfichTeteS;  /* Nom du fichier contenant la liste des tetes */
  char *NomfichExpS;   /* Nom du fichier contenant la liste des expansions */
  char *NomfichEnt;    /* Nom du fichier d'entete */
  char *tmp;           /*  */
  int i;                   /*  */
  int nbtetetotals;        /*  */
  int nbexptotals;         /*  */
  int nbtetesC;            /*  */
  int nbexpsC;             /*  */
  FILE *FichTeteS;         /* Flux sur le fichier contenant la liste des tetes */
  FILE *FichExpS;          /* Flux sur le fichier contenant la liste des expansions */
  FILE *FichEnt;           /* Flux sur le fichier d'entete */
  FILE *FichCTMax;         /* Flux sur le fichier des groupes nominaux maximaux */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/
  
  printlog("Debut ConstrMatrice\n");
  /* Constitution des noms des fichiers contenant les tetes et les expansions */

    if ((NomfichTeteS=(char *)malloc(sizeof(char)*(strlen(liste_fichier[Fichct].nom) + strlen(EXT_FICHIER_ST_CT) + strlen(EXT_FICHIER_ST_TETES) + 1))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }

  sprintf(NomfichTeteS,"%s%s%s",liste_fichier[Fichct].nom,EXT_FICHIER_ST_CT,EXT_FICHIER_ST_TETES);

    if ((NomfichExpS=(char *)malloc(sizeof(char)*(strlen(liste_fichier[Fichct].nom) + strlen(EXT_FICHIER_ST_CT) + strlen(EXT_FICHIER_ST_EXPANSIONS) + 1))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }

  sprintf(NomfichExpS,"%s%s%s",liste_fichier[Fichct].nom,EXT_FICHIER_ST_CT,EXT_FICHIER_ST_EXPANSIONS);
  
  printlog("Lecture des entetes\n");
  
  /* Nombre total de tetes distinctes
   */
  Lst_Termes.nb_tetes = liste_fichier[Fichct].nbtetes;
  
  /* Nombre total de expansions distinctes 
   */
  Lst_Termes.nb_exps = liste_fichier[Fichct].nbexps;
  
  printlog("%d\n",Lst_Termes.nb_tetes);
  printlog("%d\n",Lst_Termes.nb_exps);
  
  printlog("Lst_Termes.lst_exp\n");
  /* Allocation de la place memoire pour le tableau 
     des candidats termes en expansion
   */
  if ((Lst_Termes.lst_exps = (struct GrphTerms *)malloc(sizeof(struct GrphTerms)* Lst_Termes.nb_exps)) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
  }
  
  printlog("Lst_Termes.lst_tete\n");
  /* Allocation de la place memoire pour le tableau 
     des candidats termes en tete
   */
  if ((Lst_Termes.lst_tetes = (struct GrphTerms *)malloc(sizeof(struct GrphTerms)* Lst_Termes.nb_tetes)) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
  }

  /*++++++++++++++++++++*/

  printlog("Lecture du fichier des TETEs\n");
  /* Ouverture du fichier contenant la liste des candidats termes
     en position tete
   */
  if ((FichTeteS = fopen(NomfichTeteS,"r")) == NULL) {
      fprintf(stderr,"Erreur du fichier %s\n", NomfichTeteS);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_OUVERTURE);    
  }

  /* Initialisation de la lecture
   */
  ConstrEntetein = FichTeteS;
  typeEntete = TETE;
  Compteur=0;
  /* Lecture du  fichier des termes en position tête
   */
  ConstrEntetelex();
  printlog("%d\n",Compteur);
  /* Fermeture du fichier
   */
  fclose(FichTeteS);
  
  /*++++++++++++++++++++*/

  printlog("Lecture du fichier des EXPs\n");
  /* Ouverture du fichier contenant la liste des candidats termes
     en position expansion
   */
  if ((FichExpS = fopen(NomfichExpS,"r")) == NULL) {
      fprintf(stderr,"Erreur du fichier %s\n", NomfichExpS);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_OUVERTURE);
  }
  /* Initialisation de la lecture
   */
  ConstrEntetein = FichExpS;
  typeEntete = EXPANSION;
  Compteur=0;
  /* Lecture du  fichier des termes en position expansion
   */
  ConstrEntetelex();
  printlog("%d\n",Compteur);
  /* Fermeture du fichier
   */
  fclose(FichExpS);
  printlog("Fin ConstrEntete\n");
  
  /*++++++++++++++++++++*/

  printlog("Lecture de l'interieur de la matrice\n");
  /* Construction des relations de dependance et de composition 
   */
  /* Constitution du nom du fichier contenant les groupes nominaux 
     maximaux
   */

    if ((tmp=(char *)malloc(sizeof(char)*(strlen(liste_fichier[Fichct].nom) + strlen(EXT_FICHIER_ST_CT) + 1))) == NULL) {
      fprintf(stderr,"Erreur d'allocation mémoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
  
  sprintf(tmp,"%s%s",liste_fichier[Fichct].nom,EXT_FICHIER_ST_CT);

  /* Ouverture du fichier contenant les groupes nominaux 
     maximaux
   */
  if ((FichCTMax = fopen(tmp,"r")) == NULL) {
      fprintf(stderr,"Erreur du fichier %s\n", tmp);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_OUVERTURE);
  }

  /* Initialisation de la lecture
   */
  ConstrMatricein = FichCTMax;
  /* Lecture des groupes nominaux maximaux et ajout des relations
     de dependance et de composition
   */
  ConstrMatricelex();
  /* Terminer 
   */
  fclose(FichCTMax);
  printlog("Fin lecture de l'interieur de la matrice\n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Liberation de la memoire reservee pour un candidat terme  *
 *            represente dans un arbre (preliminaire a la construction  *
 *            d'un graphe)                                              *
 *                                                                      *
 * Argument : Pointeur sur le noeud correspondant au candidat terme     *
 *            dont on veut liberer la memoire                           *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Free_CT(Noeud *CT)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i;   /* Variable utilise dans une boucle d'iteration */
  
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  if (CT->nbfils!=0) {
    /* Le noeud n'est pas une feuille.
       On effectue la liberation de la memoire pour 
       les fils
     */
    Free_CT(CT->fils[TETE]);
    Free_CT(CT->fils[EXPANSION]);
  }
  /*  On libere la memoire des traits du noeud courant
   */
  for(i=CT->NbTraits-1;i>-1;i--){
    free(CT->Trait[i]);
  }
  /* Liberation de la memoire reservee au noeud
   */
  free(CT);
  return(EXEC_OK);
}


/************************************************************************
 *                                                                      *
 * Objectif : Recherche du lemme dans la liste des candidats termes     *
 *            en position tete                                          *
 *                                                                      *
 * Argument : Lemme recherche                                           *
 *                                                                      *
 * Retour : indice du lemme recherche (>= 0)                            *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_lemmeT(char *lemme)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i;   /* Borne inferieure de l'intervalle de recherche */
  int j;   /* Borne superieure de l'intervalle de recherche */
  int k;   /* Milieur de l'intervalle de recherche */
  
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Initialisation des bornes de l'intervalle de recherche
   */
  i=0;
  j=Lst_Termes.nb_tetes-1;

  /* Recherche du lemme dans la liste des candidats termes
     en position tete 
   */
  while(i!=j) {
    /* Calcul du mileu de l'intervalle de recherche
     */
    k = (i+j+1)/2;
    /* On regarde si le lemme recherche est avant ... 
     */
/*     fprintf(stderr,"i: %d, k:%d, j:%d ; cmp = %d\n", i,k,j,strcmp(lemme,Lst_Termes.lst_tetes[k].lemme)); */
/*     printlog("i: %d, k:%d, j:%d\n", i,k,j); */
/*     printlog("k(1): %s;\n",Lst_Termes.lst_tetes[k].lemme); */
/*     printlog("lemme: %s;\n",lemme); */
/*     if ((((typoC & TYPOPAUVRE)||(typoD & TYPOPAUVRE))&& (strcmp_sgml(lemme,Lst_Termes.lst_tetes[k].lemme)<0))|| ((!(typoC & TYPOPAUVRE))&&(!(typoD & TYPOPAUVRE))&&(strcmp(lemme,Lst_Termes.lst_tetes[k].lemme)<0))) { */
    if (strcmp(lemme,Lst_Termes.lst_tetes[k].lemme)<0) {
      /* On reduit l'intervalle a la partie inferieure
       */
      j = k-1;
/*       printlog("Pj\n"); */
    }
    else {
      /* ... ou apres
	 On reduit alors l'intervalle a la partie superieure
       */
      i = k;
/*       printlog("Pi\n"); */
    }
/*     printlog("k(2): %s\n",Lst_Termes.lst_tetes[k].lemme); */
  }
  /* On envoi l'indice du lemme recherche
   */
/*     printlog("i: %s\n",Lst_Termes.lst_tetes[i].lemme); */

  if (strcmp(Lst_Termes.lst_tetes[i].lemme, lemme)!=0) {
/*     fprintf(stderr,"Recherche du lemme %s\n", lemme); */
/*     fprintf(stderr,"Position supposee %d\n", i); */
/*     fprintf(stderr,"Valeur du lemme a la position supposee %s\n", Lst_Termes.lst_tetes[i].lemme); */
/*     fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
    return(ERR_TRAITMT);

  }
  return(i);
}


/************************************************************************
 *                                                                      *
 * Objectif : Recherche du lemme dans la liste des candidats termes     *
 *            en position expansion                                     *
 *                                                                      *
 * Argument : lemme recherche                                           *
 *                                                                      *
 * Retour : indice du lemme recherche ( >= 0 )                          *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_lemmeE(char *lemme)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i;   /* Borne inferieure de l'intervalle de recherche */
  int j;   /* Borne superieure de l'intervalle de recherche */
  int k;   /* Milieur de l'intervalle de recherche */
  
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Initialisation des bornes de l'intervalle de recherche
   */
  i=0;
  j=Lst_Termes.nb_exps-1;
  
  /* Recherche du lemme dans la liste des candidats termes
     en position tete 
   */
  while(i!=j) {
    /* Calcul du mileu de l'intervalle de recherche
     */
    k = (i+j+1)/2;
    /* On regarde si le lemme recherche est avant ... 
     */
/*     if ((((typoC & TYPOPAUVRE)||(typoD & TYPOPAUVRE))&&(strcmp_sgml(lemme,Lst_Termes.lst_exps[k].lemme)<0)) || ((!(typoC & TYPOPAUVRE))&&(!(typoD & TYPOPAUVRE))&&(strcmp(lemme,Lst_Termes.lst_exps[k].lemme)<0))){ */
    if (strcmp(lemme,Lst_Termes.lst_exps[k].lemme)<0){
      /* On reduit l'intervalle a la partie inferieure
       */
      j = k-1;
    }
    else {
      /* ... ou apres
	 On reduit alors l'intervalle a la partie superieure
       */
      i = k;
    }
  }
  /* On envoi l'indice du lemme recherche
   */
  if (strcmp(Lst_Termes.lst_exps[i].lemme, lemme)!=0) {
/*     fprintf(stderr,"Recherche du lemme %s\n", lemme); */
/*     fprintf(stderr,"Position supposee %d\n", i); */
/*     fprintf(stderr,"Valeur du lemme a la position supposee %s\n", Lst_Termes.lst_exps[i].lemme); */
/*     fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
    return(ERR_TRAITMT);
  }
  return(i);
}

/*int recherche_enteteS(Noeud *CT, int type)
{
    int i;
    
    if (type==1) {
    	i=0;
    	while((i<Lst_CT.nb_tetesS) && (!((strcmp(CT->Trait[LEMME],Lst_CT.lst_tetes[i].lemme)==0) && (strstr(CT->Trait[CATEG],Lst_CT.lst_tetes[i].categ)!=0)))) {
	    i++;
	}
	return(i);
    }
    if (type==2) {
    	i=0;
    	while((i<Lst_CT.nb_expsS) && (!((strcmp(CT->Trait[LEMME],Lst_CT.lst_exps[i].lemme)==0) && (strstr(CT->Trait[CATEG],Lst_CT.lst_exps[i].categ)!=0)))) {
	    i++;
	}
	return(i);
    }
}*/



/************************************************************************
 *                                                                      *
 * Objectif : Recherche de deux indices dans les tableaux contenant les *
 *            les tetes et les expansions (indique si une relation de   *
 *            decomposition existe deja)                                *
 *                                                                      *
 * Arguments : 1. Le candidat terme (dans le graphe) ou la recherche    *
 *               sera effectuee                                         *
 *                                                                      *
 *             2. L'indice de la tete a rechercher ( >= 0 )             *
 *                                                                      *
 *             3. L'indice de l'expansion a rechercher ( >= 0 )         *
 *                                                                      *
 * Retour : indice ( >= 0 ) de la relation de decomposition si elle     *
 *          existe, -1 sinon                                            *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_ind_decomp(struct GrphTerms *lst_comp,int ind0,int ind1)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Indice courant dans les tableaux des composants */
  
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Initialisation
   */
  i=0;
  /* Recherche de la relation de decomposition si elle existe
   */
  while((i<lst_comp->nbdecomp)&&(!((lst_comp->decomp[TETE][i]==ind0) &&(lst_comp->decomp[EXPANSION][i]==ind1)))) i++;
  
  
  if (i==lst_comp->nbdecomp) {
    /* La relation de decomposition n'existe pas 
     */
    return(ERR_PAS_TROUVE); 
  }
  else {
    /* La relation de decomposition existe, on renvoie l'indice 
     */
    return i;
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Recherche de deux indices dans les tableaux contenant les *
 *            les relations de composition et de dependance (indique si * 
 *            une relation de composition et de dependance existe deja) *
 *                                                                      *
 * Arguments : 1. Le candidat terme (dans le graphe) ou la recherche    *
 *               sera effectuee                                         *
 *                                                                      *
 *             2. L'indice de la tete a rechercher ( >= 0 )             *
 *                                                                      *
 *             3. L'indice de l'expansion a rechercher ( >= 0 )         *
 *                                                                      *
 * Retour : indice ( >= 0 ) de la relation de composition et de         *
 *           dependance si elle existe, -1 sinon                                       *
 *                                                                      *
 * Date de derniere modification : 15/07/2004                           *
 *                                                                      *
 ************************************************************************/

int recherche_ind_compdep(struct GrphTerms *lst_decomp,int ind0,int ind1)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Indice courant dans les tableaux de composition et de dependance */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/
    
  /* Initialisation
   */
  i=0;
  /* Recherche de la relation de composition et de dependance si elle existe
   */
  while((i<lst_decomp->nbcompdep)&&(!((lst_decomp->compdep[COMP][i]==ind0) &&(lst_decomp->compdep[DEP][i]==ind1)))) {
    i++;
  }
  
  if (i==lst_decomp->nbcompdep) {
    /* La relation de composition et de dependance n'existe pas 
     */
    return(ERR_PAS_TROUVE);
  }
  else {
    /* La relation de composition et de dependance existe, on renvoie 
       l'indice 
     */
    return i;
  }
}

/************************************************************************
 *                                                                      *
 * Objectif :  Test de l'existence d'un lemme dans la matrice           *
 *                                                                      *
 * Argument : 1. lemme de la relation de synonymie                      *
 *                                                                      *
 *            2. lemme dans la matrice                                  *
 *                                                                      *
 * Retour :  VRAI si le lemme est dans la matrice                       *
 *           FALSE sinon                                                *
 *                                                                      *
 * Date de derniere modification : 12/10/2003                           *
 *                                                                      *
 ************************************************************************/

int exist_lemme_matrice(char *lemme_syn, char *lemme_mat)
{
      /* Le test de la mort qui tue ;-)
	 Un des deux fichiers est en typographie pauvre ->
	       On fait alors un comparaison du lemme et de l'element courant du tableau
	       en tenant compte du codage en entite SGML
	 Aucun des deux fichier est en typographie pauvre ->
	       On fait alors une comparaison brutale du lemme et de l'element courant 
	       du tableau
       */
    if ((((typoC & TYPOPAUVRE) || (typoD & TYPOPAUVRE)) && (strcmp_sgml_carndef(lemme_syn,lemme_mat)==0)) || ((!(typoC & TYPOPAUVRE)) && (!(typoD & TYPOPAUVRE)) && (strcmp_carndef(lemme_syn,lemme_mat)==0))) {
	return(TRUE);
      } else {
	return(FALSE);
      }
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un candidat terme dans le graphe                  *
 *                                                                      *
 * Arguments : 1. Candidat terme represente sous forme d'un arbre       *
 *                                                                      *
 *             2. Position (tete (0) ou expansion (1)) du candidat      *
 *                terme a ajouter                                       *
 *                                                                      *
 * Retour : l'indice du candidat terme ( >= 0 )                         *
 *          -1 en cas d'erreur                                          *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int Ajout_CTMax_Matrice(Noeud *CT,int pos)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int indtete;         /* Indice de la tete candidat terme a ajouter ( >= 0) */
  int indexp;          /* Indice de l'expansion du candidat terme a ajouter ( >= 0) */
  int ind;             /* Indice du candidat terme a ajouter ( >= 0) */
  char *lemmect1;      /* Lemme de l'expansion du candidat terme a ajouter */
  char *lemmect2;      /* Lemme de l'expansion du candidat terme a ajouter */
  char *lemmect;       /* Lemme du candidat terme a ajouter */
  char *infolexter;    /* Informations fournies par LEXTER */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   printlog("Dans AjoutCTMAx\n"); */

  /* Recuperation du lemme du candidat terme a ajouter
   */
  if ((lemmect = get_lem_CT(CT)) == NULL) {
    return(ERR_PAS_TROUVE);
  }
  if (pos){
    /* Le candidat terme a ajouter est une expansion
     */
    /* Recherche de l'indice du candidat terme a ajouter
       dans le tableau des expansions
     */
    if ((ind = recherche_lemmeE(lemmect)) < 0) {
      return(ind);
    }
    /* Recuperation des informations fournies par LEXTER sur le candidat terme
     */
    if ((infolexter = get_ID(CT)) == NULL) {
      return(ERR_GET_ID);
    }
    
    /* On ajoute les informations pour le candidat terme
     */
    if (Lst_Termes.lst_exps[ind].donneesLexter==NULL) {
      /* Cas initial
       */
      Lst_Termes.lst_exps[ind].donneesLexter=infolexter;
    }
    else {
      /* Des informations ont deja ete ajouteees, on rajoute les nouvelles a la 
	 suite
      */
      /* Allocation memoire
       */
      if ((Lst_Termes.lst_exps[ind].donneesLexter = (char *)realloc( Lst_Termes.lst_exps[ind].donneesLexter, sizeof(char)*( strlen(Lst_Termes.lst_exps[ind].donneesLexter) + strlen(infolexter) + strlen(SEP_INFOLEXTER) + 1))) == NULL){
	fprintf(stderr,"Erreur d'allocation mémoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* Ajout du separateur
       */
      strcat(Lst_Termes.lst_exps[ind].donneesLexter,SEP_INFOLEXTER);
      /* Ajout des nouvelles informations
       */
      strcat(Lst_Termes.lst_exps[ind].donneesLexter,infolexter);
      /* Liberation de la memoire allouee
       */
      free(infolexter);
    }
    
    /* Ajout recursif des composants
     */
    if (CT->fils[TETE]->nbfils){
      /* Si la tete du candidat terme possede des fils,
	 alors ot ajoute la tete
       */
      if ((indtete = Ajout_CTMax_Matrice(CT->fils[TETE],TETE)) < 0 ) {
	return(indtete);
      }
    }
    else {
      /* Si la tete du candidat terme ne possede pas de fils,
	 alors ...
       */
      /* On calcule le lemme de la tete du candidat terme
       */
      if ((lemmect1 = get_lem_CT(CT->fils[TETE])) == NULL) {
	return(ERR_GET_LEM_CT);
      }
      /* On recherche la place du lemme dans le tableau des tetes
       */
      if ((indtete = recherche_lemmeT(lemmect1)) < 0) {
	return(indtete);
      }
      /* On recupere les informations fournies par LEXTER pour la tete
       */
      if ((infolexter = get_ID(CT->fils[TETE])) == NULL) {
	return(ERR_GET_ID);
      }
      /* On ajoute les informations pour la tete du candidat terme
       */
      if (Lst_Termes.lst_tetes[indtete].donneesLexter==NULL) {
	/* Cas initial
	 */
	Lst_Termes.lst_tetes[indtete].donneesLexter=infolexter;
      }
      else {
	/* Des informations ont deja ete ajoutees, on rajoute les nouvelles a la 
	   suite
	*/
	/* Allocation memoire
	 */
	if ((Lst_Termes.lst_tetes[indtete].donneesLexter = (char *)realloc( Lst_Termes.lst_tetes[indtete].donneesLexter, sizeof(char)*(strlen( Lst_Termes.lst_tetes[indtete].donneesLexter) + strlen(infolexter) +strlen(SEP_INFOLEXTER) + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}
	/* Ajout du separateur
	 */
	strcat(Lst_Termes.lst_tetes[indtete].donneesLexter,SEP_INFOLEXTER);
	/* Ajout des nouvelles informations
	 */
	strcat(Lst_Termes.lst_tetes[indtete].donneesLexter,infolexter);
	/* Liberation de la memoire allouee
	 */
	free(infolexter);
      }
    }

    /* On passe a l'expansion
     */
    if (CT->fils[EXPANSION]->nbfils){
      /* Si l'expansion du candidat terme possede des fils,
	 alors on ajoute l'expansion
       */
      if ((indexp = Ajout_CTMax_Matrice(CT->fils[EXPANSION],EXPANSION)) < 0) {
	return(indexp);
      }
    }
    else {
      /* Si l'expansion du candidat terme ne possede pas de fils,
	 alors ...
       */
      /* On calcule le lemme de l'expansion du candidat terme
       */
      if ((lemmect2 = get_lem_CT(CT->fils[EXPANSION])) == NULL) {
	return(ERR_GET_LEM_CT);
      }
      /* On recherche la place du lemme dans le tableau des expansions
       */
      if ((indexp = recherche_lemmeE(lemmect2)) < 0) {
	return(indexp);
      }
      /* On recupere les informations fournies par LEXTER pour la tete
       */
      if ((infolexter = get_ID(CT->fils[EXPANSION])) == NULL) {
	return(ERR_GET_ID);
      }
      /* On ajoute les informations pour la tete du candidat terme
       */
      if (Lst_Termes.lst_exps[indexp].donneesLexter==NULL) {
	/* Cas initial
	 */
	Lst_Termes.lst_exps[indexp].donneesLexter=infolexter;
      }
      else {
	/* Des informations ont deja ete ajoutees, on rajoute les nouvelles a la 
	   suite
	*/
	/* Allocation memoire
	 */
	if ((Lst_Termes.lst_exps[indexp].donneesLexter = (char *)realloc( Lst_Termes.lst_exps[indexp].donneesLexter, sizeof(char)*(strlen( Lst_Termes.lst_exps[indexp].donneesLexter) + strlen(infolexter) + strlen(SEP_INFOLEXTER) + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}
	/* Ajout du separateur
	 */
	strcat(Lst_Termes.lst_exps[indexp].donneesLexter, SEP_INFOLEXTER);
	/* Ajout des nouvelles informations
	 */
	strcat(Lst_Termes.lst_exps[indexp].donneesLexter,infolexter);
	/* Liberation de la memoire allouee
	 */
	free(infolexter);
      }
    }
    
    /* Ajout des liens de decomposition 
     */
    if (Lst_Termes.lst_exps[ind].nbdecomp==0) {
      /* C'est la premiere fois qu'on traite ce candidat terme. Il ne possede
	 donc pas de lien de decompostion. On les rajoute.
       */
      /* Allocation de la memoire
       */ 
      if ((Lst_Termes.lst_exps[ind].decomp[TETE] = (int *)malloc(sizeof(int))) == NULL) {
	fprintf(stderr,"Erreur d'allocation mémoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      if ((Lst_Termes.lst_exps[ind].decomp[EXPANSION] = (int *)malloc(sizeof(int))) == NULL) {
	fprintf(stderr,"Erreur d'allocation mémoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* On indique que l'indice du candidat terme correspondant a la tete
       */
      Lst_Termes.lst_exps[ind].decomp[TETE][Lst_Termes.lst_exps[ind].nbdecomp] = indtete;
      /* On indique que l'indice du candidat terme correspondant a l'expansion
       */
      Lst_Termes.lst_exps[ind].decomp[EXPANSION][Lst_Termes.lst_exps[ind].nbdecomp] = indexp;
      /* On augmente le nombre de decomposition du candidat terme
       */
      Lst_Termes.lst_exps[ind].nbdecomp++;
    }
    else {
      /*  On commence par regarder si la nouvelle decomposition n'existe pas deja
       */
      if (recherche_ind_decomp(&(Lst_Termes.lst_exps[ind]),indtete,indexp) == ERR_PAS_TROUVE) {
	/* La nouvelle decomposition n'existe pas deja, on peut l'ajouter
	 */
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_exps[ind].decomp[TETE] = (int *)realloc( Lst_Termes.lst_exps[ind].decomp[0], sizeof(int)*(Lst_Termes.lst_exps[ind].nbdecomp +1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}
	if ((Lst_Termes.lst_exps[ind].decomp[EXPANSION] = (int *)realloc( Lst_Termes.lst_exps[ind].decomp[1], sizeof(int)*(Lst_Termes.lst_exps[ind].nbdecomp +1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}
	/* On indique que l'indice du candidat terme correspondant a la tete
	 */
	Lst_Termes.lst_exps[ind].decomp[TETE][Lst_Termes.lst_exps[ind].nbdecomp] = indtete;
	/* On indique que l'indice du candidat terme correspondant a l'expansion
	 */
	Lst_Termes.lst_exps[ind].decomp[EXPANSION][Lst_Termes.lst_exps[ind].nbdecomp] = indexp;
	/* On augmente le nombre de decomposition du candidat terme
	 */
	Lst_Termes.lst_exps[ind].nbdecomp++;
      }
    }
  }
  else {
    /* Le candidat terme a ajouter est une tete
     */
    /* Recherche de l'indice du candidat terme a ajouter
       dans le tableau des tetes
     */
/*     printlog("Passe Passe (1a) %s\n", lemmect); */
    if ((ind = recherche_lemmeT(lemmect)) < 0) {
/*       printlog("return\n"); */
      return(ind);
    }
    /* Recuperation des informations fournies par LEXTER sur le candidat terme
     */
/*     printlog("Passe Passe (1b)\n"); */
    if ((infolexter = get_ID(CT)) == NULL) {
      return(ERR_GET_ID);
    }
/*     printlog("Passe Passe (1c)\n"); */
    
    /* On ajoute les informations pour le candidat terme
     */
    if (Lst_Termes.lst_tetes[ind].donneesLexter==NULL) {
      /* Cas initial
       */
      Lst_Termes.lst_tetes[ind].donneesLexter=infolexter;
    }
    else {
      /* Des informations ont deja ete ajouteees, on rajoute les nouvelles a la 
	 suite
      */
      /* Allocation memoire
       */
/*     printlog("Passe Passe (1d)\n"); */
      if ((Lst_Termes.lst_tetes[ind].donneesLexter = (char *)realloc( Lst_Termes.lst_tetes[ind].donneesLexter, sizeof(char)*(strlen( Lst_Termes.lst_tetes[ind].donneesLexter) + strlen(infolexter) + strlen(SEP_INFOLEXTER) + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

      /* Ajout du separateur
       */
      strcat(Lst_Termes.lst_tetes[ind].donneesLexter,SEP_INFOLEXTER);
      /* Ajout des nouvelles informations
       */
      strcat(Lst_Termes.lst_tetes[ind].donneesLexter,infolexter);
      /* Liberation de la memoire allouee
       */
/*     printlog("Passe Passe (1e)\n"); */
      free(infolexter);
    }
/*     printlog("Passe Passe (2)\n"); */


    /* Ajout recursif des composants
     */
    if (CT->fils[TETE]->nbfils){
      /* Si la tete du candidat terme possede des fils,
	 alors ot ajoute la tete
       */
      if ((indtete = Ajout_CTMax_Matrice(CT->fils[TETE],TETE)) < 0) {
	return(indtete);
      }
    }
    else {
      /* Si la tete du candidat terme ne possede pas de fils,
	 alors ...
       */
      /* On calcule le lemme de la tete du candidat terme
       */
      if ((lemmect1 = get_lem_CT(CT->fils[TETE])) == NULL){
	return(ERR_GET_LEM_CT);
      }
      /* On recherche la place du lemme dans le tableau des tetes
       */
      if ((indtete = recherche_lemmeT(lemmect1)) < 0) {
	return(indtete);
      }
      /* On recupere les informations fournies par LEXTER pour la tete
       */
      if ((infolexter = get_ID(CT->fils[TETE])) == NULL) {
	return(ERR_GET_ID);
      }
      /* On ajoute les informations pour la tete du candidat terme
       */
      if (Lst_Termes.lst_tetes[indtete].donneesLexter==NULL) {
	/* Cas initial
	 */
	Lst_Termes.lst_tetes[indtete].donneesLexter=infolexter;
      }
      else {
	/* Des informations ont deja ete ajoutees, on rajoute les nouvelles a la 
	   suite
	*/
	/* Allocation memoire
	 */
	if ((Lst_Termes.lst_tetes[indtete].donneesLexter = (char *)realloc( Lst_Termes.lst_tetes[indtete].donneesLexter, sizeof(char)*(strlen( Lst_Termes.lst_tetes[indtete].donneesLexter) + strlen(infolexter) + strlen(SEP_INFOLEXTER) + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	/* Ajout du separateur
	 */
	strcat(Lst_Termes.lst_tetes[indtete].donneesLexter,SEP_INFOLEXTER);
	/* Ajout des nouvelles informations
	 */
	strcat(Lst_Termes.lst_tetes[indtete].donneesLexter,infolexter);
	/* Liberation de la memoire allouee
	 */
	free(infolexter);
      }
    }
    
    /* On passe a l'expansion
     */
    if (CT->fils[EXPANSION]->nbfils){
      /* Si l'expansion du candidat terme possede des fils,
	 alors on ajoute l'expansion
       */
      if ((indexp = Ajout_CTMax_Matrice(CT->fils[EXPANSION],EXPANSION)) < 0) {
	return(indexp);
      }
    }
    else {
      /* Si l'expansion du candidat terme ne possede pas de fils,
	 alors ...
       */
      /* On calcule le lemme de l'expansion du candidat terme
       */
      if ((lemmect2 = get_lem_CT(CT->fils[EXPANSION])) == NULL) {
	return(ERR_GET_LEM_CT);
      }
      /* On recherche la place du lemme dans le tableau des expansions
       */
      if ((indexp = recherche_lemmeE(lemmect2)) < 0) {
	return(indexp);
      }
      /* On recupere les informations fournies par LEXTER pour la tete
       */
      if ((infolexter = get_ID(CT->fils[EXPANSION])) == NULL) {
	return(ERR_GET_ID);
      }
      /* On ajoute les informations pour la tete du candidat terme
       */
      if (Lst_Termes.lst_exps[indexp].donneesLexter==NULL) {
	/* Cas initial
	 */
	Lst_Termes.lst_exps[indexp].donneesLexter=infolexter;
      }
      else {
	/* Des informations ont deja ete ajoutees, on rajoute les nouvelles a la 
	   suite
	*/
	/* Allocation memoire
	 */
	if ((Lst_Termes.lst_exps[indexp].donneesLexter = (char *)realloc( Lst_Termes.lst_exps[indexp].donneesLexter, sizeof(char)*(strlen( Lst_Termes.lst_exps[indexp].donneesLexter) + strlen(infolexter) + strlen(SEP_INFOLEXTER) + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	/* Ajout du separateur
	 */
	strcat(Lst_Termes.lst_exps[indexp].donneesLexter,SEP_INFOLEXTER);
	/* Ajout des nouvelles informations
	 */
	strcat(Lst_Termes.lst_exps[indexp].donneesLexter,infolexter);
	/* Liberation de la memoire allouee
	 */
	free(infolexter);
      }
    }
    
    /* Ajout des liens de decomposition 
     */
    if (Lst_Termes.lst_tetes[ind].nbdecomp==0) {
      /* C'est la premiere fois qu'on traite ce candidat terme. Il ne possede
	 donc pas de lien de decompostion. On les rajoute.
       */
      /* Allocation de la memoire
       */ 
      if ((Lst_Termes.lst_tetes[ind].decomp[TETE] = (int *)malloc(sizeof(int))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

      if ((Lst_Termes.lst_tetes[ind].decomp[EXPANSION] = (int *)malloc(sizeof(int))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

      /* On indique que l'indice du candidat terme correspondant a la tete
       */
      Lst_Termes.lst_tetes[ind].decomp[TETE][Lst_Termes.lst_tetes[ind].nbdecomp] = indtete;
      /* On indique que l'indice du candidat terme correspondant a l'expansion
       */
      Lst_Termes.lst_tetes[ind].decomp[EXPANSION][Lst_Termes.lst_tetes[ind].nbdecomp] = indexp;
      /* On augmente le nombre de decomposition du candidat terme
       */
      Lst_Termes.lst_tetes[ind].nbdecomp++;
    }
    else {
      /*  On commence par regarder si la nouvelle decomposition n'existe pas deja
       */
      if (recherche_ind_decomp(&(Lst_Termes.lst_tetes[ind]),indtete,indexp) == ERR_PAS_TROUVE) {
	/* La nouvelle decomposition n'existe pas deja, on peut l'ajouter
	 */
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_tetes[ind].decomp[TETE] = (int *)realloc( Lst_Termes.lst_tetes[ind].decomp[0], sizeof(int)*(Lst_Termes.lst_tetes[ind].nbdecomp +1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	if ((Lst_Termes.lst_tetes[ind].decomp[EXPANSION] = (int *)realloc( Lst_Termes.lst_tetes[ind].decomp[1], sizeof(int)*(Lst_Termes.lst_tetes[ind].nbdecomp +1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	/* On indique que l'indice du candidat terme correspondant a la tete
	 */
	Lst_Termes.lst_tetes[ind].decomp[TETE][Lst_Termes.lst_tetes[ind].nbdecomp] = indtete;
	/* On indique que l'indice du candidat terme correspondant a l'expansion
	 */
	Lst_Termes.lst_tetes[ind].decomp[EXPANSION][Lst_Termes.lst_tetes[ind].nbdecomp] = indexp;
	/* On augmente le nombre de decomposition du candidat terme
	 */
	Lst_Termes.lst_tetes[ind].nbdecomp++;
      }
    }
  }
  
  /* Ajout de la relation de dependance et de composition pour la tete 
   */    
  if (Lst_Termes.lst_tetes[indtete].nbcompdep==0) {
    /* Cas initial: C'est la premiere fois qu'on rencontre cette tete
     */
    /* Allocation de la memoire
     */
    Lst_Termes.lst_tetes[indtete].compdep[COMP] = (int *)malloc(sizeof(int));
    Lst_Termes.lst_tetes[indtete].compdep[DEP] = (int *)malloc(sizeof(int));
    /* Ajout de la relation de composition en fonction de la position du
       candidat terme
     */
    if (pos) {
      /* Le candidat terme est en position expansion
       */
      Lst_Termes.lst_tetes[indtete].compdep[COMP][0] = (ind + 1) * -1;
    } else {
      /* Le candidat terme est en position tete
       */
      Lst_Termes.lst_tetes[indtete].compdep[COMP][0] = ind + 1;
    }
    /* Ajout de la relation de dependance
     */
    Lst_Termes.lst_tetes[indtete].compdep[DEP][0] = indexp;
    /* Augmentation du nombre de relation
     */
    Lst_Termes.lst_tetes[indtete].nbcompdep++;
  }
  else {
    if (pos) {
      /* Le candidat terme est en position expansion
       */
      /* On recherche si la relation existe deja ou non 
	 Si elle existe, ca ne sert a rien de l'ajouter en plus
       */
      if (recherche_ind_compdep(&(Lst_Termes.lst_tetes[indtete]), (ind +1) * -1,indexp) == ERR_PAS_TROUVE) {	
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_tetes[indtete].compdep[COMP] = (int *)realloc( Lst_Termes.lst_tetes[indtete].compdep[COMP], sizeof(int)*(Lst_Termes.lst_tetes[indtete].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	if ((Lst_Termes.lst_tetes[indtete].compdep[DEP] = (int *)realloc( Lst_Termes.lst_tetes[indtete].compdep[DEP], sizeof(int)*(Lst_Termes.lst_tetes[indtete].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	
	/* Ajout de la relation de composition 
	 */
	Lst_Termes.lst_tetes[indtete].compdep[COMP][ Lst_Termes.lst_tetes[indtete].nbcompdep] = (ind +1 )* -1;
	/* Ajout de la relation de dependance
	 */
	Lst_Termes.lst_tetes[indtete].compdep[DEP][Lst_Termes.lst_tetes[indtete].nbcompdep] = indexp;
	/* Augmentation du nombre de relation
	 */
	Lst_Termes.lst_tetes[indtete].nbcompdep++;
      }
    }
    else {
      /* Le candidat terme est en position tete
       */
      /* On recherche si la relation existe deja ou non 
	 Si elle existe, ca ne sert a rien de l'ajouter en plus
       */
      if (recherche_ind_compdep(&(Lst_Termes.lst_tetes[indtete]),ind + 1,indexp) == ERR_PAS_TROUVE) {
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_tetes[indtete].compdep[COMP] = (int *)realloc( Lst_Termes.lst_tetes[indtete].compdep[COMP], sizeof(int)*(Lst_Termes.lst_tetes[indtete].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	if ((Lst_Termes.lst_tetes[indtete].compdep[DEP] = (int *)realloc( Lst_Termes.lst_tetes[indtete].compdep[DEP], sizeof(int)*(Lst_Termes.lst_tetes[indtete].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	
	/* Ajout de la relation de composition 
	 */
	Lst_Termes.lst_tetes[indtete].compdep[COMP][ Lst_Termes.lst_tetes[indtete].nbcompdep] = ind + 1;
	/* Ajout de la relation de dependance
	 */
	Lst_Termes.lst_tetes[indtete].compdep[DEP][Lst_Termes.lst_tetes[indtete].nbcompdep] = indexp;
	/* Augmentation du nombre de relation
	 */
	Lst_Termes.lst_tetes[indtete].nbcompdep++;
      }
    }
  }
  
  /* Ajout des relations de dependance pour l'expansion */
  if (Lst_Termes.lst_exps[indexp].nbcompdep==0) {
    /* Cas initial: C'est la premiere fois qu'on rencontre cette expansion
     */
    /* Allocation de la memoire
     */
    if ((Lst_Termes.lst_exps[indexp].compdep[COMP] = (int *)malloc(sizeof(int))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

    if ((Lst_Termes.lst_exps[indexp].compdep[DEP] = (int *)malloc(sizeof(int))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

    /* Ajout de la relation de composition en fonction de la position du
       candidat terme
     */
    if (pos) {
      /* Le candidat terme est en position expansion
       */
      Lst_Termes.lst_exps[indexp].compdep[COMP][0] = (ind + 1)* -1;
    } else {
      /* Le candidat terme est en position tete
       */
      Lst_Termes.lst_exps[indexp].compdep[COMP][0] = ind + 1;
    }
    /* Ajout de la relation de dependance
     */
    Lst_Termes.lst_exps[indexp].compdep[DEP][0] = indtete;
    /* Augmentation du nombre de relation
     */
    Lst_Termes.lst_exps[indexp].nbcompdep++;
  }
  else {
    if (pos) {
      /* Le candidat terme est en position expansion
       */
      /* On recherche si la relation existe deja ou non 
	 Si elle existe, ca ne sert a rien de l'ajouter en plus
       */
      if (recherche_ind_compdep(&(Lst_Termes.lst_exps[indexp]),(ind + 1) * -1,indtete)== ERR_PAS_TROUVE) {
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_exps[indexp].compdep[COMP] = (int *)realloc( Lst_Termes.lst_exps[indexp].compdep[COMP], sizeof(int)*(Lst_Termes.lst_exps[indexp].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	if ((Lst_Termes.lst_exps[indexp].compdep[DEP] = (int *)realloc( Lst_Termes.lst_exps[indexp].compdep[DEP], sizeof(int)*(Lst_Termes.lst_exps[indexp].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	
	/* Ajout de la relation de composition 
	 */
	Lst_Termes.lst_exps[indexp].compdep[COMP][ Lst_Termes.lst_exps[indexp].nbcompdep] = (ind + 1) * -1;
	/* Ajout de la relation de dependance
	 */
	Lst_Termes.lst_exps[indexp].compdep[DEP][Lst_Termes.lst_exps[indexp].nbcompdep] = indtete;
	/* Augmentation du nombre de relation
	 */
	Lst_Termes.lst_exps[indexp].nbcompdep++;
      }
    }
    else {
      /* Le candidat terme est en position tete
       */
      /* On recherche si la relation existe deja ou non 
	 Si elle existe, ca ne sert a rien de l'ajouter en plus
       */
      if (recherche_ind_compdep(&(Lst_Termes.lst_exps[indexp]),ind + 1,indtete) == ERR_PAS_TROUVE) {
	/* Allocation de la memoire
	 */
	if ((Lst_Termes.lst_exps[indexp].compdep[COMP] = (int *)realloc( Lst_Termes.lst_exps[indexp].compdep[COMP], sizeof(int)*(Lst_Termes.lst_exps[indexp].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	if ((Lst_Termes.lst_exps[indexp].compdep[DEP] = (int *)realloc( Lst_Termes.lst_exps[indexp].compdep[DEP], sizeof(int)*(Lst_Termes.lst_exps[indexp].nbcompdep + 1))) == NULL) {
	  fprintf(stderr,"Erreur d'allocation mémoire\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_ALLOCATION);
	}

	
	/* Ajout de la relation de composition 
	 */
	Lst_Termes.lst_exps[indexp].compdep[COMP][ Lst_Termes.lst_exps[indexp].nbcompdep] = ind + 1;
	/* Ajout de la relation de dependance
	 */
	Lst_Termes.lst_exps[indexp].compdep[DEP][Lst_Termes.lst_exps[indexp].nbcompdep] = indtete;
	/* Augmentation du nombre de relation
	 */
	Lst_Termes.lst_exps[indexp].nbcompdep++;
      }
    }
  }
  /* On retourne l'identifiant du candidat terme ( >= 0 )
   */
  return(ind);
}


/************************************************************************
 *                                                                      *
 * Objectif :  Identification de l'intervalle où se trouve un lemme     *
 *             dans la matrice                                          *
 *                                                                      *
 * Argument : 1. lemme de la relation de synonymie                      *
 *                                                                      *
 *            2. lemme dans la matrice                                  *
 *                                                                      *
 * Retour :  PARTIE_INF si le lemme est dans la partie inférieure de    *
 *           l'intervalle                                               *
 *           PARTIE_SUP si c'est dans la partie supérieure              *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int select_intervalle_lemme_matrice(char *lemme_syn, char *lemme_mat)
{
      /* Le test de la mort qui tue ;-)
	 Un des deux fichiers est en typographie pauvre ->
	       On fait alors un comparaison du lemme et de l'element courant du tableau
	       en tenant compte du codage en entite SGML
	 Aucun des deux fichier est en typographie pauvre ->
	       On fait alors une comparaison brutale du lemme et de l'element courant 
	       du tableau
       */
      if ((((typoC & TYPOPAUVRE) || (typoD & TYPOPAUVRE)) && (strcmp_sgml_carndef(lemme_syn,lemme_mat)<0)) || ((!(typoC & TYPOPAUVRE)) && (!(typoD & TYPOPAUVRE)) && (strcmp_carndef(lemme_syn,lemme_mat)<0))) {
	return(PARTIE_INF);
      } else {
	return(PARTIE_SUP);
      }
}


/************************************************************************
 *                                                                      *
 * Objectif :  Recherche un lemme dans un tableau donne en argument     *
 *             entre deux bornes                                        *
 *                                                                      *
 * Argument : 1. Le tableau où est effectué la recherche                *
 *                                                                      *
 *            2. le lemme recherché                                     *
 *                                                                      *
 *            3. la catégorie                                           *
 *                                                                      *
 *            4. la borne inférieure                                    *
 *                                                                      *
 *            5. la borne supérieure                                    *
 *                                                                      *
 * Retour : l'indice du lemme dans le tableau, (decode) s'il existe     *
 *                                                                      *
 *          la borne supérieure + 1 s'il n'existe pas                   *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int recherche_dans_tab(struct GrphTerms *tab, char *lemme, char *categ, int inddebut, int indfin) 
{
  int i;   /* Borne inferieure de l'intervalle de recherche */
  int j;   /* Borne superieure de l'intervalle de recherche */
  int k;   /* Milieur de l'intervalle de recherche */
  char *tmp2;

  /* Initialisation des bornes de l'intervalle de recherche
   */
  i=inddebut;
  j=indfin;
  /* Recherche dicotomique
   */
  /* Tant qu'on n'a pas reduit l'intervalle a un element ... 
   */
  while(i!=j) { 
    /* On calcule le milieu de l'intervalle
     */
    k = (i+j+1)/2;
    /* Le test de la mort qui tue ;-)
       Un des deux fichiers est en typographie pauvre ->
       On fait alors un comparaison du lemme et de l'element courant du tableau
       en tenant compte du codage en entite SGML
       Aucun des deux fichier est en typographie pauvre ->
       On fait alors une comparaison brutale du lemme et de l'element courant 
       du tableau
    */
    if (select_intervalle_lemme_matrice(lemme,tab[k].lemme)){
      /* Le lemme recherche doit etre dans la partie inferieure 
	 de l'intervalle de recherche
      */
      j = k-1;
    }
    else {
      /* Le lemme recherche doit etre dans la partie superieure
	 de l'intervalle de recherche
      */
      i = k;
    }
  }
  /* On refait le test ci-dessus pour verifier qu'on l'a bien trouve
   */
  if (exist_lemme_matrice(lemme,tab[i].lemme)) {
    /* On a trouve le lemme
       Il faut initialiser la categorie si elle n'a pas ete specifie au depart
    */
    if (!categ[0]) {
      /* Pas de specification, on initialise avec la categorie du 
	 lemme trouve
      */
      tmp2 = &(tab[i].lemme[strlen(tab[i].lemme)-3]);
      strcpy(categ,tmp2);
      }
    /* Et on retourne l'indice du lemme dans le tableau
     */
    return(i);
  }
  else {
    /* S'il n'a pas ete trouve on retourne le nombre d'element du tableau
       et la categorie est alors vide
    */ 
    categ[0]=0;
    return(indfin + 1);
  }
}


/************************************************************************
 *                                                                      *
 * Objectif : Recherche dicotomique d'un lemme dans un intervalle d'un  *
 *            des deux tableaux des candidats termes. On prend en       *
 *            compte egalement de la categorie (si indiquee)            *
 *                                                                      *
 * Arguments : 1. Lemme a rechercher                                    *
 *                                                                      *
 *             2. Categorie du lemme                                    *
 *                                                                      *
 *             3. Tableau ou sera effectuee la recherche (TETE ou       *
 *                EXPANSION)                                            *
 *                                                                      *
 *             4. Borne inferieure de l'intervalle                      *
 *                                                                      *
 *             5. Borne superieure de l'intervalle                      *
 *                                                                      *
 * Retour : l'indice du lemme (decode)                                  *
 *                                                                      *
 * Date de derniere modification : 01/12/2003                           *
 *                                                                      *
 ************************************************************************/

int recherche_entete_LemS(char *lemme_orig, char *categ, int type,int inddebut,int indfin)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char *lemme;
  char *tmp2;

  /************************************************************************/    
  /* On regarde si une categorie est precisee et
     si le lemme possede une categorie 
   */
/*   printlog("lemme_orig = %s\n", lemme_orig); */

  if ((categ[0])&&(lemme_orig[strlen(lemme_orig)-1]!='.')) {
    /* si c'est oui, on construit le lemme a rechercher avec 
       la bonne categorie
     */
    /* allocation de la memoire */
    lemme = (char *)malloc(sizeof(char)*(strlen(lemme_orig)+1));
    /* On recopie le lemme d'origine dans la variable du lemme a rechercher
     */
    strcpy(lemme,lemme_orig);
    /* tmp2 est la categorie du lemme d'origine
     */
    tmp2=&lemme[strlen(lemme)-TAILLE_CATEGORIE];
    /* On remplace ca par la categorie donnee en argument
     */
    strcpy(tmp2,categ);
    /* Remarque: la maniere de faire me semble un peu tordue, meme 
       si l'autre maniere revient aussi a faire deux operations :
       lemme[strlen(lemme)-3] = 0; puis un strcat(lemme, categ);
       mais peut que ca ne marche pas (je vois pas pourquoi)
     */
  }
  else {
    /* si c'est non, on dit que le lemme qu'on recherche est le lemme d'origine
     */
    lemme = lemme_orig;
  }

  /************************************************************************/

  /* On passe a la recherche du lemme dans le bon tableau
   */
  if (type) {
    /* Recherche du lemme dans le tableau des expansions
     */
    return(recherche_dans_tab(Lst_Termes.lst_exps, lemme, categ, inddebut, indfin));
  }
  else {
    /* Recherche du lemme dans le tableau des tetes
     */
    return(recherche_dans_tab(Lst_Termes.lst_tetes, lemme, categ, inddebut, indfin));
  }
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int VerifMatr()
{
    /*int i,j;
    FILE *AffichMat;
    
    writelogS("Dans VerifMatr");
    AffichMat = fopen("SynoTerm.mat","w");
    
    for(i=0;i<Lst_CT.nb_tetes;i++){
    	if (i>=Lst_CT.nb_tetesS) {
	    fprintf(AffichMat,"%d = %s\n",i,Lst_CT.lst_tetes[i].lemme);
    	    for(j=0;j<Lst_CT.lst_tetes[i].nbcomp;j++){
		fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_tetes[i].comp[0][j]->lemme, Lst_CT.lst_tetes[i].comp[1][j]->lemme);
	    }
	}
	else {
            fprintf(AffichMat,"%d = %s <%s> \n",i,Lst_CT.lst_tetes[i].lemme,Lst_CT.lst_tetes[i].categ);
    	    for(j=0;j<Lst_CT.lst_tetes[i].nbcomp;j++){
		fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_tetes[i].comp[0][j]->lemme, Lst_CT.lst_tetes[i].comp[1][j]->lemme);
	    }
	}
	fprintf(AffichMat,"\n\n");
    	for(j=0;j<Lst_CT.lst_tetes[i].nbdecomp;j++){
	    fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_tetes[i].decomp[0][j]->lemme, Lst_CT.lst_tetes[i].decomp[1][j]->lemme);
	}
    }
    fprintf(AffichMat,"++++++++++++\n");
    for(i=0;i<Lst_CT.nb_exps;i++){
	if (i>=Lst_CT.nb_expsS){
	    fprintf(AffichMat,"%d = %s\n",i,Lst_CT.lst_exps[i].lemme);
    	    for(j=0;j<Lst_CT.lst_exps[i].nbcomp;j++){
		fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_exps[i].comp[0][j]->lemme, Lst_CT.lst_exps[i].comp[1][j]->lemme);
	    }
	}
	else {
            fprintf(AffichMat,"%d = %s <%s>\n",i,Lst_CT.lst_exps[i].lemme,Lst_CT.lst_exps[i].categ);
    	    for(j=0;j<Lst_CT.lst_exps[i].nbcomp;j++){
		fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_exps[i].comp[0][j]->lemme, Lst_CT.lst_exps[i].comp[1][j]->lemme);
	    }
	    fprintf(AffichMat,"\n******\n");
    	    for(j=0;j<Lst_CT.lst_exps[i].nbdecomp;j++){
		fprintf(AffichMat,"\t %d = %s : %s \n",j,Lst_CT.lst_exps[i].decomp[0][j]->lemme, Lst_CT.lst_exps[i].decomp[1][j]->lemme);
	    }
	}
    }
    fclose(AffichMat);*/
}

/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation associe a un candidat terme          *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             2. Indice du terme 2 (code)                              *
 *                                                                      *
 *             3. Tableau des termes                                    *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteLien(int Terme1, int Terme2, struct GrphTerms *Tab, int info, int incrProd, int flux_ecriture)
{

  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* Indice de l'element courant du tableau */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   fprintf(stderr, "ExisteLien entre %d et %d\n", Terme1, Terme2); */
/*   printlog("ExisteLien entre %d et %d\n", Terme1, Terme2); */

/*   if (Terme1 != Terme2) { */
    i = 0;
    /* On avance dans le tableau tant qu'on a pas trouve ce qu'on veut
     */
    while((i<Tab[Terme1].nbsyn)&&((Tab[Terme1].syn[REF][i]!=Terme2)||((info > 0)&&((Tab[Terme1].syn[VAL][i] & (INFERENCE|ORIGINE)) != (info & (INFERENCE|ORIGINE)))))) { /* fprintf(stderr, "==> %x ; %x\n", Tab[Terme1].syn[VAL][i], info); */
 i++ ;}
    /* Est-ce qu'on a atteint la fin du tableau
     */
    if (i<Tab[Terme1].nbsyn) {
      /* Oui donc on a trouve le bon lien
	 on retourne le booleen 'vrai'
      */
/*       printlog("ExisteLien OK\n"); */
      if (incrProd != 0) {
	Tab[Terme1].syn[PRODUCTIVITE][i] += incrProd;

	if (flux_ecriture != NO_OUTPUTSTREAM) {
	  if (Terme2 > 0) {
/* 	    fprintf(stderr, "(T) Terme1 : %d (%s) / Terme2 : %d (%s) ; Productivite : %d (%d)\n", CodeIndTete(Terme1), Tab[Terme1].lemme, Terme2, Tab[DecodeIndTete(Terme2)].lemme, Tab[Terme1].syn[PRODUCTIVITE][i], flux_ecriture); */
	    send_production(flux_ecriture,  CodeIndTete(Terme1), Tab[Terme1].lemme, Terme2, Tab[DecodeIndTete(Terme2)].lemme, Tab[Terme1].syn[PRODUCTIVITE][i]);
	  } else {
/* 	    fprintf(stderr, "(E) Terme1 : %d (%s) / Terme2 : %d (%s) ; Productivite : %d\n", CodeIndExp(Terme1), Tab[Terme1].lemme, Terme2, Tab[DecodeIndExp(Terme2)].lemme, Tab[Terme1].syn[PRODUCTIVITE][i]); */
	    send_production(flux_ecriture,  CodeIndExp(Terme1), Tab[Terme1].lemme, Terme2, Tab[DecodeIndExp(Terme2)].lemme, Tab[Terme1].syn[PRODUCTIVITE][i]);
	  }
	}
      }
/*       fprintf(stderr,"ExisteLien (%d, %d)\n", CodeIndTete(Terme1), Terme2); */
/*       printlog("ExisteLien\n"); */
      return(EXISTE);
    }
    else {
      /* Non, donc le lien n'est pas dans le tableau,
	 on retourne le booleen 'faux'
      */
/*       fprintf(stderr,"ExisteLien PAS (%d, %d)\n", CodeIndTete(Terme1), Terme2); */
/*       printlog("ExisteLien PAS\n"); */
      return(EXISTE_PAS);
    }
}



/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation TETE associe a un candidat terme     *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Tableau des Indices du terme 1                        *
 *                                                                      *
 *             2. Taille du tableau 1                                   *
 *                                                                      *
 *             3. Tableau des Indice du terme 2                         *
 *                                                                      *
 *             4. Taille du tableau 2                                   *
 *                                                                      *
 *             5. Tableau des termes                                    *
 *                                                                      *
 *             6. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteUnLienParmiLesRefT(int *RefTerm1, int taille1, int *RefTerm2, int taille2, struct GrphTerms *Tab, int info) 
{
  int i=0, j=0;

/*   printlog("ExisteUnLienParmiLesRefT\n"); */

  if ((taille1 > 0) && (taille2 > 0)) {
    do {    
      j = 0;
      while((j<taille2) && (!ExisteLien(RefTerm1[i],CodeIndTete(RefTerm2[j]), Tab, info, NO_INCRPROD, NO_OUTPUTSTREAM))) {
	
	j++;
      }
      if (j == taille2) {
	i++;
      }
    }  while((i<taille1) && (j == taille2)) ;
    if (i<taille1) {
/*       printlog("Existe\n"); */
      return(EXISTE);
    } else {
/*       printlog("Existe pas (1)\n"); */
      return(EXISTE_PAS);
    }
  } else {
/*       printlog("Existe pas (2)\n"); */
    return(EXISTE_PAS);
  }
}


/************************************************************************
 *                                                                      *
 * Objectif : Verification qu'un lien existe deja ou non dans les       *
 *            tableaux de relation EXPANSION associe a un candidat      *
 *            terme                                                     *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Tableau des Indices du terme 1                        *
 *                                                                      *
 *             2. Taille du tableau 1                                   *
 *                                                                      *
 *             3. Tableau des Indice du terme 2                         *
 *                                                                      *
 *             4. Taille du tableau 2                                   *
 *                                                                      *
 *             5. Tableau des termes                                    *
 *                                                                      *
 *             6. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Retour : 0 (le lien n'existe pas) ou 1 (le lien existe)              *
 *          -1 les deux termes sont identiques                          *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 ************************************************************************/

int ExisteUnLienParmiLesRefE(int *RefTerm1, int taille1, int *RefTerm2, int taille2, struct GrphTerms *Tab, int info) 
{
  int i=0, j=0;

  if ((taille1 > 0) && (taille2 > 0)) {
    do {    
      j = 0;
      while((j<taille2) && (!ExisteLien(RefTerm1[i],CodeIndExp(RefTerm2[j]), Tab, info, NO_INCRPROD, NO_OUTPUTSTREAM))) {
	
	j++;
      }
      if (j == taille2) {
	i++;
      }
    }  while((i<taille1) && (j == taille2)) ;
    if (i<taille1) {
      return(EXISTE);
    } else {
      return(EXISTE_PAS);
    }
  } else {
    return(EXISTE_PAS);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Arguments : 1. Tableau des termes                                    *
 *                                                                      *
 *             2. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             3. Indice du terme 2 (code)                              *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Date de derniere modification : 18/10/2003                           *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 ************************************************************************/

int AjoutRef(struct GrphTerms * Tab, int Terme1, int Terme2, int info)
{
    /* Allocation de la memoire pour le lien
       (Prise en compte du cas initial)
    */
/*    printlog("Debut AjoutRef\n");  */
    if (Tab[Terme1].nbsyn) {
      /* Le nombre de lien n'est pas nul, donc on est dans le cas initial
       */
      /* Tableau des references
       */
/*       printlog("Passe (0)\n"); */
/*       printlog("nbsyn : %d\n", Tab[Terme1].nbsyn); */
      if ((Tab[Terme1].syn[REF]=(int *)realloc(Tab[Terme1].syn[REF], (Tab[Terme1].nbsyn + 1)*sizeof(int)))==NULL) {
	fprintf(stderr,"Erreur de reallocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* Tableau de l'origine et de la validation du lien
       */
/*       printlog("nbsyn : %d (2)\n", Tab[Terme1].nbsyn); */
      if ((Tab[Terme1].syn[VAL]=(int *)realloc(Tab[Terme1].syn[VAL], (Tab[Terme1].nbsyn + 1)*sizeof(int))) == NULL) {
	fprintf(stderr,"Erreur de reallocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* Array of the production information
       */
/*       printlog("nbsyn : %d (3)\n", Tab[Terme1].nbsyn); */
      if ((Tab[Terme1].syn[PRODUCTIVITE]=(int *)realloc(Tab[Terme1].syn[PRODUCTIVITE], (Tab[Terme1].nbsyn + 1)*sizeof(int))) == NULL) {
	fprintf(stderr,"Erreur de reallocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
/*       printlog("nbsyn : %d (4)\n", Tab[Terme1].nbsyn); */
    }
    else {
      /* Le nombre de lien est nul, donc on est dans le cas initial
       */
      /* Tableau des references
       */
/*       printlog("Passe (1)\n"); */
      if ((Tab[Terme1].syn[REF]=(int *)malloc(sizeof(int)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* Tableau de l'origine et de la validation du lien
       */
      if ((Tab[Terme1].syn[VAL]=(int *)malloc(sizeof(int)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      /* Array of the production information
       */
      if ((Tab[Terme1].syn[PRODUCTIVITE]=(int *)malloc(sizeof(int)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
    }
    /* On ajoute la reference
     */
/*       printlog("Passe (2b) Terme1 = %d ; Terme2  %d\n", Terme1, Terme2); */
    Tab[Terme1].syn[REF][Tab[Terme1].nbsyn] = Terme2;
    /* On ajoute les informations concernant la validation, l'origine du lien
     */
    Tab[Terme1].syn[VAL][Tab[Terme1].nbsyn] = abs(info);

    /* The production is intialised at zero */
    Tab[Terme1].syn[PRODUCTIVITE][Tab[Terme1].nbsyn] = 1;
    /* Enfin, on augmente le nombre de lien attache a ce candidat terme
     */
    Tab[Terme1].nbsyn++;
/*     printlog("nbsyn = %d\n", Tab[Terme1].nbsyn); */

    return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Arguments : 1. Tableau des termes 1                                  *
 *                                                                      *
 *             2. Indice du terme 1 (decode)                            *
 *                                                                      *
 *             3. Tableau des termes 2                                  *
 *                                                                      *
 *             4. Indice du terme 2 (code)                              *
 *                                                                      *
 *             4. Ensemble des informations qui doivent apparaitre pour *
 *                qu'on puisse considerer que le lien existe            *
 *                                                                      *
 * Date de derniere modification : 06/10/2003                           *
 *                                                                      *
 * Retour : L'etat de l'execution de la fonction                        *
 *                                                                      *
 ************************************************************************/

int AjoutLien(struct GrphTerms * Tab1, int Terme1, int CodeIndTerme1, struct GrphTerms * Tab2, int Terme2, int CodeIndTerme2, int info) 
{
  int indFichierLstLT;

  if (Hcur - 3 >= 0) {
    indFichierLstLT = Historique[Hcur - 3];
  } else {
    indFichierLstLT = Historique[0];
  }

  /* printlog("indFichierLstLT : %d\n", indFichierLstLT); */
  /* printlog("Hcur : %d\n", Hcur); */
  /* printlog("TERMLIST : %d\n", TERMLIST); */
  /* printlog("type (0) : %d (%d - %s)\n", Historique[0], liste_fichier[Historique[0]].type, liste_fichier[Historique[0]].nom); */
  /* printlog("type (1) : %d (%d - %s)\n", Historique[1], liste_fichier[Historique[1]].type, liste_fichier[Historique[1]].nom); */
  /* printlog("type (2) : %d (%d - %s)\n", Historique[2], liste_fichier[Historique[2]].type, liste_fichier[Historique[2]].nom); */

/*   printlog("AjoutLien %d, %s, (%d), %d, %s, (%d)\n",  Terme1, Tab1[Terme1].lemme, Tab1[Terme1].nbdecomp, Terme2, Tab2[Terme2].lemme, Tab2[Terme2].nbdecomp); */

/*   printlog ("info = %d, %d\n", info, info & INFERENCE); */
/*   if (((info & INFERENCE) == 0)) { */
/*     printlog("DICOSYN"); */
/*   } */

  if (liste_fichier[indFichierLstLT].type == TERMLIST) {
    /* printlog("PasseAjout\n"); */
    /* printlog("info & INFERENCE : %d\n", info & INFERENCE); */
    /* printlog("Valide ? : %d\n",Valide_Pour_AjoutLien(Tab1[Terme1].donneesLexter, Tab2[Terme2].donneesLexter, liste_fichier[indFichierLstLT].nom)); */
    if (((info & INFERENCE) != 0) && (!Valide_Pour_AjoutLien(Tab1[Terme1].donneesLexter, Tab2[Terme2].donneesLexter, liste_fichier[indFichierLstLT].info_fichier->fichname))) {
      /* printlog("non valide\n"); */
      return(EXEC_NO);
    } else {
      /* printlog("valide\n"); */
/*     printlog("PasseAjout2\n"); */
    }
  }
/*     printlog("PasseAjout3\n"); */
  if (AjoutRef(Tab1, Terme1, CodeIndTerme2, info) != EXEC_OK) {
    fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 2 %d (%s) dans le Terme1 %d (%s)\n", Terme2, Tab2[Terme2].lemme, Terme1, Tab1[Terme1].lemme);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
    
  }
  /* On recommence pour le second candidat terme
   */
  if (AjoutRef(Tab2, Terme2, CodeIndTerme1, info) != EXEC_OK) {
    fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 1 %d (%s) dans le Terme 2 %d (%s)\n", Terme1, Tab1[Terme1].lemme, Terme2, Tab2[Terme2].lemme);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_RENVOI_STD);
  }
  return(EXEC_OK);
  
}

 

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre un candidat terme en position tete  *
 *            et un candidat terme en position expansion                *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Indication si le premier candidat terme est en        *
 *                position expansion et le second en position tete      *
 *                ou si le premier candidat terme est en tete et le     *
 *                second est en position expansion                      *
 *                                                                      *
 *             4. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute (1) ou non (0)           *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienTE(int Terme1, int Terme2, int type,int info, int incrProd, int flux_ecriture)
{
  /* On regarde quel type de lien on veut ajouter TE ou ET
   */
/*   printlog("Dans AjoutLienTE (%d)\n", info); */
  if (type) {
    /* On ajoute un lien entre un premier CT en position expansion
       et second CT en position tete
     */
    /* Verification si le lien existe deja ou non
     */
      if ((Terme1 != Terme2)&&(!ExisteLien(Terme1,CodeIndTete(Terme2), Lst_Termes.lst_exps,info, incrProd, flux_ecriture))) {
	/* Le lien n'existe pas, on l'ajoute
	 */
	return(AjoutLien(Lst_Termes.lst_exps, Terme1, CodeIndExp(Terme1), Lst_Termes.lst_tetes, Terme2, CodeIndTete(Terme2), info));
/* 	if (AjoutRef(Lst_Termes.lst_exps, Terme1, CodeIndTete(Terme2), info) != EXEC_OK) { */
/* 	  fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 2 %d (%s) dans le Terme1 %d (%s)\n", Terme2, Lst_Termes.lst_tetes[Terme2].lemme, Terme1, Lst_Termes.lst_exps[Terme1].lemme); */
/* 	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/* 	  return(ERR_RENVOI_STD); */

/* 	} */
	/* On recommence pour le second candidat terme
	 */
/* 	if (AjoutRef(Lst_Termes.lst_tetes, Terme2, CodeIndExp(Terme1), info) != EXEC_OK) { */
/* 	  fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 1 %d (%s) dans le Terme 2 %d (%s)\n", Terme1, Lst_Termes.lst_exps[Terme1].lemme, Terme2, Lst_Termes.lst_tetes[Terme2].lemme); */
/* 	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/* 	  return(ERR_RENVOI_STD); */
/* 	} */
/* 	return(EXEC_OK); */
      }
    else {
      /* Il y a eu un probleme quelque part, on retourne la valeur 0
       */
      return(EXISTE);
    }
  }
  else {
    /* On ajoute un lien entre un premier CT en position tete
       et second CT en position expansion
    */
    /* Verification si le lien existe deja ou non
     */
    if ((Terme1 != Terme2)&&(!ExisteLien(Terme1,CodeIndExp(Terme2),Lst_Termes.lst_tetes,info, incrProd, flux_ecriture))) {
      /* Le lien n'existe pas, on l'ajoute
       */
      return(AjoutLien(Lst_Termes.lst_tetes, Terme1, CodeIndTete(Terme1), Lst_Termes.lst_exps, Terme2, CodeIndExp(Terme2), info));
/* 	if (AjoutRef(Lst_Termes.lst_tetes, Terme1, CodeIndExp(Terme2), info) != EXEC_OK) { */
/* 	  fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 2 %d (%s) dans le Terme 1 %d (%s)\n", Terme2, Lst_Termes.lst_exps[Terme2].lemme, Terme1, Lst_Termes.lst_tetes[Terme1].lemme); */
/* 	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/* 	  return(ERR_RENVOI_STD); */
/* 	} */
      /* On recommence pour le second candidat terme
       */
/* 	if (AjoutRef(Lst_Termes.lst_exps, Terme2, CodeIndTete(Terme1), info) != EXEC_OK) { */
/* 	  fprintf(stderr,"Erreur lors de l'ajout de la reference du Terme 1 %d (%s) dans le Terme 2 %d (%s)\n", Terme1, Lst_Termes.lst_tetes[Terme1].lemme, Terme2, Lst_Termes.lst_exps[Terme2].lemme); */
/* 	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/* 	  return(ERR_RENVOI_STD); */
/* 	} */
/*       return(EXEC_OK); */
    }
    else {
      return(EXISTE);
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre deux candidats termes en position   *
 *            expansion                                                 *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute (1) ou non (0)           *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienE(int E1, int E2, int info, int incrProd, int flux_ecriture)
{
  /* Verification si le lien existe deja ou non
   */
/*    printlog("Dans AjoutLienE (%d - %d)\n", E1, E2); */
  if ((E1 != E2) && ((!ExisteLien(E1,CodeIndExp(E2),Lst_Termes.lst_exps,info, incrProd, flux_ecriture)))) {
    /* Le lien n'existe pas, on l'ajoute
     */
/*     printlog("AjoutLienE OK\n"); */
    return(AjoutLien(Lst_Termes.lst_exps, E1, CodeIndExp(E1), Lst_Termes.lst_exps, E2, CodeIndExp(E2), info));
/*     if (AjoutRef(Lst_Termes.lst_exps,E1, CodeIndExp(E2), info) != EXEC_OK) { */
/*       fprintf(stderr,"Erreur lors de l'ajout de la reference de l'expansion 2 %d (%s) dans l'expansion 1 %d (%s)\n", E2, Lst_Termes.lst_exps[E2].lemme, E1, Lst_Termes.lst_exps[E1].lemme); */
/*       fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*       return(ERR_RENVOI_STD);   */
/*     } */
    
/*     /\* On recommence pour le second candidat terme */
/*      *\/ */
/*     if (AjoutRef(Lst_Termes.lst_exps,E2, CodeIndExp(E1), info) != EXEC_OK) { */
/*       fprintf(stderr,"Erreur lors de l'ajout de la reference de l'expansion 1 %d (%s) dans l'expansion 2 %d (%s)\n", E1, Lst_Termes.lst_exps[E1].lemme, E2, Lst_Termes.lst_exps[E2].lemme); */
/*       fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*       return(ERR_RENVOI_STD);   */
/*     } */
/*     return(EXEC_OK); */
  }
  else {
    /* Il y a eu un probleme quelque part, on retourne la valeur 0
     */
    return(EXISTE);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien entre deux candidats termes en position   *
 *            tete                                                      *
 *                                                                      *
 *            Les deux termes doivent être différents                   *
 *                                                                      *
 * Arguments : 1. Premier candidat terme (decode)                       *
 *                                                                      *
 *             2. Second candidat terme (decode)                        *
 *                                                                      *
 *             3. Information a ajouter concernant le lien              *
 *                                                                      *
 * Retour : Indication si le lien a ete ajoute ou non                   *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int AjoutLienT(int T1, int T2, int info, int incrProd, int flux_ecriture)
{
  /* Verification si le lien existe deja ou non
   */
/*    fprintf(stderr,"Dans AjoutLienT (%d)\n", info); */
/*    printlog("Dans AjoutLienT (%d)\n", info); */
/*    printlog(" %d / %d\n", T1, T2); */
  if ((T1 != T2) && ((!ExisteLien(T1 , CodeIndTete(T2) , Lst_Termes.lst_tetes , info, incrProd, flux_ecriture)) /* || */
/* 		     (ExisteLien(T1 , CodeIndTete(T2) , Lst_Termes.lst_tetes , LIEN_EXTRAIT , 0, flux_ecriture) && (incrProd)) */
		     )) {
    /* Le lien n'existe pas, on l'ajoute
     */
/*     printlog("Ajout possible\n"); */
    return(AjoutLien(Lst_Termes.lst_tetes, T1, CodeIndTete(T1), Lst_Termes.lst_tetes, T2, CodeIndTete(T2), info));
/*     if (AjoutRef(Lst_Termes.lst_tetes,T1, CodeIndTete(T2), info) != EXEC_OK) { */
/*       fprintf(stderr,"Erreur lors de l'ajout de la reference de la tete 2 %d (%s) dans la tete 1 %d (%s)\n", T2, Lst_Termes.lst_tetes[T2].lemme, T1, Lst_Termes.lst_tetes[T1].lemme); */
/*       fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*       return(ERR_RENVOI_STD);   */
/*     } */
    
/*     /\* On recommence pour le second candidat terme */
/*      *\/ */
/*     if (AjoutRef(Lst_Termes.lst_tetes,T2, CodeIndTete(T1), info) != EXEC_OK) { */
/*       fprintf(stderr,"Erreur lors de l'ajout de la reference de la tete 1 %d (%s) dans la tete 2 %d (%s)\n", T1, Lst_Termes.lst_tetes[T1].lemme, T2, Lst_Termes.lst_tetes[T2].lemme); */
/*       fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*       return(ERR_RENVOI_STD);   */
/*     } */
/*     printlog("Fin AjoutLienT\n"); */
/*     return(EXEC_OK); */
  }
  else {
    return(EXISTE);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Construction du graphe des candidats termes               *
 *                                                                      *
 * Arguments : 1. Numero du fichier du corpus dans la table des         *
 *                fichiers                                              *
 *                                                                      *
 *             2. Nombre de groupes nominaux maximaux                   *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int ConstructGraphe(int Fichct,int nbgnmax)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/
  FILE *FichEnt;          /* Flux sur le fichier d'entete */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Ecriture dans le journal de bord
   */
  printlog("Debut de la contruction de la matrice\n");

  /* On determine si le traitement sera effectue en typographie
     pauvre ou non
   */
  typoC = typoC || liste_fichier[Fichct].typo;

  /* Construction de la matrice a partir de la lecture des fichiers du corpus
   */
  if (ConstrMatrice(Fichct,nbgnmax) != EXEC_OK) {
    fprintf(stderr,"Erreur lors de la construction de la matrice\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_TRAITMT);
  }

  /* Ecriture dans le journal de bord
   */
  printlog("Fin de construction de la matrice\n");

  /* Positionnement de la variable CG  qui indique que le graphe des
     candidats termes est construit
   */

  initTableauTypeLien();

  CG=CONSTRUIT;
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Destruction de la matrice (graphe des candidats termes)   *
 *                                                                      *
 * Argument : Aucun                                                     *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *                                                                      *
 * Date de derniere modification : 19/07/2004                           *
 *                                                                      *
 ************************************************************************/

int DestructionMat()
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* compteur sur les listes des tetes et des expansions, 
	    dans la boucle d'iteration 
	  */

  int j; /* variable non utilisee */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Positionnement a faux de la variable indiquant la typographie 
     du corpus
   */
  typoC = 0;
  /* Positionnement a faux de la variable indiquant la typographie 
     du dictionnaire
   */
  typoD = 0;

  /* Ecriture dans le journal de bord 
   */
  printlog("Destruction de la matrice\n");

  /* 
   */
  for(i=0;i<Lst_Termes.nb_tetes;i++) {
      free(Lst_Termes.lst_tetes[i].lemme);
      if (Lst_Termes.lst_tetes[i].nbdecomp) {
	  free(Lst_Termes.lst_tetes[i].decomp[TETE]);
	  free(Lst_Termes.lst_tetes[i].decomp[EXPANSION]);
      }
      if (Lst_Termes.lst_tetes[i].nbcompdep) {
	  free(Lst_Termes.lst_tetes[i].compdep[COMP]);
	  free(Lst_Termes.lst_tetes[i].compdep[DEP]);
      }

      if (Lst_Termes.lst_tetes[i].nbsyn) {
	free(Lst_Termes.lst_tetes[i].syn[REF]); /* PROBLEME */
	free(Lst_Termes.lst_tetes[i].syn[VAL]); /* PROBLEME */
      }
      if (Lst_Termes.lst_tetes[i].donneesLexter!=NULL) {
	  free(Lst_Termes.lst_tetes[i].donneesLexter);
      }
  }

  for(i=0;i<Lst_Termes.nb_exps;i++) {
      free(Lst_Termes.lst_exps[i].lemme);
      if (Lst_Termes.lst_exps[i].nbdecomp) {
	  free(Lst_Termes.lst_exps[i].decomp[TETE]);
	  free(Lst_Termes.lst_exps[i].decomp[EXPANSION]);
      }
      if (Lst_Termes.lst_exps[i].nbcompdep) {
	  free(Lst_Termes.lst_exps[i].compdep[COMP]);
	  free(Lst_Termes.lst_exps[i].compdep[DEP]);
      }
      if (Lst_Termes.lst_exps[i].nbsyn) {
	  free(Lst_Termes.lst_exps[i].syn[REF]);
	  free(Lst_Termes.lst_exps[i].syn[VAL]);
      }
      if (Lst_Termes.lst_exps[i].donneesLexter!=NULL) {
	  free(Lst_Termes.lst_exps[i].donneesLexter);
      }
  }

  /* Liberation de la memoire allouee aux tableaux des 
     candidats termes en tete et en expansion */
  free(Lst_Termes.lst_tetes);
  free(Lst_Termes.lst_exps);

  /* Positionnement a faux de la variable indiquant que le graphe 
     des candidats termes
   */
  CG = 0;
  /* Ecriture dans le journal de bord 
   */
  printlog("Fin de la destruction\n");
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : l'identifiant de la relation donné sous forme d'un entier *
 * est transformé en une chaine de caractères                           *
 *                                                                      *
 * Argument : le code de l'identifiant de la relation sous forme d'un   *
 *            entier                                                    *
 *                                                                      *
 * Retour : Etat d'execution de la fonction                             *
 *          Code de l'identification de la relation passé par adresse   *
 *                                                                      *
 * Note : Fonction à réécrire pour la généraliser à n'importe quel type *
 *        de la relation (à l'aide d'un tableau)                        *
 *                                                                      *
 * Date de derniere modification : 20/07/2004                           *
 *                                                                      *
 ************************************************************************/

int valint2valch(int val, char *chval)
{
  int val_dec = (val & TYPELIEN) >> 8;

/*   fprintf(stderr, "val_dec : %d\n", val_dec); */
/*   fprintf(stderr, "tab_typeLien.size : %d\n", tab_typeLien.size); */

  if ((val_dec > 0) && (val_dec < tab_typeLien.size)) {
    strcpy(chval, tab_typeLien.TypesLien[val_dec - 1]);
  } else {
    strcpy(chval,NON_VAL_CHAR);
  }

/*   switch(val & 0x0F00){ */
/*   case NON_VAL : strcpy(chval,NON_VAL_CHAR); */
/*     break; */
/*   case VAL_SYN : strcpy(chval,VAL_SYN_CHAR); */
/*     break; */
/*   case VAL_HYPO : strcpy(chval,VAL_HYPO_CHAR); */
/*     break; */
/*   case VAL_MERO : strcpy(chval,VAL_MERO_CHAR); */
/*     break; */
/*   case VAL_HYPER : strcpy(chval,VAL_HYPER_CHAR); */
/*     break; */
/*   case VAL_HOLO : strcpy(chval,VAL_HOLO_CHAR); */
/*     break; */
/*   case VAL_ANTO : strcpy(chval,VAL_ANTO_CHAR); */
/*     break; */
/*   case VAL_VA : strcpy(chval,VAL_VA_CHAR); */
/*     break; */
/*   default : strcpy(chval,NON_VAL_CHAR); */
/*   } */
  return(EXEC_OK);
}


/************************************************************************
 *                                                                      *
 * Objectif : l'identifiant de la relation donné sous forme d'une       *
 * chaine de caractères est codé sous forme d'un entier                 *
 *                                                                      *
 * Argument : le code de l'identifiant de la relation sous forme d'une  *
 *            chaine de caractères                                      *
 *                                                                      *
 * Retour : Identifiant de la relation sous forme d'un entier           *
 *          NON_VAL en cas d'erreur                                     *
 *                                                                      *
 * Note : Fonction à réécrire pour la généraliser à n'importe quel type *
 *        de la relation (à l'aide d'un tableau)                        *
 *                                                                      *
 * Date de derniere modification : 20/07/2004                           *
 *                                                                      *
 ************************************************************************/

int valch2valint(char* chval)
{

  int i;

  if (strcmp(chval,NON_VAL_CHAR)==0) {
    return NON_VAL;
  }
  

  i = 0;
  while((i < tab_typeLien.size) && (strcmp(chval, tab_typeLien.TypesLien[i]) != 0)) {
    i++;
  }
  if (i < tab_typeLien.size) {
    return(i>>8);
  } else {
    return NON_VAL;
  }

/*   if (strcmp(chval,NON_VAL_CHAR)==0) { */
/*     return NON_VAL; */
/*   } */
/*   if (strcmp(chval,VAL_SYN_CHAR)==0) { */
/*     return VAL_SYN; */
/*   } */
/*   if (strcmp(chval,VAL_HYPO_CHAR)==0) { */
/*     return VAL_HYPO; */
/*   } */
/*   if (strcmp(chval,VAL_MERO_CHAR)==0) { */
/*     return VAL_MERO; */
/*   } */
/*   if (strcmp(chval,VAL_HYPER_CHAR)==0) { */
/*     return VAL_HYPER; */
/*   } */
/*   if (strcmp(chval,VAL_HOLO_CHAR)==0) { */
/*     return VAL_HOLO; */
/*   } */
/*   if (strcmp(chval,VAL_ANTO_CHAR)==0) { */
/*     return VAL_ANTO; */
/*   } */
/*   if (strcmp(chval,VAL_VA_CHAR)==0) { */
/*     return VAL_VA; */
/*   } else return NON_VAL; */
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument : 1. Flux d'enregistrement                                  *
 *                                                                      *
 *            2. Premier noeud du graphe                                *
 *                                                                      *
 *            3. Identifiant correspondant au noeud passé en second     *
 *               argument                                               *
 *                                                                      *
 *            4. Deuxième noeud du graphe                               *
 *                                                                      *
 *            5. Identifiant correspondant au noeud passé en quatrième  *
 *               argument                                               *
 *                                                                      *
 *            6. Validité du lien                                       *
 *                                                                      *
 * Retour : Etat de l'exécution de la fonction                          *
 *                                                                      *
 * Note : Revoir l'allocation de ch (et l'affichage)                    *
 *                                                                      *
 * Date de derniere modification : 20/07/2004                           *
 *                                                                      *
 ************************************************************************/

int EnregistrementLien(FILE *FluxEnr, struct GrphTerms * Elmt1, int id1, struct GrphTerms * Elmt2, int id2, int val)
{
  char *ch;
  char *chval;

  if ((chval=(char *)malloc(sizeof(char)*TAILLE_MAX_VAL))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  if ((ch=(char *)malloc(sizeof(char)*(BUF_SIZE)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  
  if (valint2valch(val, chval) != EXEC_OK) {
    fprintf(stderr,"Erreur de la traduction de l'identifiant de la validité de la relation\n");
    fprintf(stderr,"Identifiant inconnu\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ID_VAL_INCONNU);
  }
  
  if (Elmt1->decomp[EXPANSION][0]==Elmt2->decomp[EXPANSION][0]) {

    send_ch_res(FluxEnr, -1, 2, id1, id2, chval, Lst_Termes.lst_tetes[Elmt1->decomp[TETE][0]].lemme,  CodeIndTete(Elmt1->decomp[TETE][0]), Lst_Termes.lst_tetes[Elmt2->decomp[TETE][0]].lemme,  CodeIndTete(Elmt2->decomp[TETE][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val);

/*     sprintf(ch,"<LIEN ID=%d_%d TYPE=%s>\n<FROM R=2>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", id1, id2, chval, Lst_Termes.lst_tetes[Elmt1->decomp[TETE][0]].lemme,  CodeIndTete(Elmt1->decomp[TETE][0]), Lst_Termes.lst_tetes[Elmt2->decomp[TETE][0]].lemme,  CodeIndTete(Elmt2->decomp[TETE][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val); */
/*     fprintf(FluxEnr,ch); */
    fflush(FluxEnr); /* DEBUG */

    return(EXEC_OK);
  } else {

    if (Elmt1->decomp[TETE][0]==Elmt2->decomp[TETE][0]) {

    send_ch_res(FluxEnr, -1, 1, id1, id2, chval, Lst_Termes.lst_exps[Elmt1->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt1->decomp[EXPANSION][0]), Lst_Termes.lst_exps[Elmt2->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt2->decomp[EXPANSION][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val);

/*       sprintf(ch,"<LIEN ID=%d_%d TYPE=%s>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", id1, id2, chval, Lst_Termes.lst_exps[Elmt1->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt1->decomp[EXPANSION][0]), Lst_Termes.lst_exps[Elmt2->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt2->decomp[EXPANSION][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val); */
/*       fprintf(FluxEnr,ch); */
      fflush(FluxEnr); /* DEBUG */

      return(EXEC_OK);
    }
    else {
      send_ch_res(FluxEnr, -1, 3, id1, id2, chval, Lst_Termes.lst_tetes[Elmt1->decomp[TETE][0]].lemme, CodeIndTete(Elmt1->decomp[TETE][0]), Lst_Termes.lst_tetes[Elmt2->decomp[TETE][0]].lemme,  CodeIndTete(Elmt2->decomp[TETE][0]), Lst_Termes.lst_exps[Elmt1->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt1->decomp[EXPANSION][0]), Lst_Termes.lst_exps[Elmt2->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt2->decomp[EXPANSION][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val);

/*       sprintf(ch,"<LIEN ID=%d_%d TYPE=%s>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", id1, id2, chval, Lst_Termes.lst_tetes[Elmt1->decomp[TETE][0]].lemme, CodeIndTete(Elmt1->decomp[TETE][0]), Lst_Termes.lst_tetes[Elmt2->decomp[TETE][0]].lemme,  CodeIndTete(Elmt2->decomp[TETE][0]), Lst_Termes.lst_exps[Elmt1->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt1->decomp[EXPANSION][0]), Lst_Termes.lst_exps[Elmt2->decomp[EXPANSION][0]].lemme, CodeIndExp(Elmt2->decomp[EXPANSION][0]), Elmt1->lemme, Elmt1->donneesLexter, Elmt2->lemme, Elmt2->donneesLexter, val); */
/*       fprintf(FluxEnr,ch); */
      fflush(FluxEnr); /* DEBUG */

      return(EXEC_OK);
    }
  }
}

int Valide_Pour_AjoutLien(char *id_corpus1, char *id_corpus2, char *nom_LT) {

  int nb_id_c1, nb_id_c2, nb_id_lt1, nb_id_lt2;

/*   printlog("Valide\n"); */
  is_TermList(id_corpus1, nom_LT, &nb_id_c1, &nb_id_lt1);
  is_TermList(id_corpus2, nom_LT, &nb_id_c2, &nb_id_lt2);

  /* printlog("id_c1 = %d, id_c2 = %d\n", nb_id_c1, nb_id_c2); */
  /* printlog("id_lt1 = %d, id_lt2 = %d\n", nb_id_lt1, nb_id_lt2); */

  if ((nb_id_c1 != 0) && (nb_id_lt1 != 0)) {
    /* printlog("OK1\n"); */
    return(1);
  } else {
    if ((nb_id_c2 != 0) && (nb_id_lt2 != 0)) {
      /* printlog("OK2\n"); */
      return(1);
    } else {
      if (((nb_id_c1 != 0) && (nb_id_c2 != 0)) || ((nb_id_lt1 != 0) && (nb_id_lt2 != 0))) {
	/* printlog("NON\n"); */
	return(0);
      } else {
      /* printlog("OK3\n"); */
	return(1);
      }
    }
  }
}

/* indique si une l'identifiant correspondant à un identifiant de termlist ou non */

int is_TermList(char *id_corpus, char *nom_LT, int *nb_id_c, int *nb_id_lt) {

  char *p;
  char *q;
  char tmp[1024];

  /* printlog("Term : %s , %s\n", nom_LT,  id_corpus); */
  *nb_id_c = 0;
  *nb_id_lt = 0;

  /* XXX A revoir pour prendre plusieurs references à des phrases */

  p = id_corpus;
  while(*p != 0) {
    /* Recherche du début de l'identifiant */
    while((*p != 0) && (*p != ':')) {
      p++;
    }
    p+=2;
    /* Recherche de la fin de l'identifiant */
    q = p;
    while((*q != 0) && (*q != '$')) {
      q++;
    }
    strncpy(tmp, p, q - p);
    tmp[q-p] = 0;
    /* printlog("TMP : %d %s %d : id_lt %d, id_c %d\n", q-p, tmp, strncmp(p, nom_LT, q - p), *nb_id_lt, *nb_id_c); */
    if (strncmp(p, nom_LT, q - p) == 0) {
      (*nb_id_lt)++;
    } else {
      (*nb_id_c)++;
    }
    p = q;
  }
  /* printlog("id_lt %d, id_c %d \n", *nb_id_lt, *nb_id_c); */
  return(0);
}


/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement des resultats de l'inference               *
 *                                                                      *
 * Argument : 1. Nom du fihier contenant les resultats                  *
 *                                                                      *
 * Retour : Etat de l'exécution de la fonction                          *
 *                                                                      *
 * Date de derniere modification : 24/12/2003                           *
 *                                                                      *
 ************************************************************************/

int EnregLiensInf(char *Result)
{
  int i,j,existdsH;
  FILE *flux_enr;
  char *ch, *ch2;
  struct Donnees_Nom_Fichier *info_fichier_result;

  int ind_Fichier_ListeTermes;

/*   int indFichierLstCT; */
/*   int indFichierLstCT_IC; /\* corpus pour l'indexation contrôlée - gestion de l'historique *\/ */
/*   int indFichierLstLT; /\* liste des termes pour l'indexation contrôlée *\/  */

/*   indFichierLstLT = Historique[Hcur - 2]; */


  printlog("Enregistrement des liens inferes\n");

  if ((ch = (char *)malloc(sizeof(char)*(strlen(".db") + strlen(Result)+1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(ch,"%s.db",Result);
  printlog("Fichier contenant les resultats : %s\n", ch);
  if ((flux_enr = fopen(ch,"w")) == NULL) {
    fprintf(stderr,"Erreur d'ouverture du fichier %s\n",ch);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_OUVERTURE);
  }
  for(i=0;i<Lst_Termes.nb_tetes;i++) {
/*     printlog("Passe %d\n", Lst_Termes.lst_tetes[i].nbsyn); */
      for(j=0;j<Lst_Termes.lst_tetes[i].nbsyn;j++) {
	
/* 	printlog("%d, %d, %d\n",Lst_Termes.lst_tetes[i].syn[REF][j],Lst_Termes.lst_tetes[i].syn[REF][j],Lst_Termes.lst_tetes[i].syn[VAL][j]); */
	  if ((Lst_Termes.lst_tetes[i].syn[REF][j]>0)&&(i<(Lst_Termes.lst_tetes[i].syn[REF][j]-1))&&((Lst_Termes.lst_tetes[i].syn[VAL][j]&INFERENCE)!=NONINF)) {
	      /* reference a une TETE */
/* 	    if (liste_fichier[indFichierLstLT].type == TERMLIST) { */
/* 	      if (is_TermList(Lst_Termes.lst_tetes[i].donneesLexter, liste_fichier[indFichierLstLT].nom) ^ is_TermList(Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom)) { */
/* /\* 		printlog("%s <==> %s : oui\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
/* 		EnregistrementLien(flux_enr, &(Lst_Termes.lst_tetes[i]), CodeIndTete(i), &(Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1]), Lst_Termes.lst_tetes[i].syn[REF][j], Lst_Termes.lst_tetes[i].syn[VAL][j]); */
/* 	      } else { */
/* /\* 		printlog("%s <==> %s : non\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
		
/* 	      } */
/* 	    } else { */
	      EnregistrementLien(flux_enr, &(Lst_Termes.lst_tetes[i]), CodeIndTete(i), &(Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1]), Lst_Termes.lst_tetes[i].syn[REF][j], Lst_Termes.lst_tetes[i].syn[VAL][j]);
/* 	    } */
	  }
	  if ((Lst_Termes.lst_tetes[i].syn[REF][j]<0)&&(i<(Lst_Termes.lst_tetes[i].syn[REF][j]*-1)-1)&&((Lst_Termes.lst_tetes[i].syn[VAL][j]&INFERENCE)!=NONINF)) {
	      /* reference a une EXPANSION */
/* 	    if (liste_fichier[indFichierLstLT].type == TERMLIST) { */
/* 	      if (is_TermList(Lst_Termes.lst_tetes[i].donneesLexter, liste_fichier[indFichierLstLT].nom) ^ is_TermList(Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[i].syn[REF][j]*-1)-1].donneesLexter, liste_fichier[indFichierLstLT].nom)) { */
/* /\* 		printlog("%s <==> %s : oui\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
/* 	      EnregistrementLien(flux_enr, &(Lst_Termes.lst_tetes[i]), CodeIndTete(i), &(Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[i].syn[REF][j]*-1)-1]), Lst_Termes.lst_tetes[i].syn[REF][j], Lst_Termes.lst_tetes[i].syn[VAL][j]); */
/* 	      } else { */
/* /\* 		printlog("%s <==> %s : non\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
		
/* 	      } */
/* 	    } else { */
	      EnregistrementLien(flux_enr, &(Lst_Termes.lst_tetes[i]), CodeIndTete(i), &(Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[i].syn[REF][j]*-1)-1]), Lst_Termes.lst_tetes[i].syn[REF][j], Lst_Termes.lst_tetes[i].syn[VAL][j]);
/* 	    } */
	  }
      }
  }
  for(i=0;i<Lst_Termes.nb_exps;i++) {
    for(j=0;j<Lst_Termes.lst_exps[i].nbsyn;j++) {
      if ((Lst_Termes.lst_exps[i].syn[REF][j]<0)&&(i<(Lst_Termes.lst_exps[i].syn[REF][j]*-1)-1)&&((Lst_Termes.lst_exps[i].syn[VAL][j]&INFERENCE)!=NONINF)) {
	/* reference a une EXPANSION */
/* 	if (liste_fichier[indFichierLstLT].type == TERMLIST) { */
/* 	  if (is_TermList(Lst_Termes.lst_exps[i].donneesLexter, liste_fichier[indFichierLstLT].nom) ^ is_TermList(Lst_Termes.lst_exps[(Lst_Termes.lst_exps[i].syn[REF][j]*-1)-1].donneesLexter, liste_fichier[indFichierLstLT].nom)) { */
/* /\* 	    printlog("%s <==> %s : oui\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
/* 	    EnregistrementLien(flux_enr, &(Lst_Termes.lst_exps[i]), CodeIndExp(i), &(Lst_Termes.lst_exps[(Lst_Termes.lst_exps[i].syn[REF][j]*-1)-1]), Lst_Termes.lst_exps[i].syn[REF][j], Lst_Termes.lst_exps[i].syn[VAL][j]); */
/* 	  } else { */
/* 	    /\* 		printlog("%s <==> %s : non\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
		
/* 	  } */
/* 	} else { */
	  EnregistrementLien(flux_enr, &(Lst_Termes.lst_exps[i]), CodeIndExp(i), &(Lst_Termes.lst_exps[(Lst_Termes.lst_exps[i].syn[REF][j]*-1)-1]), Lst_Termes.lst_exps[i].syn[REF][j], Lst_Termes.lst_exps[i].syn[VAL][j]);
/* 	} */
      }
      if ((Lst_Termes.lst_exps[i].syn[REF][j]>0)&&(i<Lst_Termes.lst_exps[i].syn[REF][j]-1)&&((Lst_Termes.lst_exps[i].syn[VAL][j]&INFERENCE)!=NONINF)) {
	/* reference a une TETE */
/* 	if (liste_fichier[indFichierLstLT].type == TERMLIST) { */
/* 	  if (is_TermList(Lst_Termes.lst_exps[i].donneesLexter, liste_fichier[indFichierLstLT].nom) ^ is_TermList(Lst_Termes.lst_tetes[Lst_Termes.lst_exps[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom)) { */
/* /\* 	    printlog("%s <==> %s : oui\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
/* 	    EnregistrementLien(flux_enr, &(Lst_Termes.lst_exps[i]), CodeIndTete(i), &(Lst_Termes.lst_tetes[Lst_Termes.lst_exps[i].syn[REF][j]-1]), Lst_Termes.lst_exps[i].syn[REF][j], Lst_Termes.lst_exps[i].syn[VAL][j]); */
/* 	  } else { */
/* 	    /\* 		printlog("%s <==> %s : non\n", Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[i].syn[REF][j]-1].donneesLexter, liste_fichier[indFichierLstLT].nom); *\/ */
		
/* 	  } */
/* 	} else { */
	  EnregistrementLien(flux_enr, &(Lst_Termes.lst_exps[i]), CodeIndExp(i), &(Lst_Termes.lst_tetes[Lst_Termes.lst_exps[i].syn[REF][j]-1]), Lst_Termes.lst_exps[i].syn[REF][j], Lst_Termes.lst_exps[i].syn[VAL][j]);
/* 	} */
      }
    }
  }
  fclose(flux_enr);

  /* Creation de l'entete Pour FichRes*/
  if ((ch2 = (char *)malloc(sizeof(char)*(strlen(".db.ent") + strlen(Result)+1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(ch2,"%s.db.ent",Result);

  if ((flux_enr = fopen(ch2,"w")) == NULL) {
    fprintf(stderr,"Erreur d'ouverture du fichier %s\n",ch2);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_OUVERTURE);
  };
  fprintf(flux_enr,"Type = new\n");
  fprintf(flux_enr,"Typographie = 1\n");/* A modifier selon le corpus utilise */ /* A CORRIGER */
  fprintf(flux_enr,"Fichier valide = 0\n"); /* A CORRIGER */
  fprintf(flux_enr,"Fichiers Origines = ");

  /* On regarde s'il y a un fichier "liste de termes" */

  ind_Fichier_ListeTermes = -1;
  i = 0;
  while((i<Hcur) && (ind_Fichier_ListeTermes == -1)) {
    if (liste_fichier[Historique[i]].type == TERMLIST) {
      ind_Fichier_ListeTermes = i;
      printlog("There is a term list\n");
    }
    i++;
  }  

  for(i=0;i<Hcur;i++) {
    printlog("Fichier origine a enregistrer : %s\n",liste_fichier[Historique[i]].nom);
    if ((liste_fichier[Historique[i]].type == CORPUS)&&(existe_prec_Historique(Historique[i],i)==EXISTE_PAS)) {
      if (ind_Fichier_ListeTermes != -1) {
	fprintf(flux_enr,"%s+%s.db;",liste_fichier[Historique[i]].info_fichier->fichname,liste_fichier[Historique[ind_Fichier_ListeTermes]].info_fichier->fichname);
        printlog("Fichier corpus+termes : %s+%s.db\n",liste_fichier[Historique[i]].info_fichier->fichname,liste_fichier[Historique[ind_Fichier_ListeTermes]].info_fichier->fichname);
      } else {
	fprintf(flux_enr,"%s.db;",liste_fichier[Historique[i]].nom);
        printlog("Fichier corpus : %s.db\n",liste_fichier[Historique[i]].nom);
      }
    }
  }
  existdsH = 0;
  /*******************************/
  printlog("Fichier Result : %s\n",Result);
  for(i=0;i<Hcur;i++) {
    printlog("Fichier Result : %s\n",Result);
    printlog("Fichier a enregistrer : %s\n",liste_fichier[Historique[i]].nom);
    if ((liste_fichier[Historique[i]].type != CORPUS)&&(liste_fichier[Historique[i]].type != TERMLIST)&&(strcmp(liste_fichier[Historique[i]].nom,Result)!=0)&&(existe_prec_Historique(Historique[i],i)==EXISTE_PAS)) {
      fprintf(flux_enr,"%s.db;",liste_fichier[Historique[i]].info_fichier->fichname);
      printlog("Fichier enregistre : %s.db\n",liste_fichier[Historique[i]].info_fichier->fichname);
    } else {
      existdsH = 1;
    }
  }
/*   printlog("Fin (1)\n"); */
  fseek(flux_enr,-1L,SEEK_CUR);
  fprintf(flux_enr,"\n");
  fclose(flux_enr);
  if (existdsH == 0) {
    strcpy(ch2,Result);
    ch2[strlen(Result)-4] = 0;
    printlog("Fichier non corpus : %s\n", ch2);
    if ((info_fichier_result = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    if (Analyse_Nom_Fichier(ch, info_fichier_result) != EXEC_OK) {
      fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_RENVOI_STD);
    }
    charger_db(ch, ch, info_fichier_result);
    i = get_table(ch); /* Interet ?? */
    Historique[Hcur++] = i;
  }
  free(ch);
  free(ch2);
}


/************************************************************************
 *                                                                      *
 * Objectif : Chargement des liens inféré                               *
 *                                                                      *
 * Argument : 1. Informations utiles sur le nom du fichier              *
 *                                                                      *
 * Retour : L'etat de l'execution de la function                        *
 *                                                                      *
 * Note : Revoir la fonction pour la généraliser, ne doit pas regarder  *
 * l'origine des données avec des identifiants explicites mais prendre  *
 * ce qu'il y a dans le fichier en entrée                               *
 *                                                                      *
 * Date de derniere modification : 02/01/2004                           *
 *                                                                      *
 ************************************************************************/

int ChargLiensInf(int flux_ecriture)
{
  int numfichier;                                           /* Numéro de l'entrée du fichier dans l'historique */
  int nb_f_o;                                               /* Nombre de fichiers origine, à charger */
  int f_o;                                                  /* rang du fichier origine */
  int numf;                                                 /* numéro du fichier */
  char *tmpbase;                                            /* Nom temporaire */
  struct Donnees_Nom_Fichier *donnees_nom_fichier_origine;  /* informations sur le fichier origine */
  char *FichierGNM;                                         /* Nom du fichier contenant les groupes nominaux */
  char *FichierSyn;                                         /* Nom du fichier contenant les relations semantiques */
  char *FichierNew;                                         /* Nom du fichier contenant des nouvelles données */
  char *FichierRes;                                         /* Nom du fichier contenant les resultats */
  int ret_read;                                             /* contient la valeur de retour d'une lecture */
  int descSyn;                                              /* Descripteur du fichier contenant les relations semantiques */
  char entree[BUF_MOT_SZ],syn[BUF_MOT_SZ];
  char *tmp;
  int i,j;
  char c;

  numfichier = nbre_fichier -1;
  nb_f_o = liste_fichier[nbre_fichier-1].nb_fichiers_origines;


  /*********************************
   *                               *
   * Lecture des fichiers origines *
   *                               *
   *********************************/
  for(f_o=0;f_o<nb_f_o;f_o++) {
    /* Recherche si le fichier origine est déjà chargé */
    if ((numf = get_table(liste_fichier[numfichier].fichiers_origines[f_o]))==-1){
      printlog("Fichier %s non chargé en mémoire\n", liste_fichier[numfichier].fichiers_origines[f_o]);
      
      if ((donnees_nom_fichier_origine = (struct Donnees_Nom_Fichier *)malloc(sizeof(struct Donnees_Nom_Fichier)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      if (Analyse_Nom_Fichier(liste_fichier[numfichier].fichiers_origines[f_o], donnees_nom_fichier_origine) != EXEC_OK) {
	fprintf(stderr,"Erreur d'execution de la fonction d'analyse des noms de fichier\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_RENVOI_STD);
      }
      if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier_origine->dirname) + strlen(donnees_nom_fichier_origine->fichname) + 2)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      sprintf(tmpbase,"%s/%s",donnees_nom_fichier_origine->dirname,donnees_nom_fichier_origine->fichname);


      if (charger_db(liste_fichier[numfichier].fichiers_origines[f_o], tmpbase, donnees_nom_fichier_origine) == -1) {
	fprintf(stderr,"Erreur lors du chargement du fichier %s\n",tmpbase);
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_CHARGDB);
      }
      /*       recup_nom_ext(liste_fichier[numfichier].fichiers_origines[f_o],&tmpbase,&ExtBase); */
      /*       charger_db(liste_fichier[numfichier].fichiers_origines[f_o],tmpbase, info_fichier); */
      if ((numf = get_table(tmpbase)) == -1) {
	fprintf(stderr,"Erreur de recuperation du rang du fichier %s dans la table de fichiers\n",liste_fichier[numfichier].fichiers_origines[f_o]);
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_TABLE);
      }
      printlog("Rang du fichier %s : %d\n",liste_fichier[numf].nom,numf);
    }    

    /**********************************************************************/

    /* XXX A VOIR */

    /* Laisser le test ??? */
    /*     if (strcmp(liste_fichier[numfichier].fichiers_origines[f_o],Fichier)!=0) {
	   Historique[Hcur++] = numf;
	   } */

    /**********************************************************************/

    Historique[Hcur++] = numf;

    switch(liste_fichier[numf].type) {
    case CORPUS :
      printlog("Chargement d'un fichier CORPUS\n");
      /* construction de la matrice */
      if (CG) {
	j=0;
	/* XXX Test a revoir */ 
	while((j<Hcur)&&(Historique[j]!=numf)) j++;
	printlog("Hcur = %d, j = %d\n", Hcur, j);
	if (j>=Hcur) {
	  /* D'abord destruction de la matrice 
	   * en memoire qui ne correspond pas a celle du fichier desamb 
	   */
	  printlog("Destruction de la matrice\n");
	  DestructionMat();	  
	}
      }
      if ((FichierGNM = (char *)malloc(sizeof(char)*(strlen(liste_fichier[numf].nom) + sizeof(char) + strlen(EXT_GNM) + 1)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      sprintf(FichierGNM,"%s%c%s",liste_fichier[numf].nom,SEP_EXT,EXT_GNM);
      print_com_msg(ENVOI_FICHIER_RESULTAT,flux_ecriture,OUI);
      print_com_msg(FichierGNM,flux_ecriture,OUI);
      if (ConstructGraphe(numf, liste_fichier[numf].nbrgnmax) != EXEC_OK) {
	fprintf(stderr,"Erreur lors de la construction du graphe\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_TRAITMT);
      }
      free(FichierGNM);
      printlog("Fin du Chargement d'un fichier CORPUS\n");
      break;

    case SYN :
      printlog("Chargement d'un fichier SYN\n");
      typoD = liste_fichier[numf].typo;
      if ((FichierSyn = (char *)malloc(sizeof(char)*(strlen(liste_fichier[numf].nom) + sizeof(char) + strlen(EXT_GNM) + 1)))==NULL) {
	fprintf(stderr,"Erreur d'allocation memoire\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ALLOCATION);
      }
      sprintf(FichierSyn,"%s%c%s",liste_fichier[numf].nom,SEP_EXT,EXT_GNM);
      
      if ((descSyn=open(FichierSyn,O_RDONLY))==-1) {
	fprintf(stderr,"Erreur d'ouverture du fichier %s\n",tmp);
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_OUVERTURE);
      }
      ret_read = 1;
      
      while(ret_read){
	/* lecture de l'entree */
	i=0;
	do {
	  if ((ret_read = read(descSyn, &(entree[i]), sizeof(char))) < 0) {
	    fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return(ERR_LECTURE);
	  }
	  i++;
	}while((ret_read != 0)&&(entree[i-1]!=':'));
	entree[i-2]=0;
/* 	printlog("Entree = %s\n", entree); */
	/* lecture du synonyme */
	if ((ret_read = read(descSyn, &c, sizeof(char))) < 0) {
	  fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_LECTURE);
	}
	i=0;
	do {
	  if ((ret_read = read(descSyn, &(syn[i]), sizeof(char))) < 0) {
	    fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return(ERR_LECTURE);
	  }
	  i++;
	}while((ret_read != 0)&&(syn[i-1]!=':'));
	syn[i-2]=0;
/* 	printlog("Synonyme = %s\n", syn); */
	/* On passe le reste */
	
	do {
	  if ((ret_read = read(descSyn, &c, sizeof(char))) < 0) {
	    fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return(ERR_LECTURE);
	  }
	}while((ret_read != 0) && (c!='\n'));
	
	if (ret_read != 0) {
	  /*        fclose(FichSyn); */
	  /* Tri optimisant la recherche */
	  if ((tmp = (char *)malloc(sizeof(char)*(max_len(entree,syn) + 1)))==NULL) {
	    fprintf(stderr,"Erreur d'allocation memoire\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return(ERR_ALLOCATION);
	  }
	  if (strcmp(entree,syn)>0){
	    strcpy(tmp,entree);
	    strcpy(entree,syn);
	    strcpy(syn,tmp);
	  }
	  free(tmp);
	
	  /************************/
	  /* Ajouter le lien dans la matrice */
	  AjoutLien_ch(entree, syn, DICOSYN);
	}
	close(descSyn);
	free(FichierSyn);
	printlog("Fin de chargement d'un fichier SYN\n");
	break;

      case NEW : 
	printlog("Chargement d'un fichier NEW\n");
	if (strcmp(liste_fichier[numfichier].nom,liste_fichier[numf].nom)!=0) {
	  /* Analyse du fichier "SGML" */
	  if ((FichierNew = (char *)malloc(sizeof(char)*(strlen(liste_fichier[numf].nom) + sizeof(char) + strlen(EXT_GNM) + 1)))==NULL) {
	    fprintf(stderr,"Erreur d'allocation memoire\n");
	    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	    return(ERR_ALLOCATION);
	  }
	  sprintf(FichierNew,"%s%c%s",liste_fichier[numf].nom,SEP_EXT,EXT_GNM);
	  Charge_Res(FichierNew, flux_ecriture);
	}
	printlog("Fin de chargement d'un fichier NEW\n");
	break;
      default :
	writelogS("Erreur de typage du fichier");
      }
/*       free(donnees_nom_fichier_origine); */
    }
  }
  /* Analyse du fichier resultat */
  printlog("Chargement du fichier resultat\n");

  if ((FichierRes = (char *)malloc(sizeof(char)*(strlen(liste_fichier[numfichier].nom) + sizeof(char) + strlen(EXT_GNM) + 1)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  sprintf(FichierRes,"%s%c%s",liste_fichier[numfichier].nom,SEP_EXT,EXT_GNM);
  Charge_Res(FichierRes, flux_ecriture);
/*   if ((tmpbase = (char *)malloc(sizeof(char)*(strlen(donnees_nom_fichier->dirname) + strlen(donnees_nom_fichier->fichname) + 2)))==NULL) { */
/*     fprintf(stderr,"Erreur d'allocation memoire\n"); */
/*     fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*     return(ERR_ALLOCATION); */
/*       } */
/*   sprintf(tmpbase,"%s/%s",donnees_nom_fichier->dirname,donnees_nom_fichier->fichname); */

/*   if (charger_db(liste_fichier[numfichier].nom, tmpbase, donnees_nom_fichier) == -1) { */
/*     fprintf(stderr,"Erreur lors du chargement du fichier %s\n",tmpbase); */
/*     fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*     return(ERR_CHARGDB); */
/*   } */
  /*   Historique[Hcur++] = numfichier; */
  printlog("Fin du chargement du fichier resultat\n");

  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien dans le graphe à partir des lemmes des    *
 *            deux termes (entree et syn)                               *
 *                                                                      *
 * Argument : 1. lemme du premier terme ou mot                          *
 *                                                                      *
 *            2. lemme du deuxième terme ou mot                         *
 *                                                                      *
 *            3. Information sur le lien ajouté                         *
 *                                                                      *
 * Retour :  Etat de l'exécution de la fonction                         *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int AjoutLien_ch(char *entree, char *syn, int info)
{
  int indentree,indsyn;
  char *categ;

  if ((categ = (char *)malloc(sizeof(char)*(CAT_SIZE)))==NULL) {
    fprintf(stderr,"Erreur d'allocation memoire\n");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_ALLOCATION);
  }
  
/*   printlog("Debut AjoutLien_ch\n"); */
  categ[0]=0;

  indentree = recherche_entete_LemS(entree, categ, EXPANSION, 0, Lst_Termes.nb_exps-1);

  indsyn=0;
  while(indentree<Lst_Termes.nb_exps){
    if (indentree+1<Lst_Termes.nb_exps)
      indsyn = recherche_entete_LemS(syn, categ, EXPANSION, indentree+1, Lst_Termes.nb_exps-1);
    if (indsyn<Lst_Termes.nb_exps) {
      /* Ajout du lien */
      if (AjoutLienE(indentree, indsyn, info, NO_INCRPROD, NO_OUTPUTSTREAM)) {
      }
      categ[0]=0;
    }
    if ((!indentree) || ((indentree = recherche_entete_LemS(entree, categ, EXPANSION, 0, indentree-1)) > indentree -1)) {
      /* On se place a la fin du tableau
       */
      indentree=Lst_Termes.nb_exps;
    }
  }

  categ[0]=0;
  indentree = recherche_entete_LemS(entree, categ, TETE, 0, Lst_Termes.nb_tetes-1);
  indsyn=0;
  while(indentree<Lst_Termes.nb_tetes){
    if (indentree+1<Lst_Termes.nb_tetes)
      indsyn = recherche_entete_LemS(syn, categ, TETE, indentree+1, Lst_Termes.nb_tetes-1);

    if (indsyn<Lst_Termes.nb_tetes) {
      /* Ajout du lien */
      if (AjoutLienT(indentree, indsyn, info, NO_INCRPROD, NO_OUTPUTSTREAM) == EXEC_OK) {
      }
      categ[0]=0;
    }
    if ((!indentree) || ((indentree = recherche_entete_LemS(entree, categ, TETE, 0, indentree-1)) > indentree - 1)) {
      /* On se place a la fin du tableau
       */
      indentree=Lst_Termes.nb_tetes;
    }
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Ajout d'un lien dans le graphe à partir des indices des   *
 *            lemmes des deux termes (entree et syn)                    *
 *                                                                      *
 * Argument : 1. indice du lemme du premier terme ou mot (code)         *
 *                                                                      *
 *            2. indice du lemme du deuxième terme ou mot               *
 *                                                                      *
 *            3. Information sur le lien ajouté                         *
 *                                                                      *
 * Retour :  Etat de l'exécution de la fonction                         *
 *                                                                      *
 * Date de modification : 18/11/2003                                    *
 *                                                                      *
 ************************************************************************/

int AjoutLien_int(int indentree, int indsyn, int info)
{
  if (indentree>0) {
    if (indsyn>0){
      AjoutLienT(DecodeIndTete(indentree) , DecodeIndTete(indsyn), info, NO_INCRPROD, NO_OUTPUTSTREAM);
    } else {
      AjoutLienTE(DecodeIndTete(indentree), DecodeIndExp(indsyn), TE, info, NO_INCRPROD, NO_OUTPUTSTREAM);
    }
  } else {
    if (indsyn<0) {
      AjoutLienE(DecodeIndExp(indentree), DecodeIndExp(indsyn), info, NO_INCRPROD, NO_OUTPUTSTREAM);
    } else {
      AjoutLienTE(DecodeIndExp(indentree), DecodeIndTete(indsyn), ET, info, NO_INCRPROD, NO_OUTPUTSTREAM);
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Modification de la valeur de validité d'une relation      *
 *                                                                      *
 * Argument : 1. Nouvelle valeur                                        *
 *                                                                      *
 *            2. Type de relation                                       *
 *                                                                      *
 *            3. Identifiant du premier terme                           *
 *                                                                      *
 *            4. Identifiant du deuxième terme                          *
 *                                                                      *
 * Retour : Etat de l'exécution de la relation                          *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int Modif_Val(int nval, char *typel, int IdTI1,int IdTI2, int val_type)
{
  int idxSyn1, idxSyn2, tl;
  
  if (nval == 0) {
    tl = UNVAL;
  } else {
    tl = val_type; /*valch2valint(typel);*/
  }

/*   printlog("%d\n",tl); */

  if (IdTI1>0) {
/*   printlog("%d (%d)\n",IdTI1, DecodeIndTete(IdTI1)); */
/*   printlog("%s\n", Lst_Termes.lst_tetes[DecodeIndTete(IdTI1)].lemme); */
    if ((idxSyn1=IndexLien(Lst_Termes.lst_tetes[DecodeIndTete(IdTI1)].syn[REF],Lst_Termes.lst_tetes[DecodeIndTete(IdTI1)].nbsyn,IdTI2,Lst_Termes.lst_tetes[DecodeIndTete(IdTI1)].syn[VAL],0)) == ERR_PAS_TROUVE) {
      return(idxSyn1);
    }
    if ((Chg_Val(&(Lst_Termes.lst_tetes[DecodeIndTete(IdTI1)].syn[VAL][idxSyn1]), nval, tl)) != EXEC_OK) {
      fprintf(stderr,"Erreur lors du changement de valeur\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_CHG_VAL);
    }
    
    if (IdTI2>0){
/*   printlog("%d (%d)\n",IdTI2, DecodeIndTete(IdTI2)); */

/*   printlog("%s\n", Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].lemme); */
      if ((idxSyn2=IndexLien(Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[REF],Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].nbsyn,IdTI1,Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[VAL],0)) == ERR_PAS_TROUVE) {
	return(idxSyn1);
      }
      if ((Chg_Val(&(Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[VAL][idxSyn2]), nval, tl)) != EXEC_OK) {
	fprintf(stderr,"Erreur lors du changement de valeur\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_CHG_VAL);
      }
    } else {
      if ((idxSyn2=IndexLien(Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].syn[REF],Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].nbsyn,IdTI1,Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].syn[VAL],0)) == ERR_PAS_TROUVE) {
	return(idxSyn1);
      }
      if ((Chg_Val(&(Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].syn[VAL][idxSyn2]), nval, tl)) != EXEC_OK) {
	fprintf(stderr,"Erreur lors du changement de valeur\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_CHG_VAL);
      }
    }
  } else {
    if ((idxSyn1=IndexLien(Lst_Termes.lst_exps[DecodeIndExp(IdTI1)].syn[REF],Lst_Termes.lst_exps[DecodeIndExp(IdTI1)].nbsyn,IdTI2,Lst_Termes.lst_exps[DecodeIndExp(IdTI1)].syn[VAL],0)) == ERR_PAS_TROUVE) {
      return(idxSyn1);
    }
    if ((Chg_Val(&(Lst_Termes.lst_exps[DecodeIndExp(IdTI1)].syn[VAL][idxSyn1]), nval, tl)) != EXEC_OK) {
      fprintf(stderr,"Erreur lors du changement de valeur\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_CHG_VAL);
    }
    
    if (IdTI2<0) {
      if ((idxSyn2=IndexLien(Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].syn[REF],Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].nbsyn,IdTI1,Lst_Termes.lst_exps[DecodeIndExp(IdTI2)].syn[VAL],0)) == ERR_PAS_TROUVE) {
	return(idxSyn1);
      }
      if ((Chg_Val(&(Lst_Termes.lst_exps[(IdTI2*-1)-1].syn[VAL][idxSyn2]), nval, tl)) != EXEC_OK) {
	fprintf(stderr,"Erreur lors du changement de valeur\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_CHG_VAL);
      }
    } else {
      if ((idxSyn2=IndexLien(Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[REF],Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].nbsyn,IdTI1,Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[VAL],0)) == ERR_PAS_TROUVE) {
	return(idxSyn1);
      }
      if ((Chg_Val(&(Lst_Termes.lst_tetes[DecodeIndTete(IdTI2)].syn[VAL][idxSyn2]), nval, tl)) != EXEC_OK) {
	fprintf(stderr,"Erreur lors du changement de valeur\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_CHG_VAL);
      }
    }
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Recherche de l'identifiant de l'autre terme dans la liste *
 *            des relations du terme courant                            *
 *                                                                      *
 * Argument : 1. Tableau des références correspondant aux relations     *
 *                                                                      *
 *            2. Taille du tableau                                      *
 *                                                                      *
 *            3. Référence recherchée                                   *
 *                                                                      *
 *            4. Tableau de validité des relations                      *
 *                                                                      *
 *            5. information sur la relation                            *
 *                                                                      *
 * Retour : l'indice du terme en relation si trouvé                     *
 *          ERR_PAS_TROUVE sinon                                        *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int IndexLien(int *TabRef, int TailleTabRef, int Ref, int *TabVal, int info) 
{
  int i;
  
  i = 0;

/*   printlog("Ref = %d : TailleTabRef = %d\n", Ref,TailleTabRef); */
  while((i<TailleTabRef)&&((TabRef[i]!=Ref)||((TabVal[i] & INFERENCE) == (info & INFERENCE)))) {
    i++;
  };
  if (i<TailleTabRef) {
    return(i);
  }
  else {
/*     printlog("Index non trouve\n"); */
    fprintf(stderr,"Terme en relation pas trouvé %d\n", Ref);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    return(ERR_PAS_TROUVE);
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Changement de la valeur d'information associée à la       *
 *            relation                                                  *
 *                                                                      *
 * Argument : 1. Information courante associée à la relation            *
 *                                                                      *
 *            2. Type de modification (INITIALISATION ou ajout)         *
 *                                                                      *
 *            3. Type d'information sur la validité de la relation      *
 *                                                                      *
 * Retour : Etat de l'exécution de la relation                          *
 *                                                                      *
 * Date de modification : 20/07/2004                                    *
 *                                                                      *
 ************************************************************************/

int Chg_Val(int *infolien, int val, int type) 
{
  if (val == INITIALISATION){
    *infolien = (*infolien) & (~TYPELIEN);
  } else {
    *infolien = ((*infolien) & (~TYPELIEN)) | type;
  }

  

  return(EXEC_OK);
}

int AjoutTypeLien(char *typeL, int typeL_int)
{
/*   int typeLien_int = atoi($typeL); */
  
  /* tab_typeLien;*/
  
/*   printlog("typeL_int : %d\n", typeL_int >> 8); */
/*   printlog("typeL size : %d\n", tab_typeLien.size); */
 
  if ((typeL_int != UNVAL) && (typeL_int >> 8) >= tab_typeLien.size) {
    tab_typeLien.TypesLien = (char **) realloc(tab_typeLien.TypesLien, tab_typeLien.size * sizeof(char *));
    tab_typeLien.TypesLien[tab_typeLien.size] = (char *) malloc((strlen(typeL) + 1) * sizeof(char));
    strcpy(tab_typeLien.TypesLien[tab_typeLien.size],typeL);
    tab_typeLien.size++;
  }
  return(EXEC_OK);
}

int initTableauTypeLien(void)
{

  tab_typeLien.TypesLien = (char **) malloc(NB_TYPE_LIEN_INIT * sizeof(char *));

  tab_typeLien.TypesLien[0] = (char *) malloc((strlen("SYN") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[0],"SYN");

  tab_typeLien.TypesLien[1] = (char *) malloc((strlen("HYPO") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[1],"HYPO");

  tab_typeLien.TypesLien[2] = (char *) malloc((strlen("MERO") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[2],"MERO");

  tab_typeLien.TypesLien[3] = (char *) malloc((strlen("HYPER") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[3],"HYPER");

  tab_typeLien.TypesLien[4] = (char *) malloc((strlen("HOLO") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[4],"HOLO");

  tab_typeLien.TypesLien[5] = (char *) malloc((strlen("ANTO") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[5],"ANTO");

  tab_typeLien.TypesLien[6] = (char *) malloc((strlen("VA") + 1) * sizeof(char));
  strcpy(tab_typeLien.TypesLien[6],"VA");

  tab_typeLien.size = NB_TYPE_LIEN_INIT;

  return(EXEC_OK);
}
