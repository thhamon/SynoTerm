package Occ_input;

use strict;

sub new {
    my ($type) = @_;

    return bless {}, $type;
}


sub set_field {
    my ($self, $key, $value) = @_;

    $self->{$key} = $value;
}

sub is_MNP {
    my ($self) = @_;
    
    if ((exists $self->{"MNP"}) && ($self->{"MNP"} == 1)) {
	return(1);
    } else {
	return(0);
    }
    
}

1;

