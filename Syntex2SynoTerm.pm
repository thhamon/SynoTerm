#!/usr/bin/perl

# 	$Log: Syntex2SynoTerm.pm,v $
# 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
# 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
# 	
# 	Revision 1.2  2002/07/09 14:13:31  ht
# 	Correstion de quelques bugs
# 	Mise en module
#	


package Syntex2SynoTerm;

use strict;
require HTML::Entities;

require OccTerme;
require Terme;


# Fichiers a générer

#   menelas.db
#   menelas.db.ent
#   menelas.db.exps
#   menelas.db.phr (OK)
#   menelas.db.tetes



############################################################
#
#            Module de transformation des sorties de 
#               Syntex au format d'entree de SynoTerm
#            Copyright (C) 2002 Thierry Hamon (LIPN)
#            thierry.hamon@lipn.univ-paris13.fr
#                      9 juillet  2002
#
############################################################

#######################################################################
#
# Last Update: 02/07/2002 (mm/dd/yyyy date format)
# 
# Copyright (C) 2002 Thierry Hamon
#
# Written by thierry.hamon@lipn.univ-paris13.fr
#
# Author : Thierry Hamon
# Email : thierry.hamon@lipn.univ-paris13.fr
# URL : http://www-lipn.univ-paris13.fr/~hamon
#
########################################################################

#
# Passage du fichier syntex_seq.txt au format *.db.phr
#

sub Syntex2SynoTerm () {

    my $self = shift;
    my @arguments = @_;

    my $basename_in;
    my $basename_out;
    my @tabphr;
    my %tabhLstTermes;
    my @tabLstOccTermes;
    my $nbGNM;
    my %tabhTetes;
    my %tabhExps;

    print STDERR join "::", @arguments;
    
    ($basename_in, $basename_out) = &LectureArg(@arguments);
    
    &Lecture_Phr($basename_in . "_seq.txt", \@tabphr);
    &Gen_Phr ($basename_out . ".db.phr", \@tabphr);
    &Lecture_Liste ($basename_in . "_liste.txt", \%tabhLstTermes);
    &Lecture_Occ ($basename_in . "_occ.txt", \%tabhLstTermes, \@tabLstOccTermes);
    &Lecture_ad ($basename_in . "_ad.txt", \%tabhLstTermes);
    &Lecture_dep($basename_in . "_dep.txt", \%tabhLstTermes);
    &Gen_LemmeSynoTerm(\%tabhLstTermes);
    $nbGNM = &Gen_db($basename_out . ".db", \@tabLstOccTermes);
    &Gen_dbexps($basename_out . ".db.exps", \@tabLstOccTermes, \%tabhLstTermes, \%tabhExps);
    &Gen_dbtetes($basename_out . ".db.tetes", \@tabLstOccTermes, \%tabhLstTermes, \%tabhTetes);
    &Gen_dbent($basename_out . ".db.ent", $nbGNM, \%tabhLstTermes, \@tabLstOccTermes, \%tabhTetes, \%tabhExps);
}

sub LectureArg () {
#
# Lecture de l'ensemble des arguments
#
    my @arguments = @_;

    if ($#arguments != 1) {
	print STDERR "***** Argument manquant\n";
	print STDERR "Syntaxe : $0 Base_du_fichier_Syntex Base_de_fichier_SynoTerm\n";
	exit(-1);
    }

    return ($arguments[0], $arguments[1]);
}

sub Lecture_Phr() {
# Ouverture du fichier contenant les phrases du corpus

    my ($nomfichier, $ref_tabphr) = @_;
    my $ligne;
    my $idphr;
    my $numphr;
    my $phr;
    
    open FICHIERPHR, $nomfichier or die print STDERR "Fichier inexistant\n";

# Lecture et tri des phrases du corpus

    print STDERR "Lecture et tri des phrases du corpus (fichier $nomfichier)\n";

# Ligne contenant les champs
    $ligne = <FICHIERPHR>;

#  syntex_seq.txt : 
#  	Liste des phrases avec leur identifiant
#  	    idseq : identifiant de la phrase
#  	    numseq : numéro de la phrase dans le corpus
#  	    seq : phrase

    while($ligne = <FICHIERPHR>) {
	chomp $ligne;
	($idphr, $numphr, $phr) = split /\t/, $ligne;
	$ref_tabphr->[$numphr] = "$idphr-$numphr" . "|" . HTML::Entities::encode($phr, "\200-\377");
    }
    
    close FICHIERPHR;
}

sub Gen_Phr () {
# affichage et passage au format SynoTerm
    my ($nomfichier, $ref_tabphr) = @_;
    my $i;
    my @tabphr = @$ref_tabphr;

    open SynoTermPHR, ">$nomfichier";

    for($i = 1; $i<$#$ref_tabphr;$i++) {
	print SynoTermPHR $ref_tabphr->[$i];
	print SynoTermPHR "\n";
    }
    close SynoTermPHR;
}

sub Lecture_Liste () {
############################################################

#  syntex_liste.txt : 


#  num : numero du terme
#  cat : catégorie
#  lemme : lemme
#  forme : forme
#  freq : frequence
#  long : longueur
#  nbexp : nombre d'expansions
#  proT : productivité en tete
#  prodE : productivité en expansion
#  nbasc : nombre d'ascendants
#  val : val
#  nbvoisterm : nombre de voisins AD terme
#  nbvoiscont : nombre de voisins AD contexte
#  nbvar : nombre de termes en variation

    my ($nomfichier, $ref_tabhLstTermes) = @_;
    my $ligne;
    my $idtrm;
    my $cat;
    my $lemme;
    my $forme;
    my $freq;
    my $long;
    my $nbexp;
    my $prodT;
    my $prodE;
    my $nbasc; 
    my $val;
    my $nbvoisterm;
    my $nbvoiscont;
    my $nbvar;
    my $Terme;

# Ouverture du fichier contenant les informations sur les termes

    open FICHIERLISTE, $nomfichier or die print STDERR "Fichier inexistant\n";

# Lecture des occurrences des termes

    print STDERR "Lecture des occurrences des termes (fichier $nomfichier)\n";

    $ligne = <FICHIERLISTE>;
    
    while($ligne = <FICHIERLISTE>) {
	chomp $ligne;
	($idtrm, $cat, $lemme, $forme, $freq, $long, $nbexp, $prodT, $prodE, $nbasc, $val, $nbvoisterm, $nbvoiscont, $nbvar) = split /\t/, $ligne;
	$Terme = Terme->nouvTrm($idtrm);
	$ref_tabhLstTermes->{$idtrm} = $Terme;
	$ref_tabhLstTermes->{$idtrm}->setFFStd($forme);
	$ref_tabhLstTermes->{$idtrm}->setLemme($lemme);
	$ref_tabhLstTermes->{$idtrm}->setCat($cat);
	$ref_tabhLstTermes->{$idtrm}->setNbOcc($freq);
	$ref_tabhLstTermes->{$idtrm}->setLong($long);
    }
    
    close(FICHIERLISTE);
}

sub Lecture_Occ () {
############################################################

#  syntex_occ.txt : 

	
#  iddoc : identifiant du document
#  idseq : numéro de la séquence
#  num : numéro du terme
#  formeocc : forme

    my ($nomfichier, $ref_tabhLstTermes, $ref_tabLstOccTermes) = @_;
    my $ligne;
    my $compteurTrm;
    my $idseq;
    my $numseq;
    my $idtrm;
    my $formeocc;
    my $typeocc;
    my $Terme;
    my $OccTerme;

# Ouverture du fichier contenant les occurrences des termes

    open FICHIEROCC, $nomfichier or die print STDERR "Fichier inexistant\n";

# Lecture des occurrences des termes

    print STDERR "Lecture des occurrences des termes (fichier $nomfichier)\n";

    $ligne = <FICHIEROCC>;

    $compteurTrm = 0;
    while($ligne = <FICHIEROCC>) {
	chomp $ligne;
	($idseq, $numseq, $idtrm, $formeocc, $typeocc) = split /\t/, $ligne;
	if (exists $ref_tabhLstTermes->{$idtrm}) {
#  	$Terme = Terme->nouvTrm($idtrm);
#  	$tabhLstTermes{$Terme->{"IdTrm"}} = $Terme;
	    $Terme = $ref_tabhLstTermes->{$idtrm};
	    $OccTerme = OccTerme->nouvOccTrm($compteurTrm, $Terme);
	    $Terme->addOcc($OccTerme);
	
#    $tabhLstTermes{$OccTerme->{"Terme"}->{"IdTrm"}} = $OccTerme->{"Terme"};
	    push @$ref_tabLstOccTermes, $OccTerme;
	
	    $OccTerme->setRefPhr("$idseq-$numseq");
	    $OccTerme->setTypeOcc($typeocc);
	    $OccTerme->setFF($formeocc);
	
	    $compteurTrm++;
	}
    }
    close FICHIEROCC;
}


sub Lecture_ad () {
############################################################

#  syntex_ad.txt : 

#  	Dépendance syntaxique Tete/Expansion avec relateur

#  prodcontexte : productivité du contexte
#  prodterme : productivité du terme
#  numrecteur : numéro du recteur
#  rel : relation
#  numterme : numéro du terme
#  recteur : recteur
#  terme : terme
#  numsynt : numéro du syntgame
#  freq : fréquence

# Ouverture du fichier contenant les dépendances
    my ($nomfichier, $ref_tabhLstTermes) = @_;
    my $ligne;
    my $prodcontexte;
    my $prodterme;
    my $idtete;
    my $rel;
    my $idexp;
    my $tete;
    my $exp;
    my $idtrm;
    my $freq;

    open FICHIERAD, $nomfichier or die print STDERR "Fichier inexistant\n";

# Lecture des dépendances
    
    print STDERR "Lecture des dépendances (fichier $nomfichier)\n";

    $ligne = <FICHIERAD>;

    while($ligne = <FICHIERAD>) {
	chomp $ligne;

	($prodcontexte, $prodterme, $idtete, $rel, $idexp, $tete, $exp, $idtrm, $freq) = split /\t/, $ligne;
	if ((exists $ref_tabhLstTermes->{$idtrm}) && (exists $ref_tabhLstTermes->{$idtete}) && (exists $ref_tabhLstTermes->{$idexp})) {
	    $ref_tabhLstTermes->{$idtrm}->setIdTete($ref_tabhLstTermes->{$idtete});
	    $ref_tabhLstTermes->{$idtrm}->setIdExp($ref_tabhLstTermes->{$idexp});
	    $ref_tabhLstTermes->{$idtrm}->setRelateur($rel);
	    $ref_tabhLstTermes->{$idtete}->setIdGNC($ref_tabhLstTermes->{$idtrm});
	    $ref_tabhLstTermes->{$idexp}->setIdGNC($ref_tabhLstTermes->{$idtrm});
	}
    }
    
    close FICHIERDEP;
}

sub Lecture_dep() {
############################################################

#  syntex_dep.txt : 

#  	Dépendance syntaxique Tete/Expansion

# num1 : numero du terme 1
# num2 : numero du terme 2
# rel : relation entre terme 1 et terme 2

    my ($nomfichier, $ref_tabhLstTermes) = @_;
    my $ligne;
    my $idtrm;
    my $idcomp;
    my $typerel;
    my $rang;

# Ouverture du fichier contenant les dépendances

    open FICHIERDEP, $nomfichier or die print STDERR "Fichier inexistant\n";

# Lecture des dépendances

    print STDERR "Lecture des dépendances (fichier $nomfichier)\n";

    $ligne = <FICHIERDEP>;
    
    while($ligne = <FICHIERDEP>) {
	chomp $ligne;
	($idtrm, $idcomp, $typerel) = split /\t/, $ligne;
	
	if ((exists $ref_tabhLstTermes->{$idtrm}) && (exists $ref_tabhLstTermes->{$idcomp})) {
	    if ($typerel eq "Ts") {
		if (($rang = $ref_tabhLstTermes->{$idtrm}->RechercheTete($idcomp))!=0) {
		    $rang--;
		    $ref_tabhLstTermes->{$idcomp}->delIdGNC($ref_tabhLstTermes->{$rang});
		    $ref_tabhLstTermes->{$ref_tabhLstTermes->{$idtrm}->getIdExp()->[$rang]->{"IdTrm"}}->delIdGNC($ref_tabhLstTermes->{$rang});
		    $ref_tabhLstTermes->{$idtrm}->delDep($rang);
		}
	    }
	    if ($typerel eq "Es") {
		if (($rang = $ref_tabhLstTermes->{$idtrm}->RechercheExp($idcomp))!=0) {
		    $rang--;
		    $ref_tabhLstTermes->{$idcomp}->delIdGNC($rang);
		    $ref_tabhLstTermes->{$ref_tabhLstTermes->{$idtrm}->getIdTete()->[$rang]->{"IdTrm"}}->delIdGNC($rang);
		    $ref_tabhLstTermes->{$idtrm}->delDep($rang);
		}
	    }
#  	if (($typerel eq "T") && (!($ref_tabhLstTermes{$idtrm}->existTete($idcomp)))) {
#  	    $ref_tabhLstTermes{$idtrm}->setIdTete($ref_tabhLstTermes{$idcomp});
#  	    $ref_tabhLstTermes{$idtrm}->setRelateur("");
#  	}
#  	if (($typerel eq "E") && (!$ref_tabhLstTermes{$idtrm}->existExp($idcomp))){
#  	    $ref_tabhLstTermes{$idtrm}->setIdExp($ref_tabhLstTermes{$idcomp});
#  	    $ref_tabhLstTermes{$idtrm}->setRelateur("");
#  	}
	}
    }
    close FICHIERDEP;
}

sub Gen_LemmeSynoTerm() {
    my ($ref_tabhLstTermes) = @_;
    my $idtrm;

    print STDERR "Génération des lemmes de type SynoTerm\n";

    foreach $idtrm (keys %$ref_tabhLstTermes) {
	$ref_tabhLstTermes->{$idtrm}->setLemmeCat();
    }
}

sub Gen_db() {
############################################################

# Génération du fichier : .db (Liste des groupes nominaux maximaux)
    my ($nomfichier, $ref_tabLstOccTermes) = @_;
    my $compteurTrm;
    my $compteurGNM;
    my %tabhTermeNumOcc;
    my $OccTerme;
    my @tabLemmes;
    my $lemme;

    print STDERR "Génération du fichier : ";
    print STDERR $nomfichier;
    print STDERR " (Liste des groupes nominaux maximaux)\n";


    open FICHIERDB, ">$nomfichier";

    $compteurTrm = 0;
    $compteurGNM = 0;
    %tabhTermeNumOcc = ();
    foreach $OccTerme (@$ref_tabLstOccTermes) {
	$tabhTermeNumOcc{$OccTerme->{"Terme"}->{"IdTrm"}}++;
	if (($OccTerme->isGNM_Phr() == 1)&&($OccTerme->{"Terme"}->isGNC())) {

	    @tabLemmes = $OccTerme->getLemmeFrmSynoTermIn(\%tabhTermeNumOcc);
	    foreach $lemme (@tabLemmes) {
		print FICHIERDB "$compteurGNM:";
		print FICHIERDB HTML::Entities::encode("$lemme:", "\200-\377");
		print FICHIERDB $OccTerme->getRefPhr();
		print FICHIERDB "\n";
		$compteurGNM++;
	    }
	}
	$compteurTrm++;
    }
    close FICHIERDB;
    return $compteurGNM;
}


############################################################

sub Gen_dbexps () {
# Génération du fichier : .db.exps (Liste des expansions)

    my ($nomfichier, $ref_tabLstOccTermes, $ref_tabhLstTermes, $ref_tabhExps) = @_;
    my %tabhTermeNumOcc;
    my $compteurGNM;
    my $OccTerme;
    my $compteurTrm;
    my @tabLemmes;
    my $LemmeCat;
    my $idtrm;
 

    print STDERR "Génération du fichier : ";
    print STDERR $nomfichier;
    print STDERR " (Liste des expansions)\n";

    $compteurGNM = 0;
    %tabhTermeNumOcc = ();
    foreach $OccTerme (@$ref_tabLstOccTermes) {
	$tabhTermeNumOcc{$OccTerme->{"Terme"}->{"IdTrm"}}++;
	if ($OccTerme->isGNM_Phr() == 1) {
	    @tabLemmes = $OccTerme->getExpFrmSynoTermIn(\%tabhTermeNumOcc);
	    foreach $idtrm (@tabLemmes) {
		foreach $LemmeCat ($ref_tabhLstTermes->{$idtrm}->getLemmeCat()) {
		    $ref_tabhExps->{HTML::Entities::encode("$LemmeCat:" . $ref_tabhLstTermes->{$idtrm}->getNbOcc(), "\200-\377")}++;
		}
		$compteurGNM++;
	    }
	}
    }

    open FICHIERDEXPS, ">$nomfichier";
    foreach $LemmeCat (sort keys %$ref_tabhExps) {
	print FICHIERDEXPS "$LemmeCat\n";
    }
    close FICHEIRDBEXPS;
}

############################################################

sub Gen_dbtetes () {

# Génération du fichier : .db.tetes (Liste des tetes)

############################################################

    my ($nomfichier, $ref_tabLstOccTermes, $ref_tabhLstTermes, $ref_tabhTetes) = @_;
    my %tabhTermeNumOcc;
    my $compteurGNM;
    my $OccTerme;
    my $compteurTrm;
    my @tabLemmes;
    my $LemmeCat;
    my $idtrm;
    
    print STDERR "Génération du fichier : ";
    print STDERR $nomfichier;
    print STDERR " (Liste des tetes)\n";

    $compteurGNM = 0;
    %tabhTermeNumOcc = ();
    foreach $OccTerme (@$ref_tabLstOccTermes) {
	$tabhTermeNumOcc{$OccTerme->{"Terme"}->{"IdTrm"}}++;
	if ($OccTerme->isGNM_Phr() == 1) {
	    foreach $LemmeCat ($OccTerme->{"Terme"}->getLemmeCat()) {
		$ref_tabhTetes->{HTML::Entities::encode("$LemmeCat:" . $OccTerme->{"Terme"}->getNbOcc(), "\200-\377")}++;
	    }
	    @tabLemmes = $OccTerme->getTeteFrmSynoTermIn(\%tabhTermeNumOcc);
	    foreach $idtrm (@tabLemmes) {
		foreach $LemmeCat ($ref_tabhLstTermes->{$idtrm}->getLemmeCat()) {
		    $ref_tabhTetes->{HTML::Entities::encode("$LemmeCat:" . $ref_tabhLstTermes->{$idtrm}->getNbOcc(), "\200-\377")}++;
#  		print "$LemmeCat:$idtrm:";
#  		print $tabhLstTermes{$idtrm}->getLemme();
#  		print "\n";
		}
		$compteurGNM++;
	    }
	}
    }

    open FICHIERDTETES, ">$nomfichier";
    foreach $LemmeCat (sort keys %$ref_tabhTetes) {
	print FICHIERDTETES "$LemmeCat\n";
    }
    
    close FICHEIRDBTETES;
}

############################################################

sub Gen_dbent () {

    my ($nomfichier, $nbGNM, $ref_tabhLstTermes, $ref_tabLstOccTermes, $ref_tabhTetes, $ref_tabhExps) = @_;

# Génération du fichier : .db.ent (Entete du corpus)

    print STDERR "Génération du fichier : ";
    print STDERR $nomfichier;
    print STDERR " (Entete du corpus)\n";

    open FICHIERDBENT, ">$nomfichier";

    print FICHIERDBENT "Type = corpus\n";
    print FICHIERDBENT "Typographie = 0\n";
    print FICHIERDBENT "Champs = IDCTMAX;<Tete|Coord|Exp>;IdPhrase\n";
    print FICHIERDBENT "Nombre de GNM = $nbGNM\n";
    print FICHIERDBENT "Nombre de Candidats Termes = ";
    print FICHIERDBENT scalar keys %$ref_tabhLstTermes;
    print FICHIERDBENT "\n";
    print FICHIERDBENT "Nombre de Candidats Termes differents = ";
    print FICHIERDBENT scalar @$ref_tabLstOccTermes;
    print FICHIERDBENT "\n";
    print FICHIERDBENT "Nombre de tetes = ";
    print FICHIERDBENT scalar keys %$ref_tabhTetes;
    print FICHIERDBENT "\n";
    print FICHIERDBENT "Nombre d'expansions = ";
    print FICHIERDBENT scalar keys %$ref_tabhExps;
    print FICHIERDBENT "\n";

    close FICHIERDBENT;   
}

1;

