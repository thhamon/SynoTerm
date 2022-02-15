package List_Occ_input;


use strict;

sub new {
    my ($type) = @_;

    return bless {}, $type;
}


sub set_field {
    my ($self, $key, $value) = @_;

    $self->{$key} = $value;
}

sub add_Occ {
    my ($self, $id, $occ) = @_;

    $self->{$id} = $occ;

}

sub set_current_occ {
    my ($self, $value) = @_;

    $self->{"current_occ"} = $value;
    
}

sub get_current_occ {
    my ($self) = @_;

#     print $self->{"current_occ"} . "\n";
    return($self->{"current_occ"});
    
}

sub exists_MNP {
    my ($self) = @_;

    my $id;
    my @tab_id = ();

    foreach $id (keys %$self) {
	if ($self->{$id}->is_MNP) {
	    push @tab_id, $id;
	}
    }
    return(@tab_id);
}

1;

