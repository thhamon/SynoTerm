#!/usr/bin/perl

package Acabit2SynoTerm;

use strict;
use HTML::Entities;

use OccTerme;
use Terme;

my $regex_idtrm = "\[\\w-\\.\]+";

#&Acabit2SynoTerm(@ARGV);

sub Acabit2SynoTerm() {

    my $self = shift;
    my @arguments = @_;

    my $basename_in;
    my $basename_out;
    my %tabhCorpusBal;

    my @tabsortiesACABIT;
    my %tabhCleCorpus;
    my %tabhCorpus;
    my %tabhLstTermes;
    my %tabhLCTermes;
    my %tabhTermesLC;
    my @tabLstOccTermes;
    my $nbGNM;
    my $compteurTrm;
    my %tabhTetes;
    my %tabhExps;
    my %tabhTrad;

    &lectTableTradCat(\%tabhTrad);
    ($basename_in, $basename_out) = &LectureArg(@arguments);
    &Lecture_Corpus_Balise($basename_in . ".tag", \%tabhCorpusBal);
    &GenPhr_ACABIT($basename_out . ".db.phr", \%tabhCorpusBal);
    &AnalyseCorpusBalise($basename_in . ".tag", $basename_out . ".db", \%tabhCorpusBal, \%tabhLstTermes, \%tabhLCTermes, \%tabhTermesLC, \@tabLstOccTermes, \%tabhTrad);
    &Gen_LemmeSynoTerm(\%tabhLstTermes);
    $nbGNM = &Gen_db($basename_out . ".db", \@tabLstOccTermes,\%tabhCorpusBal);
    &Gen_dbexps($basename_out . ".db.exps", \@tabLstOccTermes, \%tabhLstTermes, \%tabhExps);
    &Gen_dbtetes($basename_out . ".db.tetes", \@tabLstOccTermes, \%tabhLstTermes, \%tabhTetes);
    &Gen_dbent($basename_out . ".db.ent", $nbGNM, \%tabhLstTermes, \@tabLstOccTermes, \%tabhTetes, \%tabhExps);
#    $nbGNM = &LectureCand_Acabit($basename_in . ".lem", \%tabhLstTermes);
#    print join "\n", @tabsortiesACABIT;

}

sub LectureArg () {
#
# Lecture de l'ensemble des arguments
#
    my @arguments = @_;

    if ($#arguments != 0) {
	 print STDERR "***** Argument manquant\n";
	 print STDERR "Syntaxe : $0 Base_des_fichiers_Acabit\n";
	 exit(-1);
    }

    return ($arguments[0], $arguments[0]);
}

#	$ligne =~ s/\/export\/home\/LIPN-SF3\/users\/RCLN\/ht\/Projets\/CADERIGE\/FichiersLRI\///g;

sub Lecture_Corpus_Balise () {
    my ($corpus_bal, $ref_tabhCorpusBal) = @_;
    my $ligne;
    my $cle = "";

    open CORPUSBAL, $corpus_bal;
    do {
	 $ligne = "";
	 while (($ligne = <CORPUSBAL>) && ($ligne !~ /^<FIC NUM/)) {
#	    chomp $ligne;
	     if ($ligne ne "\n") {
		 $ref_tabhCorpusBal->{$cle} = $ref_tabhCorpusBal->{$cle} . $ligne;
		 # print STDERR "$cle->$ligne\n";
	     }
	 }
	 if ($cle ne "") {
	     $ref_tabhCorpusBal->{$cle} =~ s/\n+/ /g;
	 }
	 if ($ligne =~ /^<FIC NUM/) {
	     chomp $ligne;
	     $ligne =~ /\"([^\"\/]+)\"\>$/; #  "
	     $cle = $1; 
	     #$ref_tabhCleCorpus{$cle} = $ligne; 
	     $ref_tabhCorpusBal->{$cle} = "";
	 }
    }while($ligne ne "");

}
sub GenPhr_ACABIT() {
    my ($nomfichier, $ref_tabhCorpusBal) = @_;
    my $cle;
    my $ligne;
    my $ligne_aff;

    open FICHIERPHR, ">$nomfichier";

    foreach $cle (sort keys %$ref_tabhCorpusBal) {
	 print FICHIERPHR "$cle|";	
	 $ligne = $ref_tabhCorpusBal->{$cle};
	 $ligne =~ s/<[^>]+>//g;

	 while ($ligne =~ /([^\/]+)\/[^\/]+\/([^\/]+)\/[^ _]+([ _])/gc) {
	     $ligne_aff = "$1$3";
	     print FICHIERPHR HTML::Entities::encode("$ligne_aff", "\200-\377");
#		 print FICHIERPHR "$1$3";
	 }

#	chomp $ref_tabhCorpus->{$cle};
#	print $ref_tabhCorpus->{$cle};
	 print FICHIERPHR "\n";
    }
    close FICHIERPHR
}

sub AnalyseCorpusBalise() {
    my ($corpus_bal, $nomfichier, $ref_tabhCorpusBal, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $ref_tabhTrad) = @_;
    my $ligne;
    my $id1;
    my $id2;
    my $parttrm;
    my $parttrm2;
    my $cle;
    my $elt1;
    my $elt2;
    my $elt3;
    my $elt4;
    my $terme;
    my $termine;
    my $Terme;
    my $idtrm = 0;
    my $idtrm_gnm;
    my $compteurGNM;
    my $compteurTrm;
    my $lemme;
    my $LemmeS;
    my $LemmeCatS;
    my $Cat ;
    my $CatS;
    my $formeS;
    my $lemmeCat;
    my $formeStd;
    my $OccTerme;
    my @tabTerme;
    my $i;
    my $idgnm;
    my @tabTermeCorpus;

    foreach $cle (sort keys %$ref_tabhCorpusBal) {
	$ligne = $ref_tabhCorpusBal->{$cle};
#	$ligne2 = $ref_tabhCorpusBal->{$cle};

#  	 do {
#  	     # Suppression du balisage des candidats termes inclus dans des candidats termes
#  	     #  	while($ligne =~ /(<TERME idtrm=([0-9]+) nbocc=[0-9]+>)(.*?)(<\/TERME idtrm=\2>)/gc) {
#  	     #  	    print "$1\n$2\n$3\n$4\n";
#  	     #  	}
#  	     # On remet de le compteur au debut de la ligne
#  	     # $ligne =~ s/^(.)/\1/;
#  	     while($ligne =~ /(<TERME idtrm=($regex_idtrm) nbocc=[0-9]+>)(.*?)(<\/TERME idtrm=\2>)/gc) {
#  		 $parttrm = $3;
#  		 $elt1 = $1;
#  		 $elt3 = $3;
#  		 $elt4 = $4;
#  		 if ($parttrm =~ s/(<TERME idtrm=($regex_idtrm) nbocc=[0-9]+>)(.*?)(<\/TERME idtrm=\2>)/\3/gc) {
#  		     $ligne =~ s/$elt1$elt3$elt4/$elt1$parttrm$elt4/;
#  		 }
#  	     }

#  	     # Cas particulier de termes ayant les memes identifiants (variantes morpho-syntaxiques)

#  	     # On remet de le compteur au debut de la ligne

#  	     # $ligne =~ s/^(.)/\1/;
#  	     while($ligne =~ s/(<TERME idtrm=($regex_idtrm) nbocc=([0-9]+)>(.*?)<TERME idtrm=\2 nbocc=$regex_idtrm>(.*?)<\/TERME idtrm=\2>(.*?)<\/TERME idtrm=\2>)/<TERME idtrm=\2 nbocc=\3>\4\5\6<\/TERME idtrm=\2>/gc){};

#  	     # On remet de le compteur au debut de la ligne
#  	     # $ligne =~ s/^(.)/\1/;

#  	     # Regroupement des candidats termes enchasses dans des candidats termes
#  	     while($ligne =~ /<TERME idtrm=($regex_idtrm) nbocc=([0-9]+)>([^<]+)<\/TERME idtrm=($regex_idtrm)>/gc) {
#  		 $parttrm = $3;
#  		 $id1 = $1;
#  		 $id2 = $4;

#  		 $parttrm =~ s/<TERME idtrm=($id2) nbocc=([0-9]+)>//;
#  		 $parttrm =~ s/<\/TERME idtrm=($id1)>//;
#  		 if ($id1 != $id2) {
#  		     $ligne =~ s/<TERME idtrm=$id1 nbocc=([0-9]+)>$parttrm<\/TERME idtrm=$id2>/$parttrm/;
#  		     $ligne =~ s/<\/TERME idtrm=$id1>/<\/TERME idtrm=$id2>/;
#  		 }
#  	     }

#  	     # On remet de le compteur au debut de la ligne
#  	     # $ligne =~ s/^(.)/\1/;

#  	     $termine = 1;
#  	     # Stockage des groupes nominaux maximaux
#  	     while(($termine == 1) && ($ligne =~ /(<TERME idtrm=($regex_idtrm) nbocc=[0-9]+>)(.*?)(<\/TERME idtrm=\2>)/gc)) {
#  		 $terme = $3;
#  		 if ($terme =~ /</) {
#  		     $termine = 0;
#  		 }
#  	     }
#  	 } while($termine == 0);
#  	 # On remet de le compteur au debut de la ligne
#  	 $ligne =~ s/^(.)/\1/;

	 # Stockage des groupes nominaux maximaux

	@tabTermeCorpus = &AnalyseLigne($ligne);
	
#  	print join ":", @tabTermeCorpus;

	foreach $terme (@tabTermeCorpus) {
#	    print "Out: $terme : $cle\n";

#	print "\n";
	
#  	next;
#  	while($ligne =~ /(<TERME idtrm=($regex_idtrm) nbocc=[0-9]+>)(.*?)(<\/TERME idtrm=\2>)/gc) {
#  	     $terme = $3;
	     $lemmeCat = "";
	     $formeStd = "";
	     $lemme = "";
	     @tabTerme = ();
	     while ($terme =~ /([^\/]+)\/([^\/]+)\/([^\/]+)\/([^ ]+)([ _])*/gc) {
		 $LemmeS = $3;
		 $Cat  = &TradCat($2, $ref_tabhTrad);
		 $CatS = substr($Cat,0,3);
		 $lemme = $lemme . "$3$5";
		 $formeStd = $formeStd . "$1$5";
		 $lemmeCat = $lemmeCat . "$3__$CatS=";
		 $LemmeCatS = "$3__$CatS";
		 $formeS = $1;
#		 print "$formeS : $LemmeCatS\n";
		 if (!exists $ref_tabhLCTermes->{$LemmeCatS}) {
		     $Terme = Terme->nouvTrm($idtrm);
		     $ref_tabhLstTermes->{$idtrm} = $Terme;
		     $ref_tabhLstTermes->{$idtrm}->setLemme($LemmeS);
		     $ref_tabhLstTermes->{$idtrm}->setCat($Cat);
		     $ref_tabhLstTermes->{$idtrm}->setFFStd($formeS);
		     $ref_tabhTermesLC->{$idtrm} = $LemmeCatS;
		     $idtrm++;
		     push @{$Terme->{"LemmeCat"}}, $LemmeCatS;
		     $ref_tabhLCTermes->{$LemmeCatS} = $Terme;
		 } else {
		     $Terme = $ref_tabhLCTermes->{$LemmeCatS};
		 }
		 # Creation de l'objet occurrence
		 $OccTerme = OccTerme->nouvOccTrm($compteurTrm, $Terme);
		 $Terme->addOcc($OccTerme);
		 $Terme->IncrNbOcc();
		 push @$ref_tabLstOccTermes, $OccTerme;

		 $OccTerme->setRefPhr("$cle");
		 $OccTerme->setTypeOcc("O");
		 $OccTerme->setFF($formeS);

		 $compteurTrm++;
		 push @tabTerme, $Terme;
	     }
	     chop $lemmeCat;
	     # Creation de l'objet Terme s'il n'a pas ete deja rencontre
	     if (!exists $ref_tabhLCTermes->{$lemmeCat}) {
		 $Terme = Terme->nouvTrm($idtrm);
		 $ref_tabhLstTermes->{$idtrm} = $Terme;
		 $ref_tabhLstTermes->{$idtrm}->setLemme($lemme);
		 $ref_tabhLstTermes->{$idtrm}->setCat("SN");
		 $ref_tabhLstTermes->{$idtrm}->setFFStd($formeStd);
		 push @{$Terme->{"LemmeCat"}}, $lemmeCat;
		 $ref_tabhTermesLC->{$idtrm} = $lemmeCat;
		 $idgnm = $idtrm;
		 $idtrm++;
		 $compteurGNM++;
		 $ref_tabhLCTermes->{$lemmeCat} = $Terme;
	     } else {
		 $Terme = $ref_tabhLCTermes->{$lemmeCat};
		 $idgnm = $Terme->{"IdTrm"};
	     }

	     # Creation de l'objet occurrence
	     $OccTerme = OccTerme->nouvOccTrm($compteurTrm, $Terme);
	     $Terme->addOcc($OccTerme);
	     $Terme->IncrNbOcc();


	     $OccTerme->setRefPhr("$cle");
	     $OccTerme->setTypeOcc("ISOL");
	     $OccTerme->setFF($formeStd);

	     $compteurTrm++;
	     ($idtrm_gnm, $idtrm) = CalculDep(\@tabTerme,$ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm, 0, $#tabTerme, $compteurTrm, $idgnm, $cle, $ref_tabhTrad);
	     push @$ref_tabLstOccTermes, $OccTerme;
  	}


    }

}

sub CalculDep () {

    my ($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad) = @_;
    my $k;
    my $idtete = -1;
    my $idexp = -1;
    my $lemmeCat;
    my $lemme;
    my $formeStd;
    my $Terme;
    my $idtrm;
    my $OccTerme;
    my $rel;
    my $idprep;

#    print STDERR "+++> $idgnm\n";
#       print STDERR "$i ::: $j\n";
    if ($i < $j) {
	# Creation des relations de dependance
	for($k=$i;$k<=$j;$k++) {
#	    print "$k : " . $ref_tabTerme->[$k]->getCat() . "\n";
	    if ($ref_tabTerme->[$k]->getCat() eq &TradCat("IN", $ref_tabhTrad)) {
		# Type NN IN NN (N of N)
		# Calcul pour la tete
		($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i, $k - 1, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#		print STDERR "idtete = $idtete\n";
		# Calcul pour l'expansion
		($idexp, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k + 1, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#		print STDERR "idexp = $idexp\n";
		$idprep = $ref_tabTerme->[$k]->getIdTrm();
		($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "+", $idgnm, $idtete, $idexp, $idprep);
		return ($idtrm, $idtrm_max);
	    }
	    if (($ref_tabTerme->[$k]->getCat() eq &TradCat("CC", $ref_tabhTrad)) || ($ref_tabTerme->[$k]->getCat() eq &TradCat(",", $ref_tabhTrad))) {
		if (($ref_tabTerme->[$k-1]->getCat() =~ /^Nom/) && ($ref_tabTerme->[$k+1]->getCat() =~ /^Nom/)) {
		    # cas : NN CC NN
		    # Calcul pour la tete
		    ($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i, $k - 1, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#		    print STDERR "idtete = $idtete\n";
		    # Calcul pour l'expansion
		    ($idexp, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k + 1, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#		    print STDERR "idexp = $idexp\n";
		    $idprep = $ref_tabTerme->[$k]->getIdTrm();
		    ($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "+", $idgnm, $idtete, $idexp, $idprep);
			return ($idtrm, $idtrm_max);
		    
		} else {
#		    if (($ref_tabTerme->[$k-1]->getCat() =~ /^JJ/) && ($ref_tabTerme->[$k+1]->getCat() =~ /^JJ/)) {
#			print STDERR "cas : JJ CC JJ\n";
			# cas : JJ CC JJ
			# Calcul pour la tete
			($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k+1, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#			print STDERR "idtete = $idtete\n";
			# Calcul pour l'expansion
			($idexp, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i, $k - 1, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#			print STDERR "idexp = $idexp\n";
			$idprep = $ref_tabTerme->[$k]->getIdTrm();
#			print STDERR "$idgnm\n";
			
			# print STDERR "\n";
			($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "-", $idgnm, $idtete, $idexp, $idprep);
			return ($idtrm, $idtrm_max);
			
# 		    } else {
# 			# Autres cas
# 		    }
		    
		}
	    }
	    if ($ref_tabTerme->[$k]->getCat() =~ /^V/) {
		
	    }
# 	    print STDERR "$k " . $ref_tabTerme->[$k]->getCat() . "\n";
	    if ($ref_tabTerme->[$k]->getCat() =~ /Adv/) {
		# Type RB JJ [**]
		# Calcul pour la tete
		($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k+1, $k+1, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#   		print STDERR "idtete = $idtete\n";
		# Calcul pour l'expansion
		($idexp, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k, $k, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#   	        print STDERR "idexp = $idexp\n";
		$idprep = -1;
		($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "-", $idgnm, $idtete, $idexp, $idprep);
# 		print STDERR "-> $idtrm\n";
		$idexp = $idtrm;
		($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $k+2, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
# 		print STDERR "++>idtete = $idtete\n";
		$idprep = -1;
		if ($idtete != -1) {
		    ($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "-", $idgnm, $idtete, $idexp, $idprep);
		}
		return ($idtrm, $idtrm_max);
	    }
	}
#   	print STDERR "$i :: $k :: $j\n";

	if (($i < $j) && ($k == $j + 1)) {
	    # Type NN NN
	    # Calcul pour la tete
	    ($idtete, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i+1, $j, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#    	    print STDERR "idtete = $idtete\n";
	    # Calcul pour l'expansion
	    ($idexp, $idtrm_max) = CalculDep($ref_tabTerme, $ref_tabhLstTermes, $ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabLstOccTermes, $idtrm_max, $i, $i, $compteurTrm, $idgnm, $refphr, $ref_tabhTrad);
#      	    print STDERR "idexp = $idexp\n";
#  	    print STDERR $ref_tabhLstTermes->{$idexp}->{"IdTrm"};
#  	    print STDERR "\n";
		# Calcul du lemme du nouveau terme

	    $idprep = -1;
	    ($idtrm, $idtrm_max) = InitialiseDependance($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, "-", $idgnm, $idtete, $idexp, $idprep);
	    return ($idtrm, $idtrm_max);
	}
    } else {	
#    	print STDERR $ref_tabTerme->[$i]->getLemme();
#    	print STDERR " ";
#    	print STDERR $ref_tabTerme->[$i]->getIdTrm();
#    	print STDERR "\n";
	if (exists $ref_tabTerme->[$i]) {
	    $idtete = $ref_tabTerme->[$i]->getIdTrm();
	}
# 	print STDERR "-->$idtete\n";
	return ($idtete, $idtrm_max);
    }
}

sub Gen_db() {
############################################################

# Génération du fichier : .db (Liste des groupes nominaux maximaux)
    my ($nomfichier, $ref_tabLstOccTermes, $ref_tabhCorpusBal) = @_;
    my $compteurTrm;
    my $compteurGNM;
    my %tabhTermeNumOcc;
    my %tabhTermeNumOcc_Phr;
    my $OccTerme;
    my @tabLemmes;
    my $lemme;


#     print STDERR "Génération du fichier : ";
#     print STDERR $nomfichier;
#     print STDERR " (Liste des groupes nominaux maximaux)\n";

    
    open FICHIERDB, ">$nomfichier";

    $compteurTrm = 0;
    $compteurGNM = 0;
    %tabhTermeNumOcc = ();
    %tabhTermeNumOcc_Phr = ();

    foreach $OccTerme (@$ref_tabLstOccTermes) {
	$tabhTermeNumOcc{$OccTerme->{"Terme"}->{"IdTrm"}}++;
	$tabhTermeNumOcc_Phr{$OccTerme->{"RefTrmPhr"}}->{$OccTerme->{"Terme"}->{"IdTrm"}}++;
	if (($OccTerme->isGNM_Phr() == 1)&&($OccTerme->{"Terme"}->isGNC())) {
	    @tabLemmes = $OccTerme->getLemmeFrmSynoTermIn(\%tabhTermeNumOcc);
	    $lemme = $tabLemmes[$tabhTermeNumOcc_Phr{$OccTerme->{"RefTrmPhr"}}->{$OccTerme->{"Terme"}->{"IdTrm"}} - 1];
		print FICHIERDB "$compteurGNM:";
		print FICHIERDB HTML::Entities::encode("$lemme:", "\200-\377");
		print FICHIERDB $OccTerme->getRefPhr();
		print FICHIERDB "\n";
		$compteurGNM++;
	} else {
	}

	$compteurTrm++;
    }
    close FICHIERDB;
    return $compteurGNM;
}

sub InitialiseDependance () {

    my ($ref_tabhLCTermes, $ref_tabhTermesLC, $ref_tabhLstTermes, $ref_tabLstOccTermes, $idtrm_max, $refphr, $ordre, $idgnm, $idtete, $idexp, $idprep) = @_;
    my $Terme;
    my $OccTerme;
    my $lemmeCat;
    my $lemme;
    my $formeStd;
    my $compteurTrm;
    my $idtrm;

    if ($ordre eq "-") {
	if ($idprep != -1) {
	    $lemmeCat = $ref_tabhTermesLC->{$idexp} . "=" . $ref_tabhTermesLC->{$idprep} . "=" . $ref_tabhTermesLC->{$idtete};
	    $lemme = $ref_tabhLstTermes->{$idexp}->getLemme() . " " . $ref_tabhLstTermes->{$idprep}->getLemme() . " " . $ref_tabhLstTermes->{$idtete}->getLemme();
	    $formeStd = $ref_tabhLstTermes->{$idexp}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idprep}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idtete}->getFormeLemmatiseeStd();
	} else {
	    $lemmeCat = $ref_tabhTermesLC->{$idexp} . "=" . $ref_tabhTermesLC->{$idtete};
	    $lemme = $ref_tabhLstTermes->{$idexp}->getLemme() . " " . $ref_tabhLstTermes->{$idtete}->getLemme();
	    $formeStd = $ref_tabhLstTermes->{$idexp}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idtete}->getFormeLemmatiseeStd();	    
	}
    }
    if ($ordre eq "+") {
	if ($idprep != -1) {
	    $lemmeCat = $ref_tabhTermesLC->{$idtete} . "=" . $ref_tabhTermesLC->{$idprep} . "=" . $ref_tabhTermesLC->{$idexp};
	    $lemme = $ref_tabhLstTermes->{$idtete}->getLemme() . " " . $ref_tabhLstTermes->{$idprep}->getLemme() . " " . $ref_tabhLstTermes->{$idexp}->getLemme();
	    $formeStd = $ref_tabhLstTermes->{$idtete}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idprep}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idexp}->getFormeLemmatiseeStd();
	} else {
	    $lemmeCat = $ref_tabhTermesLC->{$idtete} . "=" . $ref_tabhTermesLC->{$idexp};
	    $lemme = $ref_tabhLstTermes->{$idtete}->getLemme() . " " . $ref_tabhLstTermes->{$idexp}->getLemme();
	    $formeStd = $ref_tabhLstTermes->{$idtete}->getFormeLemmatiseeStd() . " " . $ref_tabhLstTermes->{$idexp}->getFormeLemmatiseeStd();	    

	}
    }

    if (!exists($ref_tabhLCTermes->{$lemmeCat})) {
	$Terme = Terme->nouvTrm($idtrm_max);
	$ref_tabhLstTermes->{$idtrm_max} = $Terme;
	$ref_tabhLstTermes->{$idtrm_max}->setLemme($lemme);
	$ref_tabhLstTermes->{$idtrm_max}->setCat("SNom");
	$ref_tabhLstTermes->{$idtrm_max}->setFFStd($formeStd);
	push @{$Terme->{"LemmeCat"}}, $lemmeCat;
	# $ref_tabhLstTermes->{$idtrm_max}->setNbOcc($freq);
	$ref_tabhLCTermes->{$lemmeCat} = $Terme;
	$ref_tabhTermesLC->{$idtrm_max} = $lemmeCat;
	$idtrm = $idtrm_max;
	$idtrm_max++;
    } else {
	$Terme = $ref_tabhLCTermes->{$lemmeCat};
	$idtrm = $Terme->getIdTrm();
    }

    if ($idtrm != $idgnm) {
	# Creation de l'objet occurrence
	$OccTerme = OccTerme->nouvOccTrm($compteurTrm, $Terme);
	$Terme->addOcc($OccTerme);
		
	push @$ref_tabLstOccTermes, $OccTerme;
		
	$OccTerme->setRefPhr($refphr);
	$OccTerme->setTypeOcc("O");
	$OccTerme->setFF($formeStd);
	
	$compteurTrm++;
    }

    # Initialisation des relations de dependance
    $ref_tabhLstTermes->{$idtrm}->setIdTete($ref_tabhLstTermes->{$idtete});
    $ref_tabhLstTermes->{$idtrm}->setIdExp($ref_tabhLstTermes->{$idexp});
    if ($idprep != -1) {
	$ref_tabhLstTermes->{$idtrm}->setRelateur($ref_tabhLstTermes->{$idprep}->getCat() . "|" . $ref_tabhLstTermes->{$idprep}->getLemme());
    } else { 
	$ref_tabhLstTermes->{$idtrm}->setRelateur("PREC");
    }
    $ref_tabhLstTermes->{$idtete}->setIdGNC($ref_tabhLstTermes->{$idtrm});
    $ref_tabhLstTermes->{$idexp}->setIdGNC($ref_tabhLstTermes->{$idtrm});
#    	    print STDERR "++++> $idtrm ;; $idtrm_max\n";

#    print STDERR "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    return ($idtrm, $idtrm_max);
}

sub printtabH() {
    my ($ref_tabhLCTermes) = @_;
    my $cle;

    print STDERR "------------------------------------------------------------------------\n";
    foreach $cle (sort {$ref_tabhLCTermes->{$a}->{"IdTrm"} <=> $ref_tabhLCTermes->{$b}->{"IdTrm"}} keys %$ref_tabhLCTermes) {
	print STDERR $ref_tabhLCTermes->{$cle}->{"IdTrm"} . " : $cle\n";
    }
    print STDERR "------------------------------------------------------------------------\n";
}

sub printtab() {
    my ($ref_tab) = @_;
    my $cle;
    my $i;

    print STDERR "------------------------------------------------------------------------\n";
    for($i=0;$i<scalar @$ref_tab;$i++) {
	print STDERR $ref_tab->[$i]->{"Terme"}->{"Lemme"} . "\n";
    }
    print STDERR "------------------------------------------------------------------------\n";
}

sub Gen_LemmeSynoTerm() {
    my ($ref_tabhLstTermes) = @_;
    my $idtrm;

#     print STDERR "Génération des lemmes de type SynoTerm\n";

#      foreach $idtrm (keys %$ref_tabhLstTermes) {
#  	$ref_tabhLstTermes->{$idtrm}->{LemmeCat} = "";
#      }


    foreach $idtrm (keys %$ref_tabhLstTermes) {
#  	print $ref_tabhLstTermes->{$idtrm}->getLemmeCat();
#  	print "\n";
	$ref_tabhLstTermes->{$idtrm}->setLemmeCat("--force");
#  	print $ref_tabhLstTermes->{$idtrm}->getLemmeCat();
#  	print "\n";
    }
}

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
 

#     print STDERR "Génération du fichier : ";
#     print STDERR $nomfichier;
#     print STDERR " (Liste des expansions)\n";

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
    
#     print STDERR "Génération du fichier : ";
#     print STDERR $nomfichier;
#     print STDERR " (Liste des tetes)\n";

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

#     print STDERR "Génération du fichier : ";
#     print STDERR $nomfichier;
#     print STDERR " (Entete du corpus)\n";

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

    close FICHEIRDBENT;   
}

sub lectTableTradCat()  {
    my ($ref_tabhTrad) = @_;
    my $ligne;
    my $catAcB;
    my $catST;

    open TABLE, "Acabit2SynoTerm.coresp";

    while($ligne = <TABLE>) {
	chomp $ligne;
	($catAcB, $catST) = split /\t/, $ligne;
	$ref_tabhTrad->{$catAcB} = $catST;
    }

    close TABLE;
}

sub TradCat() {
    my ($cat, $ref_tabhTrad) = @_;

    if (exists  $ref_tabhTrad->{$cat}) {
	return $ref_tabhTrad->{$cat};
    } else  {
	warn ($cat);
	return ("Nom??");
    }
}


sub AnalyseLigne() {
    my $ligne = $_[0];
    my $terme;
    my $terme_int;
    my $suite;
    my @tabTermeCorpus;

    if ($ARGV[0] eq '-debug') {
	print STDERR "+++++++++++\n";
	print STDERR "$ligne\n";
	print STDERR "-----------\n";
    }
    if ($ligne =~ /^([^<]+)(.*)/) {
#	print "$1 ";
	$ligne = $2;
    }

    # On tombe sur un terme

    if ($ARGV[0] eq '-debug') {
	print STDERR "-->$ligne\n\n";
    }    
    if ($ligne =~ /(^<TERME idtrm=($regex_idtrm) nbocc=([0-9]+)>(.*?)<\/TERME idtrm=\2>)/) {
	if ($ARGV[0] eq '-debug') {
	    print STDERR "1-->$1;\n\n";
	    print STDERR "2-->$2;\n\n";
	    print STDERR "3-->$3;\n\n";
	    print STDERR "4-->$4;\n\n";
	}
	$terme = $1;
	$terme_int = $4;
	$suite = $';

  	if ($terme_int =~ /<[^<]+>/) {
  	    # Commme il peut contenir un balisage de terme, on s'en occupe d'abord
  	    # de l'interieur
  	    &AnalyseLigne($terme_int);
  	}

	$terme =~ s/<[^>]+>//g;

	push @tabTermeCorpus, $terme;
#  	print "In : $terme\n";
 	push @tabTermeCorpus, &AnalyseLigne($suite);	

    }

    return (@tabTermeCorpus);
}

1;
