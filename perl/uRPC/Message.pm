
use warnings;
use strict;

package uRPC::Message;
use parent 'uRPC::Base';

use constant MESSAGE_REQUEST => 0;
use constant MESSAGE_RESPONSE => 1;

my $MAGIC = 'uRPC';

sub _init {
    my $self = shift;
    my %args = @_;

    $self->{_message_type} = MESSAGE_REQUEST;
    $self->{_error} = 0;
    $self->{_request_id} = 0;
    $self->{_rpc} = 0;
    $self->{_payload} = undef;

    if (exists $args{Raw}) {
        $self->deserialize($args{Raw});
    }

    return $self->SUPER::_init(%args);
}

sub rpc {
    my $self = shift;

    if (scalar @_) {
        $self->{_rpc} = $_[0];
    }
    return $self->{_rpc};
}

sub request_id {
    my $self = shift;

    if (scalar @_) {
        $self->{_request_id} = $_[0];
    }
    return $self->{_request_id};
}

sub payload {
    my $self = shift;

    if (scalar @_) {
        $self->{_payload} = $_[0];
    }
    return $self->{_payload};
}

sub is_error {
    my $self = shift;

    return ($self->{_error})?1:0;
}

sub error {
    my $self = shift;
    my ($msg) = @_;

    $self->{_error} = 1;
    $self->{_payload} = $msg;
}

sub serialize {
    my $self = shift;

    return pack('a4SCa*',
        $MAGIC,
        #FLAGS,
        $self->request_id(),
        $self->rpc(),
        $self->payload(),
    );
}

sub deserialize {
    my $self = shift;
    my ($buf) = @_;

    my ($magic_chk, $request_id, $rpc, $payload) = unpack('a4SCa*', $buf);
    if ($magic_chk ne $MAGIC) {
        $self->error("Bad magic");
    } else {
        $self->request_id($request_id);
        $self->rpc($rpc);
        $self->payload($payload);
    }
    return 1;
}

1;
