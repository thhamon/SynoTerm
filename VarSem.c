/* 	$Log: VarSem.c,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.2  2001/05/01 19:42:35  thierry
 * 	*** empty log message ***
 *	 */
#include "VarSem.h"

char nomlog[TAILLE_FICHIER_LOG];
int CG;
int type_devel;

/************************************************************************
 *                                                                      *
 * Objectif : Ecriture d'un message dans un fichier de log              *
 *                                                                      *
 * Argument : 1. le formatage (type printf)                             *
 *            2, ... les arguments                                      *
 *                                                                      *
 * Retour : l'etat d'execution de la fonction                           *
 *                                                                      *
 * Date de derniere modification : 22/09/2003                           *
 *                                                                      *
 * NB : la fonction s'inspire d'un exemple de man stdarg                *
 *                                                                      *
 ************************************************************************/

int printlog(char *strformat, ...) {
  va_list p_arg;
  int     d;
  char    c, *s;

  FILE *fichierlog;
  int TempsProg;
  time_t TimeProg;
  char *tmpstr;

  if ((fichierlog=fopen(nomlog,"a"))==NULL){
    fprintf(stderr,"Erreur a l'ouverture du fichier %s", nomlog);
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    exit(ERR_FICHIER_NOEXIST);
  }

  if ((TimeProg = time(NULL)) == (time_t)-1) {
    fprintf(stderr,"Erreur lors de la recuperation du temps (fonction time)");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    exit(ERR_TIME);    
  }
  if ((tmpstr=ctime(&TimeProg)) == NULL) {
    fprintf(stderr,"Erreur lors de la recuperation du temps (fonction time)");
    fprintf(stderr,"fichier %s, ligne %d\n",__FILE__, __LINE__ - 2);
    exit(ERR_TIME);    
  }
  tmpstr[strlen(tmpstr)-1]=0;

  fprintf(fichierlog,"%s; -- ",tmpstr);
  
  va_start  (p_arg, strformat);
  while(*strformat)
    if ((c= (*strformat ++))=='%') {
      switch  (c=(*strformat ++)) {
      case 's': /* chaîne */
	s = va_arg (p_arg, char *);
	fprintf(fichierlog,"%s", s);
	break;
      case 'd':  /* entier */
	d = va_arg (p_arg, int);
	fprintf (fichierlog,"%d", d);
	break;
      default:
	fprintf(fichierlog,"%%c",c);
      }
    } else {
      fprintf(fichierlog,"%c",c);
    }
  va_end (p_arg);

  fclose(fichierlog);

  return(EXEC_OK);
}

/************************************************************************/

int writelogS(char *strmessage)
{
  FILE *fichlog;
  int TempsProg;
  time_t TimeProg;
  char *tmpstr;

  if ((fichlog=fopen(nomlog,"a"))==NULL){
    fprintf(stderr,"Erreur a l'ouverture du fichier SynoTerm.log");
    exit(-1);
  }

  TimeProg = time(NULL);

  tmpstr=ctime(&TimeProg);
  tmpstr[strlen(tmpstr)-1]=0;
  fprintf(fichlog,"%s; --> %s\n",tmpstr,strmessage);
 
  fclose(fichlog);
  return(1);
}

int writelogI(int entmessage)
{  
  FILE *fichlog;
  int TempsProg;
  time_t TimeProg;
  char tmpctime[100];
 
  if ((fichlog=fopen(nomlog,"a"))==NULL){
    fprintf(stderr,"Erreur a l'ouverture du fichier SynoTerm.log");
    exit(-1);
  }
   
  TempsProg = clock();
  TimeProg = time(NULL);
  strcpy(tmpctime,ctime(&TimeProg));
  tmpctime[strlen(tmpctime)-1]=0;
  fprintf(fichlog,"%s; --> %d\n",tmpctime,entmessage);

  fclose(fichlog);
  return(1);
}

int writelogD(double entmessage)
{  
  FILE *fichlog;
  int TempsProg;
  time_t TimeProg;
  char tmpctime[100];
 
  if ((fichlog=fopen(nomlog,"a"))==NULL){
    fprintf(stderr,"Erreur a l'ouverture du fichier SynoTerm.log");
    exit(-1);
  }
   
  TempsProg = clock();
  TimeProg = time(NULL);
  strcpy(tmpctime,ctime(&TimeProg));
  tmpctime[strlen(tmpctime)-1]=0;
  fprintf(fichlog,"%s; --> %G\n",tmpctime,entmessage);

  fclose(fichlog);
  return(1);
}

int ecrit_sock(int sock, char *msg)
{
    int longueur;

    /*writelogS("ecrit_sock");*/
    longueur = strlen(msg);
    /*writelogI(longueur);
    writelogS(msg);*/
    write(sock, &longueur, sizeof (int));
    write(sock, msg, longueur);
    
    return 0;
}

char *lit_sock(int sockn)
{
    int recep,longueur,i;
    char *msg;
    
    /*writelogS("lit_sockn");*/
    msg = NULL;
    recep = read(sockn, &longueur, sizeof (int));
    if (recep > 0) {
        /*writelogI(longueur);*/
    	msg = (char *)malloc(sizeof(char)*(longueur+1));
	/*sleep(1);*/
	i=0;
	while(i<longueur){
	    recep = read(sockn, &msg[i], sizeof(char));
	    if (recep > 0) i++;
	}
	msg[longueur] = 0;
	/*writelogS(msg);*/
	return msg;
	/*if (read(sockn, msg, longueur) == 0) {
	    fprintf(stderr,"Erreur a la reception");
	    return NULL;
	}
	else {
	    writelogI(longueur);
	    msg[longueur] = 0;
	    writelogS(msg);
	    return msg;
	}*/
    }
    return NULL;
}

/*
 * Separation du nom de l'extension
 */

int recup_nom_ext(char *nom_ext, char **nom, char **ext)
{
  char *lst_pt;
  
  lst_pt = (char *)strrchr(nom_ext,'.');
  if (lst_pt) {
    *nom = (char *)malloc(sizeof(char)*(lst_pt-nom_ext+1));
    strncpy(*nom,nom_ext,lst_pt-nom_ext);
    nom[0][lst_pt-nom_ext]=0;
    /*fprintf(stderr,"%s|\n",*nom);*/
    lst_pt++;
    *ext = (char *)malloc(sizeof(char)*(strlen(lst_pt)+1));
    strcpy(*ext,lst_pt);
  }
  else{
    *nom = (char *)malloc(sizeof(char)*(strlen(nom_ext)+1));
    strcpy(*nom,nom_ext);
    *ext = 0;
  }
}

/* 
 * envoi d'un ordre dans les descripteurs de socket
 */

int EnvoiOrdre(int nbsock, int *lst_sock, char *ordre)
{
    int i;
    /*char tmp[3000];*/

    for(i=0;i<nbsock;i++) {
	/*sprintf(tmp,ordre,i);*/
	ecrit_sock(lst_sock[i], ordre);
    }
    return(1);
}

int lit_champ(FILE *FluxF, char *str, char sep)
{
    int c;
    c = 0;
    do{
	fread(&(str[c]),sizeof(char),1,FluxF);
	c++;
    }while((!feof(FluxF))&&(str[c-1] != sep));
    str[c-1] = 0;
    return(1);
}


/************************************************************************
 *                                                                      *
 * Objectif : Calcul de la longueur maximale de deux chaînes de         *
 *            caractères                                                *
 *                                                                      *
 * Argument : 1. La première chaine de caractères                       *
 *            2. La deuxieme chaine de caractères                       *
 *                                                                      *
 * Retour :  La longueur maximale                                       *
 *                                                                      *
 * Date de derniere modification : 15/12/2003                           *
 *                                                                      *
 ************************************************************************/

int max_len(char *str1, char *str2) {

  if (strlen(str1) > strlen(str2)) {
    return(strlen(str1));
  } else {
    return(strlen(str2));
  }

}
