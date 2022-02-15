/* 	$Log: Synonymie.c,v $
/* 	Revision 1.6  2008/04/07 07:18:04  ht
/* 	correction in deisplaying inflected forms of the terms
/* 	
/* 	Revision 1.5  2007/10/08 08:16:12  ht
/* 	modification in the computing initial relations
/* 	
/* 	Revision 1.4  2007/08/01 08:48:17  ht
/* 	modification of the Makefile
/* 	bug fixes
/* 	addition of the functionality allowing to load WordNet from WordNet file
/* 	
/* 	Revision 1.3  2006/11/27 16:08:42  ht
/* 	nombreuses corrections
/* 	gestion des fichiers simplifiés (pour l'instant)
/* 	
/* 	Revision 1.2  2006/11/23 14:04:01  ht
/* 	correction de bugs
/* 	
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.5  2002/09/15 21:49:56  thierry
 * 	Modification de la fonction EnregLiensInf afin de corriger le bug sur
 * 	l'enregistrement des noms des fichiers dans l'entete du fichier
 * 	contenant les resultats (le nom du fichier contenant les resultats
 * 	apparaissait dans la liste des fichiers a charger !).
 *
 * 	Revision 1.4  2002/07/04 15:49:01  ht
 * 	*** empty log message ***
 *
 * 	Revision 1.3  2002/07/02 23:04:45  thierry
 * 	*** empty log message ***
 *
 * 	Revision 1.2  2001/05/01 19:39:39  thierry
 * 	*** empty log message ***
 *	 */

/**********************************************************************
 *          Inclusion des bibliotheques declarees localement          *
 **********************************************************************/

#include "Synonymie.h"
/* #include "VarSem.h" */
#include "stringSGML.h"
#include "Fichier.h"

/************************************************************************
 *                Gestion du graphe des candidats termes                *
 *                    et de l'inference des relations                   *
 ************************************************************************/

/************************************************************************
 *                                                                      *
 * Contenu : Flux sur le fichier contenant le resultat de l'inference   *
 *           des relations de synonymie                                 *
 *                                                                      *
 ************************************************************************/

FILE *FichResSyn;

/************************************************************************
 *                                                                      *
 * Contenu : Contient l'identifiant du lien courant et le nombre de     *
 *           relations inferees (inutilise ?)                           *
 *                                                                      *
 ************************************************************************/

int idlien;

/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur l'expansion en   *
 *            fonction des deux candidats termes synonymes fournis en   *
 *            en argument                                               *
 *                                                                      *
 * Arguments : 1. Flux sur le fichier contenant les resultats et        *
 *                notamment le nouveau lien trouve                      *
 *                                                                      *
 *             2. Identifiant dans le tableaux des expansions, du       *
 *                premier candidat terme (decode)                       * 
 *                                                                      *
 *             3. Identifiant dans le tableaux des expansions, du       *
 *                second candidat terme (decode)                        * 
 *                                                                      *
 *             4. Informations a placer sur le lien nouvellement ajoute *
 *                (origine, regle d'inference, etc.)                    *
 *                                                                      *
 *             5. Flux d'ecriture                                       *
 *                                                                      *
 * Date de derniere modification : 18/11/2003                           *
 *                                                                      *
 ************************************************************************/

int Rule_1(FILE *FichResSyn, int E1, int E2, int info, int flux_ecriture)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int i; /* utilise dans la boucle principale d'iteration,
	    element courant dans le tableau des compositions 
	    et dependance du candidat terme 1 */
  int j; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,
	    element courant dans le tableau des compositions 
	    et dependance du 'frere' candidat terme 1  */
  int frere; /* Frere du candidat terme 1 (lie par une relation de
	        dependance */
/*   char ch[10000]; /\* Chaine de caracteres contenant les donnees a ajouter  */
/* 		     dans le fichier resultat *\/ */


  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/
  
  /* Parcours de toutes les relations de dependance du premier 
     candidat terme en expansion
   */
  for(i=0;i<Lst_Termes.lst_exps[E1].nbcompdep;i++){
    /* Recuperation du candidat terme lie par une relation de dependance
       avec le premier cadidat terme, c'est-a-dire le 'frere'
     */
    frere = Lst_Termes.lst_exps[E1].compdep[DEP][i];
    /* Parcours de toutes les relations de dependance du frere
     */
    for(j=0;j<Lst_Termes.lst_tetes[frere].nbcompdep;j++){
      /* On regarde s'il y a une relation de dependance entre le frere et 
	 le deuxieme candidat terme en position expansion
       */
      if (Lst_Termes.lst_tetes[frere].compdep[DEP][j] == E2) {
	/* C'est le cas ...
	   Creation du lien de synonymie entre les candidats termes E1 et E2
	   On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
	   sont les expansions
	 */
	if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] > 0 ) && ( Lst_Termes.lst_tetes[frere].compdep[COMP][j] > 0)) {
	  /* Position des candidats termes : Tete / Tete */
	  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	   */
	  if ((AjoutLienT(DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info|REGLE1|NON_VAL, NO_INCRPROD, flux_ecriture)  == EXEC_OK)&&(FichResSyn != NULL)) {
	    /* Le lien n'existe pas
	       Enregistrement du lien dans le fichier des resultats
	       Creation de l'enregistrement
	     */
	    send_ch_res(FichResSyn, flux_ecriture, 1, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1);

/* 	    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1); */
	    
	    /* Ecriture des informations du nouveau lien de synonymie
	       dans le fichier
	     */
/* 	    fprintf(FichResSyn,"%s", ch); */
/* 	    print_com_msg(ch,flux_ecriture,NON); */
	    /* Propagation du lien avec les  regles qui conviennent.
	       Comme les candidats termes sont tous les deux en position tete,
	       on applique la regle 2 (variation sur la tete) et la regle 3 
	       (variation sur la tete et l'expansion)
	     */
	    Rule_2(FichResSyn, DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info,  flux_ecriture);
	    Rule_3T(FichResSyn, DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info, flux_ecriture);
	  }
	  /* On incremente l'identifiant de la relation de synonymie courant
	   */
	  idlien++;
	}
	else {
	  /* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
	     sont les expansions
	   */
	  if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_tetes[frere].compdep[COMP][j] < 0)) {
	    /* Position des candidats termes : Expansion / Expansion */
	    /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	     */
	    if ((AjoutLienE(DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info|REGLE1|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
	      /* Le lien n'existe pas
		 Enregistrement du lien dans le fichier des resultats
		 Creation de l'enregistrement
	       */
	      send_ch_res(FichResSyn, flux_ecriture, 1, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1);
/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1); */
	      
	      /* Ecriture des informations du nouveau lien de synonymie
		 dans le fichier
	      */
/* 	      fprintf(FichResSyn,"%s", ch); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
	      /* Propagation du lien avec les  regles qui conviennent.
		 Comme les candidats termes sont tous les deux en position expansion,
		 on applique la regle 1 (variation sur l'expansion) et la regle 3 
		 (variation sur la tete et l'expansion)
	      */
	      Rule_1(FichResSyn, DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info, flux_ecriture);
	      Rule_3E(FichResSyn, DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), info, flux_ecriture);
	    }
	    /* On incremente l'identifiant de la relation de synonymie courant
	     */
	    idlien++;
	  }
	  else {
	    /* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
	       sont les expansions
	    */
	    if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_tetes[frere].compdep[COMP][j] > 0)) {
	      /* Position des candidats termes : Expansion / Tete */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienTE(DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), ET,info|REGLE1 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) && (FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		*/
		send_ch_res(FichResSyn, flux_ecriture, 1, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1);
/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1); */
		
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		*/
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	    else {
	      /* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
		 sont les expansions
	      */
	      /* Position des candidats termes : Tete / Expansion */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienTE(DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j]), TE, info|REGLE1 | NON_VAL, NO_INCRPROD, flux_ecriture)  == EXEC_OK) && (FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		*/
		send_ch_res(FichResSyn, flux_ecriture, 1, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j] , "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1);

/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j] , Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1); */
		
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		*/
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	  }
	}
      }
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur la tete en       *
 *            fonction des deux candidats termes synonymes fournis en   *
 *            en argument                                               *
 *                                                                      *
 * Arguments : 1. Flux sur le fichier contenant les resultats et        *
 *                notamment le nouveau lien trouve                      *
 *                                                                      *
 *             2. Identifiant dans le tableaux des tetes, du            *
 *                premier candidat terme (decode)                       * 
 *                                                                      *
 *             3. Identifiant dans le tableaux des tetes, du            *
 *                second candidat terme (decode)                        * 
 *                                                                      *
 *             4. Informations a placer sur le lien nouvellement ajoute *
 *                (origine, regle d'inference, etc.)                    *
 *                                                                      *
 *             5. Flux d'ecriture                                       *
 *                                                                      *
 * Date de derniere modification : 18/11/2003                           *
 *                                                                      *
 ************************************************************************/

int Rule_2(FILE *FichResSyn, int T1, int T2, int info, int flux_ecriture)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/
  
  int i; /* utilise dans la boucle principale d'iteration,
	    element courant dans le tableau des compositions 
	    et dependance du candidat terme 1 */
  int j; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,
	    element courant dans le tableau des compositions 
	    et dependance du 'frere' candidat terme 1  */
  int frere; /* Frere du candidat terme 1 (lie par une relation de
	        dependance */
/*   char ch[10000]; /\* Chaine de caracteres contenant les donnees a ajouter  */
/* 		     dans le fichier resultat *\/ */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   printlog("Dans Rule 2\n"); */

  /* Parcours de toutes les relations de dependance du premier 
     candidat terme en tete
   */
  for(i=0;i<Lst_Termes.lst_tetes[T1].nbcompdep;i++){
    /* Recuperation du candidat terme lie par une relation de dependance
       avec le premier cadidat terme, c'est-a-dire le 'frere'
     */
    frere = Lst_Termes.lst_tetes[T1].compdep[DEP][i];
    /* Parcours de toutes les relations de dependance du frere
     */
    for(j=0;j<Lst_Termes.lst_exps[frere].nbcompdep;j++){
      /* On regarde s'il y a une relation de dependance entre le frere et 
	 le deuxieme candidat terme en position tete
       */
      if (Lst_Termes.lst_exps[frere].compdep[DEP][j] == T2) {
	/* C'est le cas ...
	   Creation du lien de synonymie entre les candidats termes T1 et T2
	   On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
	   sont les tetes
	 */
/* 	printlog("Inference relation T1 <---> T2\n"); */
	if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] > 0 ) && ( Lst_Termes.lst_exps[frere].compdep[COMP][j] > 0)) {
	  /* Position des candidats termes : Tete / Tete */
	  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	   */
	  if ((AjoutLienT(DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info|REGLE2|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
	    /* Le lien n'existe pas
	       Enregistrement du lien dans le fichier des resultats
	       Creation de l'enregistrement
	     */
		send_ch_res(FichResSyn, flux_ecriture, 2, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2);

/* 	    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2); */
		    
	    /* Ecriture des informations du nouveau lien de synonymie
	       dans le fichier
	     */
/* 	    fprintf(FichResSyn,"%s", ch); */
/* 	    print_com_msg(ch,flux_ecriture,NON); */
	    /* Propagation du lien avec les  regles qui conviennent.
	       Comme les candidats termes sont tous les deux en position tete,
	       on applique la regle 2 (variation sur la tete) et la regle 3 
	       (variation sur la tete et l'expansion)
	     */
	    Rule_2(FichResSyn, DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info, flux_ecriture);
	    Rule_3T(FichResSyn, DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info, flux_ecriture);
	  }
	  /* On incremente l'identifiant de la relation de synonymie courant
	   */
	  idlien++;
	}
	else {
	  /* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
	     sont les tetes
	   */
	  if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_exps[frere].compdep[COMP][j] < 0)) {
	    /* Position des candidats termes : Expansion / Expansion */
	    /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	     */
	    if ((AjoutLienE(DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info|REGLE2|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
	      /* Le lien n'existe pas
		 Enregistrement du lien dans le fichier des resultats
		 Creation de l'enregistrement
	       */
	      send_ch_res(FichResSyn, flux_ecriture, 2, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2);

/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2); */
		    
	      /* Ecriture des informations du nouveau lien de synonymie
		 dans le fichier
	       */
/* 	      fprintf(FichResSyn,"%s", ch); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
	      /* Propagation du lien avec les  regles qui conviennent.
		 Comme les candidats termes sont tous les deux en position expansion,
		 on applique la regle 1 (variation sur l'expansion) et la regle 3 
		 (variation sur la tete et l'expansion)
	      */
	      Rule_1(FichResSyn, DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info, flux_ecriture);
	      Rule_3E(FichResSyn, DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j]), info, flux_ecriture);
	    }
	    /* On incremente l'identifiant de la relation de synonymie courant
	     */
	    idlien++;
	  }
	  else {
	    /* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
	       sont les tetes
	    */
	    if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_exps[frere].compdep[COMP][j] > 0)) {
	      /* Position des candidats termes : Expansion / Tete */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienTE(DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j]), ET, info|REGLE2 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		*/
	      send_ch_res(FichResSyn, flux_ecriture, 2, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2);
/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2); */
		    
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		*/
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	    else {
	      /* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
		 sont les tetes
	      */
	      /* Position des candidats termes : Tete / Expansion */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienTE(DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j]), TE, info|REGLE2 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		*/
	      send_ch_res(FichResSyn, flux_ecriture, 2, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2);

/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[frere].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE2); */
		    
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		*/
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	  }
	}
      }
    }
  }
}


/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur la tete et       *
 *            l'expansion en fonction des deux candidats termes         *
 *            synonymes, en position tete, fournis en argument          *
 *                                                                      *
 * Arguments : 1. Flux sur le fichier contenant les resultats et        *
 *                notamment le nouveau lien trouve                      *
 *                                                                      *
 *             2. Identifiant dans le tableaux des tetes, du premier    *
 *                candidat terme (decode)                               * 
 *                                                                      *
 *             3. Identifiant dans le tableaux des tetes, du second     *
 *                candidat terme (decode)                               * 
 *                                                                      *
 *             4. Informations a placer sur le lien nouvellement ajoute *
 *                (origine, regle d'inference, etc.)                    *
 *                                                                      *
 *             5. Flux d'ecriture                                       *
 *                                                                      *
 * Date de derniere modification : 18/11/2003                           *
 *                                                                      *
 ************************************************************************/

int Rule_3T(FILE *FichResSyn, int T1, int T2, int info, int flux_ecriture)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/
  
  int i; /* utilise dans la boucle principale d'iteration,
	    element courant dans le tableau des compositions 
	    et dependance du candidat terme 1 */
  int j; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,  
	    element courant dans le tableau des synonymes 
	    du 'frere' candidat terme 1 */
  int k; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,
	    element courant dans le tableau des compositions 
	    et dependance du 'frere' candidat terme 1 */
  int frere; /* Frere du candidat terme 1 (lie par une relation de
		dependance */
  int freresyn; /* Synonyme du frere du candidat terme 1 */
/*   char ch[10000]; /\* Chaine de caracteres contenant les donnees a ajouter  */
/* 		     dans le fichier resultat *\/ */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Parcours de toutes les relations de dependance du premier 
     candidat terme en tete
   */
  for(i=0;i<Lst_Termes.lst_tetes[T1].nbcompdep;i++){
    /* Recuperation du candidat terme lie par une relation de dependance
       avec le premier candidat terme, c'est-a-dire le 'frere'
     */
    frere = Lst_Termes.lst_tetes[T1].compdep[DEP][i];
    /* Parcours de toutes les relations de synonymie du frere
     */
    for(k=0;k<Lst_Termes.lst_exps[frere].nbsyn;k++){
      /* Recuperation d'un synonyme du frere
       */
      freresyn = Lst_Termes.lst_exps[frere].syn[REF][k];
      /* Decodage du synonyme. On sait si c'est un synonyme 
	 en position tete ou expansion
       */
      if (freresyn<0) {
	/* Synonyme en position tete
	 */
	freresyn = (freresyn * -1) -1;
	/* Sinon, il n'y a pas de decodage a effectuer
	 */
	/* Parcours de toutes les relations de dependance du synonyme du frere
	 */
	for(j=0;j<Lst_Termes.lst_exps[freresyn].nbcompdep;j++){
	  /* On regarde s'il y a une relation de dependance entre le synonyme
	       du frere et le deuxieme candidat terme en position tete
	   */
	  if (Lst_Termes.lst_exps[freresyn].compdep[DEP][j] == T2) {
	    /* C'est le cas ...
	       Creation du lien de synonymie entre les candidats termes T1 et T2
	       On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
	       sont les tetes
	     */
	    if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] > 0 ) && ( Lst_Termes.lst_exps[freresyn].compdep[COMP][j] > 0)) {
	      /* Position des candidats termes : Tete / Tete */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienT(DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info|REGLE3|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		 */

	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere) , Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere) , Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		  
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		 */
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
		/* Propagation du lien avec les  regles qui conviennent.
		   Comme les candidats termes sont tous les deux en position tete,
		   on applique la regle 2 (variation sur la tete) et la regle 3 
		   (variation sur la tete et l'expansion)
		 */
		Rule_2(FichResSyn, DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info, flux_ecriture);
		Rule_3T(FichResSyn, DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info, flux_ecriture);
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	    else {
	      /* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
		 sont les tetes
	       */
	      if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_exps[freresyn].compdep[COMP][j] < 0)) {
		  /* Position des candidats termes : Expansion / Expansion */
		  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		   */
		if ((AjoutLienE(DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info|REGLE3|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		  /* Le lien n'existe pas
		     Enregistrement du lien dans le fichier des resultats
		     Creation de l'enregistrement
		   */
	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		  sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		    
		  /* Ecriture des informations du nouveau lien de synonymie
		     dans le fichier
		   */
/* 		  fprintf(FichResSyn,"%s", ch); */
/* 		  print_com_msg(ch,flux_ecriture,NON); */
		  /* Propagation du lien avec les  regles qui conviennent.
		     Comme les candidats termes sont tous les deux en position expansion,
		     on applique la regle 1 (variation sur l'expansion) et la regle 3 
		     (variation sur la tete et l'expansion)
		   */
		  Rule_1(FichResSyn, DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info, flux_ecriture);
		  Rule_3E(FichResSyn, DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), info, flux_ecriture);
		  }
		  /* On incremente l'identifiant de la relation de synonymie courant
		   */
		idlien++;
	      }
	      else {
		/* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
		   sont les tetes
		 */
		if ((Lst_Termes.lst_tetes[T1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_exps[freresyn].compdep[COMP][j] > 0)) {
		  /* Position des candidats termes : Expansion / Tete */
		  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		   */
		  if ((AjoutLienTE(DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), ET, info|REGLE3 | NON_VAL, NO_INCRPROD, flux_ecriture)  == EXEC_OK) &&(FichResSyn != NULL)) {
		    /* Le lien n'existe pas
		       Enregistrement du lien dans le fichier des resultats
		       Creation de l'enregistrement
		     */

	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		      
		    /* Ecriture des informations du nouveau lien de synonymie
		       dans le fichier
		     */
/* 		    fprintf(FichResSyn,"%s", ch); */
/* 		    print_com_msg(ch,flux_ecriture,NON); */
		  }
		  /* On incremente l'identifiant de la relation de synonymie courant
		   */
		  idlien++;
		}
		else {
		  /* On regarde la position des candidats termes CT1 et CT2 dont T1 et T2 
		     sont les tetes
		   */
		  /* Position des candidats termes : Tete / Expansion */
		  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		   */
		  if ((AjoutLienTE(DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j]), TE, info|REGLE3 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		    /* Le lien n'existe pas
		       Enregistrement du lien dans le fichier des resultats
		       Creation de l'enregistrement
		     */

	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere), Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		      
		    /* Ecriture des informations du nouveau lien de synonymie
		       dans le fichier
		     */
/* 		    fprintf(FichResSyn,"%s", ch); */
/* 		    print_com_msg(ch,flux_ecriture,NON); */
		  }
		  /* On incremente l'identifiant de la relation de synonymie courant
		   */
		  idlien++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur la tete et       *
 *            l'expansion en fonction des deux candidats termes         *
 *            synonymes, en position expansion, fournis en argument     *
 *                                                                      *
 * Arguments : 1. Flux sur le fichier contenant les resultats et        *
 *                notamment le nouveau lien trouve                      *
 *                                                                      *
 *             2. Identifiant dans le tableaux des expansions, du       *
 *                premier candidat terme (decode)                       * 
 *                                                                      *
 *             3. Identifiant dans le tableaux des expansions, du       *
 *                second candidat terme  (decode)                       * 
 *                                                                      *
 *             4. Informations a placer sur le lien nouvellement ajoute *
 *                (origine, regle d'inference, etc.)                    *
 *                                                                      *
 *             5. Flux d'ecriture                                       *
 *                                                                      *
 * Date de derniere modification : 05/11/2003                           *
 *                                                                      *
 ************************************************************************/

int Rule_3E(FILE *FichResSyn, int E1, int E2, int info, int flux_ecriture)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/
  
  int i; /* utilise dans la boucle principale d'iteration,
	    element courant dans le tableau des compositions 
	    et dependance du candidat terme 1 */
  int j; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,  
	    element courant dans le tableau des synonymes 
	    du 'frere' candidat terme 1 */
  int k; /* utilise dans la boucle d'iteration imbrique 
	    dans la boucle principale,
	    element courant dans le tableau des compositions 
	    et dependance du 'frere' candidat terme 1 */
  int frere; /* Frere du candidat terme 1 (lie par une relation de
		dependance */
  int freresyn; /* Synonyme du frere du candidat terme 1 */
/*   char ch[10000]; /\* Chaine de caracteres contenant les donnees a ajouter  */
/* 		     dans le fichier resultat *\/ */
    
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

  /* Parcours de toutes les relations de dependance du premier 
     candidat terme en expansion
   */
  for(i=0;i<Lst_Termes.lst_exps[E1].nbcompdep;i++){
    /* Recuperation du candidat terme lie par une relation de dependance
       avec le premier cadidat terme, c'est-a-dire le 'frere'
    */
    frere = Lst_Termes.lst_exps[E1].compdep[DEP][i];
    /* Parcours de toutes les relations de synonymie du frere
     */
    for(k=0;k<Lst_Termes.lst_tetes[frere].nbsyn;k++){
      /* Recuperation d'un synonyme du frere
       */
      freresyn = Lst_Termes.lst_tetes[frere].syn[REF][k];
      /* Decodage du synonyme. On sait si c'est un synonyme 
	 en position tete ou expansion
       */
      if (freresyn>0) {
	/* Synonyme en position expansion
	 */
	freresyn = freresyn -1;
	/* Sinon, il n'y a pas de decodage a effectuer
	 */
	/* Parcours de toutes les relations de dependance du synonyme du frere
	 */
	for(j=0;j<Lst_Termes.lst_tetes[freresyn].nbcompdep;j++){
	  /* On regarde s'il y a une relation de dependance entre le synonyme
	     du frere et le deuxieme candidat terme en position tete
	   */
	  if (Lst_Termes.lst_tetes[freresyn].compdep[DEP][j] == E2) {
	    /* C'est le cas ...
	       Creation du lien de synonymie entre les candidats termes E1 et E2
	       On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
	       sont les expansions
	     */
	    if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] > 0 ) && ( Lst_Termes.lst_tetes[freresyn].compdep[COMP][j] > 0)) {
	      /* Position des candidats termes : Tete / Tete */
	      /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
	       */
	      if ((AjoutLienT(DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info|REGLE3|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		/* Le lien n'existe pas
		   Enregistrement du lien dans le fichier des resultats
		   Creation de l'enregistrement
		 */
	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		    
		/* Ecriture des informations du nouveau lien de synonymie
		   dans le fichier
		 */
/* 		fprintf(FichResSyn,"%s", ch); */
/* 		print_com_msg(ch,flux_ecriture,NON); */
		/* Propagation du lien avec les  regles qui conviennent.
		   Comme les candidats termes sont tous les deux en position tete,
		   on applique la regle 2 (variation sur la tete) et la regle 3 
		     (variation sur la tete et l'expansion)
		 */
		Rule_2(FichResSyn, DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info, flux_ecriture);
		Rule_3T(FichResSyn, DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info, flux_ecriture);
	      }
	      /* On incremente l'identifiant de la relation de synonymie courant
	       */
	      idlien++;
	    }
	    else {
	      /* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
		 sont les expansions
	       */
	      if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_tetes[freresyn].compdep[COMP][j] < 0)) {
		/* Position des candidats termes : Expansion / Expansion */
		/* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		 */
		if ((AjoutLienE(DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info|REGLE3|NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		  /* Le lien n'existe pas
		     Enregistrement du lien dans le fichier des resultats
		     Creation de l'enregistrement
		   */
	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		  sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		    
		  /* Ecriture des informations du nouveau lien de synonymie
		     dans le fichier
		   */
/* 		  fprintf(FichResSyn,"%s", ch); */
/* 		  print_com_msg(ch,flux_ecriture,NON); */
		  /* Propagation du lien avec les  regles qui conviennent.
		     Comme les candidats termes sont tous les deux en position expansion,
		     on applique la regle 1 (variation sur l'expansion) et la regle 3 
		     (variation sur la tete et l'expansion)
		   */
		  Rule_1(FichResSyn, DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info, flux_ecriture);
		  Rule_3E(FichResSyn, DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), info, flux_ecriture);
		}
		/* On incremente l'identifiant de la relation de synonymie courant
		 */
		idlien++;
	      }
	      else {
		/* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
		   sont les tetes
		 */
		if ((Lst_Termes.lst_exps[E1].compdep[COMP][i] < 0 ) && ( Lst_Termes.lst_tetes[freresyn].compdep[COMP][j] > 0)) {
		  /* Position des candidats termes : Expansion / Tete */
		  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		   */
		  if ((AjoutLienTE(DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), ET, info|REGLE3 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		    /* Le lien n'existe pas
		       Enregistrement du lien dans le fichier des resultats
		       Creation de l'enregistrement
		     */
	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);

/* 		    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		    
		    /* Ecriture des informations du nouveau lien de synonymie
		       dans le fichier
		     */
/* 		    fprintf(FichResSyn,"%s", ch); */
/* 		    print_com_msg(ch,flux_ecriture,NON); */
		  }
		  /* On incremente l'identifiant de la relation de synonymie courant
		   */
		  idlien++;
		}
		else {
		  /* TETE / EXPANSION */
		  /* On regarde la position des candidats termes CT1 et CT2 dont E1 et E2 
		     sont les tetes
		   */
		  /* Position des candidats termes : Tete / Expansion */
		  /* On ajoute le lien entre les deux candidats termes CT1 et CT2 s'il n'existe pas
		   */
		  if ((AjoutLienTE(DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i]), DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j]), TE, info|REGLE3 | NON_VAL, NO_INCRPROD, flux_ecriture) == EXEC_OK) &&(FichResSyn != NULL)) {
		    /* Le lien n'existe pas
		       Enregistrement du lien dans le fichier des resultats
		       Creation de l'enregistrement
		     */
	      send_ch_res(FichResSyn, flux_ecriture, 3, Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], "NV", Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3);
/* 			  sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[freresyn].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[frere].lemme, CodeIndTete(frere), Lst_Termes.lst_tetes[freresyn].lemme, CodeIndTete(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_exps[DecodeIndExp(Lst_Termes.lst_tetes[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */
		    
		    /* Ecriture des informations du nouveau lien de synonymie
		       dans le fichier
		     */
/* 		    fprintf(FichResSyn,"%s", ch); */
/* 		    print_com_msg(ch,flux_ecriture,NON); */
		  }
		  /* On incremente l'identifiant de la relation de synonymie courant
		   */
		  idlien++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur l'expansion pour *
 *            une relation donnee                                       *
 * Arguments : 1. Flux sur le fichier qui contiendra les resultats      *
 *                des regles d'inference                                *
 *                                                                      *
 *             2. Lemme de l'entree (premier mot de la relation)        *
 *                                                                      *
 *             3. Lemme du synonyme (deuxieme mot de la relation)       *
 *                                                                      *
 *             4. Informations liees a la relation a ajouter            *
 *                                                                      *
 *             5. Identifiant du flux d'ecriture                        *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 02/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Regle1(FILE *FichResSyn, char *entree, char *syn, int info, int flux_ecrit)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int indentree; /* Identifiant du candidat terme correspondant 
		    a l'entre */
  int indsyn; /* Indentifiant du candidat terme correspondant 
		    au synonyme */
  char categ[TAILLE_CATEGORIE + 1]; /* Categorie des candidats termes */

  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   printlog("Dans Regle 1\n"); */

  /* Initialisation de la categorie */
  categ[0]=0;
  /* Recherche du lemme de l'entree dans le tableau des expansions 
     On recupere l'identifiant du candidat terme correspondant a l'entree
   */
  indentree = recherche_entete_LemS(entree, categ, EXPANSION, 0, Lst_Termes.nb_exps-1);

  /* Initialisation de l'identifiant du candidat terme qui correspondra 
     au synonyme
   */
  indsyn=0;

  /* On traite toutes les candidats termes correspondant a l'entree
     (on peut avoir plusieurs categories pour le meme lemme, s'il n'y
      a pas de categorie specifiee)
      On regarde si on n'est pas arrive a la fin du tableau des 
      expansion
   */
  while(indentree<Lst_Termes.nb_exps){
    /* On verifie que le candidat terme n'est pas le dernier du tableau
     */
    if (indentree+1<Lst_Termes.nb_exps) {
      /* Recherche du lemme du synonyme dans le tableau des expansions 
	 On recupere l'identifiant du candidat terme correspondant au synonyme
      */  
      indsyn = recherche_entete_LemS(syn, categ, EXPANSION, indentree+1,Lst_Termes.nb_exps-1);

      /* On verifie que le candidat terme synonyme a bien ete trouve */
      if (indsyn<Lst_Termes.nb_exps) {
	/* Ajout du lien entre les candidats termes, en position expansion, 
	   correspondant a l'entree et le synonyme
	*/
	if (AjoutLienE(indentree, indsyn, info, NO_INCRPROD, flux_ecrit) == EXEC_OK) {
	  /* Le lien a ete ajoute, on applique les regles de variation sur l'expansion
	   */
/* 	  printlog("Passe R1\n"); */
	  Rule_1(FichResSyn, indentree, indsyn, info, flux_ecrit);
	  /* Le lien a ete ajoute, on applique les regles de variation sur la
	     variation sur la tete et l'expansion
	  */
	  Rule_3E(FichResSyn, indentree, indsyn, info, flux_ecrit);
	}
	/* Reinitialisation de la categorie
	 */
	categ[0]=0;
      }
    }  
    /* Le candidat terme correspondant a l'entree existe */
    /* Recherche du lemme de l'entree dans le tableau des expansions 
       On recupere l'identifiant du candidat terme correspondant a l'entree
    */

    if ((!indentree) || ((indentree = recherche_entete_LemS(entree, categ, EXPANSION, 0, indentree-1)) > indentree -1)) {
      /* On se place a la fin du tableau
       */
	indentree=Lst_Termes.nb_exps;
      }
  }
  return(EXEC_OK);
}

/************************************************************************
 *                                                                      *
 * Objectif : Application de la regle de variation sur la tete pour     *
 *            une relation donnee                                       *
 * Arguments : 1. Flux sur le fichier qui contiendra les resultats      *
 *                des regles d'inference                                *
 *                                                                      *
 *             2. Lemme de l'entree (premier mot de la relation)        *
 *                                                                      *
 *             3. Lemme du synonyme (deuxieme mot de la relation)       *
 *                                                                      *
 *             4. Informations liees a la relation a ajouter            *
 *                                                                      *
 *             5. Identifiant du flux d'ecriture                        *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 01/12/2003                           *
 *                                                                      *
 ************************************************************************/

int Regle2(FILE *FichResSyn,char *entree, char *syn, int info, int flux_ecrit)
{
  /*************************************
   * Declaration des variables locales *
   *************************************/

  int indentree; /* Identifiant du candidat terme correspondant 
		    a l'entre */
  int indsyn; /* Indentifiant du candidat terme correspondant 
		 au synonyme */
  char categ[TAILLE_CATEGORIE + 1]; /* Categorie des candidats termes */
   
  /***********************************************
   * Fin de la declaration des variables locales *
   ***********************************************/

/*   printlog("Dans Regle 2\n"); */

  /* Initialisation de la categorie */
  categ[0]=0;
  /* Recherche du lemme de l'entree dans le tableau des tetes 
     On recupere l'identifiant du candidat terme correspondant a l'entree
  */
  indentree = recherche_entete_LemS(entree, categ, TETE, 0, Lst_Termes.nb_tetes-1);

  /* Initialisation de l'identifiant du candidat terme qui correspondra 
     au synonyme
  */
  indsyn=0;

  /* On traite toutes les candidats termes correspondant a l'entree
     (on peut avoir plusieurs categories pour le meme lemme, s'il n'y
     a pas de categorie specifiee)
     On regarde si on n'est pas arrive a la fin du tableau des 
     tetes
  */
  while(indentree<Lst_Termes.nb_tetes){
    /* On verifie que le candidat terme n'est pas le dernier du tableau
     */
    if (indentree+1<Lst_Termes.nb_tetes)
      /* Recherche du lemme du synonyme dans le tableau des expansions 
	 On recupere l'identifiant du candidat terme correspondant au synonyme
      */  
      indsyn = recherche_entete_LemS(syn, categ, TETE, indentree+1, Lst_Termes.nb_tetes-1);
    
    /* On verifie que le candidat terme synonyme a bien ete trouve */
    if (indsyn<Lst_Termes.nb_tetes) {
      /* Ajout du lien entre les candidats termes, en position expansion, 
	 correspondant a l'entree et le synonyme
      */
      if (AjoutLienT(indentree, indsyn, info, NO_INCRPROD, flux_ecrit) == EXEC_OK) {
	/* Le lien a ete ajoute, on applique les regles de variation sur la tete
	 */
/* 	  printlog("Passe R2\n"); */
	Rule_2(FichResSyn, indentree, indsyn, info, flux_ecrit);
	/* Le lien a ete ajoute, on applique les regles de variation sur la
	   variation sur la tete et l'expansion
	*/
	Rule_3T(FichResSyn, indentree, indsyn, info, flux_ecrit);
      }
      /* Reinitialisation de la categorie
       */
      categ[0]=0;
    }
    /* Le candidat terme correspondant a l'entree existe */
    /* Recherche du lemme de l'entree dans le tableau des tetes
       On recupere l'identifiant du candidat terme correspondant a l'entree
    */
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
 * Objectif : A decrire                                                 *
 *                                                                      *
 * Arguments : A decrire                                                *
 *                                                                      *
 * Retour: l'etat de l'execution de la fonction                         *
 *                                                                      *
 * Date de derniere modification : 27/12/2003                           *
 *                                                                      *
 ************************************************************************/

int InferenceCTSynE1(int NomFichSyn, int Fichct, char *FichResult, int info, int nbgnmax, int flux_ecrit)
{
  /* printlog("Hcur : %d\n", Hcur); */
  /* printlog("TERMLIST : %d\n", TERMLIST); */
  /* printlog("type (0) : %d (%d - %s)\n", Historique[0], liste_fichier[Historique[0]].type, liste_fichier[Historique[0]].nom); */
  /* printlog("type (1) : %d (%d - %s)\n", Historique[1], liste_fichier[Historique[1]].type, liste_fichier[Historique[1]].nom); */
  /* printlog("type (2) : %d (%d - %s)\n", Historique[2], liste_fichier[Historique[2]].type, liste_fichier[Historique[2]].nom); */

  /* Laisser le fichier FichResult ??? */

    int FichSyn;
/*     FILE *FileResEnt;
    FILE *FichEnt; */
    char entree[BUF_MOT_SZ],syn[BUF_MOT_SZ],*tmp,c;
    int i;
    int ret_read;

    if (!CG) {
      if (ConstructGraphe(Fichct, nbgnmax) != EXEC_OK) {
	fprintf(stderr,"Erreur lors de la construction du graphe\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_TRAITMT);
      }
    }    
    print_com_msg("DebutInfer",flux_ecrit,OUI);

    /**********************************/
    /* Debut de l'inference des liens */
    /**********************************/

    /************************************************/
    /* Ouverture du fichier temporarire de stockage */
    /* des résultats                                */
    /************************************************/
    
    printlog("Ouverture du fichier resultat %s\n", FichResult);

/*     if ((tmp = (char *)malloc(sizeof(char)*(strlen(FichResult) + strlen(EXT_FICHIER_ST_TMP) + 1)))==NULL) { */
/*       fprintf(stderr,"Erreur d'allocation memoire\n"); */
/*       fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
/*       return(ERR_ALLOCATION); */
/*     } */
/*     sprintf(tmp,"%s%s",FichResult,EXT_FICHIER_ST_TMP); */
    if ((FichResSyn = fopen(FichResult,"a"))==NULL) {
      fprintf(stderr,"Erreur d'ouverture du fichier de sortie %s\n",FichResult);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_OUVERTURE);
    }
/*     free(tmp); */

    /*************************************/
    /* Lecture des liens du dictionnaire */    
    /*************************************/

    printlog("Ouverture du fichier %s%s\n", liste_fichier[NomFichSyn].nom,EXT_FICHIER_ST_CT);
    if ((tmp = (char *)malloc(sizeof(char)*(strlen(liste_fichier[NomFichSyn].nom) + strlen(EXT_FICHIER_ST_CT) + 1)))==NULL) {
      fprintf(stderr,"Erreur d'allocation memoire\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ALLOCATION);
    }
    sprintf(tmp,"%s%s",liste_fichier[NomFichSyn].nom,EXT_FICHIER_ST_CT); 
    typoD = liste_fichier[NomFichSyn].typo;
    
    if ((FichSyn=open(tmp,O_RDONLY))==-1) {
      fprintf(stderr,"Erreur d'ouverture du fichier %s\n",tmp);
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_OUVERTURE);
    }
    free(tmp);
    
    /************************/
    /* Debut de l'inference */
    /************************/

    idlien = 0;

    ret_read = 1;
    while(ret_read){
      /* lecture de l'entree */
      i=0;
      do {
	if ((ret_read = read(FichSyn, &(entree[i]), sizeof(char))) < 0) {
	  fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_LECTURE);
	}
	i++;
      }while((ret_read != 0)&&(entree[i-1]!=':'));
      entree[i-2]=0;
      /* lecture du synonyme */
      if ((ret_read = read(FichSyn, &c, sizeof(char))) < 0) {
	fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_LECTURE);
      }
      i=0;
      do {
	if ((ret_read = read(FichSyn, &(syn[i]), sizeof(char))) < 0) {
	  fprintf(stderr,"Erreur de lecture du fichier des synonymes\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_LECTURE);
	}
	i++;
      }while((ret_read != 0)&&(syn[i-1]!=':'));
      syn[i-2]=0;
      /* On passe le reste */

/* 	printlog(" entree = %s / syn = %s\n", entree, syn); */
    
      do {
	if ((ret_read = read(FichSyn, &c, sizeof(char))) < 0) {
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
	/* Appel des regles d'inference */
/* 	printlog("Syn : %s <=> %s\n", entree, syn); */
/* 	printlog("Appel Regles\n"); */
	
	if (Regle1(FichResSyn, entree, syn, info, flux_ecrit) != EXEC_OK) {
	  fprintf(stderr,"Erreur lors de l'appel de la regle 1\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_TRAITMT);
	}

	if (Regle2(FichResSyn, entree, syn, info,  flux_ecrit) != EXEC_OK) {
	  fprintf(stderr,"Erreur lors de l'appel de la regle 2\n");
	  fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	  return(ERR_TRAITMT);
	}
      }
    }
    printlog("FinInfer\n");
    print_com_msg("FinInfer",flux_ecrit,OUI);
    fclose(FichResSyn);    
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int Nouv_Infer(char *terme1, char *terme2, int info)
{
    char tmp[1000];
    
    if (strcmp(terme1,terme2)>0){
	strcpy(tmp,terme1);
	strcpy(terme1,terme2);
	strcpy(terme2,tmp);
    }

    Regle1(NULL,terme1,terme2, info,  0);
    Regle2(NULL,terme1,terme2, info, 0);
    
}



/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int ExtractLiensInit(int NomFichSyn, int Fichct, char *FichResult, int info, int nbgnmax, int sock, int flux_ecriture)
{
    FILE *FichSyn,*FileResEnt, *FichResLInit;
    FILE *FichEnt;
    char entree[1000],syn[1000],tmp[1000],c;
    int i,j;
    char NomFichRes[3000], NomfichEnt[3000],*msg;
    int indentree,indsyn;
    char categ[4];
    int ResInfInv = 0;
/*     char ch[10000]; */

/*     fprintf(stderr, "==>%x\n", info); */

    if (!CG) ConstructGraphe(Fichct, nbgnmax);
    
    printlog("DebutExtract\n");
    if (sock!=-1) {
      ecrit_sock(sock, "DebutExtract");
    }
    else {
      print_com_msg(ENVOI_DEBUT_EXTRACTION,flux_ecriture,OUI);
    }

    /**********************************/
    /* Debut de l'inference des liens */
    /**********************************/
    
    sprintf(tmp,"%s.tmp",FichResult);
/*     printlog("Ouverture du fichier resultat %s\n", tmp); */
/*     if ((FichResSyn = fopen(tmp,"a"))==NULL) { */
/*         writelogS("Erreur d'ouverture du fichier de sortie"); */
/*     } */

    writelogS("Appel Regles Inverses");

    /* Lecture des liens du dictionnaire */
    
    sprintf(tmp,"%s.db",liste_fichier[NomFichSyn].nom); 
    typoD = liste_fichier[NomFichSyn].typo;

    writelogI(typoC);
    writelogI(typoD);
    
    printlog("%s\n",tmp);
    printlog("Ouverture du fichier des synonymes %s\n", tmp);
    FichSyn=fopen(tmp,"r");

    strcpy(NomFichRes,tmp);
    NomFichRes[strlen(tmp) -2] = 0;
    strcat(NomFichRes,"linit.syn");

    writelogS(NomFichRes);

    printlog("Ouverture du fichier resultat linit %s\n", NomFichRes);
    FichResLInit=fopen(NomFichRes,"w");
    
    idlien = 0;
    while(!feof(FichSyn)){
    	/* lecture de l'entree */
	i=0;
	do {
	    fread(&(entree[i]),sizeof(char),1,FichSyn);
	    i++;
	}while((!feof(FichSyn))&&(entree[i-1]!=':'));
	entree[i-2]=0;
	/* lecture du synonyme */
	fread(&c,sizeof(char),1,FichSyn);
	i=0;
	do {
	    fread(&(syn[i]),sizeof(char),1,FichSyn);
	    i++;
	}while((!feof(FichSyn))&&(syn[i-1]!=':'));
	syn[i-2]=0;
	/* On passe le reste */
    
	do {
	    fread(&c,sizeof(char),1,FichSyn);
	}while((!feof(FichSyn))&&(c!='\n'));
	
	if (strcmp(entree,syn)>0){
	   strcpy(tmp,entree);
	   strcpy(entree,syn);
	   strcpy(syn,tmp);
	}


    /************************/

/* 	writelogS("Debut de l'extraction"); */
	categ[0]=0;
	indentree = recherche_entete_LemS(entree, categ, TETE, 0,Lst_Termes.nb_tetes-1);
	indsyn=0;
	while(indentree<Lst_Termes.nb_tetes){
	  if (indentree+1<Lst_Termes.nb_tetes)
	    indsyn = recherche_entete_LemS(syn, categ, TETE, indentree+1,Lst_Termes.nb_tetes-1);
/*  	  printlog("(1) indentree = %d (%s) / indsyn = %d (%s)\n", indentree, entree, indsyn, syn); */
/*  	  fprintf(stderr,"(1) indentree = %d (%s) / indsyn = %d (%s)\n", indentree, entree, indsyn, syn); */
	  if (indsyn<Lst_Termes.nb_tetes) {
	    /* Ajout du lien */
	    ResInfInv = 0;

/*     fprintf(stderr, "==>%x\n", info); */
	    if (AjoutLienT(indentree, indsyn, info, NO_INCRPROD, flux_ecriture) == EXEC_OK) {
	      /* Regle 1 inverse */
/* 	      printlog("relation have been added\n"); */
  	      ResInfInv |= Regle1Inv(FichResLInit,indentree, indsyn, TETE, info, flux_ecriture); 
	      
/* 	      /\* Regle 2 inverse *\/ */
    	      ResInfInv |= Regle2Inv(FichResLInit,indentree, indsyn, TETE, info, flux_ecriture);
	      
/* 	      /\* Regle 3 inverse *\/ */
  	      ResInfInv |= Regle3Inv(FichResLInit,indentree, indsyn, TETE, info, flux_ecriture);

  	      ResInfInv |= Regle3Inv(FichResLInit,indentree, indsyn, TETE, info, flux_ecriture);

	      /* A placer autrement */
/* 	      if (ResInfInv == 0) { */
/* 		fprintf(FichResLInit, "%s : %s : 1 : 0 : NOInf\n", entree, syn); */
/* 	      } */
	    }
	    categ[0]=0;
	  }
	  if (indentree)
	    indentree = recherche_entete_LemS(entree, categ, TETE, indentree+1, Lst_Termes.nb_tetes-1);/* 	    indentree = recherche_entete_LemS(entree, categ, TETE, 0, indentree+1); */
	  else indentree=Lst_Termes.nb_tetes;
	}
/* 	printlog("ajout sur les expansions\n"); */
	categ[0]=0;
	indentree = recherche_entete_LemS(entree, categ, EXPANSION, 0, Lst_Termes.nb_exps-1);
	indsyn=0;
	while(indentree<Lst_Termes.nb_exps){
	  if (indentree+1<Lst_Termes.nb_exps)
	    indsyn = recherche_entete_LemS(syn, categ, EXPANSION, indentree+1,Lst_Termes.nb_exps-1);
/* 	  printlog("(2) indentree = %d (%s) / indsyn = %d (%s)\n", indentree, entree, indsyn, syn);  */
	  if (indsyn<Lst_Termes.nb_exps) {
	    /* Ajout du lien */
	    if (AjoutLienE(indentree, indsyn, info, NO_INCRPROD, flux_ecriture) == EXEC_OK) {
  	      ResInfInv |= Regle3Inv(FichResLInit,indentree, indsyn, EXPANSION, info, flux_ecriture);

	    }
	    categ[0]=0;
	  }
	  if (indentree)
	    indentree = recherche_entete_LemS(entree, categ, EXPANSION, indentree+1, Lst_Termes.nb_exps-1);
/* 	    indentree = recherche_entete_LemS(entree, categ, TETE, 0, indentree+1); */
	  else indentree=Lst_Termes.nb_exps;
	}

/* 	printlog("Fin ajout sur les expansions\n"); */


    }



    fclose(FichSyn);
    fclose(FichResLInit);
    printlog("FinExtract\n");
    if (sock!=-1) {
	ecrit_sock(sock, "FinExtract");
    }
    else {
      print_com_msg(ENVOI_FIN_EXTRACTION,flux_ecriture,OUI);
    }
/*     fclose(FichResSyn);     */
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int Regle1Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture)
{
  int i,j;
  int e1, e2;
  int ResInfInv = 0;
  int Terme1, Terme2;
/*   char ch[10000]; */

/*   fprintf(stderr,"Debut Regle1Inv (%d %d)\n", entree, syn); */

  if (entree!=syn) {
    if (pos) { /* EXPANSION */
      for(i=0;i<Lst_Termes.lst_exps[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_exps[syn].nbdecomp;j++) {

/* 	  printlog("%s / %s\n", Lst_Termes.lst_exps[entree].lemme,Lst_Termes.lst_exps[syn].lemme); */
	  if (Lst_Termes.lst_exps[entree].decomp[TETE][i]==Lst_Termes.lst_exps[syn].decomp[TETE][j]){
	    ResInfInv = 1;
	    if (AjoutLienE(Lst_Termes.lst_exps[entree].decomp[EXPANSION][i], Lst_Termes.lst_exps[syn].decomp[EXPANSION][j] , info|REGLE1|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK)  {
	      e1 = Lst_Termes.lst_exps[entree].decomp[EXPANSION][i];
	      e2 = Lst_Termes.lst_exps[syn].decomp[EXPANSION][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R1I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R1I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -1,  (e1+1)*-1, (e2+1)*-1, "NV", \
			  Lst_Termes.lst_exps[entree].lemme, (entree +1)*-1, Lst_Termes.lst_exps[syn].lemme, \
			  (syn+1)*-1, Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter, \
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE1);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */

/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", (e1+1)*-1, (e2+1)*-1, \ */
/* 		      Lst_Termes.lst_exps[entree].lemme, (entree +1)*-1, Lst_Termes.lst_exps[syn].lemme, \ */
/* 		      (syn+1)*-1, Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,\ */
/* 		      Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE1); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
	      
	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
  	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }
	  }
	}
      }
    } else { /* TETE */
      for(i=0;i<Lst_Termes.lst_tetes[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_tetes[syn].nbdecomp;j++) {
	  if (Lst_Termes.lst_tetes[entree].decomp[TETE][i]==Lst_Termes.lst_tetes[syn].decomp[TETE][j]){
	    ResInfInv = 1;
	    if (AjoutLienE(Lst_Termes.lst_tetes[entree].decomp[EXPANSION][i], Lst_Termes.lst_tetes[syn].decomp[EXPANSION][j], info|REGLE1|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK)  {
	      e1 = Lst_Termes.lst_tetes[entree].decomp[EXPANSION][i];
	      e2 = Lst_Termes.lst_tetes[syn].decomp[EXPANSION][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R1I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R1I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -1, (e1+1)*-1, (e2+1)*-1, "NV", \
			  Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \
			  syn+1, Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,	\
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE1);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */

/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", (e1+1)*-1, (e2+1)*-1, \ */
/* 		      Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \ */
/* 		      syn+1, Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,\ */
/* 		      Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE1); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
 	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
  	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
  	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }
	  }
	}
      }
    }
  }
/*   printlog("Fin Regle1Inv (%d %d)\n", entree, syn); */
  return(ResInfInv);
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int Regle2Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture)
{
  int i,j;
  int t1, t2;
  int ResInfInv = 0;
/*   char ch[10000]; */

/*   fprintf(stderr,"Debut Regle2Inv (%d %d)\n", entree, syn); */
  if (entree!= syn) {
    if (pos) { /* EXPANSION */
      for(i=0;i<Lst_Termes.lst_exps[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_exps[syn].nbdecomp;j++) {
	  if (Lst_Termes.lst_exps[entree].decomp[EXPANSION][i]==Lst_Termes.lst_exps[syn].decomp[EXPANSION][j]){
	    ResInfInv = 1;
	    if (AjoutLienT(Lst_Termes.lst_exps[entree].decomp[TETE][i], Lst_Termes.lst_exps[syn].decomp[TETE][j] , info|REGLE2|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK)  {
	      t1 = Lst_Termes.lst_exps[entree].decomp[TETE][i];
	      t2 = Lst_Termes.lst_exps[syn].decomp[TETE][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R2I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R2I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -2, t1+1, t2+1, "NV", \
		      Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \
		      (syn+1)*-1, Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			  Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter, \
		      Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE2);
/* 	      send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */

/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", t1+1, t2+1, \ */
/* 		      Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \ */
/* 		      (syn+1)*-1, Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,\ */
/* 		      Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE2); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
	      Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
  	      Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
  	      Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }
	  }
	}
      }
    } else { /* TETE */
      for(i=0;i<Lst_Termes.lst_tetes[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_tetes[syn].nbdecomp;j++) {
	  if (Lst_Termes.lst_tetes[entree].decomp[EXPANSION][i]==Lst_Termes.lst_tetes[syn].decomp[EXPANSION][j]){
	    ResInfInv = 1;
	    if (AjoutLienT(Lst_Termes.lst_tetes[entree].decomp[TETE][i], Lst_Termes.lst_tetes[syn].decomp[TETE][j], info|REGLE2|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK)  {
	      t1 = Lst_Termes.lst_tetes[entree].decomp[TETE][i];
	      t2 = Lst_Termes.lst_tetes[syn].decomp[TETE][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R2I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R2I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -2, t1+1, t2+1, "NV", \
		      Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \
		      syn+1, Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter, \
		      Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE2);
/* 	      send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */

/* 	      sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=2I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", t1+1, t2+1, \ */
/* 		      Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \ */
/* 		      syn+1, Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,\ */
/* 		      Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE2); */
/* 	      print_com_msg(ch,flux_ecriture,NON); */
	      Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
  	      Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
  	      Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }
	  }
	}
      }
    }
  }
/*   printlog("Fin Regle2Inv (%d %d)\n", entree, syn); */
  return(ResInfInv);
}

/************************************************************************
 *                                                                      *
 * Objectif :                                                           *
 *                                                                      *
 * Argument :                                                           *
 *                                                                      *
 ************************************************************************/

int Regle3Inv(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture)
{

  int ResInfInv = 0;

/*    fprintf(stderr,"Debut Regle3Inv (%d %d)\n", entree, syn); */
  ResInfInv = Regle3Inv_depuisCT(FichResLI, entree, syn, pos, info, flux_ecriture);
  ResInfInv |= Regle3Inv_depuisComposants(FichResLI, entree, syn, pos, info, flux_ecriture);

  return(ResInfInv);
}

/* Depuis les CTs et pour lesquels il exuste un composant synonyme */

int Regle3Inv_depuisCT(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture)
{
  int i,j;
  int e1, e2;
  int t1, t2;
  int ResInfInv = 0;
/*   char ch[10000]; */

/*   fprintf(stderr,"Debut Regle3Inv_depuisCT (%d %d)\n", entree, syn); */
  if (entree!=syn) {
    if (pos) { /* EXPANSION */

      for(i=0;i<Lst_Termes.lst_exps[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_exps[syn].nbdecomp;j++) {
	  if ((ExisteUnLienParmiLesRefT(Lst_Termes.lst_exps[entree].decomp[TETE], Lst_Termes.lst_exps[entree].nbdecomp, Lst_Termes.lst_exps[syn].decomp[TETE], Lst_Termes.lst_exps[syn].nbdecomp, Lst_Termes.lst_tetes, info) == EXISTE) &&
	      (AjoutLienE(Lst_Termes.lst_exps[entree].decomp[EXPANSION][i], Lst_Termes.lst_exps[syn].decomp[EXPANSION][j] , info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    ResInfInv = 1;
	    e1 = Lst_Termes.lst_exps[entree].decomp[EXPANSION][i];
	    e2 = Lst_Termes.lst_exps[syn].decomp[EXPANSION][j];
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	    send_ch_res(NULL, flux_ecriture, -3, (e1+1)*-1, (e2+1)*-1, "NV", \
		    Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \
		    (syn+1)*-1, Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter, \
		    Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	    send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */

/* 	    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", (e1+1)*-1, (e2+1)*-1, \ */
/* 		    Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \ */
/* 		    (syn+1)*-1, Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,\ */
/* 		    Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3); */
/* 	    print_com_msg(ch,flux_ecriture,NON); */

	    Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	  }
	  if ((ExisteUnLienParmiLesRefE(Lst_Termes.lst_exps[entree].decomp[EXPANSION], Lst_Termes.lst_exps[entree].nbdecomp, Lst_Termes.lst_exps[syn].decomp[EXPANSION], Lst_Termes.lst_exps[syn].nbdecomp, Lst_Termes.lst_exps, info) == EXISTE) &&
	      (AjoutLienT(Lst_Termes.lst_exps[entree].decomp[TETE][i], Lst_Termes.lst_exps[syn].decomp[TETE][j] , info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_exps[entree].decomp[TETE][i];
	    t2 = Lst_Termes.lst_exps[syn].decomp[TETE][j];
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	    send_ch_res(NULL, flux_ecriture, -3, t1+1, t2+1, "NV", \
		    Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \
		    (syn+1)*-1, Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,	\
		    Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	    send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */

/* 	    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3I>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", t1+1, t2+1, \ */
/* 		    Lst_Termes.lst_exps[entree].lemme, (entree+1)*-1, Lst_Termes.lst_exps[syn].lemme, \ */
/* 		    (syn+1)*-1, Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,\ */
/* 		    Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3); */
/* 	    print_com_msg(ch,flux_ecriture,NON); */
	    Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	  }
	}
      }
    } else { /* TETE */

/*       printlog("Passe TETE\n"); */
      for(i=0;i<Lst_Termes.lst_tetes[entree].nbdecomp;i++) {
	for(j=0;j<Lst_Termes.lst_tetes[syn].nbdecomp;j++) {
	  if ((ExisteUnLienParmiLesRefT(Lst_Termes.lst_tetes[entree].decomp[TETE], Lst_Termes.lst_tetes[entree].nbdecomp, Lst_Termes.lst_tetes[syn].decomp[TETE], Lst_Termes.lst_tetes[syn].nbdecomp, Lst_Termes.lst_tetes, info) == EXISTE) &&
	      (AjoutLienE(Lst_Termes.lst_tetes[entree].decomp[EXPANSION][i], Lst_Termes.lst_tetes[syn].decomp[EXPANSION][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    ResInfInv = 1;
	    e1 = Lst_Termes.lst_tetes[entree].decomp[EXPANSION][i];
	    e2 = Lst_Termes.lst_tetes[syn].decomp[EXPANSION][j];
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	    send_ch_res(NULL, flux_ecriture, -3, (e1+1)*-1, (e2+1)*-1, "NV", \
		    Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \
		    syn+1, Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter, \
		    Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	    send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */
	    
	    Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	  }

	  if ((ExisteUnLienParmiLesRefE(Lst_Termes.lst_tetes[entree].decomp[EXPANSION], Lst_Termes.lst_tetes[entree].nbdecomp, Lst_Termes.lst_tetes[syn].decomp[EXPANSION], Lst_Termes.lst_tetes[syn].nbdecomp, Lst_Termes.lst_exps, info) == EXISTE) &&
	      (AjoutLienT(Lst_Termes.lst_tetes[entree].decomp[TETE][i], Lst_Termes.lst_tetes[syn].decomp[TETE][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_tetes[entree].decomp[TETE][i];
	    t2 = Lst_Termes.lst_tetes[syn].decomp[TETE][j];
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	    send_ch_res(NULL, flux_ecriture, -3, t1+1, t2+1, "NV", \
		    Lst_Termes.lst_tetes[entree].lemme, entree+1, Lst_Termes.lst_tetes[syn].lemme, \
		    syn+1, Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,	\
		    Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	    send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */

	    Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	  }
	}
      }
    }
  }
/*   printlog("Fin Regle3Inv (%d %d)\n", entree, syn); */
  return(ResInfInv);
}

/* Depuis les composants et on ajoute une relation en leur CT */


int Regle3Inv_depuisComposants(FILE *FichResLI, int entree, int syn, int pos, int info, int flux_ecriture)
{
  int i,j;
  int ct1, ct2;
  int e1, e2; 
  int t1, t2;
  int ResInfInv = 0;

/*   fprintf(stderr,"Debut Regle3Inv_depuisComposants (%d %d) pos = %d\n", entree, syn, pos); */
  if (entree!=syn) {
    if (pos) { /* EXPANSION */
      for(i=0;i<Lst_Termes.lst_exps[entree].nbcompdep;i++) {
	ct1 = Lst_Termes.lst_exps[entree].compdep[COMP][i];
	for(j=0;j<Lst_Termes.lst_exps[syn].nbcompdep;j++) {
	  ct2 = Lst_Termes.lst_exps[syn].compdep[COMP][j];
/* 	  printlog("analyse cas %d, %d\n", ct1, ct2); */
/* 	  printlog("dep :  %d, %d\n", Lst_Termes.lst_exps[entree].compdep[DEP][i], Lst_Termes.lst_exps[syn].compdep[DEP][j]); */
	  if ((ct1 > 0) && (ct2 > 0) 
	      && (ExisteLien(DecodeIndTete(ct1),ct2, Lst_Termes.lst_tetes, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
	      && (AjoutLienT(Lst_Termes.lst_exps[entree].compdep[DEP][i], Lst_Termes.lst_exps[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    /* ct : TT */
/* 	    printlog("Propagation\n"); */
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_exps[entree].compdep[DEP][i];
	    t2 = Lst_Termes.lst_exps[syn].compdep[DEP][j];
/* 	    printlog("Trouvé le lien (TT) : %d , %d\n", t1, t2); */
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (E-TT)\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	    send_ch_res(NULL, flux_ecriture, -3, CodeIndTete(t1), CodeIndTete(t2), "NV", \
			Lst_Termes.lst_exps[entree].lemme, CodeIndExp(entree), Lst_Termes.lst_exps[syn].lemme, \
			CodeIndExp(syn), Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter,	\
			Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	    send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */
	    	      
	    Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }
	  if ((ct1 > 0) && (ct2 < 0) 
	      && (ExisteLien(DecodeIndTete(ct1), ct2, Lst_Termes.lst_tetes, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
	      && (AjoutLienT(Lst_Termes.lst_exps[entree].compdep[DEP][i], Lst_Termes.lst_exps[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    /* ct : TE */
/* 	    printlog("Propagation\n"); */
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_exps[entree].compdep[DEP][i];
	    t2 = Lst_Termes.lst_exps[syn].compdep[DEP][j];
/* 	    printlog("Trouvé le lien (TE) : %d , %d\n", t1, t2); */
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (E-TE)\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndTete(t1), CodeIndTete(t2), "NV", \
			  Lst_Termes.lst_exps[entree].lemme, CodeIndExp(entree), Lst_Termes.lst_exps[syn].lemme, \
			  CodeIndExp(syn), Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			  Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter, \
			  Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }
	  if ((ct1 < 0) && (ct2 > 0) 
	      && (ExisteLien(DecodeIndExp(ct1), ct2, Lst_Termes.lst_exps, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
	      && (AjoutLienT(Lst_Termes.lst_exps[entree].compdep[DEP][i], Lst_Termes.lst_exps[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    /* ct : ET */
/* 	    printlog("Propagation\n"); */
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_exps[entree].compdep[DEP][i];
	    t2 = Lst_Termes.lst_exps[syn].compdep[DEP][j];
/* 	    printlog("Trouvé le lien (ET) : %d , %d\n", t1, t2); */
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (E-ET)\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndTete(t1), CodeIndTete(t2), "NV", \
			  Lst_Termes.lst_exps[entree].lemme, CodeIndExp(entree), Lst_Termes.lst_exps[syn].lemme, \
			  CodeIndExp(syn), Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			  Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter, \
			  Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }
	  if ((ct1 < 0) && (ct2 < 0) 
	      && (ExisteLien(DecodeIndExp(ct1), ct2, Lst_Termes.lst_exps, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
	      && (AjoutLienT(Lst_Termes.lst_exps[entree].compdep[DEP][i], Lst_Termes.lst_exps[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	    /* ct : EE */
/* 	    printlog("Propagation\n"); */
	    ResInfInv = 1;
	    t1 = Lst_Termes.lst_exps[entree].compdep[DEP][i];
	    t2 = Lst_Termes.lst_exps[syn].compdep[DEP][j];
/* 	    printlog("Trouvé le lien (EE) : %d , %d\n", t1, t2); */
	    fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (E-EE)\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme);
/* 	    printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndTete(t1), CodeIndTete(t2), "NV", \
			  Lst_Termes.lst_exps[entree].lemme, CodeIndExp(entree), Lst_Termes.lst_exps[syn].lemme, \
			  CodeIndExp(syn), Lst_Termes.lst_exps[entree].donneesLexter, Lst_Termes.lst_exps[syn].donneesLexter, \
			  Lst_Termes.lst_tetes[t1].lemme, Lst_Termes.lst_tetes[t1].donneesLexter, \
			  Lst_Termes.lst_tetes[t2].lemme, Lst_Termes.lst_tetes[t2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndTete(t1), Lst_Termes.lst_tetes[t1].lemme, CodeIndTete(t2), Lst_Termes.lst_tetes[t2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle2Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	      Regle3Inv(FichResLI, t1, t2, TETE ,info, flux_ecriture);
	    }

	}
      }
    } else { /* TETE */
/*       printlog("pos = TETE %d (%s) %d (%s)\n",Lst_Termes.lst_tetes[entree].nbcompdep, Lst_Termes.lst_tetes[entree].lemme,Lst_Termes.lst_tetes[syn].nbcompdep,Lst_Termes.lst_tetes[syn].lemme); */
      for(i=0;i<Lst_Termes.lst_tetes[entree].nbcompdep;i++) {
	ct1 = Lst_Termes.lst_tetes[entree].compdep[COMP][i];
	for(j=0;j<Lst_Termes.lst_tetes[syn].nbcompdep;j++) {
	  ct2 = Lst_Termes.lst_tetes[syn].compdep[COMP][j];
/* 	  printlog("ct1 : %d, ct2 : %d\n", ct1, ct2); */
	    if ((ct1>0) && (ct2>0) 
		&& (ExisteLien(DecodeIndTete(ct1),ct2, Lst_Termes.lst_tetes, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
		&& (AjoutLienE(Lst_Termes.lst_tetes[entree].compdep[DEP][i], Lst_Termes.lst_tetes[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	      /* ct : TT */
	      ResInfInv = 1;
	      e1 = Lst_Termes.lst_tetes[entree].compdep[DEP][i];
	      e2 = Lst_Termes.lst_tetes[syn].compdep[DEP][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (T-TT)\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndExp(e1), CodeIndExp(e2), "NV", \
			  Lst_Termes.lst_tetes[entree].lemme, CodeIndTete(entree), Lst_Termes.lst_tetes[syn].lemme, \
			  CodeIndTete(syn), Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,	\
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }
	    if ((ct1>0) && (ct2<0) 
		&& (ExisteLien(DecodeIndTete(ct1),ct2, Lst_Termes.lst_tetes, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
		&& (AjoutLienE(Lst_Termes.lst_tetes[entree].compdep[DEP][i], Lst_Termes.lst_tetes[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	      /* ct : TE */
	      ResInfInv = 1;
	      e1 = Lst_Termes.lst_tetes[entree].compdep[DEP][i];
	      e2 = Lst_Termes.lst_tetes[syn].compdep[DEP][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (T-TE)\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndExp(e1), CodeIndExp(e2), "NV", \
			  Lst_Termes.lst_tetes[entree].lemme, CodeIndTete(entree), Lst_Termes.lst_tetes[syn].lemme, \
			  CodeIndTete(syn), Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,	\
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }
	    if ((ct1<0) && (ct2>0) 
		&& (ExisteLien(DecodeIndExp(ct1),ct2, Lst_Termes.lst_exps, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
		&& (AjoutLienE(Lst_Termes.lst_tetes[entree].compdep[DEP][i], Lst_Termes.lst_tetes[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	      /* ct : ET */
	      ResInfInv = 1;
	      e1 = Lst_Termes.lst_tetes[entree].compdep[DEP][i];
	      e2 = Lst_Termes.lst_tetes[syn].compdep[DEP][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (T-ET)\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndExp(e1), CodeIndExp(e2), "NV", \
			  Lst_Termes.lst_tetes[entree].lemme, CodeIndTete(entree), Lst_Termes.lst_tetes[syn].lemme, \
			  CodeIndTete(syn), Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,	\
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }
	    if ((ct1<0) && (ct2<0) 
		&& (ExisteLien(DecodeIndExp(ct1),ct2, Lst_Termes.lst_exps, info, NO_INCRPROD, flux_ecriture) == EXISTE) 
		&& (AjoutLienE(Lst_Termes.lst_tetes[entree].compdep[DEP][i], Lst_Termes.lst_tetes[syn].compdep[DEP][j], info|REGLE3|NON_VAL, INCRPROD_ONESTEP, flux_ecriture) == EXEC_OK))  {
	      /* ct : EE */
	      ResInfInv = 1;
	      e1 = Lst_Termes.lst_tetes[entree].compdep[DEP][i];
	      e2 = Lst_Termes.lst_tetes[syn].compdep[DEP][j];
	      fprintf(FichResLI, "%s : %s : 1 : 0 : R3I (T-EE)\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme);
/* 	      printlog("%s : %s : 1 : 0 : R3I\n", Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e2].lemme); */

	      send_ch_res(NULL, flux_ecriture, -3, CodeIndExp(e1), CodeIndExp(e2), "NV", \
			  Lst_Termes.lst_tetes[entree].lemme, CodeIndTete(entree), Lst_Termes.lst_tetes[syn].lemme, \
			  CodeIndTete(syn), Lst_Termes.lst_tetes[entree].donneesLexter, Lst_Termes.lst_tetes[syn].donneesLexter, \
			  Lst_Termes.lst_exps[e1].lemme, Lst_Termes.lst_exps[e1].donneesLexter,	\
			  Lst_Termes.lst_exps[e2].lemme, Lst_Termes.lst_exps[e2].donneesLexter,LIEN_EXTRAIT|REGLE3);
/* 	      send_production(flux_ecriture,  CodeIndExp(e1), Lst_Termes.lst_exps[e1].lemme, CodeIndExp(e2), Lst_Termes.lst_exps[e2].lemme, 1); */
	      
	      Regle1Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle2Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	      Regle3Inv(FichResLI, e1, e2, EXPANSION ,info, flux_ecriture);
	    }

	}
      }
    }
  }
/*   printlog("Fin Regle3Inv_depuisComposants (%d %d)\n", entree, syn); */
  return(ResInfInv);

}
