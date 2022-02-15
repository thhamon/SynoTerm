# 	$Log: OccTerme.pm,v $
# 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
# 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
# 	
# 	Revision 1.4  2002/07/08 17:10:48  ht
# 	*** empty log message ***
#
# 	Revision 1.3  2002/07/02 23:22:55  thierry
# 	*** empty log message ***
#
# 	Revision 1.2  2002/07/02 13:54:50  ht
# 	Première version
#	

#######################################################################
#
# Last Update: 2/07/2002 (mm/dd/yyyy date format)
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

package OccTerme;

use OccTerme;

sub nouvOccTrm {
    # Création d'un objet OccTerme
    # arg1: La classe
    # arg2: L'identifiant de l'occurrence du terme
    # arg2: L'identifiant du terme
    # arg3: Le lemme
    # arg4: La catégorie
    # arg5: Le lemme 'a la SynoTerm' (Lemme__Cat)
    # arg6: La forme standard

    my $classe = shift;

    my $self = {
	"IdOccTrm" => shift,
	"RefTrmPhr" => "",
	"RefTrmGNM" => "",
	"FF" => "",
	"TypeOcc" => "",
	"Terme" => shift
	    
    };
    bless $self, $classe;

    return $self;
}

sub setRefPhr {
    # Ajout de réference a une phrase dans le 
    # tableau correspondant
    # arg 1 : l'objet correspondant au terme
    # arg suivants: La  référence a ajouter

    my $self = shift;

    $self->{"RefTrmPhr"} = shift;
}

sub getRefPhr {
    # Ajout de réference a une phrase dans le 
    # tableau correspondant
    # arg 1 : l'objet correspondant au terme
    # arg suivants: La  référence a ajouter

    my $self = shift;

    return $self->{"RefTrmPhr"};
}

sub setRefGNM {
    # Ajout de réferences aux groupes nomianux maximaux
    # dans le tableau correspondant
    # arg 1 : l'objet correspondant au terme
    # arg suivants: La liste des références a ajouter

    my $self = shift;
    $self->{"RefTrmGNM"} = shift;
}

sub setFF {
    # Ajout de la forme fléchie du terme
    # arg 1 : l'objet correspondant au terme
    # arg suivants: La  forme fléchie a ajouter

    my $self = shift;
    $self->{"FF"} = shift;
}

sub getFF {
    # Retourne les formes fléchies du terme
    # arg 1 : l'objet correspondant au terme
    # arg 2 : le rang de l'occurrence recherchee

    my ($obj) = @_;
    return $obj->{"FF"};
}



sub isGNM_Phr () {
    # Indique si l'objet est un groupe nominal maximal dans la phrase 
    # dont l'identifiant est donnée en argument
    # Renvoie la liste des identifiants de phrases dans lequel le terme est 
    # GNM

    my $obj = shift;

    if ($obj->{"TypeOcc"} eq "ISOL") {
	return 1;
    } else {
	return 0;
    }
}

sub setTypeOcc () {
    # Ajoute le type d'occurrence de chaque terme (Partie ou non 
    # d'un syntagme nominal, Tete, etc.) pour l'occurence $id

    my ($obj, $typeocc) = @_;

    $obj->{"TypeOcc"} = $typeocc;

    return $obj->{"TypeOcc"};
}

sub getTypeOcc () {
    # Retourne le type d'occurrence de chaque terme (Partie ou non 
    # d'un syntagme nominal, Tete, etc.) pour l'occurence $id

    my $obj = shift;

    return $obj->{"TypeOcc"};

}

sub getLemmeFrmSynoTermIn () {
    my $obj = shift;
    my $rtabhLstTermeNumOcc = shift;
    my $i = 0;
    my @tabLemmes = ();
    my @lemme1;
    my @lemme2;
    my $lemme;
    my $j;
    my $Rel;
    my $nblemme;
    my $occTrmTete;
    my $occTrmExp;

    my @tabTete = @{$obj->{"Terme"}->getIdTete()};
    my @tabExp = @{$obj->{"Terme"}->getIdExp()};
#        print STDERR $obj->{"Terme"}->getLemme() . " : " . scalar @tabTete . " : " . scalar @tabExp . "\n";
    if (scalar @tabTete != 0) {
#	print STDERR scalar @{$tabExp[0]->{"LstOcc"}};
#  	if (scalar @{$tabExp[0]->{"LstOcc"}} > 2) {
#  	    print STDERR @{$tabExp[0]->{"LstOcc"}}->[2]->getRefPhr();
#  	    print STDERR "\n";
#          }
	$Rel = $obj->{"Terme"}->getRelateurFrmSynoTermIn();
	for($i=0;$i<scalar @tabTete;$i++) {
#  	    print STDERR $tabTete[$i]->{"IdTrm"};
#  	    print STDERR " : ";
	    if ($$rtabhLstTermeNumOcc{$tabTete[$i]->{"IdTrm"}}-1 < 0) {
		$occTrmTete = 0;
	    } else {
		$occTrmTete = $$rtabhLstTermeNumOcc{$tabTete[$i]->{"IdTrm"}}-1;
	    }
#  	    print STDERR $tabExp[$i]->{"IdTrm"};
#  	    print STDERR "\n";
	    if ($$rtabhLstTermeNumOcc{$tabExp[$i]->{"IdTrm"}}-1 < 0) {
		$occTrmExp = 0;
	    } else {
		$occTrmExp = $$rtabhLstTermeNumOcc{$tabExp[$i]->{"IdTrm"}}-1;
	    }
#  	    print STDERR "$i :: $occTrmTete : $occTrmExp\n";
	    if (($tabTete[$i]->{"LstOcc"}->[$occTrmTete]->getRefPhr() eq $obj->getRefPhr()) && ($tabExp[$i]->{"LstOcc"}->[$occTrmExp]->getRefPhr() eq $obj->getRefPhr())) {
		@lemme1 = $tabTete[$i]->{"LstOcc"}->[$occTrmTete]->getLemmeFrmSynoTermIn($rtabhLstTermeNumOcc);
		@lemme2 = $tabExp[$i]->{"LstOcc"}->[$occTrmExp]->getLemmeFrmSynoTermIn($rtabhLstTermeNumOcc);
#    		print STDERR "\t" . scalar @lemme1 . " : " . scalar @lemme2 . "\n";
		if (scalar @lemme1 <= scalar @lemme2) {
		    $nblemme = scalar @lemme1;
		} else {
		    $nblemme =scalar @lemme2;
		}
		for($j=0;$j< $nblemme ;$j++) {
		    # Generation du lemme au bon format
		    $lemme = "{" . $lemme1[$j] . "|" . $Rel;
		    $lemme = $lemme . "|" . $lemme2[$j] . "}";
		    push @tabLemmes, $lemme;
#  		    print STDERR "------------------------------------------------------------------------\n";
#  		    foreach $elt (@tabLemmes) {
#  			print STDERR "$elt\n";
#  		    }
#  		    print STDERR "------------------------------------------------------------------------\n";
		}
	    }
	}
    } else {
	# Generation du lemme au bon format
	$lemme = "<" . $obj->{"Terme"}->{"Cat"} . ">";
	$lemme = $lemme . $obj->{"Terme"}->{"Lemme"};
	$lemme = $lemme . " [" . $obj->getFF() . " ]";
	push @tabLemmes, $lemme;
    }
    return @tabLemmes;
}


sub getExpFrmSynoTermIn () {
    my $obj = shift;
    my $rtabhLstTermeNumOcc = shift;
    my $i;
    my @tabExps;
    my @exp1;
    my @exp2;
    my $exp;
    my $j;
    my $Rel;

    my @tabTete = @{$obj->{"Terme"}->getIdTete()};
    my @tabExp = @{$obj->{"Terme"}->getIdExp()};


    if (scalar @tabTete != 0) {

#	$Rel = $obj->{"Terme"}->getRelateurFrmSynoTermIn();
	for($i=0;$i<scalar @tabTete;$i++) {
	    @exp1 = $tabTete[$i]->{"LstOcc"}->[$$rtabhLstTermeNumOcc{$tabTete[$i]->{"IdTrm"}}-1]->getExpFrmSynoTermIn($rtabhLstTermeNumOcc);
	    @exp2 = $tabExp[$i]->{"LstOcc"}->[$$rtabhLstTermeNumOcc{$tabExp[$i]->{"IdTrm"}}-1]->getExpFrmSynoTermIn($rtabhLstTermeNumOcc);
	    push @tabExps, @exp1;
	    push @tabExps, @exp2;
	    push @tabExps, $tabExp[$i]->{"IdTrm"};
	}
    }

    return @tabExps;
}

sub getTeteFrmSynoTermIn () {
    my $obj = shift;
    my $rtabhLstTermeNumOcc = shift;
    my $i;
    my @tabTetes;
    my @tete1;
    my @tete2;
    my $tete;
    my $j;
    my $Rel;

    my @tabTete = @{$obj->{"Terme"}->getIdTete()};
    my @tabExp = @{$obj->{"Terme"}->getIdExp()};


    if (scalar @tabTete != 0) {

#	$Rel = $obj->{"Terme"}->getRelateurFrmSynoTermIn();
	for($i=0;$i<scalar @tabTete;$i++) {
	    @tete1 = $tabTete[$i]->{"LstOcc"}->[$$rtabhLstTermeNumOcc{$tabTete[$i]->{"IdTrm"}}-1]->getTeteFrmSynoTermIn($rtabhLstTermeNumOcc);
	    @tete2 = $tabExp[$i]->{"LstOcc"}->[$$rtabhLstTermeNumOcc{$tabExp[$i]->{"IdTrm"}}-1]->getTeteFrmSynoTermIn($rtabhLstTermeNumOcc);
	    push @tabTetes, @tete1;
	    push @tabTetes, @tete2;
	    push @tabTetes, $tabTete[$i]->{"IdTrm"};
	}
    }

    return @tabTetes;
}


1;
