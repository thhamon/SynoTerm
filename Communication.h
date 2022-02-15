#include "Fichier.h"

#if !defined(COMMUNICATION_H)
#define COMMUNICATION_H


#define OUI 1
#define NON 0

#define ENVOI_FICHIER_RESULTAT "FichierGNM"
#define ENVOI_DEBUT_INFERENCE "DebutInfer"
#define ENVOI_FIN_INFERENCE "FinInfer"
#define ENVOI_DEBUT_EXTRACTION "DebutExtract"
#define ENVOI_FIN_EXTRACTION "FinExtract"


#define NB_ARG_R1 12
#define NB_ARG_R2 12
#define NB_ARG_R3 16
#define NB_ARG_R1I 12
#define NB_ARG_R2I 12
#define NB_ARG_R3I 12
#define NB_ARG_DEFAULT 12

#define NO_OUTPUTSTREAM -1

#endif

/************************************************************************
 *                                                                      *
 * Objectif : Envoi du message en argument dans le fulx d'ecriture      *
 *                                                                      *
 * Argument : 1. Le message                                             *
 *                                                                      *
 *            2. le flux d'ecriture                                     *
 *                                                                      *
 *            3. Ajout d'une caractere de passage a la ligne (OUI/NON)  *
 *                                                                      *
 * Retour : l'etat d'execution                                          *
 *                                                                      *
 * Date de derniere modification : 30/09/2003                           *
 *                                                                      *
 ************************************************************************/

int print_com_msg(char *msg_prn, int flux_ecrit, int);

int send_ch_res(FILE *FichRes, int flux_ecriture, int rule, ... );

int send_production(int flux_ecriture, int Terme1, char *, int Terme2, char *, int production);
