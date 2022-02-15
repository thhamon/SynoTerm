/* 	$Log: stringSGML.2.c,v $
/* 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
/* 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
/* 	
 * 	Revision 1.2  2001/05/01 19:41:50  thierry
 * 	*** empty log message ***
 *	 */
#include <stdio.h>

int strcmp_sgml(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    writelogS("dans strcmp_sgml");
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if (ch1[i]==ch2[j]) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	        if (ch1[++i]==ch2[j]){
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
		    iold = i;
	    	    if (ch2[++j]==ch1[i]) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    return ch1[i]-ch2[j];
}

int strncmp_sgml(char *ch1, char *ch2, int n) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    writelogS("dans strncmp_sgml");
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;

    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if (ch1[i]==ch2[j]) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	        if (ch1[++i]==ch2[j]){
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
		    iold = i;
	    	    if (ch2[++j]==ch1[i]) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}


int strcmpi_sgml(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&&(ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	    	++i;
	        if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
		    iold=i;
	    	    ++j;
		    if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    return ch1[i]-ch2[j];
}

int strncmpi_sgml(char *ch1, char *ch2,int n) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    
    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&&(ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	    	++i;
	        if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
		    iold=i;
	    	    ++j;
		    if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}

int strcmp_sgml_carndef(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.')) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	        if (ch1[++i]==ch2[j]){
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
/*  		else return ch1[i]-ch2[j]; */
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
		    iold = i;
	    	    if (ch2[++j]==ch1[i]) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
/*  		    else return ch1[i]-ch2[j]; */
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    return ch1[i]-ch2[j];
}

int strncmp_sgml_carndef(char *ch1, char *ch2, int n) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;

    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.')) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        jold=j;
	        if (ch1[++i]==ch2[j]){
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[jold]-ch2[jold];
	    }
	    else {
	    	if (ch2[j] == '&'){
                    iold=i;
	    	    if (ch2[++j]==ch1[i]) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[iold]-ch2[iold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}


int strcmpi_sgml_carndef(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.') || ((ch2[j]>='a') && (ch2[j]<='z') && (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a') && (ch1[i]<='z') && (ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        iold=i;
	    	++i;
	        if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[iold]-ch2[iold];
	    }
	    else {
	    	if (ch2[j] == '&'){
	            jold=j;
	    	    ++j;
		    if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[jold]-ch2[jold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    return ch1[i]-ch2[j];
}

int strncmpi_sgml_carndef(char *ch1, char *ch2,int n) 
{
    int len1,len2,i,j,l,k,iold = 0,jold = 0;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    
    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.') || ((ch2[j]>='a') && (ch2[j]<='z') &&(ch1[i]==ch2[j]-32))|| ((ch1[i]>='a') && (ch1[i]<='z') && (ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    if (ch1[i] == '&'){
	        iold=i;
	    	++i;
	        if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    while((i<len1)&&(ch1[i]!=';')) i++;
		    i++;
		    j++;
		}
		else return ch1[iold]-ch2[iold];
	    }
	    else {
	    	if (ch2[j] == '&'){
	            jold=j;
	    	    ++j;
		    if ((ch1[i]==ch2[j])||((ch2[j]>='a')&&(ch2[j]<='z')&& (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a')&&(ch1[i]<='z')&&(ch1[i]-32==ch2[j]))) {
		    	while((j<len2)&&(ch2[j]!=';')) j++;
			i++;
			j++;
		    }
		    else return ch1[jold]-ch2[jold];
	    	}
	    	else return ch1[i]-ch2[j];
	    }
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}

int strcmp_carndef(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.')) {
	    i++;
	    j++;
	}
	else {
	    return ch1[i]-ch2[j];
	}
    }
    return ch1[i]-ch2[j];
}

int strncmp_carndef(char *ch1, char *ch2, int n) 
{
    int len1,len2,i,j,l,k;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;

    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.')) {
	    i++;
	    j++;
	}
	else {
	    return ch1[i]-ch2[j];
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}


int strcmpi_carndef(char *ch1, char *ch2) 
{
    int len1,len2,i,j,l,k;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    while((i<len1)&&(j<len2)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.') || ((ch2[j]>='a') && (ch2[j]<='z') && (ch1[i]==ch2[j]-32))|| ((ch1[i]>='a') && (ch1[i]<='z') && (ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    return ch1[i]-ch2[j];
	}
    }
    return ch1[i]-ch2[j];
}

int strncmpi_carndef(char *ch1, char *ch2,int n) 
{
    int len1,len2,i,j,l,k;
    
    len1=strlen(ch1);
    len2=strlen(ch2);
    i=0;
    j=0;
    
    while((i<len1)&&(i<n)&&(j<len2)&&(j<n)) {
    	if ((ch1[i]==ch2[j])||(ch1[i] == '.')||(ch2[j] == '.') || ((ch2[j]>='a') && (ch2[j]<='z') &&(ch1[i]==ch2[j]-32))|| ((ch1[i]>='a') && (ch1[i]<='z') && (ch1[i]-32==ch2[j]))) {
	    i++;
	    j++;
	}
	else {
	    return ch1[i]-ch2[j];
	}
    }
    if ((i==n)||(j==n)) return 0;
    else return ch1[i]-ch2[j];
}
