
use warnings;
use strict;

use Socket qw(sockaddr_in);
use IO::Socket::INET;
use uRPC::Message::Request;
use uRPC::Message::Response;

package uRPC::Server;
use parent 'uRPC::Base';

my $DEFAULT_MAX_PACKET_SIZE = 64;

sub _init {
    my $self = shift;
    my %args = @_;

    $self->{_listen_addr} = $args{ListenAddr};
    $self->{_listen_port} = $args{ListenPort};
    $self->{_max_packet_size} = $args{MaxPacketSize};
    unless ($self->{_max_packet_size}) {
        $self->{_max_packet_size} = $DEFAULT_MAX_PACKET_SIZE;
    }

    return $self->SUPER::_init(@_);
}

sub listen_addr {
    my $self = shift;
    return $self->{_listen_addr};
}

sub listen_port {
    my $self = shift;
    return $self->{_listen_port};
}

sub max_packet_size {
    my $self = shift;
    return $self->{_max_packet_size};
}

sub rpc_register {
    my $self = shift;
    my ($req) = @_;

    my $res = uRPC::Message::Response->new();
    $res->payload(int(rand(65536)));
    return $res;
}

sub handle_rpc { }

sub run {
    my $self = shift;

    my $sock = IO::Socket::INET->new(
        LocalAddr => $self->listen_addr(),
        LocalPort => $self->listen_port(),
        Proto => 'udp',
        ReusePort => 1,
    ) or die "Could not listen on '" .
        $self->listen_addr() . ":" .  $self->listen_port() . "': $@";

    my $msg_buf;
    while ($sock->recv($msg_buf, $self->max_packet_size())) {
        my($port, $host) = Socket::sockaddr_in($sock->peername);
        print "Packet from $host:$port\n";
        my $req = uRPC::Message::Request->new(Raw => $msg_buf);
        print "received: " . $req->payload() . "\n";
        my $res;
        if ($req->rpc() == 0) {
            $res = $self->rpc_register($req);
        } else {
            $res = $self->handle_rpc($req);
        }
        $sock->send($res->serialize());
    }
}

1;
