# 	$Log: Terme.pm,v $
# 	Revision 1.4  2008/04/07 07:18:04  ht
# 	correction in deisplaying inflected forms of the terms
# 	
# 	Revision 1.3  2007/11/12 14:50:48  ht
# 	modification des règles d'inférence des relations initiales
# 	correction de la recherche des formes fléchies de termes
# 	
# 	Revision 1.2  2007/10/08 08:16:12  ht
# 	modification in the computing initial relations
# 	
# 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
# 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
# 	
# 	Revision 1.8  2002/07/19 15:28:15  ht
# 	Ajout de la méthode getCat
#
# 	Revision 1.7  2002/07/04 15:48:30  ht
# 	*** empty log message ***
#
# 	Revision 1.6  2002/07/02 13:52:27  ht
# 	Modification complete des objets
#
# 	Revision 1.5  2002/05/14 15:34:32  ht
# 	Ajout de la méthode setIdExp
#
# 	Revision 1.4  2002/05/14 15:31:31  ht
# 	Modification de l'ojet Terme
# 	ainsi que des methodes
# 	L'ancien objet Terme est conserve temporairement
# 	pour des raisons compatibilité avec l'existant
# 	(dans SynoTerm.pl), de meme pour les méthodes
#	

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

use strict;

package Terme;

use GNMParser;


########################################################################
#                                                                      #
# Objectif : Constructeur de l'objet Term                              #
#                                                                      #
# Argument : 1. aucun                                                  #
#                                                                      #
# Retour : L'objet construit                                           #
#                                                                      #
# Date de dernière modification : 19/10/2004                           #
#                                                                      #
########################################################################

sub nouvTrm {
    # Création d'un objet Terme
    # arg1: La classe
    # arg2: L'identifiant du terme
    # arg3: Le lemme
    # arg4: La catégorie
    # arg5: Le lemme 'a la SynoTerm' (Lemme__Cat)
    # arg6: La forme standard

    my $classe = shift;
    my @tabrefPhr = ();
    my @tabrefGNM = ();
    my %tabFF = ();
    my @tabGNC = ();
    my @tabTete = ();
    my @tabExp = ();
    my @tabRel = ();
    my %Typeocc = ();
    my @tabOcc = ();
    my @LemmeCat = ();
    my @MesuresStat = ();
    my @tabIndex = ();

    my $self = {
	"IdTrm" => shift,
	"Lemme" => shift,
	"Cat" => shift,
	"LemmeCat" => \@LemmeCat,
	"RefTrmPhr" => \@tabrefPhr,
	"FFStd" => shift,
	"IdTete" => \@tabTete,
	"IdExp" => \@tabExp,
	"NbOcc" => 0,
	"Relateur" => "",
	"IdGNC" => \@tabGNC,
	"Long" => 0,
	"TypeOcc" => \%Typeocc,
	"LstOcc" => \@tabOcc,
	"MesuresStat" => \@MesuresStat,
	"Index" => \@tabIndex
    };
    bless $self, $classe;

    return $self;
}

########################################################################
#                                                                      #
# Objectif : Ajout de réferences a une phrase dans le tableau          #
#             correspondant                                            #
#                                                                      #
# Argument : 1. l'objet correspondant au terme                         #
#                                                                      #
#            arg suivants: La liste des références a ajouter           #
#                                                                      #
# Retour : l'état de l'exécution de la fonction                        #
#                                                                      #
# Date de dernière modification : 19/10/2004                           #
#                                                                      #
########################################################################


sub addRefPhr {

    my $self = shift;

    push @{$self->{"RefTrmPhr"}}, @_;

    return(0);
}

########################################################################
#                                                                      #
# Objectif : Récupération des  réferences aux phrases dans le tableau  #
#            correspondant                                             #
#                                                                      #
# Argument : 1. l'objet correspondant au terme                         #
#                                                                      #
# Retour : la référence à la liste des références des pages            #
#                                                                      #
# Date de dernière modification : 19/10/2004                           #
#                                                                      #
########################################################################

sub getRefPhr {

    my $self = shift;

    return $self->{"RefTrmPhr"};
}

########################################################################
#                                                                      #
# Objectif : Ajout d'une occurrence dans la liste des occurrences du   #
#            terme                                                     #
#                                                                      #
# Argument : 1. l'objet correspondant au terme                         #
#                                                                      #
# Retour : l'état de l'exécution de la fonction                        #
#                                                                      #
# Date de dernière modification : 19/10/2004                           #
#                                                                      #
########################################################################

sub addOcc () {
    my $self = shift;
    push @{$self->{"LstOcc"}}, @_;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getOcc () {
    my ($self, $numFF) = @_;

    if ($numFF eq "") {
	return @{$self->{"LstOcc"}};
    } else {
	return ${$self->{"LstOcc"}}[$numFF];
    } 
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setFFStd {
    # Ajout de la forme fléchie standard du terme
    # arg 1 : l'objet correspondant au terme
    # arg 2: La  forme fléchie standard a ajouter

    my $self = shift;
    push @{$self->{"FFStd"}}, @_;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getFFStd {
    # Retourne la forme fléchie standard du terme
    # arg 1 : l'objet correspondant au terme

    my ($obj) = @_;
    return @{$obj->{"FFStd"}};
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub new {
    my ($pkg,$idt,$lmt,$reft) = @_;

    my $idgnmphr;
    
    my @tmp = split /\$/, $reft;

    my @tablstRef;
    foreach $idgnmphr (@tmp) {
	# print STDERR "$idgnmphr\n";
	push @tablstRef, split /:/,$idgnmphr;
    }

    # print STDERR @tablstRef; print STDERR "\n";
    # print STDERR \@tablstRef; print STDERR "\n";

    my $terme = {
	"IdTrm" => $idt,
	"LmTrm" => $lmt,
	"RefTrm" => \@tablstRef,
    };
    bless $terme, 'Terme';
    return $terme;
}

sub addRefTrm {
    my ($obj, $reft)= @_;

    my @tmp = split /\$/, $reft;
    my $idgnmphr;

    my @tablstRef;
    foreach $idgnmphr (@tmp) {
	push @tablstRef, split /:/,$idgnmphr;
    }

    $obj->{"RefTrm"} = \@tablstRef,
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getFormeFlechie2 {
    my ($obj, $rLstGNMIdGNM) = @_;

    my $Trm;
    my $m;

    my $motlm;

    my $FormFlech;

    if (!scalar(%$rLstGNMIdGNM)) {
	$Trm = $obj->{"LmTrm"};
	$Trm =~ s/=/ /g;
	$Trm =~ s/__...//g;
	return $Trm;
    } else {
	}
    my $rtabtmp = $obj->{"RefTrm"};
    my @tabtmp = @$rtabtmp;

    my $gnm = $rLstGNMIdGNM->{$obj->{"RefTrm"}->[0]};
    return($gnm);


    my @lstlem = split /=/,$obj->{"LmTrm"};
    if (scalar(@lstlem) == 1) {
	$FormFlech = "";
	$motlm = $obj->{"LmTrm"};
	$motlm =~ s/__...//g;
	
	$motlm = quotemeta($motlm);
	if ($gnm =~ /<[^>]+>$motlm \[([^\]]+)\]/) {
	    $FormFlech = $1;
	}
    } else {
	my $Premier = $lstlem[0];
	$Premier =~ s/__...//g;
	$Premier = $Premier . " ";
	my $Dernier = $lstlem[$#lstlem];
	$Dernier =~ s/__...//g;
	$Dernier = $Dernier . " ";

	my $mot = "";
	$motlm = "";
	my $motglob = "";
	while(($motlm ne $Premier)&&($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|[\-\+]\|)/cg)) {
	    $motglob = $1;
	    $motlm = $2;
	};

	my @lstFF;
	# $mot = $1;
	while(($motlm ne $Dernier)&&($motglob =~ /<[^>]+>([^\[]+)\[([^\]]+)\]|([\+\-]\|)/)) {
	    $motlm = $1;
	    $mot = $2;
	    if ($motlm ne "") {
		push @lstFF, $mot;
	    } else {
		if ($motglob eq "-|") {
		    my $mottmp = pop @lstFF;
		    if ($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|\-\|)/cg) {
			$motglob = $1;
			if ($motglob =~ /<[^>]+>([^\[]+)\[([^\]]+)\]/) {
			    $motlm = $1;
			    $mot = $2;
			    push @lstFF, $mot;
			    push @lstFF, $mottmp;
			}
		    }
		}
	    }
	    if (($motlm ne "$Dernier ")&&($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|\-\|)/cg)) {
		$motglob = $1;
	    }
	}

	$FormFlech = "";
	foreach $m (@lstFF) {
	    $FormFlech = $FormFlech . $m;
	}
	chop $FormFlech;
    }

    $FormFlech =~ s/ de les / des /g;
    $FormFlech =~ s/ de le / du /g;
    $FormFlech =~ s/ &agrave; les / aux /g;
    $FormFlech =~ s/ &agrave; le / au /g;
    $FormFlech =~ s/ d' / d'/g;
    $FormFlech =~ s/ l' / l'/g;
    $FormFlech =~ s/ , /, /g;
    $FormFlech =~ s/ \.$/\./g;

    return $FormFlech;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getFormeFlechie3 {
    my ($obj, $INFLXFORM) = @_;

    warn $obj->getIdTrm . "\n";
#    $obj->get_term_inflx_form;
}

sub getFormeFlechie {
    my ($obj, $rLstGNMIdGNM) = @_;

    my $Trm;
    my $m;
    my $if_term = "";

    my $motlm;

    if (!scalar(%$rLstGNMIdGNM)) {
	$Trm = $obj->{"LmTrm"};
	$Trm =~ s/=/ /g;
	$Trm =~ s/__...//g;
	return $Trm;
    } else {
	}
    my $rtabtmp = $obj->{"RefTrm"};
    my @tabtmp = @$rtabtmp;

    my $gnm = $rLstGNMIdGNM->{$obj->{"RefTrm"}->[0]};

#        print STDERR $obj->{"LmTrm"} . "\n";
#        print STDERR $gnm . "\n";
#        print STDERR join(";",@{$obj->{"RefTrm"}}) . "\n";

    my @lstlem = split /=/,$obj->{"LmTrm"};
    if (scalar(@lstlem) == 1) {
	$if_term = "";
	$motlm = $obj->{"LmTrm"};
	$motlm =~ s/__...//g;

# 	print STDERR "$motlm\n";

	$motlm = quotemeta($motlm);
	if ($gnm =~ /<[^>]+>$motlm \[([^\]]+)\]/) {
	    $if_term = $1;
	}
# 	print STDERR "==>$if_term\n";
    } else {
	my $first_word = $lstlem[0];
	$first_word =~ s/__...//g;
# 	$first_word = $first_word . " ";

	my $last_word = $lstlem[$#lstlem];
	$last_word =~ s/__...//g;
# 	$last_word = $last_word . " ";

# 	print STDERR "$first_word / $last_word\n";

	my $parser = GNMParser->new();

# 	print STDERR "Analysis of $gnm\n";

	$if_term = "";
	$parser->YYData->{GNM} = $gnm;
	$parser->YYData->{GNM_INF} = \$if_term;
	$parser->YYData->{FIRST_WORD} = $first_word;
	$parser->YYData->{LAST_WORD} = $last_word;

	$parser->YYParse(yylex => \&GNMParser::_Lexer, yyerror => \&GNMParser::_Error);


# 	my $mot = "";
# 	$motlm = "";
# 	my $motglob = "";
# 	while(($motlm ne $Premier)&&($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|[\-\+]\|)/cg)) {
# 	    $motglob = $1;
# 	    $motlm = $2;
# 	};

# 	my @lstFF;
# 	# $mot = $1;
# 	while(($motlm ne $Dernier)&&($motglob =~ /<[^>]+>([^\[]+)\[([^\]]+)\]|([\+\-]\|)/)) {
# 	    $motlm = $1;
# 	    $mot = $2;
# 	    if ($motlm ne "") {
# 		push @lstFF, $mot;
# 	    } else {
# 		if ($motglob eq "-|") {
# 		    my $mottmp = pop @lstFF;
# 		    if ($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|\-\|)/cg) {
# 			$motglob = $1;
# 			if ($motglob =~ /<[^>]+>([^\[]+)\[([^\]]+)\]/) {
# 			    $motlm = $1;
# 			    $mot = $2;
# 			    push @lstFF, $mot;
# 			    push @lstFF, $mottmp;
# 			}
# 		    }
# 		}
# 	    }
# 	    if (($motlm ne "$Dernier ")&&($gnm =~ /(<[^>]+>([^\[]+)\[[^\]]+\]|\-\|)/cg)) {
# 		$motglob = $1;
# 	    }
# 	}

# 	$FormFlech = "";
# 	foreach $m (@lstFF) {
# 	    $FormFlech = $FormFlech . $m;
# 	}
# 	chop $FormFlech;
    }

    $if_term =~ s/ de les / des /g;
    $if_term =~ s/ de le / du /g;
    $if_term =~ s/ &agrave; les / aux /g;
    $if_term =~ s/ &agrave; le / au /g;
    $if_term =~ s/ d' / d'/g;
    $if_term =~ s/ l' / l'/g;
    $if_term =~ s/ , /, /g;
    $if_term =~ s/ \.$/\./g;

    $if_term =~ s/ $//;

    return $if_term;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getFormeLemmatiseeStd () {
    my $obj = shift @_;
    
    my $Trm;

    $Trm = $obj->{"LmTrm"};
    $Trm =~ s/=/ /g;
    $Trm =~ s/__...//g;
    return $Trm;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub Affiche {
    my ($obj,) = @_;

    print STDERR $obj->{"IdTrm"}; print STDERR ",\n";
    print STDERR $obj->{"Lemme"}; print STDERR ",\n";
    print STDERR @{$obj->{"RefTrmPhr"}}; print STDERR",\n";
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getIdTrm() {
    my $obj = shift @_;

    return $obj->{"IdTrm"};
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setLemme () {
    # affectation du lemme du terme
    # arg1: le lemme

    my ($obj, $lemme) = @_;

    if ($lemme ne "") {
	$obj->{"Lemme"} = $lemme;
    } else {
	$obj->{"Lemme"} = $obj->getFormeLemmatiseeStd();    
    }
    return $obj;

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getLemme () {
    # affectation du lemme du terme
    # arg1: le lemme

    my ($obj) = @_;

    return $obj->{"Lemme"};

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setLemmeCat () {
    my $self = shift;
    my $LemmeCat;
    my @tabtetes;
    my @tabexps;
    my $tete;
    my $exp;
    my $lemmecattete;
    my $lemmecatexp;
    my $i;
#    my $tmp;

#     $tmp = $self->getLemmeCat();
#      print "===> " . $self->getIdTrm();
#      print " $tmp\n";

    if (($self->getLemmeCat() eq "") || ($_[0] eq "--force")) {

	if ($self->isGNC() == 0) {
	    $LemmeCat = $self->{"Lemme"} . "__" . substr($self->{"Cat"},0,3);
#  	    print "++>$LemmeCat\n";
	    push @{$self->{"LemmeCat"}}, $LemmeCat;
	} else {
	    @tabtetes = @{$self->getIdTete()};
	    @tabexps = @{$self->getIdExp()};

	    foreach $tete  (@tabtetes) {
		$tete->setLemmeCat(@_);
#  		print "-->" . $tete->getLemmeCat() . "\n"; 
	    }

	    foreach $exp  (@tabexps) {
		$exp->setLemmeCat(@_);
#  		print "-->" . $exp->getLemmeCat() . "\n"; 
	    }


  	    for($i=0;$i<scalar @tabtetes;$i++) {
  		foreach $lemmecattete ($tabtetes[$i]->getLemmeCat()) {
  		    foreach $lemmecatexp ($tabexps[$i]->getLemmeCat()) {
#  			print "T : $lemmecattete\nE : $lemmecatexp\n";
			$LemmeCat = $lemmecattete . "=" . $lemmecatexp;
			if ($_[0] eq "--force") {
			    @{$self->{"LemmeCat"}} = ($LemmeCat);
			} else {
			    push @{$self->{"LemmeCat"}}, $LemmeCat;
			}
#  			print "CT : $LemmeCat\n";
  		    }		    
  		}
  	    }
	}
    }

    return $self;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getLemmeCat() {
    my ($obj) = @_;

    return $obj->{"LemmeCat"}->[0];
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getLemmeFrmSynoTerm () {
    # affectation du lemme du terme
    # arg1: le lemme

    my ($obj) = @_;

    return $obj->{"LemmeCat"};

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setCat () {
    # affectation de la catégorie du terme
    # arg1: la catégorie

    my ($obj, $cat) = @_;
    my $lemme_cat;

    if ($cat ne "") {
	if ($cat eq "SNom") {
	    $cat = "Nom";
	}
	if ($cat eq "V") {
	    $cat = "Vrb";
	}
    } else {
	$lemme_cat = $obj->{"LmTrm"};
	if ($lemme_cat !~ /=/) {
	    $lemme_cat =~ /__(...)/;
	    $cat = $1;
	} else {
	    $cat = "GN";
	}
    }
    $obj->{"Cat"} = $cat;
    return $obj;

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getCat() {
    my $obj = shift @_;

    return $obj->{"Cat"};
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setIdTete () {
    # affectation de l'identifiant de la tete du terme
    # arg1: l'identifiant de la tete

    my ($obj, $idtete) = @_;

    if ($obj->existTete($idtete->{"IdTrm"}) == 0) {
	push @{$obj->{"IdTete"}}, $idtete;
    }
    return $#{$obj->{"IdTete"}};

}

#  sub isTete () {
#      # Indique si l'objet est la tete d'un terme
#      # Renvoie vrai ou faux

#      my ($obj) = @_;

#      return ($obj->{"IdTrm"} eq {$obj->{"IdGNC"}}->{"IdTete"});
#  }

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub existTete () {
    # Indique si la tete du terme est l'identifiant donné en
    # argument
    # retourne VRAI ou FAUX

    my ($obj, $idtete) = @_;
    my $tete;

    foreach $tete (@{$obj->{"IdTete"}}) {
  	if ($tete->{"IdTrm"} eq $idtete) {
  	    return 1;
  	}
    }
    return 0;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub RechercheTete () {
    # Indique si la tete du terme est l'identifiant donné en
    # argument
    # retourne VRAI ou FAUX

    my ($obj, $idtete) = @_;
    my $tete;
    my $i;

    for($i=0;$i< scalar @{$obj->{"IdTete"}};$i++) {
	if ($obj->{"IdTete"}->[$i]->{"IdTrm"} eq $idtete) {
	    return ($i+1);
	}
    }

    return 0;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getIdTete () {
    # Renvoie l'identifiant de la tete du terme

    my $obj = shift @_;

    return ($obj->{"IdTete"});
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setIdExp () {
    # affectation de l'identifiant de l'expansion du terme
    # arg1: l'identifiant de l'expansion

    my ($obj, $idexp) = @_;

    if ($obj->existExp($idexp->{"IdTrm"}) == 0) {
	push @{$obj->{"IdExp"}}, $idexp;
    }
    return $#{$obj->{"IdExp"}};

}

#  sub isExp () {
#      # Indique si l'objet est l'expansion d'un terme
#      # Renvoie vrai ou faux

#      my ($obj) = @_;

#      return ($obj->{"IdTrm"} eq {$obj->{"IdGNC"}}->{"IdExp"});
#  }

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub existExp () {
    # Indique si l'expansion du terme est l'identifiant donné en
    # argument
    # retourne VRAI ou FAUX

    my ($obj, $idexp) = @_;
    my $exp;
    my $i;

    foreach $exp (@{$obj->{"IdExp"}}) {
	if ($exp->{"IdTrm"} eq $idexp) {
	    return 1;
	}
    }
    return 0;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub RechercheExp () {
    # Indique si l'expansion du terme est l'identifiant donné en
    # argument
    # retourne VRAI ou FAUX

    my ($obj, $idexp) = @_;
    my $exp;
    my $i;

    for($i=0;$i< scalar @{$obj->{"IdExp"}};$i++) {
	if ($obj->{"IdExp"}->[$i]->{"IdTrm"} eq $idexp) {
	    return ($i + 1);
	}
    }
#      foreach $exp (@{$obj->{"IdExp"}}) {
#  	if ($exp->{"IdTrm"} eq $idexp) {
#  	    return 1;
#  	}
#      }
    return 0;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub delDep() {
    my ($obj, $id) = @_;

    splice @{$obj->{"IdExp"}}, $id, 1;
    splice @{$obj->{"IdTete"}}, $id, 1;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getIdExp () {
    # Renvoie l'identifiant de l'expansion du terme

    my $obj = shift @_;

    return ($obj->{"IdExp"});
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setRelateur () {
    # affectation du relateur (preposition/fonction 
    # syntaxique entre la tete ou l'expansion
    # arg1: relateur

    my $obj = shift;

    $obj->{"Relateur"} = shift;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getRelateur () {
    # Retourne le relateur (preposition/fonction 
    # syntaxique entre la tete ou l'expansion
    # arg1: relateur

    my ($obj) = @_;

    return $obj->{"Relateur"};

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getRelateurFrmSynoTermIn () {
    # Retourne le relateur (preposition/fonction 
    # syntaxique entre la tete ou l'expansion
    # arg1: relateur

    my ($obj) = @_;
    my $rel = $obj->{"Relateur"};

    if ($rel eq "") {
	$rel = "+";
    }

    if ($rel =~ /([^\|]+)\|(.*)/) {
	$rel = "<$1>$2 [$2 ]";
	
    }    

    if (($rel eq "ADV")||($rel eq "EPI")||($rel eq "OBJ")||($rel eq "ADJ")) {
	$rel = "+";
    }    
    if ($rel eq "SUJ") {
	$rel = "-";
    }    

    if ($rel eq "PREC") {
	$rel = "-";
    }    
    if ($rel eq $obj->{"Relateur"}) {
	print STDERR "****** Erreur ";
	print STDERR $obj->{"IdTrm"};
	print STDERR "\n";
	exit;
    }
    return $rel;

}


########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub IncrNbOcc () {
    # affectation du nombre d'occurrence  du terme
    # arg1: nombre d'occurrence

    my ($obj, $nbocc) = @_;

    $obj->{"NbOcc"}++;
    return $obj;

}


########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setNbOcc () {
    # affectation du nombre d'occurrence  du terme
    # arg1: nombre d'occurrence

    my ($obj, $nbocc) = @_;

    $obj->{"NbOcc"} = $nbocc;
    return $obj;

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getNbOcc () {
    # Retounr le nombre d'occurrence  du terme

    my ($obj) = @_;

    return $obj->{"NbOcc"};

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setIdGNC () {
    # affectation de l'identifiant du pere (groupe nominal complexe) du terme
    # arg1: l'identifiant du groupe nominal complexe

    my ($obj, $idpere) = @_;

    push @{$obj->{"IdGNC"}}, $idpere;
    return $#{$obj->{"IdGNC"}};

}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub isGNC () {
    # Indique si l'objet est un groupe nominal complexe
    # Renvoie vrai ou faux

    my ($obj) = @_;

    return scalar @{$obj->getIdTete()};
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getIdGNC () {
    # Renvoie l'identifiant de l'ensemble des peres (le groupe nominal complexe) du terme
    # Valeur de retour : un tableau

    my $obj = shift @_;

    return ($obj->{"IdGNC"});
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub delIdGNC () {
    # Renvoie l'identifiant de l'ensemble des peres (le groupe nominal complexe) du terme
    # Valeur de retour : un tableau

    my ($obj, $id) = @_;

    splice @{$obj->{"IdGNC"}}, $id, 1;
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub isGNM () {
    # Indique si l'objet est un groupe nominal maximal
    # Renvoie la liste des identifiants de phrases dans lequel le terme est 
    # GNM


}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub isGNM_old () {
    # Indique si l'objet est un groupe nominal maximal
    # Renvoie la liste des identifiants de phrases dans lequel le terme est 
    # GNM

    my ($obj, $idseq, $ref_LstTermes) = @_;
    my $seq;
    my $seq2;
    my $numseq;
    my $reftabGNC;
    my $reftabPhr;
    my $idgnc;
    my @tabIdGNC;
    my @tabPhr;
    my @tmp = ();
    my %LstTermes = %$ref_LstTermes;

    if ($idseq eq "") {
	@tabIdGNC = @{$obj->getIdGNC()}; 
	if (scalar @tabIdGNC != 0) {
	    foreach $seq2 (@{$obj->{"RefTrmPhr"}}) {
		foreach $idgnc (@tabIdGNC) {
		    foreach $seq (@{$LstTermes{$idgnc}->{"RefTrmPhr"}}) {
			if ($seq eq $seq2) {
			    push @tmp, $idgnc;
			}
		    }
		}
		if (scalar @tmp == 0) {
		    push @tabPhr, $seq2;
		}
		@tmp = ();
	    }
	    return @tabPhr;
	} else {return @tmp;}
    }
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub isGNM_Phr_old () {
    # Indique si l'objet est un groupe nominal maximal dans la phrase 
    # dont l'identifiant est donnée en argument
    # Renvoie la liste des identifiants de phrases dans lequel le terme est 
    # GNM

    my ($obj, $idseq) = @_;
    my $seq;
    my $seq2;
    my $numseq;
    my $reftabGNC;
    my $reftabPhr;
    my $idgnc;
    my @tabIdGNC;
    my @tabPhr;
    my @tmp = ();
    my @tabGNC;

    @tabIdGNC = @{$obj->getIdGNC()}; 
    if (scalar @tabIdGNC != 0) {
	foreach $idgnc (@tabIdGNC) {
	    foreach $seq (@{$idgnc->{"RefTrmPhr"}}) {
		if ($seq eq $idseq) {
		    push @tabGNC, $idgnc;
		}
	    }
	}
	if (scalar @tabGNC != 0) {return 0}
	else {return 1}
    } else {return 1}
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub isGNCof () {
    # Indique si l'objet est le groupe nominal complexe dont 
    # l'identifiant donne en argument la tete ou l'expansion
    # Renvoie vrai ou faux

    my ($obj, $IdTrm) = @_;    
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setCompTrm () {
    # Initialise la relation de dependance syntaxique entre le terme et son compose
    # le compose doit exister dans la liste des termes
    # le type de relation de dependance pris en compte sont 'T' et 'E'

    my ($obj, $idcomp, $typerel) = @_;

#      if (exists($LstTermes{$idcomp}) {
#  	if ($typerel eq "T") {
	    
#  	}
#  	if ($typerel eq "E") {
	    
#  	}
#      }
    
}

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setLong () {
    # Initialise la longueur du terme (nombre de mots pleins)

    my ($obj, $long) = @_;

    $obj->{"Long"} = $long;
    return $obj;


}    

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getLong () {
    # Retourne la longueur du terme (nombre de mots pleins)

    my ($obj) = @_;

    return $obj->{"Long"};
}    

#*****************************************************
# Description des mesures statistiques stockees dans 
# le tableau @MesuresStat
#        0: Nombre d'occurrences du terme
#        1: Nombre d'occurrence du terme (n1) 
#           tel que n1 soit la tete des couples (n1 .)
#        2: Nombre d'occurrence du terme (n2) 
#           tel que n2 soit l'expansion des couples (. n2)
#        3: Nombre de termes/couples tel que n1 soit la 
#           tete (n1 .)
#        4: Nombre de termes/couples tel que n{ soit la 
#           expansion (. n2)

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setMesStat_nbc () {
    # Calcul de la mesure statistique : 
    #    Nombre d'occurrence du terme

    my ($obj) = @_;
    my $mes = 0;
    
    if ($obj->isGNC() != 0) {
	 $mes = $obj->getNbOcc();
    }
#    print STDERR $obj->getNbOcc();
    $obj->{"MesuresStat"}->[0] = $mes;
    return $mes;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getMesStat_nbc () {
    # Retour de la mesure statistique : 
    #    Nombre d'occurrence du terme

    my ($obj) = @_;
    return $obj->{"MesuresStat"}->[0];
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setMesStat_n1 () {
    # Calcul de la mesure statistique : 
    #    Nombre d'occurrence du terme n1
    #    tel que n1 est la tete de couples (n1 .)

    my ($obj) = @_;
    my $mes = 0;
    my $comp;

    if ($obj->isGNC() != 0) {
# 	print join " : ", @{$obj->getIdTete()};
	foreach $comp (@{$obj->getIdTete()}) {
# 	    print STDERR "--> $comp\n";
	    $mes =+ $comp->getNbOcc();
	}
    }
    $obj->{"MesuresStat"}->[1] = $mes;
    return $mes;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getMesStat_n1 () {
    # Retour de la mesure statistique : 
    #    Nombre d'occurrence du terme n1
    #    tel que n1 est la tete de couples (n1 .)

    my ($obj) = @_;
    return $obj->{"MesuresStat"}->[1];
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setMesStat_n2 () {
    # Calcul de la mesure statistique : 
    #    Nombre d'occurrence du terme n2
    #    tel que n2 est l'expansion de couples (. n2)

    my ($obj) = @_;
    my $mes = 0;
    my $comp;

    if ($obj->isGNC() != 0) {
	foreach $comp (@{$obj->getIdExp()}) {
	    $mes =+ $comp->getNbOcc();
	}
    }
    $obj->{"MesuresStat"}->[2] = $mes;
    return $mes;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getMesStat_n2 () {
    # Retour de la mesure statistique : 
    #    Nombre d'occurrence du terme n2
    #    tel que n2 est l'expansion de couples (. n2)

    my ($obj) = @_;
    return $obj->{"MesuresStat"}->[2];
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setMesStat_nc1 () {
    # Calcul de la mesure statistique : 
    #    Nombre de couples ou le terme n1
    #    est la tete

    my ($obj) = @_;
    my $mes = 0;

    if ($obj->isGNC() != 0) {
	$mes = scalar @{$obj->getIdTete()};
    }
    $obj->{"MesuresStat"}->[3] = $mes;
    return $mes;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getMesStat_nc1 () {
    # Retour de la mesure statistique : 
    #    Nombre de couples ou le terme n1
    #    est la tete

    my ($obj) = @_;
    return $obj->{"MesuresStat"}->[3];
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub setMesStat_nc2 () {
    # Calcul de la mesure statistique : 
    #    Nombre de couples ou le terme n2
    #    est l'expansion

    my ($obj) = @_;
    my $mes = 0;

    if ($obj->isGNC() != 0) {
	$mes = @{scalar $obj->getIdExp()};
    }
    $obj->{"MesuresStat"}->[4] = $mes;
    return $mes;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getMesStat_nc2 () {
    # Retour de la mesure statistique : 
    #    Nombre de couples ou le terme n2
    #    est l'expansion

    my ($obj) = @_;
    return $obj->{"MesuresStat"}->[4];
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub addIndex() {
    my ($obj, $idx) = @_;

    push @{$obj->{"Index"}}, $idx;
}

#--------------------------------------------------

########################################################################
#                                                                      #
# Objectif :                                                           #
#                                                                      #
# Argument : 1.                                                        #
#                                                                      #
# Retour :                                                             #
#                                                                      #
# Date de dernière modification :   /10/2004                           #
#                                                                      #
########################################################################

sub getIndex() {
    my ($obj) = @_;

    return @{$obj->{"Index"}}
}

1;
