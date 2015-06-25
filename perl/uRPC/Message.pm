
use warnings;
use strict;

package uRPC::Message;
use parent 'uRPC::Base';

use constant MESSAGE_REQUEST => 0;
use constant MESSAGE_RESPONSE => 1;

my $CRC_POLYNOMIAL = 0x96;

my $FLAGS_MSG_TYPE = 0;
my $FLAGS_ERROR = 1;

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

    # 2 byte: request id
    # 1 byte: RPC #
    # 1 byte: Flags (bit field)
    #     bit 0: request/reply
    #     bit 1: error
    # 1 byte: reserved
    # 2 byte: payload length
    # 1 byte: CRC8 of headers for alignment verification

    my $flags;
    vec($flags, $FLAGS_MSG_TYPE, 1) = $self->{_message_type};
    vec($flags, $FLAGS_ERROR, 1) = $self->{_error};

    my $header = pack('SCaCS',
        $self->request_id(),
        $self->rpc(),
        $flags,
        0, # reserved
        length($self->payload()),
    );
    return pack('a7Ca*',
        $header,
        crc8($header, $CRC_POLYNOMIAL),
        $self->payload(),
    );
}

sub deserialize {
    my $self = shift;
    my ($buf) = @_;

    my ($header, $crc) = unpack('a7C', $buf);
    if (crc8($header, $CRC_POLYNOMIAL) != $crc) {
        $self->error("Bad header");
        return 0;
    }

    my ($request_id,
        $rpc,
        $flags,
        undef, # reserved
        $payload_length,
        $header_crc,
        $payload) = unpack('SCaCSCa*', $buf);

    $self->request_id($request_id);
    $self->rpc($rpc);

    $self->{_message_type} = vec($flags, $FLAGS_MSG_TYPE, 1);

    if (vec($flags, $FLAGS_ERROR, 1)){
        $self->error($payload);
    }

    $self->payload($payload);
    return 1;
}

sub crc8 {
    my ($msg, $poly) = @_;

    my $crc = 0;
    foreach my $byte (unpack('C*', $msg)) {
        $crc ^= $byte;
        for my $i (0..7) {
            if ($crc & 0x80) {
                $crc = ($crc << 1) & 0xFF;
                $crc ^= $poly;
            } else {
                $crc = ($crc << 1) & 0xFF;
            }
        }
    }
    return $crc;
}

1;
