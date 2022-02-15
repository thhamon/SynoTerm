#include "Communication.h"

/************************************************************************
 *                                                                      *
 * Objectif : Envoi du message en argument dans le flux d'ecriture      *
 *                                                                      *
 * Argument : 1. Le message                                             *
 *                                                                      *
 * Retour : l'etat d'execution                                          *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int print_com_msg(char *msg_prn, int flux_ecrit, int NL) {
/*   char *msg; */
  char c=SEP_LIGNE;;

  if (flux_ecrit != -1) {

    /*   if ((msg = (char *)malloc(sizeof(char)*(strlen(msg_prn) + 1)))==NULL) { */
    /*     fprintf(stderr,"Erreur d'allocation memoire\n"); */
    /*     fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2); */
    /*     return(ERR_ALLOCATION); */
    /*   } */
    /*   strcpy(msg,msg_prn); */
    /*   printlog(msg_prn); */
    /*   printlog("\n"); */
    if (write(flux_ecrit, msg_prn, sizeof(char) * strlen(msg_prn)) == -1) {
      fprintf(stderr,"Erreur de lecture de la chaine sur le flux de communication\n");
      fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
      return(ERR_ECRITURE);
    }
    /*   free(msg); */

    if (NL == OUI) {
      /*     printlog("NL\n"); */
      if (write(flux_ecrit, &c, sizeof(char)) == -1) {
	fprintf(stderr,"Erreur de lecture de la chaine sur le flux de communication\n");
	fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
	return(ERR_ECRITURE);
      }
    } else {
      /*         printlog("non NL\n"); */
    }
  }
  return(EXEC_OK);
}

int send_production(int flux_ecriture, int Terme1, char *Lemme1, int Terme2, char *Lemme2, int production) {
  char ch[BUF_SIZE_COM];

  print_com_msg("PRODUCTIVITY",flux_ecriture,OUI);
  sprintf(ch, "%d", Terme1);
  print_com_msg(ch,flux_ecriture,OUI);
  print_com_msg(Lemme1,flux_ecriture,OUI);
  sprintf(ch, "%d", Terme2);
  print_com_msg(ch,flux_ecriture,OUI);
  print_com_msg(Lemme2,flux_ecriture,OUI);
  sprintf(ch, "%d", production);
  print_com_msg(ch,flux_ecriture,OUI);
  return(0);
}

/************************************************************************
 *                                                                      *
 * Objectif : Gestion de l'enregistrement du lien calculé, avec les     *
 *            informations associés                                     *
 *                                                                      *
 * Arguments : 1. Flux sur le fichier contenant les resultats et        *
 *                notamment le nouveau lien trouve                      *
 *                                                                      *
 *             2. Flux d'ecriture                                       *
 *                                                                      *
 *             3. Numéro de la règle                                    *
 *                                                                      *
 *             4. (et suivant) informations concernant le lien          *
 *                                                                      *
 * Date de derniere modification : 10/08/2006                           *
 *                                                                      *
 ************************************************************************/

int send_ch_res(FILE *FichRes, int flux_ecriture, int rule, ... ) {
  va_list p_arg;
  int num_arg;
  int d_arg1, d_arg2;
  char ch[BUF_SIZE_COM];
  char *str;

  switch (rule) {
  case 1: num_arg == NB_ARG_R1; break;
  case 2: num_arg == NB_ARG_R2; break;
  case 3: num_arg == NB_ARG_R3; break;
  default: num_arg=NB_ARG_DEFAULT;
  }
  va_start  (p_arg, rule);
  
  d_arg1 = va_arg (p_arg, int);
  d_arg2 = va_arg (p_arg, int);
  str = va_arg (p_arg, char *);
  sprintf(ch, "<LIEN ID=%d_%d TYPE=%s>\n", d_arg1, d_arg2, str);
  if (FichRes != NULL) {fprintf(FichRes, ch);}
  if (flux_ecriture != -1) {print_com_msg(ch,flux_ecriture,NON);}

  sprintf(ch, "<FROM R=%d>", rule);
  if (FichRes != NULL) {  fprintf(FichRes, ch);}
  if (flux_ecriture != -1) {  print_com_msg(ch,flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}
  d_arg1 = va_arg (p_arg, int);
  sprintf(ch, "(%d) <=> ", d_arg1);
  if (FichRes != NULL) {fprintf(FichRes, ch);}
  if (flux_ecriture != -1) {  print_com_msg(ch,flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {  fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}
  d_arg1 = va_arg (p_arg, int);
  sprintf(ch, "(%d)</FROM>\n", d_arg1);
  if (FichRes != NULL) {  fprintf(FichRes, ch);}
  if (flux_ecriture != -1) {  print_com_msg(ch,flux_ecriture,NON);}


  if (rule < 0) {
    if (FichRes != NULL) {fprintf(FichRes, "<ORIG>");}
    if (flux_ecriture != -1) {  print_com_msg("<ORIG>",flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");}
    if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);}
    str = va_arg (p_arg, char *);
    if (FichRes != NULL) {fprintf(FichRes, str);}
    if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "\n</CT>");}
    if (flux_ecriture != -1) {  print_com_msg("\n</CT>",flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "\n<=>");}
    if (flux_ecriture != -1) {  print_com_msg("\n<=>",flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");}
    if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);}
    str = va_arg (p_arg, char *);
    if (FichRes != NULL) {fprintf(FichRes, str);}
    if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "\n</CT>");}
    if (flux_ecriture != -1) {  print_com_msg("\n</CT>",flux_ecriture,NON);}
    if (FichRes != NULL) {fprintf(FichRes, "</ORIG>\n");}
    if (flux_ecriture != -1) {  print_com_msg("</ORIG>\n",flux_ecriture,NON);}
  }  
  if (rule == 3) {
    sprintf(ch, "<FROM R=%d>", rule);
    if (FichRes != NULL) {    fprintf(FichRes, ch);}
    if (flux_ecriture != -1) {    print_com_msg(ch,flux_ecriture,NON);}
    str = va_arg (p_arg, char *);
    if (FichRes != NULL) {fprintf(FichRes, str);}
    if (flux_ecriture != -1) {    print_com_msg(str,flux_ecriture,NON);}
    d_arg1 = va_arg (p_arg, int);
    sprintf(ch, "(%d) <=> ", d_arg1);
    if (FichRes != NULL) {fprintf(FichRes, ch);}
    if (flux_ecriture != -1) {    print_com_msg(ch,flux_ecriture,NON);}
    str = va_arg (p_arg, char *);
    if (FichRes != NULL) {fprintf(FichRes, str);}
    if (flux_ecriture != -1) {    print_com_msg(str,flux_ecriture,NON);}
    d_arg1 = va_arg (p_arg, int);
    sprintf(ch, "(%d)</FROM>\n", d_arg1);
    if (FichRes != NULL) {fprintf(FichRes, ch);}
    if (flux_ecriture != -1) {    print_com_msg(ch,flux_ecriture,NON);}

/*     if (rule < 0) { */
/*       if (FichRes != NULL) {fprintf(FichRes, "<ORIG>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("<ORIG>",flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);} */
/*       str = va_arg (p_arg, char *); */
/*       if (FichRes != NULL) {fprintf(FichRes, str);} */
/*       if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "\n</CT>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("\n</CT>",flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "\n<=>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("\n<=>",flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);} */
/*       str = va_arg (p_arg, char *); */
/*       if (FichRes != NULL) {fprintf(FichRes, str);} */
/*       if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "\n</CT>");} */
/*       if (flux_ecriture != -1) {  print_com_msg("\n</CT>",flux_ecriture,NON);} */
/*       if (FichRes != NULL) {fprintf(FichRes, "</ORIG>\n");} */
/*       if (flux_ecriture != -1) {  print_com_msg("</ORIG>\n",flux_ecriture,NON);} */
/*   } */

  }

  if (FichRes != NULL) {fprintf(FichRes, "<GEN>");}
  if (flux_ecriture != -1) {  print_com_msg("<GEN>",flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}

  if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");}
  if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}

  if (FichRes != NULL) {fprintf(FichRes, "\n</CT>\n<=> ");}
  if (flux_ecriture != -1) {  print_com_msg("\n</CT>\n<=> ",flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}

  if (FichRes != NULL) {fprintf(FichRes, "\n<CT>");}
  if (flux_ecriture != -1) {  print_com_msg("\n<CT>",flux_ecriture,NON);}
  str = va_arg (p_arg, char *);
  if (FichRes != NULL) {fprintf(FichRes, str);}
  if (flux_ecriture != -1) {  print_com_msg(str,flux_ecriture,NON);}

  if (FichRes != NULL) {fprintf(FichRes, "\n</CT>\n</GEN>\n");}
  if (flux_ecriture != -1) {  print_com_msg("\n</CT>\n</GEN>\n",flux_ecriture,NON);}

  d_arg1 = va_arg (p_arg, int);
  sprintf(ch, "<VAL>%d</VAL>\n</LIEN>\n", d_arg1);
  if (FichRes != NULL) {fprintf(FichRes, ch);}
  if (flux_ecriture != -1) {  print_com_msg(ch,flux_ecriture,NON);}

  va_end (p_arg);


/* 	    sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=1>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_exps[E1].compdep[COMP][i], Lst_Termes.lst_tetes[frere].compdep[COMP][j], Lst_Termes.lst_exps[E1].lemme, CodeIndExp(E1), Lst_Termes.lst_exps[E2].lemme, CodeIndExp(E2), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[E1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[frere].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE1); */
/* 		sprintf(ch,"<LIEN ID=%d_%d TYPE=NV>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<FROM R=3>%s(%d) <=> %s(%d)</FROM>\n<GEN>%s\n<CT>%s\n</CT>\n<=> %s\n<CT>%s\n</CT>\n</GEN>\n<VAL>%d</VAL>\n</LIEN>\n", Lst_Termes.lst_tetes[T1].compdep[COMP][i], Lst_Termes.lst_exps[freresyn].compdep[COMP][j], Lst_Termes.lst_tetes[T1].lemme, CodeIndTete(T1), Lst_Termes.lst_tetes[T2].lemme, CodeIndTete(T2), Lst_Termes.lst_exps[frere].lemme, CodeIndExp(frere) , Lst_Termes.lst_exps[freresyn].lemme, CodeIndExp(freresyn), Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_tetes[T1].compdep[COMP][i])].donneesLexter, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].lemme, Lst_Termes.lst_tetes[DecodeIndTete(Lst_Termes.lst_exps[freresyn].compdep[COMP][j])].donneesLexter,DICOSYN|REGLE3); */

      return(EXEC_OK);

}

