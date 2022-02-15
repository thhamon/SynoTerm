package CompConx;

#	$Log: CompConx.pm,v $
#	Revision 1.2  2008/04/07 07:18:04  ht
#	correction in deisplaying inflected forms of the terms
#	
#	Revision 1.1.1.1  2006/05/03 11:35:11  ht
#	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
#	
#	Revision 1.3  2001/05/20 14:58:29  thierry
#	*** empty log message ***
#
#	Revision 1.2  2001/05/01 18:58:14  thierry
#	*** empty log message ***
#	

sub new {
    my ($pkg,$rLien,$Trm1,$Trm2) = @_;
    
    my $cle = "", $cleId = "";

    if ($Trm1->{"LmTrm"} ge $Trm2->{"LmTrm"}) {
	$cle = $Trm1->{"LmTrm"} . " : " . $Trm2->{"LmTrm"};
    } else {
	$cle = $Trm2->{"LmTrm"} . " : " . $Trm1->{"LmTrm"};
    }

    if ($Trm1->{"IdTrm"} ge $Trm2->{"IdTrm"}) {
	$cleId = $Trm1->{"IdTrm"} . " : " . $Trm2->{"IdTrm"};
    } else {
	$cleId = $Trm2->{"IdTrm"} . " : " . $Trm1->{"IdTrm"};
    }

    my $CC = {
	"LstTermes" => {
	    $Trm1->{"LmTrm"} => { $Trm1->{"IdTrm"} => $Trm1,}, 
	    $Trm2->{"LmTrm"} => { $Trm2->{"IdTrm"} => $Trm2,},},
	"LstLiens" => { $cle => { $cleId => $rLien, },},
    };
    bless $CC, 'CompConx';
    return $CC;
}

sub RechercherTerme {
    my ($obj, $trm) = @_;

    return exists($obj->{"LstTermes"}->{$trm->{"LmTrm"}});
}

sub RechercherLien {
    my ($obj, $lien) = @_;

    my $cle = "";
    my $cleId = "";

    if ($lien->{"trm1"}->{"LmTrm"} ge $lien->{"trm2"}->{"LmTrm"}) {
	$cle = $lien->{"trm1"}->{"LmTrm"} . " : " . $lien->{"trm2"}->{"LmTrm"};
    } else {
	$cle = $lien->{"trm2"}->{"LmTrm"} . " : " . $lien->{"trm1"}->{"LmTrm"};
    }

    if ($lien->{"trm1"}->{"IdTrm"} ge $lien->{"trm2"}->{"IdTrm"}) {
	$cleId = $lien->{"trm1"}->{"IdTrm"} . " : " . $lien->{"trm2"}->{"IdTrm"};
    } else {
	$cleId = $lien->{"trm2"}->{"IdTrm"} . " : " . $lien->{"trm1"}->{"IdTrm"};
    }
    
    return exists($obj->{"LstLiens"}->{$cle});
}

sub RechercherTrm1Trm2 {
    my ($obj, $terme1, $terme2) = @_;

    my $cle = "";
    my $current_key = "";
    my $searchedkey1 = "";
    my $searchedkey2 = "";

    if (($terme1 !~ /__\.\.\./o) && ($terme2 !~ /__\.\.\./o)) {
	if ($terme1 ge $terme2) {
	    $cle = $terme1 . " : " . $terme2;
	} else {
	    $cle = $terme2 . " : " . $terme1;
	}
    } else {
	$searchedkey1 = quotemeta($terme1 . " : " . $terme2);
	$searchedkey2 = quotemeta($terme2 . " : " . $terme1);


	foreach $current_key (keys %{$obj->{"LstLiens"}} ) {
	    if (($current_key =~ /^$searchedkey1$/) || ($current_key =~ /^$searchedkey2$/)) {
# 		print STDERR "Searching $searchedkey2 or $searchedkey1...\n";
# 		print STDERR "Scanning $current_key\n";
# 		print STDERR "==>Found\n";
		$cle = $current_key;
		last;
	    }
	}
	
	
    }


    if (exists($obj->{"LstLiens"}->{$cle})) {
	return $cle;
    } else {
	return "";
    }
}

sub insererL {
    my ($obj, $lien) = @_;

    my $cle = "";
    my $cleId =  "";

    if ($lien->{"trm1"}->{"LmTrm"} ge $lien->{"trm2"}->{"LmTrm"}) {
	$cle = $lien->{"trm1"}->{"LmTrm"} . " : " . $lien->{"trm2"}->{"LmTrm"};
    } else {
	$cle = $lien->{"trm2"}->{"LmTrm"} . " : " . $lien->{"trm1"}->{"LmTrm"};
    }

    if ($lien->{"trm1"}->{"IdTrm"} ge $lien->{"trm2"}->{"IdTrm"}) {
	$cleId = $lien->{"trm1"}->{"IdTrm"} . " : " . $lien->{"trm2"}->{"IdTrm"};
    } else {
	$cleId = $lien->{"trm2"}->{"IdTrm"} . " : " . $lien->{"trm1"}->{"IdTrm"};
    }

#    print STDERR "$cle\n";
#    print STDERR "$cleId\n";

    if (!exists($obj->{"LstLiens"}->{$cle})) {
	    $obj->{"LstLiens"}->{$cle} = {$cleId => $lien, };
    } else {
	if (!exists($obj->{"LstLiens"}->{$cle}->{$cleId})) {
	    $obj->{"LstLiens"}->{$cle}->{$cleId} = $lien;
	}
    }
}

sub insererT {
    my ($obj, $Trm) = @_;

    if (!exists($obj->{"LstTermes"}->{$Trm->{"LmTrm"}})) {
	$obj->{"LstTermes"}->{$Trm->{"LmTrm"}} = { $Trm->{"IdTrm"} => $Trm,};
    } else {
	if (!exists($obj->{"LstTermes"}->{$Trm->{"LmTrm"}}->{$Trm->{"IdTrm"}})) {
	    $obj->{"LstTermes"}->{$Trm->{"LmTrm"}}->{$Trm->{"IdTrm"}} = $Trm;
	}
    }
}

sub fusion {
    my ($obj, $CC2) = @_;
    
    my $rh = $CC2->{"LstTermes"};
    
    foreach $lmt (keys %$rh) {
	my $rh2 = $rh->{$lmt};
	foreach $idt (keys %$rh2) {
	    $obj->insererT($rh2->{$idt});
        } 
    }

    $rh = $CC2->{"LstLiens"};
    foreach $l (keys %$rh) {
	my $rh2 = $rh->{$l};
	foreach $idl (keys %$rh2) {
	    $obj->insererL($rh2->{$idl});
        }
    }
}

	#if (!exists($obj->{"LstTermes"}->{$t})) {
				#    $obj->{"LstTermes"}->{$t} = 0;
				#}

	#if (!exists($obj->{"LstLiens"}->{$l})) {
	#    $obj->{"LstLiens"}->{$l} = 0;
	#}

sub RechercherLienParType {
    my ($obj, $typeL, $rHLiensT) = @_;

    my $rh = $obj->{"LstLiens"};

    foreach $cle (keys %$rh) {
	my $rh2 = $rh->{$cle};
	my @tabcleid = keys %$rh2;
	$cleid = pop @tabcleid;
	if (($rh2->{$cleid}->{"TypeL"} eq $typeL) && (($rh2->{$cleid}->{"Validation"} & 0x0F00) != 0x0F00)){
	    $rHLiensT->{$cle} = $rh2;
	}
    }
}

1;
