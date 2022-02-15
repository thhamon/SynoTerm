/* 	$Log: VarSem.Socket.c,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.2  2001/05/01 19:42:46  thierry
 * 	*** empty log message ***
 *	 */
#include "VarSem.h"

int writelogS(char *strmessage)
{
  FILE *fichlog;
  int TempsProg;
  time_t TimeProg;
  char *tmpstr;

  if ((fichlog=fopen("SynoTerm.log","a"))==NULL){
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
 
  if ((fichlog=fopen("SynoTerm.log","a"))==NULL){
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


/* 
 * Fonction principale
 */

main(int argc, char **argv) {

    int vers_fils[2];  /* descripteurs du pipe "pere->fils" */
    int vers_pere[2];  /* descripteurs du pipe "fils->pere" */
    int pid, pid2, pid3;
    int socket_Interf,nsock,len,resbind;
    char cmd[1000];
    char tmp[100];
    /*struct sockaddr_un adresse;*/
    struct sockaddr_in serveur;
    
    system("rm -f SynoTerm.log");
    
    /*strcpy(cmd,"Interface/SynoTerm.tcl ");*/
    /*writelogS("*************************************************");
    writelogS("Extraction de liens semantiques\n\t\t        dans des documents techniques v0.4-2b");
    writelogS("*************************************************");
    writelogS("Lancement des programmes");*/
    
    if ((socket_Interf = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr,"Pas de socket !\n");
	exit(0);
    };
    
    bzero(&serveur, sizeof(serveur));
    serveur.sin_family = AF_INET;
    serveur.sin_addr.s_addr = INADDR_ANY;
    serveur.sin_port = htons(PORT);
    if (bind(socket_Interf, (struct sockaddr *) &serveur,sizeof(serveur)) < 0) {
	fprintf(stderr,"Pas de bind !\n");
	exit(0);
	sleep(1);
    };
    len = sizeof(serveur);
    if (getsockname(socket_Interf, (struct sockaddr *) &serveur, &len) < 0) {
	fprintf(stderr,"Impossible de recuperer le nom du socket !\n");
	exit(0);
    };
    fprintf(stdout,"Socket port %d\n",ntohs(serveur.sin_port));
    sprintf(cmd,"Interface/SynoTerm.tcl %d &",ntohs(serveur.sin_port));
    listen(socket_Interf, 5);
    system(cmd);
    printf("(attente d'un nouveau client)\n");
    nsock = accept(socket_Interf, (struct sockaddr *) 0, (int *) 0);
    printf("(nouveau client accepte)\n");
    close(nsock);
    close(socket_Interf);

    
    exit(1);

    if ((argc>1)&&(strcmp(argv[1],"-nI")==0)) {
	printf("*************************************************");
	printf("Extraction de liens semantiques\n\t\t        dans des documents techniques v0.4-2b");
	printf("*************************************************");
	printf("\n\n");
	
	Commandes();
    }
    else {
    
    
    pipe ( vers_fils );
    pipe ( vers_pere );
     
    if ( pid = fork (), pid == 0 ){		/* dans le fils   */
	close ( 0 );			/* redirection de stdin */
	dup ( vers_fils[0] );
	close ( 1 );			/* redirection de stdout*/
	dup ( vers_pere[1] );

	/* fermeture de pipes    */
	close ( vers_fils[0] );
	/*close ( vers_fils[1] );*/
	/*close ( vers_pere[0] );*/
	close ( vers_pere[1] );
	
	writelogS("Lancement de l'Interface");
	execlp ( cmd, NULL );      /* exec the new cmd */
    }
    else 
    	if ( pid > 0 ) {		/* dans le pere  */
	    close ( 0 );	/* redirection de stdin */
	    dup ( vers_pere[0] );
	    close ( 1 );	/* redirection de stdout  */
	    dup ( vers_fils[1] );

	    setbuf ( stdout, NULL );        /* pas de sortie bufferisee */
	    /* fermeture de pipes */
	    /*close ( vers_fils[0] );*/
	    close ( vers_fils[1] );
	    close ( vers_pere[0] );
	    /*close ( vers_pere[1] );*/
		
	    if ( pid2 = fork (), pid2 == 0 ) {
		writelogS("Lancement de l'affichage des log");
		system("xterm -geometry -0-25 -e tail -f SynoTerm.log");
	    }
	    else 
		if ( pid2 > 0 ) {
	/* Creation de la socket */
	
	if ((socket_Interf=socket(AF_INET, SOCK_STREAM, 0))==-1){
	    fprintf(stderr,"erreur a l'ouverture de la socket");
	    writelogS("erreur a l'ouverture de la socket");
	}
	bzero(&serveur, sizeof(serveur));
	serveur.sin_family = AF_INET;
	serveur.sin_addr.s_addr = INADDR_ANY;
	serveur.sin_port = htons(7777);
	if (bind(socket_Interf, (struct sockaddr *) &serveur,sizeof(serveur)) < 0) {
	    fprintf(stderr,"Pas de bind !\n");
	    writelogS("erreur a l'association de la socket");
	    close(socket_Interf);
	    exit(0);
	};
	len = sizeof(serveur);
	if (getsockname(socket_Interf, (struct sockaddr *) &serveur, &len) < 0) {
	    fprintf(stderr,"Impossible de recuperer le nom du socket !\n");
	    exit(0);
	};
	fprintf(stderr,"Socket port %d Adress %d\n",ntohs(serveur.sin_port),serveur.sin_addr.s_addr);
	listen(socket_Interf, 10);
	nsock = accept(socket_Interf, (struct sockaddr *) 0, (int *) 0);

	
	
		    writelogS("Lancement des Commandes");
		    Commandes();
		    kill(pid2 + 2,9);
		    close(nsock);
		}
		else {	/* erreur!       */
		    fprintf ( stderr,"Couldn't fork process %s\n", cmd );
		    exit ( 1 );
		}
	}
	else {	/* erreur!       */
	    fprintf ( stderr,"Couldn't fork process %s\n", cmd );
	    exit ( 1 );
	}
	}
}

int Commandes() 
{
    char MsgRecep[1000],MsgEnv[1000],NomFich[1000];
    char *NomBase, *ExtBase, tmp[100];
    int Fich1, Fich2, *lst_chp, nb_chp;
    int *liste_corsp[2], i;
	

    nbre_fichier = 0;
    i=0;
    while(i<NBFICHIER){
    	FermerF(i,NULL);
	i++;
    }
    do {
	if (fgets(MsgRecep,1000,stdin)!=NULL) {
	    MsgRecep[strlen(MsgRecep)-1] = 0;
	    writelogS(MsgRecep);
	    if (strcmp(MsgRecep,"Load_db")==0) 
		if (fgets(NomFich,1000,stdin)!=NULL) {
		    NomFich[strlen(NomFich)-1] = 0;
		    writelogS("Ajout de la table pour le dico");
		    writelogS(NomFich);
		    charger_db(NomFich, NULL);
		    writelogS("Fin Ajout de la table pour le dico");
		}
	    if (strcmp(MsgRecep,"Load_desamb")==0)
		if (fgets(NomFich,1000,stdin)!=NULL) {
	    	    NomFich[strlen(NomFich)-1] = 0;
		    writelogS("Chargement d'un fichier .desamb");
		    recup_nom_ext(NomFich,&NomBase,&ExtBase);
		    strcpy(tmp,NomBase);
		    strcat(tmp,".db");
		    Charge_desamb(NomFich,tmp);
		    writelogS("Fin Chargement d'un fichier .desamb");
	        }
	    if (strcmp(MsgRecep,"Load_syn")==0)
		if (fgets(NomFich,1000,stdin)!=NULL) {
	    	    NomFich[strlen(NomFich)-1] = 0;
		    writelogS("Chargement du dictionnaire INALF7");
		    recup_nom_ext(NomFich,&NomBase,&ExtBase);
		    charge_Dico_INaLF7(NomFich,NomBase);
		    writelogS("Fin Chargement du dictionnaire INALF7");
		}
	    if (strcmp(MsgRecep,"Load_dic")==0)
	    	if (fgets(NomFich,1000,stdin)!=NULL) {
	    	    NomFich[strlen(NomFich)-1] = 0;
		    writelogS("Chargement d'un dictionnaire");
		    writelogS("Operation non disponible");
		    writelogS("Fin Chargement d'un dictionnaire");
		}
	    if (strcmp(MsgRecep,"Infoall")==0) {
		writelogS("Info pour toutes les tables");
		info_table(ALL);
		writelogS("Fin Info pour toutes les tables");
	    }
	    if (strcmp(MsgRecep,"Infosyn")==0) {
		writelogS("Info pour les tables syn");
		info_table(SYN);
		writelogS("Fin Info pour les tables syn");
	    }
	    if (strcmp(MsgRecep,"Infodesamb")==0) {
		writelogS("Info pour les tables desamb");
		info_table(DESAMB);
		writelogS("Fin Info pour les tables desamb");
	    }
	    if (strcmp(MsgRecep,"Infonew")==0) {
		writelogS("Info pour les tables dbnew");
		info_table(NEW);
		writelogS("Fin Info pour les tables dbnew");
	    }
	    if (strcmp(MsgRecep,"SynElimDoubl")==0) {
		writelogS("Elimination des doublons Syn");
	    	if (fgets(NomFich,1000,stdin)!=NULL) {
	    	    NomFich[strlen(NomFich)-1] = 0;
		    writelogS(NomFich);
		    Fich1 = get_table(NomFich);
		    if (Fich1!=-1){
		    	writelogI(Fich1);
		    	deldoublon_db(Fich1);
		    }
		}
		writelogS("Fin Elimination des doublons Syn");
	    }
	    if (strcmp(MsgRecep,"SynElimDoublC")==0) {
		writelogS("Elimination des doublons Croises Syn");
	    	if (fgets(NomFich,1000,stdin)!=NULL) {
	    	    NomFich[strlen(NomFich)-1] = 0;
		    writelogS(NomFich);
		    Fich1 = get_table(NomFich);
		    if (Fich1!=-1){
		    	writelogI(Fich1);
			deldoublonCr_db(Fich1);
		    }
		}
		writelogS("Fin Elimination des doublons Croises Syn");
	    }
	    if (strcmp(MsgRecep,"FiltrageCTSyn")==0) {
		writelogS("Filtrage CTSyn");
		if (fgets(NomFich,1000,stdin)!=NULL) {
		    NomFich[strlen(NomFich)-1] = 0;
		    writelogS(NomFich);
		    Fich1 = get_table(NomFich);
		    if ((Fich1!=-1)&&(fgets(NomFich,1000,stdin)!=NULL)){
		     	NomFich[strlen(NomFich)-1] = 0;
			writelogS(NomFich);
			Fich2 = get_table(NomFich);
			Filtrage(Fich1, Fich2);
		    }
		}
		writelogS("Fin Filtrage CTSyn");
	    }
	    if (strcmp(MsgRecep,"InferenceCTSyn")==0) {
		writelogS("Inference CTSyn");
		if (fgets(NomFich,1000,stdin)!=NULL) {
		    NomFich[strlen(NomFich)-1] = 0;
		    writelogS(NomFich);
		    Fich1 = get_table(NomFich);
		    if ((Fich1!=-1)&&(fgets(NomFich,1000,stdin)!=NULL)){
		     	NomFich[strlen(NomFich)-1] = 0;
			writelogS(NomFich);
			Fich2 = get_table(NomFich);
			if ((Fich2!=-1)&&(fgets(NomFich,1000,stdin)!=NULL)){
			   NomFich[strlen(NomFich)-1] = 0;
			   writelogS(NomFich);
			   InferenceCTSyn(Fich1, Fich2,1,NomFich);
			}
		    }
		}
		writelogS("Fin Inference CTSyn");
	    }
	    if (strcmp(MsgRecep,"Validation")==0) {
	    	
	    }
	}
    } while(strcmp(MsgRecep,"Quit")!=0);
}

