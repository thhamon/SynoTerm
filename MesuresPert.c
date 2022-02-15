/* 	$Log: MesuresPert.c,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.3  2001/08/03 11:44:42  thierry
 * 	Ajout des commentaires
 *
 * 	Revision 1.2  2001/05/01 19:40:48  thierry
 * 	*** empty log message ***
 *	 */
#include "MesuresPert.h"

/***********************************************************************
 *                                                                     *
 * Calcul de mesures de pertinence (pour l'instant a l'etat de projet) *
 *                                                                     *
 ***********************************************************************/

/************************************************************************
 *                                                                      *
 * Contenu : Table contenant les candidats termes classes en fonction   *
 *           de la diversite de Shannon                                 *
 *                                                                      *
 ************************************************************************/

struct Classif_Div_S Tab_Classif_DS;

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la diversite de Shannon                         *
 *                                                                      *
 * Argument : Indication du calcul d'une diversite normalisee           *
 *                                                                      *
 ************************************************************************/

int Calcul_Diversite(int norm)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i_dep; /* index des candidats termes simples */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Calcul de la diversite de Shannon pour les termes simples en 
     position tete 
   */
  for(i_dep=0;i_dep<Lst_Termes.nb_tetes;i_dep++) {
      Calcul_Diversite_TS(i_dep, TETE, norm);
  }
  /* Calcul de la diversite de Shannon pour les termes simples en 
     position expansion
   */
  for(i_dep=0;i_dep<Lst_Termes.nb_exps;i_dep++) {
      Calcul_Diversite_TS(i_dep,EXPANSION, norm);
  }
  return(0);
}

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement de la diversite de Shannon dans un fichier *
 *                                                                      *
 * Argument : Nom du fichier                                            *
 *                                                                      *
 ************************************************************************/

int Affiche_Diversite(char *NomFichier)
{

  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i_dep; /* index des candidats termes */
  FILE *FluxFichier; /* Flux sur le fichier qui contiendra les resultats */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Ouverture du fichier en ecriture 
   */
  if ((FluxFichier = fopen(NomFichier, "w")) == 0 ) {
    fprintf(stderr,"Erreur d'ouverture du fichier %s\n", NomFichier);
    return(-1);
  }
  
  /* Enregistrement de la diversite de Shannon pour les candidats termes
     en position tete
   */
  for(i_dep=0;i_dep<Lst_Termes.nb_tetes;i_dep++) {
    /* Patron N (Prep) N2
     */
    fprintf(FluxFichier, "T : N1 (Prep) N2 : %s : %G\n",Lst_Termes.lst_tetes[i_dep].lemme, Lst_Termes.lst_tetes[i_dep].Mesures_Pertinence[DIVERSITE_SHANNON1]);
    /* Patron N Adj
     */
    fprintf(FluxFichier, "T : N Adj : %s : %G\n",Lst_Termes.lst_tetes[i_dep].lemme, Lst_Termes.lst_tetes[i_dep].Mesures_Pertinence[DIVERSITE_SHANNON2]);
  }
  /* Enregistrement de la diversite de Shannon pour les candidats termes
     en position expansion
   */
  for(i_dep=0;i_dep<Lst_Termes.nb_exps;i_dep++) {
    /* Patron N (Prep) N2
     */
    fprintf(FluxFichier, "E : N1 (Prep) N2 : %s : %G\n",Lst_Termes.lst_exps[i_dep].lemme, Lst_Termes.lst_exps[i_dep].Mesures_Pertinence[DIVERSITE_SHANNON1]);
    /* Patron N Adj
     */
    fprintf(FluxFichier, "E : N Adj : %s : %G\n",Lst_Termes.lst_exps[i_dep].lemme, Lst_Termes.lst_exps[i_dep].Mesures_Pertinence[DIVERSITE_SHANNON2]);
  }
  /* Fermeture du fichier
   */
  fclose(FluxFichier);
  return(0);
}

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la diversite de Shannon pour un terme simple    *
 *                                                                      *
 * Argument : 1. Index du terme simples                                 *
 *                                                                      *
 *            2. Indication s'il s'agit d'un candidat terme en position *
 *               tete ou expansion                                      *
 *                                                                      *
 *            3. Indication du calcul d'une diversite normalisee        *
 *                                                                      *
 ************************************************************************/

int Calcul_Diversite_TS(int indT,int pos,int norm)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int j_dep; /* Index des relations de dependance et de composition */
  int ProdReelle1, ProdReelle2; /* Productions (nombres d'occurrences) 
                                   totales et reelles des candidats termes */
  int cat; /* code de la categorie grammaticale */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  if (pos) { /* Position expansion */
    /* Initialisation pour le candidat terme
     */
    Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] = 0;
    Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] = 0;
    ProdReelle1 = 0;
    ProdReelle2 = 0;

    /* On calcule la production totale du candidat terme en fonction
       de la production des candidats termes dans lequel il est un composant
       (le pere)
     */
    for(j_dep=0;j_dep<Lst_Termes.lst_exps[indT].nbcompdep;j_dep++) {
      /* Recuperation de la categorie grammaticale du candidat terme
       */
      cat = Get_CatGram(Lst_Termes.lst_exps[indT].lemme);

      if ((cat == CAT_CT)||(cat == CAT_NOM)) {
	/* Cas ou le candidat terme est un candidat terme complexe 
	   ou un candidat terme simple et un nom
	 */
	if (Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] < 0) {
	  /* Le candidat terme pere est une expansion
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle1 += Lst_Termes.lst_exps[(Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] * -1)-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] -= nlogn(Lst_Termes.lst_exps[(Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] * -1)-1].Prod);

	} else {
	  /* Le candidat terme pere est une tete
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle1 += Lst_Termes.lst_tetes[Lst_Termes.lst_exps[indT].compdep[COMP][j_dep]-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] -= nlogn(Lst_Termes.lst_tetes[Lst_Termes.lst_exps[indT].compdep[COMP][j_dep]-1].Prod);
	}
      } else {
	/* Cas ou le candidat terme est ni un candidat terme complexe 
	   ni un candidat terme simple et un nom, c'est-a-dire un adjectif
	 */
	if (Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] < 0) {
	  /* Le candidat terme pere est une expansion
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle2 += Lst_Termes.lst_exps[(Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] * -1)-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] -= nlogn(Lst_Termes.lst_exps[(Lst_Termes.lst_exps[indT].compdep[COMP][j_dep] * -1)-1].Prod);
	} else {
	  /* Le candidat terme pere est une tete
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle2 += Lst_Termes.lst_tetes[Lst_Termes.lst_exps[indT].compdep[COMP][j_dep]-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] -= nlogn(Lst_Termes.lst_tetes[Lst_Termes.lst_exps[indT].compdep[COMP][j_dep]-1].Prod);
	}
      }
    }
    if (norm) {
      /* Calcul de la diversite de Shannon normalisee
       */
      if (ProdReelle1) {
	/* Verification que 'ProdReelle1' est bien differente de 0
	 */
	/* Normalisation 
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] /= ProdReelle1;
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] += log(ProdReelle1);
      }
      if (ProdReelle2) {
	/* Verification que 'ProdReelle2' est bien differente de 0
	 */
	/* Normalisation
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] /= ProdReelle2;
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] += log(ProdReelle2);
      }
    } else {
      /* Calcul du diversite de Shannon non normalise
       */
      if (ProdReelle1) {
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] += nlogn(ProdReelle1);
      }
      if (ProdReelle2) {
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_exps[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] += nlogn(ProdReelle2);
      }
    }

  } else { /* Position Tete */
    /* Initialisation pour le candidat terme
     */
    Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] = 0;
    Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] = 0;
    ProdReelle1 = 0;
    ProdReelle2 = 0;

    /* On calcule la production totale du candidat terme en fonction
       de la production des candidats termes dans lequel il est un composant
       (le pere)
     */
    for(j_dep=0;j_dep<Lst_Termes.lst_tetes[indT].nbcompdep;j_dep++) {
      /* Recuperation de la categorie grammaticale du candidat terme
       */
      cat = Get_CatGram(Lst_Termes.lst_exps[Lst_Termes.lst_tetes[indT].compdep[DEP][j_dep]].lemme);

      if ((cat == CAT_CT)||(cat == CAT_NOM)) {
	/* Cas ou le candidat terme est un candidat terme complexe 
	   ou un candidat terme simple et un nom
	 */
	if (Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] < 0) {
	  /* Le candidat terme pere est une expansion
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle1 += Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] * -1)-1].Prod;
	  
	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] -= nlogn(Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] * -1)-1].Prod);
	} else {
	  /* Le candidat terme pere est une tete
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle1 += Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep]-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] -= nlogn(Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep]-1].Prod);
	}
      } else {
	/* Cas ou le candidat terme est ni un candidat terme complexe 
	   ni un candidat terme simple et un nom, c'est-a-dire un adjectif
	 */
	if (Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] < 0) {
	  /* Le candidat terme pere est une expansion
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle2 += Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] * -1)-1].Prod;
	  
	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] -= nlogn(Lst_Termes.lst_exps[(Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep] * -1)-1].Prod);
	} else {
	  /* Le candidat terme pere est une tete
	   */
	  /* Calcul de la production reelle du candidat terme courant
	   */
	  ProdReelle2 += Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep]-1].Prod;

	  /* Calcul de la diversite de Shannon
	   */
	  Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] -= nlogn(Lst_Termes.lst_tetes[Lst_Termes.lst_tetes[indT].compdep[COMP][j_dep]-1].Prod);
	}
      }
    }
    if (norm) {
      /* Calcul de la diversite de Shannon normalisee
       */
      if (ProdReelle1) {
	/* Verification que 'ProdReelle1' est bien differente de 0
	 */
	/* Normalisation 
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] /= ProdReelle1;
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] += log(ProdReelle1);
      }
      if (ProdReelle2) {
	/* Verification que 'ProdReelle2' est bien differente de 0
	 */
	/* Normalisation
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] /= ProdReelle2;
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] += log(ProdReelle2);
      }
    } else {
      /* Calcul du diversite de Shannon non normalise
       */
      if (ProdReelle1) {
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON1] += nlogn(ProdReelle1);
      }
      if (ProdReelle2) {
	/* Ajout du n * log(n)
	 */
	Lst_Termes.lst_tetes[indT].Mesures_Pertinence[DIVERSITE_SHANNON2] += nlogn(ProdReelle2);
      }
    }
  }
  return(0);
} 

/************************************************************************
 *                                                                      *
 * Objectif : Calcul de n*log(n)                                        *
 *                                                                      *
 * Argument : valeur de n                                               *
 *                                                                      *
 ************************************************************************/

double nlogn(double n)
{
  if (n <= 0) {
    /* Si n est inferieur ou egal a 0
       le log n'est pas defini
     */
    return((double)0);
  }
  else {
    /* n est superieur strictement a 0
     */
    return(n * log(n));
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Classement des candidats termes en fonction de la valeur  * 
 *            de la diversite de Shannon, de la categorie grammaticale  * 
 *            et la position dans le candidat terme.                    *
 *                                                                      *
 ************************************************************************/

int Classif_Div_Shannon()
{
  /*************************************
   * Declaration des variables locales *
   *************************************/
  
  int max_N1, i_N1, j_N1, k_N1;          /* Indice de recherche de valeur maximale   */
  int max_N2, i_N2, j_N2, k_N2;          /* max_XX contient l'indice de la valeur    */
  int max_N, i_N = 0, j_N, k_N;          /* maximale courante de la diversite de     */
  int max_Adj, i_Adj, j_Adj, k_Adj;      /* Shannon                                  */
  int compteur_tetes, compteur_exps;     /* Indice des tableaux des tetes/expansions */
  int i;                                 /* Indice standard de parcours de table     */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* classification de la diversite des tetes 
   */

  /* Initialisation de la table qui contiendra le classement des
     candidats termes en fonction de la diversite de Shannon
   */
  for(i=0;i<MAX_ELT_DIV;i++) {
    Tab_Classif_DS.Div_N1[i] = -1;
    Tab_Classif_DS.Div_N[i] = -1;
    Tab_Classif_DS.Div_N2[i] = -1;
    Tab_Classif_DS.Div_Adj[i] = -1;
  }

  /* Initialisation des bornes superieures
   */
  max_N1 = 0;
  max_N = 0;

  /* Classification des candidats termes en position tete
   */
  for(compteur_tetes = 1;compteur_tetes<Lst_Termes.nb_tetes;compteur_tetes++) {
    if (Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON1] != 0) {
      /* La diversite de Shannon de tetes n'est pas nulle
       */
      /* Recherche de la valeur maximale courante
       */
      j_N1 = max_N1;
      i_N1 = 0;
      while(i_N1 != j_N1) {
	k_N1 = (i_N1 + j_N1) / 2;
	if ((Tab_Classif_DS.Div_N1[k_N1] !=-1)&&(Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON1] < Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[k_N1]].Mesures_Pertinence[DIVERSITE_SHANNON1])) {
	  i_N1 = k_N1 + 1;
	} else {
	  /* cas ou Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON1] 
	     <= Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[k_N1]].Mesures_Pertinence[DIVERSITE_SHANNON1] 
           */
	  j_N1 = k_N1;
	}
      }
      if (j_N1 < MAX_ELT_DIV) {
	/* Si l'indice est inferieur au nombre d'elements dans les tables
	   tout va bien
	 */
	/* On deplace les elements de la tabla afin de mettre le nouvel element
	 */
	for(i=max_N1-1;i>j_N1;i--) {
	  Tab_Classif_DS.Div_N1[i] = Tab_Classif_DS.Div_N1[i-1];
	}
	/* On place le nouvel element
	 */
	Tab_Classif_DS.Div_N1[j_N1] = compteur_tetes;
	if (max_N1 < MAX_ELT_DIV) {
	  /* Si la borne superieure est inferieure au nombre d'elements dans la table 
	     tout va bien
	   */
	  max_N1++;
	}
      }
    } else {
      if (Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON2] != 0) {
	/* La diversite de Shannon des expansions n'est pas nulle
	 */
	/* Recherche de la valeur maximale courante
	 */
	j_N = max_N;
	i_N = 0;
	while(i_N != j_N) {
	  k_N = (i_N + j_N) / 2;
	  if ((Tab_Classif_DS.Div_N[k_N] !=-1)&&(Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON2] < Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N[k_N]].Mesures_Pertinence[DIVERSITE_SHANNON2])) {
	    i_N = k_N + 1;
	  } else {
	    /* cas ou Lst_Termes.lst_tetes[compteur_tetes].Mesures_Pertinence[DIVERSITE_SHANNON2] 
	       <= Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[k_N1]].Mesures_Pertinence[DIVERSITE_SHANNON2] 
	     */
	    j_N = k_N;
	  }
	}
	if (j_N < MAX_ELT_DIV) {
	  /* Si l'indice est inferieur au nombre d'elements dans les tables
	     tout va bien
	   */
	  /* On deplace les elements de la tabla afin de mettre le nouvel element
	   */
	  for(i=max_N-1;i>j_N;i--) {
	    Tab_Classif_DS.Div_N[i] = Tab_Classif_DS.Div_N[i-1];
	  }
	  /* On place le nouvel element
	   */
	  Tab_Classif_DS.Div_N[j_N] = compteur_tetes;
	  if (max_N < MAX_ELT_DIV) {
	    /* Si la borne superieure est inferieure au nombre d'elements dans la table 
	       tout va bien
	     */
	    max_N++;
	  }
	}
      }
    }
  }

  /* Classification de la diversite des expansions 
   */

  /* Initialisation des bornes superieures
   */
  max_N2 = 0;
  max_Adj = 0;

  /* Classification des candidats termes en position tete
   */
  for(compteur_exps = 1;compteur_exps<Lst_Termes.nb_exps;compteur_exps++) {
    if (Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON1] != 0) {
      /* La diversite de Shannon de tetes n'est pas nulle
       */
      /* Recherche de la valeur maximale courante
       */
      j_N2 = max_N2;
      i_N2 = 0;
      while(i_N2 != j_N2) {
	k_N2 = (i_N2 + j_N2) / 2;
	if ((Tab_Classif_DS.Div_N2[k_N2] !=-1)&&(Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON1] < Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[k_N2]].Mesures_Pertinence[DIVERSITE_SHANNON1])) {
	  i_N2 = k_N2 + 1;
	} else {
	  /* cas ou Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON1] 
	     <= Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[k_N2]].Mesures_Pertinence[DIVERSITE_SHANNON1] 
           */
	  j_N2 = k_N2;
	}
      }
      if (j_N2 < MAX_ELT_DIV) {
	/* Si l'indice est inferieur au nombre d'elements dans les tables
	   tout va bien
	 */
	/* On deplace les elements de la tabla afin de mettre le nouvel element
	 */
	for(i=max_N2-1;i>j_N2;i--) {
	  Tab_Classif_DS.Div_N2[i] = Tab_Classif_DS.Div_N2[i-1];
	}
	/* On place le nouvel element
	 */
	Tab_Classif_DS.Div_N2[j_N2] = compteur_exps;
	if (max_N2 < MAX_ELT_DIV) {
	  /* Si la borne superieure est inferieure au nombre d'elements dans la table 
	     tout va bien
	   */
	  max_N2++;
	}
      }
    } else {
      if (Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON2] != 0) {
	/* La diversite de Shannon des expansions n'est pas nulle
	 */
	/* Recherche de la valeur maximale courante
	 */
	j_Adj = max_Adj;
	i_Adj = 0;
	while(i_Adj != j_Adj) {
	  k_Adj = (i_Adj + j_Adj) / 2;
	  if ((Tab_Classif_DS.Div_Adj[k_Adj] !=-1)&&(Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON2] < Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[k_Adj]].Mesures_Pertinence[DIVERSITE_SHANNON2])) {
	    i_Adj = k_Adj + 1;
	  } else {
	    /* cas ou Lst_Termes.lst_exps[compteur_exps].Mesures_Pertinence[DIVERSITE_SHANNON2] 
	       <= Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[k_Adj]].Mesures_Pertinence[DIVERSITE_SHANNON2] 
	     */
	    j_Adj = k_Adj;
	  }
	}
	if (j_Adj < MAX_ELT_DIV) {
	  /* Si l'indice est inferieur au nombre d'elements dans les tables
	     tout va bien
	   */
	  /* On deplace les elements de la tabla afin de mettre le nouvel element
	   */
	  for(i=max_Adj-1;i>j_Adj;i--) {
	    Tab_Classif_DS.Div_Adj[i] = Tab_Classif_DS.Div_Adj[i-1];
	  }
	  /* On place le nouvel element
	   */
	  Tab_Classif_DS.Div_Adj[j_Adj] = compteur_exps;
	  if (max_Adj < MAX_ELT_DIV) {
	    /* Si la borne superieure est inferieure au nombre d'elements dans la table 
	       tout va bien
	     */
	    max_Adj++;
	  }
	}
      }
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Enregistrement  dans un fichier, des candidats termes     *
 *            classes suivant la diversite de Shannon                   *
 *                                                                      *
 * Argument : Nom du fichier                                            *
 *                                                                      *
 ************************************************************************/

int Affiche_Classif_Div_Shannon(char *NomFichier)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i;                  /*  */
  FILE *FluxFichier;      /* Flux sur le fichier contenant les valeurs de la
			     diversite de Shannon
			   */
  int nb1;                /* Nombre dl'element dont la diversite est definie
			     et differente de de 0 */
  int nb2;                /* Nombre dl'element quelle que soit la valeur de
			     la diversite */
  double moy1;            /* Valeur moyenne de la diversite de Shannon 
			     lorsqu'elle est definie et differente de 0
			   */
  double moy2;            /* Valeur moyenne de la diversite de Shannon quelle 
			     que soit la valeur
			   */
  double max;             /* Valeur maximale de la diversite de Shannon */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Ouverture du fichier
   */
  if ((FluxFichier = fopen(NomFichier, "a")) == 0 ) {
    fprintf(stderr,"Erreur d'ouverture du fichier %s\n", NomFichier);
    return(-1);
  }

  /* Affichage des valeurs de la diversite de Shannon pour N1 dans le
     cas du patron N1 (Prep) N2
   */
  fprintf(FluxFichier,"\n------------\n\n");
  fprintf(FluxFichier,"Diversite N1 (N1 (Prep) N2)\n");
  /* Recuperation de la valeur maximale
   */
  max = Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[0]].Mesures_Pertinence[DIVERSITE_SHANNON1];
  for(i=0;(Tab_Classif_DS.Div_N1[i]!=-1)&&(i<MAX_ELT_DIV);i++) {
    /* Affichage de la valeur pour chaque element de la table
     */
    fprintf(FluxFichier,"%d : %s : %G : %.2G\n",i, Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[i]].lemme, Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[i]].Mesures_Pertinence[DIVERSITE_SHANNON1], Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N1[i]].Mesures_Pertinence[DIVERSITE_SHANNON1]/max);
  }

  /* Initialisation des variables
   */
  nb1 = 0;
  moy1 = 0;
  nb2 = 0;
  moy2 = 0;
  /* Calcul de la somme des diversite ...
   */
  for(i = 0;i < Lst_Termes.nb_tetes;i++) {
    if (Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON1] !=0) {
      /* ... lorsqu'elle est definie et differente de 0
       */
      nb1++;
      moy1 += fabs(Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON1]);
    }
    /* ... quelle que soit la valeur
     */
    nb2++;
    moy2 += fabs(Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON1]);

  }
  /* Calcul de la moyenne
   */
  moy1 /= nb1;
  moy2 /= nb2;

  /* Affichage de la moyenne
   */
  fprintf(FluxFichier,"\nmoy1 = %G\nmoy2 = %G\n\n",moy1,moy2);

  /* Affichage des valeurs de la diversite de Shannon pour N1 dans le
     cas du patron N1 Adj
   */
  fprintf(FluxFichier,"\n------------\n\n");
  fprintf(FluxFichier,"Diversite N (N Adj)\n");
  /* Recuperation de la valeur maximale
   */
  max = Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N[0]].Mesures_Pertinence[DIVERSITE_SHANNON2];
  for(i=0;(Tab_Classif_DS.Div_N[i]!=-1)&&(i<MAX_ELT_DIV);i++) {
    /* Affichage de la valeur pour chaque element de la table
     */
    fprintf(FluxFichier,"%d : %s : %G : %.2G\n",i, Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N[i]].lemme, Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N[i]].Mesures_Pertinence[DIVERSITE_SHANNON2], Lst_Termes.lst_tetes[Tab_Classif_DS.Div_N[i]].Mesures_Pertinence[DIVERSITE_SHANNON2]/max);
  }

  /* Initialisation des variables
   */
  nb1 = 0;
  moy1 = 0;
  nb2 = 0;
  moy2 = 0;
  /* Calcul de la somme des diversite ...
   */
  for(i = 0;i < Lst_Termes.nb_tetes;i++) {
    if (Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON2] !=0) {
      /* ... lorsqu'elle est definie et differente de 0
       */
      nb1++;
      moy1 += fabs(Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON2]);
    }
    /* ... quelle que soit la valeur
     */
    nb2++;
    moy2 += fabs(Lst_Termes.lst_tetes[i].Mesures_Pertinence[DIVERSITE_SHANNON2]);

  }
  /* Calcul de la moyenne
   */
  moy1 /= nb1;
  moy2 /= nb2;

  /* Affichage de la moyenne
   */
  fprintf(FluxFichier,"\nmoy1 = %G\nmoy2 = %G\n\n",moy1,moy2);

  /* Affichage des valeurs de la diversite de Shannon pour N2 dans le
     cas du patron N1 (Prep) N2
   */
  fprintf(FluxFichier,"\n------------\n\n");
  fprintf(FluxFichier,"Diversite N2 (N1 (Prep) N2)\n");
  /* Recuperation de la valeur maximale
   */
  max = Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[0]].Mesures_Pertinence[DIVERSITE_SHANNON1];
  for(i=0;(Tab_Classif_DS.Div_N2[i]!=-1)&&(i<MAX_ELT_DIV);i++) {
    /* Affichage de la valeur pour chaque element de la table
     */
    fprintf(FluxFichier,"%d : %s : %G : %.2G\n",i, Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[i]].lemme, Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[i]].Mesures_Pertinence[DIVERSITE_SHANNON1], Lst_Termes.lst_exps[Tab_Classif_DS.Div_N2[i]].Mesures_Pertinence[DIVERSITE_SHANNON1]/max);
  }

  /* Initialisation des variables
   */
  nb1 = 0;
  moy1 = 0;
  nb2 = 0;
  moy2 = 0;
  /* Calcul de la somme des diversite ...
   */
  for(i = 0;i < Lst_Termes.nb_exps;i++) {
    if (Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON1] !=0) {
      /* ... lorsqu'elle est definie et differente de 0
       */
      nb1++;
      moy1 += fabs(Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON1]);
    }
    /* ... quelle que soit la valeur
     */
    nb2++;
    moy2 += fabs(Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON1]);

  }
  /* Calcul de la moyenne
   */
  moy1 /= nb1;
  moy2 /= nb2;

  /* Affichage de la moyenne
   */
  fprintf(FluxFichier,"\nmoy1 = %G\nmoy2 = %G\n\n",moy1,moy2);

  /* Affichage des valeurs de la diversite de Shannon pour Adj dans le
     cas du patron N1 Adj
   */
  fprintf(FluxFichier,"\n------------\n\n");
  fprintf(FluxFichier,"Diversite Adj (N Adj)\n");
  /* Recuperation de la valeur maximale
   */
  max = Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[0]].Mesures_Pertinence[DIVERSITE_SHANNON2];
  for(i=0;(Tab_Classif_DS.Div_Adj[i]!=-1)&&(i<MAX_ELT_DIV);i++) {
    /* Affichage de la valeur pour chaque element de la table
     */
    fprintf(FluxFichier,"%d : %s : %G : %.2G\n",i, Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[i]].lemme, Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[i]].Mesures_Pertinence[DIVERSITE_SHANNON2], Lst_Termes.lst_exps[Tab_Classif_DS.Div_Adj[i]].Mesures_Pertinence[DIVERSITE_SHANNON2]/max);
  }

  /* Initialisation des variables
   */
  nb1 = 0;
  moy1 = 0;
  nb2 = 0;
  moy2 = 0;
  /* Calcul de la somme des diversite ...
   */
  for(i = 0;i < Lst_Termes.nb_exps;i++) {
    if (Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON2] !=0) {
      /* ... lorsqu'elle est definie et differente de 0
       */
      nb1++;
      moy1 += fabs(Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON2]);
    }
    /* ... quelle que soit la valeur
     */
    nb2++;
    moy2 += fabs(Lst_Termes.lst_exps[i].Mesures_Pertinence[DIVERSITE_SHANNON2]);

  }
  /* Calcul de la moyenne
   */
  moy1 /= nb1;
  moy2 /= nb2;

  /* Affichage de la moyenne
   */
  fprintf(FluxFichier,"\nmoy1 = %G\nmoy2 = %G\n\n",moy1,moy2);

  /* Fermeture du fichier
   */
  fclose(FluxFichier);
  return(0);
}

/************************************************************************
 *                                                                      *
 * Objectif : Recuperation de la categorie grammaticale a partir d'un   *
 *            lemme                                                     *
 *            Remarque : a placer dans un autre fichier                 *
 *                                                                      *
 * Argument : le lemme                                                  *
 *                                                                      *
 * Retour : Code de la categorie                                        *
 *                                                                      *
 ************************************************************************/

int Get_CatGram(char *lemme)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  char cat[4]; /* Categorie grammaticale sous forme d'une chaine de 
                  caracteres */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  if (strchr(lemme, '=') != NULL) {
    /* On regarde s'il s'agit d'un candidat terme complexe
       dans ce cas on retourne le code CT
     */
    return CAT_CT;
  } else {
    /* Il s'agit d'un candidat terme simple
     */
    /* On recupere les trois caracteres qui suivent le dernier 
       caractere '_' 
     */
    strncpy(cat,strrchr(lemme,'_') +1, 3);
    cat[3] = 0;
    if (strcmp(cat,"Nom") == 0) {
      /* Il s'agit d'un nom */
      return(CAT_NOM);
    } else {
      /* Il s'agit d'un adjectif
       */
      return(CAT_ADJ);
    }
  }
}
