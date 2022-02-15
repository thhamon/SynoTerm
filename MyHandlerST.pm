
package MyHandlerST;

use Terme_input;
use List_Occ_input;
use Occ_input;
use strict;

sub new {
    my ($type) = @_;
    my %term_list = {};
    my @stack_elements = ();
    my $current_node = -1;

    return bless {"term_list" => {} , "stack_elements" => \@stack_elements, "current_node" => -1, "current_node_occ" => -1}, $type;
}

sub start_element {
    my ($self, $element) = @_;

#     if ($element->{Name} eq "TERM_EXTRACTION_RESULTS") {
# 	$terminology = 1;
#     }
    
    $self->set_top_stack($element->{Name});
    my $XML_tag = $element->{Name};

#     print  "$XML_tag \n";
    # init occ lists
    if (uc($XML_tag) eq "LIST_OCCURRENCES") {
	# new term candidate
	my @tmp;
	$self->get_current_node()->set_field("List_occurrences", List_Occ_input->new()); 
	return;
    }
    # init syntactic analysis
    if (uc($XML_tag) eq "SYNTACTIC_ANALYSIS") {
	# new term candidate
	$self->get_current_node()->new_SA();
	return;
    }

    if (exists $element->{Attributes}->{"POSITION"}) {
 	$self->set_top_stack_attr($element->{Attributes}->{POSITION});
 	$self->set_top_stack_attr("POSITION");
    }
}

sub end_element {
    my ($self, $element) = @_;

    my $elmt;
    my $element_Data = "";

#       print "End element: $element->{Name}\n";
#      print "End element: $element->{Data}\n";

    my $XML_tag = $element->{"Name"};
    

    while((defined($elmt = $self->get_top_stack())) && ($elmt ne $XML_tag)) {
	$element_Data = $elmt . $element_Data;
    }

#      print "element_data = $element_Data\n";
#      print "XML_tag = $XML_tag\n";

#     $self->print_stack();

    if ($element_Data !~ /^\s*$/) {
	
	# init term info
	if (uc($XML_tag) eq "ID") {
	    my $XML_tag_father = $self->get_top_stack();
# 	    print "XML_tag_father : $XML_tag_father\n";
	    if ($XML_tag_father eq "TERM_CANDIDATE") {
# 		print STDERR "PAsse\n";
		# new term candidate
		my $terme = Terme_input->new();
#  		print STDERR "ID : " . $element_Data . "\n";
		$self->{"term_list"}->{$element_Data} = $terme; 
		$self->set_current_node($terme);
	    }
	    if ($XML_tag_father eq "OCCURRENCE") {
		# new term candidate
		my $occ = Occ_input->new();
		$self->get_current_node()->get_list_Occ()->add_Occ($element_Data, $occ); 
		$self->get_current_node()->get_list_Occ()->set_current_occ($occ);
	    }
	    $self->set_top_stack($XML_tag_father);
	    return;
	}

	# misc info
# 	print STDERR $element_Data . "\n";
	$self->add_term_list_term_info($XML_tag, "FORM", "IF", "TERM_CANDIDATE", $element_Data) && return;
	$self->add_term_list_term_info($XML_tag, "LEMMA", "LF", "TERM_CANDIDATE", $element_Data) && return;
	$self->add_term_list_term_info($XML_tag, "SYNTACTIC_CATEGORY", "POS", "MORPHOSYNTACTIC_FEATURES", $element_Data) && return;
	# 
	$self->add_term_list_occ_info($XML_tag, "DOC", "DOC", "OCCURRENCE", $element_Data) && return;
	$self->add_term_list_occ_info($XML_tag, "MNP", "MNP", "OCCURRENCE", $element_Data) && return;
	$self->add_term_list_occ_info($XML_tag, "SENTENCE", "SENT", "OCCURRENCE", $element_Data) && return;
	$self->add_term_list_occ_info($XML_tag, "START_POSITION", "StP", "OCCURRENCE", $element_Data) && return;
	$self->add_term_list_occ_info($XML_tag, "END_POSITION", "EndP", "OCCURRENCE", $element_Data) && return;
	#
	$self->add_term_list_SA_info($XML_tag, "HEAD", "H", "SYNTACTIC_ANALYSIS", $element_Data) && return;

	if ($self->add_term_list_SA_info($XML_tag, "MODIFIER", "M", "SYNTACTIC_ANALYSIS", $element_Data) ) {
  	    $XML_tag = $self->get_top_stack_attr();
  	    $self->add_term_list_SA_info($XML_tag, "POSITION", "pos", "SYNTACTIC_ANALYSIS", $self->get_top_stack_attr()); # MODIFIER -> SYNTACTIC_ANALYSIS
	    
	    return;
	}
	$self->add_term_list_SA_info($XML_tag, "PREP", "prep", "SYNTACTIC_ANALYSIS", $self->get_top_stack()) && return;
	$self->add_term_list_SA_info($XML_tag, "DETERMINER", "det", "SYNTACTIC_ANALYSIS", $self->get_top_stack()) && return;
	$self->set_top_stack($XML_tag);
# 	print "Out\n";
    }



}

sub characters {
    my ($self, $element) = @_;


    if ($element->{"Data"} !~ /^\s*$/) {
	$self->set_top_stack($element->{"Data"});
# 	print STDERR $element->{"Data"} . "\n";
    }

}

sub add_term_list_term_info {
    my ($self, $XML_tag, $tag, $hash_tag, $context_tag, $data) = @_;

    if (uc($XML_tag) eq $tag) {
	my $XML_tag_father = $self->get_top_stack();
# 	print "($XML_tag_father) - " . $data . "\n";
	if ($XML_tag_father eq $context_tag) {
# 	    print "Data: $data\n";
	    # new term candidate
	    $data =~ s/^\s//g;
	    $self->get_current_node()->set_field($hash_tag, $data);
	    $self->set_top_stack($XML_tag_father);
	    return 1;	    
	}
	$self->set_top_stack($XML_tag_father);
    }
    return 0;
}

sub add_term_list_occ_info {
    my ($self, $XML_tag, $tag, $hash_tag, $context_tag, $data) = @_;

#     print "($self, $XML_tag, $tag, $hash_tag, $context_tag, $data)\n";

    if (uc($XML_tag) eq $tag) {
	my $XML_tag_father = $self->get_top_stack();
# 	print "($XML_tag_father) - " . $data . "\n";
	if ($XML_tag_father eq $context_tag) {
# 	    print "Data: $data\n";
	    # new term candidate
	    $data =~ s/^\s//g;
	    $self->get_current_node()->get_list_Occ()->get_current_occ()->set_field($hash_tag, $data); 
	    $self->set_top_stack($XML_tag_father);
	    return 1;	    
	}
	$self->set_top_stack($XML_tag_father);
    }
    return 0;
}

sub add_term_list_SA_info {
    my ($self, $XML_tag, $tag, $hash_tag, $context_tag, $data) = @_;

#    if ($tag eq "POSITION") { 
#         print STDERR "($self, $XML_tag, $tag, $hash_tag, $context_tag, $data)\n";
# 	print STDERR $self->get_current_node()->get_term_lemma() . "\n";
#     }
    if (uc($XML_tag) eq $tag) {
	my $XML_tag_father = $self->get_top_stack();
# 	if ($tag eq "POSITION") { 
# 	    print STDERR "XML_tag_father : $XML_tag_father\n";
# 	}
	if ($XML_tag_father eq $context_tag) {
#  	    print "Data (1): $data\n";
	    # new term candidate
# 	    if ($hash_tag eq "M") {
# 		print STDERR "$hash_tag (1) : $data\n";
# 	    }
	    $data =~ s/^\s+//g;
# 	    if ($hash_tag eq "M") {
# 		print STDERR "$hash_tag (2) : $data\n";
# 	    }
#   	    print "Data (2): $data ($hash_tag)\n";
	    $self->get_current_node()->set_field_SA($hash_tag, $data); 
# 	    if ($tag eq "POSITION") { 
# 		print STDERR $self->get_current_node()->get_term_lemma() . " : " . $self->get_current_node()->get_field_SA($hash_tag) . "\n";
# 	    }
	    $self->set_top_stack($XML_tag_father);
# 	    print "OK\n";
	    return 1;	    
	}
	$self->set_top_stack($XML_tag_father);
    }
    return 0;
}


sub get_top_stack {
    my ($self) = @_;
    return pop (@{$self->{"stack_elements"}});
}

sub set_top_stack {
    my ($self, $element) = @_;
    push (@{$self->{"stack_elements"}}, $element);
}


sub get_top_stack_attr {
    my ($self) = @_;
    return pop (@{$self->{"stack_attribut"}});
}

sub set_top_stack_attr {
    my ($self, $attribut) = @_;
    push (@{$self->{"stack_attribut"}}, $attribut);
}

sub stack_size {
    my ($self) = @_;
  return scalar(@{$self->{"stack_elements"}});
}

sub is_empty {
    my ($self) = @_;
  return $#{$self->{"stack_elements"}} == -1;
}

sub print_stack {
    my ($self) = @_;
    my $i;

    print "VVVVVVVVVVVVVVVVVVVVVVV\n";
    for($i = $self->stack_size() - 1; $i > -1; $i--) {
	print $self->{stack_elements}->[$i] . "\n";
    }
    print "-----------------------\n";
}




sub get_term_node {
    my ($self, $node) = @_;

    if (defined($node)) {
	return($self->{"term_list"}->{$node});
    }
    return($self);

}
sub get_term_occ_doc {
    my ($self) = @_;

}

sub get_term_occ_sent {
    my ($self) = @_;

}

sub get_term_occ_StartPos {
    my ($self) = @_;

}


sub get_term_occ_EndPos {
    my ($self) = @_;

}


sub get_term_occST {
    my ($self) = @_;

}


sub get_term_list_occ_doc {
    my ($self,$node) = @_;


}


sub get_term_list_occ {
    my ($self, $node) = @_;

    if (!defined($node)) {
	$node = $self->get_current_node();
    }
    print STDERR join(",",keys %{$self->{"term_list"}->{$node}->{"List_occurrences"}}) . "\n";
    return(keys %{$self->{"term_list"}->{$node}->{"List_occurrences"}});
    
}

sub get_term_occ_n {
    my ($self,$pos,$node) = @_;

    if (defined($node)) {
	return($self->get_term_node()->get_term_list_occ()->{$pos});
    }
    return($self->get_term_list_occ()->[$pos]);
}


sub get_term_nb_occ {
    my ($self,$node) = @_;

    if (!defined($node)) {
	$node = $self->get_current_node();
    }
    return(scalar(keys %{$self->{"term_list"}->{$node}->{"List_occurrences"}}));
}

sub get_term_list_occ_sent {
    my ($self) = @_;

    
}

sub get_term_list_occST {
    my ($self) = @_;

}


sub get_current_node {
    my ($self) = @_;

    return($self->{"current_node"});
}



sub set_current_node {
    my ($self, $id) = @_;

    $self->{"current_node"} = $id;
}

sub is_top_occ() {
    my ($self) = @_;


}


sub is_top_list_occ() {
    my ($self) = @_;
    my $occ;
    my @output_tab;

    foreach $occ ($self->get_term_list_occ()) {
	if (exists $self->get_term_occ_n($occ)->{"MNP"}) {
	    push @output_tab, $occ;
	}
    }
    return @output_tab;

}

sub is_top {
    my ($self) = @_;

    $self->is_top_occ();
}

sub get_node {
    my ($self, $node_id) = @_;

    return $self->{"term_list"}->{$node_id};
}

1;


