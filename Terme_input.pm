
package Terme_input;


use strict;

sub new {
    my ($type) = @_;

    return bless {}, $type;
}


sub set_field {
    my ($self, $key, $value) = @_;

#     print "$value\n";
    $self->{$key} = $value;
}

sub new_SA {
    my ($self) = @_;

    $self->{"syntactic_analysis"} = {}; 
}

sub get_list_Occ {
    my ($self) = @_;

#     print $self->{"List_occurrences"};
#     print "\n";
    return($self->{"List_occurrences"});
}

sub get_SA {
    my ($self) = @_;

    return($self->{"syntactic_analysis"});
}

sub set_field_SA {
    my ($self, $key, $value) = @_;

    $self->get_SA()->{$key} = $value;
}

sub get_field_SA {
    my ($self, $key) = @_;

    return($self->get_SA()->{$key});
}

sub get_term_lemma {
    my ($self) = @_;

    my $tmplemma = $self->{"LF"};

    $tmplemma =~ s/ /_/g;
    $tmplemma =~ s/:/;/g;
    $tmplemma =~ s/\[/(/g;
    $tmplemma =~ s/\]/)/g;


    return ($tmplemma);

}

sub get_term_lemma_pos {
    my ($self, $PTB2ST) = @_;


    return ($self->get_term_lemma() . "__" . $self->get_term_POSTAGST_norm($PTB2ST));

}

sub get_term_inflx_form {
    my ($self) = @_;


    my $tmpinf = $self->{"IF"};

    $tmpinf =~ s/ /_/g;
    $tmpinf =~ s/:/;/g;
    $tmpinf =~ s/\[/(/g;
    $tmpinf =~ s/\]/)/g;


    return ($tmpinf);

    return ($self->{"IF"});
}
sub get_term_inflx_form2 {
    my ($self) = @_;

    return ($self->{"IF"});
}

sub get_term_POSTAG {
    my ($self) = @_;

    return ($self->{"POS"});
}

sub get_term_POSTAGST {
    my ($self,$PTB2ST) = @_;

    if (defined($PTB2ST->{$self->{"POS"}})) {
	return ($PTB2ST->{$self->{"POS"}});
    } else {
	warn $self->{"POS"} . "not found - setting with the default POS \n";
	return ("Nom");
    }
}

sub get_term_POSTAGST_norm {
    my ($self,$PTB2ST) = @_;

    if (defined($PTB2ST->{$self->{"POS"}})) {
	$PTB2ST->{$self->{"POS"}} =~ /^(...)/;
	return ($1);
    } else {
	warn $self->{"POS"} . "not found - setting with the default POS \n";
	return ("Nom");
    }
}

sub get_term_SA_head_id {
    my ($self) = @_;

    return($self->{"syntactic_analysis"}->{"H"});
}

sub get_term_head {
    my ($self, $handler) = @_;

#       print STDERR "H :" .  $self->get_term_SA_head_id() . ";\n";
#       print STDERR "info : " . $handler->get_node($self->get_term_SA_head_id())->{"LF"} . "\n";

    return($handler->get_node($self->get_term_SA_head_id()));
}

sub get_term_SA_modifier_id {
    my ($self) = @_;

    return($self->{"syntactic_analysis"}->{"M"});
}

sub get_term_modifier {
    my ($self, $handler) = @_;

#       print "M :" .  $self->get_term_SA_modifier_id() . ";\n";
#       print "info : " . $handler->get_node($self->get_term_SA_modifier_id())->{"LF"} . "\n";

    return($handler->get_node($self->get_term_SA_modifier_id()));
}

sub get_term_lemmaST {
    my ($self, $handler,$PTB2ST) = @_;
    my $lemmaST = "";

    if (!($self->exists_term_SA())) {
	$lemmaST = $self->get_term_lemma() . "__" . $self->get_term_POSTAGST_norm($PTB2ST);
	$lemmaST =~ s/ /=/g;
    } else {
	$lemmaST = $self->get_term_head($handler)->get_term_lemmaST($handler,$PTB2ST); 
 	$lemmaST = $lemmaST . "=" . $self->get_term_modifier($handler)->get_term_lemmaST($handler,$PTB2ST);
    }
    return $lemmaST;

}

sub get_term_lemmaST_head {
    my ($self, $handler, $PTB2ST) = @_;

    my @tab_comp = ();

    push @tab_comp, $self->get_term_head($handler)->get_term_lemmaST($handler,$PTB2ST);
    if ($self->get_term_head($handler)->exists_term_SA()) {
	push @tab_comp, $self->get_term_head($handler)->get_term_lemmaST_head($handler,$PTB2ST);    
    }
    if ($self->get_term_modifier($handler)->exists_term_SA()) {
	push @tab_comp, $self->get_term_modifier($handler)->get_term_lemmaST_head($handler,$PTB2ST);    
    }
    return(@tab_comp);
}

sub get_term_lemmaST_modifier {
    my ($self, $handler,$PTB2ST) = @_;

    my @tab_comp = ();

    push @tab_comp, $self->get_term_modifier($handler)->get_term_lemmaST($handler,$PTB2ST);
    if ($self->get_term_modifier($handler)->exists_term_SA()) {
	push @tab_comp, $self->get_term_modifier($handler)->get_term_lemmaST_modifier($handler,$PTB2ST);    
    }
    if ($self->get_term_head($handler)->exists_term_SA()) {
	push @tab_comp, $self->get_term_head($handler)->get_term_lemmaST_modifier($handler,$PTB2ST);    
    }

    return(@tab_comp);
}



sub get_term_node {
    my ($self, $node) = @_;

    if (defined($node)) {
	return($self->{"term_list"}->{$node});
    }
    return($self);

}

sub get_term_SA_lemma {
    my ($self, $PTB2ST) = @_;
    
#    if ($self->get_term_POSTAGST($PTB2ST)

    return "<" . $self->get_term_POSTAGST($PTB2ST) . ">" . $self->get_term_lemma() . " [" . $self->get_term_inflx_form() . " ]";
}

sub get_term_SA {
    my ($self, $handler, $PTB2ST) = @_;
    my $SA_head;
    my $SA_modifier;
    my $SA_prep;
    my $SA_det;
    my $SA;

    if ($self->exists_term_SA()) {
#       print STDERR $self->{"LF"} . "\n";
#       print STDERR $self->get_term_SA_head_id() . ";\n";
#       print STDERR $self->get_term_SA_modifier_id() . ";\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#  	print "==>" . $self->get_term_head($handler)->{"LF"} . "\n";
 	$SA_head = $self->get_term_head($handler)->get_term_SA($handler, $PTB2ST); 
#  	print "-->" .  $self->get_term_modifier($handler)->get_term_lemma() . "\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#   	print "-->";
# 	print $self->get_term_modifier($handler)->{"LF"} . "\n";
	$SA_modifier = $self->get_term_modifier($handler)->get_term_SA($handler, $PTB2ST); 
# 	print "==\n";
# 	# printing
	$SA = "{" . $SA_head . "|";

# 	print STDERR $self->get_term_lemma() . " : " . $self->get_term_SA_modifier_pos() . "\n";
	if ($self->get_term_SA_modifier_pos() eq "BEFORE") {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
# 		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
	    } else {
		$SA = $SA . "-";
	    }
	} else {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
#  		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
 		$SA = $SA . $self->get_term_SA_modifier_Prep() . $self->get_term_SA_modifier_Det();
	    } else {
		$SA = $SA . "+";
	    }
	}
	$SA = $SA . "|" . $SA_modifier . "}";
    } else {
	return $self->get_term_SA_lemma($PTB2ST);
    }
    return ($SA);
}

sub get_term_lemSA_lem {
    my ($self, $handler, $PTB2ST) = @_;
    my $SA_head;
    my $SA_modifier;
    my $SA_prep;
    my $SA_det;
    my $SA;
    my $std_head_lemma;
    my $std_modifier_lemma;
    my $std_lemma;

    if ($self->exists_term_SA()) {
#       print STDERR $self->{"LF"} . "\n";
#       print STDERR $self->get_term_SA_head_id() . ";\n";
#       print STDERR $self->get_term_SA_modifier_id() . ";\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#  	print "==>" . $self->get_term_head($handler)->{"LF"} . "\n";
 	($SA_head, $std_head_lemma) = $self->get_term_head($handler)->get_term_lemSA_lem($handler, $PTB2ST); 
#  	print "-->" .  $self->get_term_modifier($handler)->get_term_lemma() . "\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#   	print "-->";
# 	print $self->get_term_modifier($handler)->{"LF"} . "\n";
	($SA_modifier, $std_modifier_lemma) = $self->get_term_modifier($handler)->get_term_lemSA_lem($handler, $PTB2ST); 
# 	print "==\n";
# 	# printing
	$SA = $SA_head;
	$std_lemma = $std_head_lemma;


# 	print STDERR $self->get_term_lemma() . " : " . $self->get_term_SA_modifier_pos() . "\n";
	if ($self->get_term_SA_modifier_pos() eq "BEFORE") {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
# 		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
	    } else {
		$SA .= "=";
		$std_lemma = $std_modifier_lemma . "=" . $std_head_lemma;
	    }
	} else {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
#  		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
 		$SA .= $self->get_term_SA_modifier_Prep() . $self->get_term_SA_modifier_Det();
		$std_lemma = $std_head_lemma . "=" . $self->get_term_SA_modifier_Prep() . "=" . $self->get_term_SA_modifier_Det() . $std_modifier_lemma;
	
	    } else {
		$SA .= "=";
		$std_lemma = $std_head_lemma . "=" . $std_modifier_lemma;
	    }
	}
	$SA .= $SA_modifier;
    } else {
	return ($self->get_term_lemma_pos($PTB2ST), $self->get_term_lemma_pos($PTB2ST));
    }
    return($SA, $std_lemma);

}


sub get_term_lemSA_lem_FF {
    my ($self, $handler, $PTB2ST) = @_;
    my $SA_head;
    my $SA_modifier;
    my $SA_prep;
    my $SA_det;
    my $SA;
    my $std_head_lemma;
    my $std_modifier_lemma;
    my $std_lemma;

    if ($self->exists_term_SA()) {
#       print STDERR $self->{"LF"} . "\n";
#       print STDERR $self->get_term_SA_head_id() . ";\n";
#       print STDERR $self->get_term_SA_modifier_id() . ";\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#  	print "==>" . $self->get_term_head($handler)->{"LF"} . "\n";
 	($SA_head, $std_head_lemma) = $self->get_term_head($handler)->get_term_lemSA_lem_FF($handler, $PTB2ST); 
#  	print "-->" .  $self->get_term_modifier($handler)->get_term_lemma() . "\n";
# 	print join(":",keys %{$self->{"syntactic_analysis"}}). "\n";
# 	print join(":",values %{$self->{"syntactic_analysis"}}). "\n";
#   	print "-->";
# 	print $self->get_term_modifier($handler)->{"LF"} . "\n";
	($SA_modifier, $std_modifier_lemma) = $self->get_term_modifier($handler)->get_term_lemSA_lem_FF($handler, $PTB2ST); 
# 	print "==\n";
# 	# printing
	$SA = $SA_head;
	$std_lemma = $std_head_lemma;


# 	print STDERR $self->get_term_lemma() . " : " . $self->get_term_SA_modifier_pos() . "\n";
	if ($self->get_term_SA_modifier_pos() eq "BEFORE") {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
# 		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
	    } else {
		$SA .= "=";
		$std_lemma = $std_modifier_lemma . "=" . $std_head_lemma;
	    }
	} else {
	    if ($self->exists_term_SA_modifier_Prep() || $self->exists_term_SA_modifier_Prep()) {
#  		$SA = $SA . $self->get_term_SA_lemma($self->get_term_SA_modifier_Prep()) . $self->get_term_SA_lemma($self->get_term_SA_modifier_Det());
 		$SA .= $self->get_term_SA_modifier_Prep() . $self->get_term_SA_modifier_Det();
		$std_lemma = $std_head_lemma . "=" . $self->get_term_SA_modifier_Prep() . "=" . $self->get_term_SA_modifier_Det() . $std_modifier_lemma;
	
	    } else {
		$SA .= "=";
		$std_lemma = $std_head_lemma . "=" . $std_modifier_lemma;
	    }
	}
	$SA .= $SA_modifier;
    } else {
	return ($self->get_term_lemma_pos($PTB2ST), $self->get_term_lemma_pos($PTB2ST), $self->get_term_inflx_form());
    }
    return($SA, $std_lemma, $self->get_term_inflx_form());

}

# get_term_inflx_form

# ???
# exists_term_SA_lemST -
# get_term_head_lemST
# get_term_modifier_lemST
# exists_term_SA_modifier_Prep_lemST
# exists_term_SA_modifier_Prep_lemST
# get_term_SA_modifier_Prep_lemST
# get_term_SA_modifier_Det_lemST
# get_term_SA_lemma_lemST

#	return($self->{"term_list"}->{$node}->{"syntactic_analysis"}->{"H"});

sub get_term_SA_modifier_pos {
    my ($self) = @_;

    return($self->{"syntactic_analysis"}->{"pos"});

}

sub get_term_SA_modifier_Prep {
    my ($self) = @_;

    return($self->{"syntactic_analysis"}->{"prep"});

}

sub get_term_SA_modifier_Det {
    my ($self) = @_;

    return($self->{"syntactic_analysis"}->{"det"});

}

sub exists_term_SA_modifier_Prep {
    my ($self) = @_;

    if ( exists $self->{"syntactic_analysis"}->{"prep"}) {
	return 1;
    } else {
	return 0;
    }

}

sub exists_term_SA_modifier_Det {
    my ($self) = @_;

    if (exists $self->{"syntactic_analysis"}->{"det"}) {
	return 1;
    } else {
	return 0;
    }

}


sub exists_term_SA {
    my ($self) = @_;

    if (exists $self->{"syntactic_analysis"}) {
	return 1;
    } else {
	return 0;
    }
}

sub exists_term_occ {
    my ($self) = @_;

}

sub exists_term_list_occ {
    my ($self) = @_;

}

sub exists_term_lemma {
    my ($self, $node) = @_;

    if (!defined($node)) {
	$node = $self->get_current_node();
    }
    if (exists $self->{"term_list"}->{$node}->{"LF"}) {
	return 1;
    } else {
	return 0;
    }
}

sub exists_term_POSTAG {
    my ($self, $node) = @_;

    if (!defined($node)) {
	$node = $self->get_current_node();
    }
    if (exists $self->{"term_list"}->{$node}->{"POS"}) {
	return 1;
    } else {
	return 0;
    }
}

sub exists_term_inflxform {
    my ($self, $node) = @_;

    if (!defined($node)) {
	$node = $self->get_current_node();
    }
    if (exists $self->{"term_list"}->{$node}->{"IF"}) {
	return 1;
    } else {
	return 0;
    }
}

sub exists_as_MNP {
    my ($self) = @_;

    if (scalar ($self->get_list_Occ()->exists_MNP())) {
	return 1;
    } else {
	return 0;
    }
}

1;
