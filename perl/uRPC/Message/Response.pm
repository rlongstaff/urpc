
use warnings;
use strict;

package uRPC::Message::Response;
use parent 'uRPC::Message';

sub _init {
    my $self = shift;

    $self->{_message_type} = uRPC::Message::MESSAGE_RESPONSE;
    return $self->SUPER::_init(@_);
}
1;
