
use warnings;
use strict;

package uRPC::Message::Request;
use parent 'uRPC::Message';

sub _init {
    my $self = shift;

    $self->{_message_type} = uRPC::Message::MESSAGE_REQUEST;

    return $self->SUPER::_init(@_);
}

1;
