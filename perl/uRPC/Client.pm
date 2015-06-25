
use warnings;
use strict;

use IO::Socket::INET;
use uRPC::Message::Request;
use uRPC::Message::Response;

package uRPC::Client;
use parent 'uRPC::Base';

my $DEFAULT_MAX_PACKET_SIZE = 64;

sub _init {
    my $self = shift;
    my %args = @_;

    $self->{_max_packet_size} = $args{MaxPacketSize};
    unless ($self->{_max_packet_size}) {
        $self->{_max_packet_size} = $DEFAULT_MAX_PACKET_SIZE;
    }

    $self->{_session_id} = undef;
    $self->{_sock} = undef;

    return $self->SUPER::_init(@_);
}

sub max_packet_size {
    my $self = shift;
    return $self->{_max_packet_size};
}

sub session_id {
    my $self = shift;
    return $self->{_session_id};
}

sub request {
    my $self = shift;
    my ($rpc, $payload) = @_;

    my $req = uRPC::Message::Request->new();
    $req->rpc($rpc);
    $req->payload($payload);

    $self->{_sock}->send($req->serialize());
    my $msg_buf;
    $self->{_sock}->recv($msg_buf, $self->max_packet_size());
    my $resp = uRPC::Message::Response->new(Raw => $msg_buf);
    return $resp;
}

sub connect {
    my $self = shift;
    my ($host, $port) = @_;

    die "No host specified!"
        unless $host;

    die "No port specified!"
        unless $port;

    $self->{_host} = $host;
    $self->{_port} = $port;

    $self->{_sock} = IO::Socket::INET->new(
        PeerHost => $host,
        PeerPort => $port,
        Proto => 'udp',
    ) or die "Could not connect to '$host:$port': $@";
}

sub register {
    my $self = shift;

    my $response = $self->request(0, 'register'); #TODO complex payload
    $self->{_session_id} = $response->payload(); #TODO complex payload

    return $response;
}

1;
