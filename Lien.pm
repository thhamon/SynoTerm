#	$Log: Lien.pm,v $
#	Revision 1.5  2008/04/07 07:18:04  ht
#	correction in deisplaying inflected forms of the terms
#	
#	Revision 1.4  2007/02/28 11:33:34  ht
#	quelques modifs
#	
#	Revision 1.3  2006/11/30 17:28:51  ht
#	corrections
#	intégration partielle de la possiblité d'ajouter les réuslts extérieurs tels que
#	ceux de Faster (a continuer)
#	
#	Revision 1.2  2006/11/27 16:08:42  ht
#	nombreuses corrections
#	gestion des fichiers simplifiés (pour l'instant)
#	
#	Revision 1.1.1.1  2006/05/03 11:35:11  ht
#	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
#	
#	Revision 1.3  2001/05/20 15:01:37  thierry
#	*** empty log message ***
#
#	Revision 1.2  2001/05/01 18:55:54  thierry
#	*** empty log message ***
#	

use strict;

package Lien;


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
    my ($pkg,$Trm1, $Trm2, $tl, $tr, $Val) = @_;

    my $nouvtl = $tl;
    if ($tl eq "NV") {
	$nouvtl = "SYN";
    }

    my $lien = {
	"trm1" => $Trm1,
	"trm2" => $Trm2,
        "TypeL" => $nouvtl,
        "TypeR" => $tr,
	"Validation" => $Val,
	"Productivity" => 1,
    };
    bless $lien, 'Lien';
    return $lien;
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

sub getCodeLien {
    my ($obj, ) = @_;
    return $obj->{"Validation"};
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

sub setCodeLien {
    
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

sub getTypeLien {
    
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

sub setTypeLien {
    
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

    print STDERR $obj->{"trm1"}->{"LmTrm"}; print STDERR " : ";
    print STDERR $obj->{"trm1"}->{"IdTrm"}; print STDERR " : ";
    print STDERR $obj->{"trm2"}->{"LmTrm"}; print STDERR " : ";
    print STDERR $obj->{"trm2"}->{"IdTrm"}; print STDERR " : ";
    print STDERR $obj->{"TypeL"}; print STDERR " : ";
    print STDERR $obj->{"TypeR"}; print STDERR " : ";
    print STDERR $obj->getvalInt(); print STDERR " : ";
    print STDERR $obj->{"Validation"}; print STDERR "\n";
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

sub PrintVal {
    my ($obj,) = @_;
    my $restmp;

    printf "%010d", 6;
    printf "%010d", length ("Validation");
    print "Validation";
    $restmp = $obj->getvalInt();
    printf "%010d", length ($restmp);
    print  $restmp;
    $restmp = $obj->{"TypeL"};
    printf "%010d", length ($restmp);
    print  $restmp;
    $restmp = $obj->{"trm1"}->{"IdTrm"};
    printf "%010d", length ($restmp);
    print  $restmp;
    $restmp = $obj->{"trm2"}->{"IdTrm"};
    printf "%010d", length ($restmp);
    print  $restmp;
    $restmp = $obj->{"Validation"} & 0x0F00;
#    print STDERR "$restmp\n";
    printf "%010d", length ($restmp);
    print  $restmp;
    open FICHIERTMP, ">>Validation.svgrd.list";
    print FICHIERTMP $obj->{"trm1"}->{"LmTrm"}; print FICHIERTMP "|";
    print FICHIERTMP $obj->{"trm2"}->{"LmTrm"}; print FICHIERTMP "|";
    print FICHIERTMP $obj->{"TypeL"}; print FICHIERTMP "|";
    print FICHIERTMP $obj->{"Validation"}; print FICHIERTMP "|";
    print FICHIERTMP $obj->{"TypeR"}; print FICHIERTMP "\n";
    close FICHIERTMP;

    # print STDIN $obj->{"Validation"}; print "\n";
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

sub getvalStr {
    my ($obj,) = @_;

    if (($obj->{"Validation"}  & 0x0F00) == 0) {
	return "?";
    }
    if (($obj->{"Validation"} & 0x0F00) == 0x0F00) {
	return "N";
    }
    if ((($obj->{"Validation"} & 0x0F00) != 0)&&(($obj->{"Validation"} & 0x0F00) != 0x0F00)) {
	return "Y";
    }
    return "?";
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

sub getvalInt {
    my ($obj,) = @_;

    if (($obj->{"Validation"}  & 0x0F00) == 0) {
	return "-1";
    }
    if (($obj->{"Validation"} & 0x0F00) == 0x0F00) {
	return "0";
    }
    if ((($obj->{"Validation"} & 0x0F00) != 0)&&(($obj->{"Validation"} & 0x0F00) != 0x0F00)) {
	return "1";
    }
    return "-1";
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

sub setvalStr {
    my ($obj, $type, $val) = @_;
	
    if (($val eq "?")) {
	$obj->{"Validation"} = $obj->{"Validation"} & 0xF0FF;
	return $obj->{"Validation"};
        $obj->{"TypeL"} = $type;
    }
    if (($val eq "N")) {
	$obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0F00;
	return $obj->{"Validation"};
    }
    if (($val eq "Y" )) {
	if ($type eq "SYN") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0100;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "HYPO") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0200;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "MERO") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0300;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "HYPER") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0400;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "HOLO") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0500;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "ANTO") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0600;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
	if ($type eq "VA") {
	    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | 0x0700;
	    $obj->{"TypeL"} = $type;
	    return $obj->{"Validation"};
	}
    }
    return "?";
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

sub setval {
    my ($obj,$type, $val) = @_;
#     print STDERR "Nouv Val $val\n";

    $obj->{"Validation"} = ($obj->{"Validation"} & 0xF0FF) | ($val << 8);
#     print STDERR "Validation = " . $obj->{"Validation"} . "\n";

    $obj->{"TypeL"} = $type;
    return $obj->{"Validation"};

#    return $type;
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

sub setvalInt {
    my ($obj,$val) = @_;

    $obj->{"Validation"} = $val;
    if (($val  & 0x0F00) == 0x0000) {
	$obj->{"TypeL"} = "SYN";
	return $val;
    }
    if (($val & 0x0F00) == 0x0F00) {
	$obj->{"TypeL"} = "SYN";
	return $val;
    }
    if (($val & 0x0F00) == 0x0100) {
	$obj->{"TypeL"} = "SYN";
	return $val;
    }
    if (($val & 0x0F00) == 0x0200) {
	$obj->{"TypeL"} = "HYPO";
	return $val;
    }
    if (($val & 0x0F00) == 0x0300) {
	$obj->{"TypeL"} = "MERO";
	return $val;
    }
    if (($val & 0x0F00) == 0x0400) {
	$obj->{"TypeL"} = "HYPER";
	return $val;
    }
    if (($val & 0x0F00) == 0x0500) {
	$obj->{"TypeL"} = "HOLO";
	return $val;
    }
    if (($val & 0x0F00) == 0x0600) {
	$obj->{"TypeL"} = "ANTO";
	return $val;
    }
    if (($val & 0x0F00) == 0x0700) {
	$obj->{"TypeL"} = "VA";
	return $val;
    }
    $obj->{"TypeL"} = "SYN";
    return $val;
}

sub getProductivity {
    my ($obj) = @_;
    return $obj->{"Productivity"};
}

sub setProductivity {
    my ($obj, $productivity) = @_;
    $obj->{"Productivity"} = $productivity;
    return($obj->{"Productivity"});
}


1;
