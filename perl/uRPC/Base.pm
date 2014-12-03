
use warnings;
use strict;

package uRPC::Base;

sub new {
    my $proto = shift;
    $proto = ref $proto || $proto;
    my %args = @_;

    my $self = {};
    $self = bless($self, $proto);
    return $self->_init(%args);
}

sub _init {
    my $self = shift;
    my %args = @_;

    return $self;
}

1;
