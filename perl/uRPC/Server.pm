
use warnings;
use strict;

use Socket qw(sockaddr_in);
use IO::Socket::INET;
use uRPC::Message::Request;
use uRPC::Message::Response;

package uRPC::Server;
use parent 'uRPC::Base';

my $LISTEN_ADDR = '127.0.0.1';
my $LISTEN_PORT = 31415;
my $MAX_PACKET_SIZE = 64; #TODO location?

sub _init {
    my $self = shift;
    my %args = @_;

    return $self->SUPER::_init(@_);
}

sub handle_rpc {
    my $self = shift;
    my ($req) = @_;

    return rpc_echo($req);
}

sub rpc_register {
    my $self = shift;
    my ($req) = @_;

    my $res = uRPC::Message::Response->new();
    $res->payload(int(rand(65536)));
    return $res;
}

sub rpc_echo {
    my $self = shift;
    my ($req) = @_;

    my $res = uRPC::Message::Response->new();
    unless ($req) {
        $res->error("bad request");
    } else {
        $res->payload($req->payload());
    }
    return $res;
 }

sub run {
    my $self = shift;

    my $sock = IO::Socket::INET->new(
        LocalAddr => $LISTEN_ADDR,
        LocalPort => $LISTEN_PORT,
        Proto => 'udp',
        ReusePort => 1,
    ) or die "Could not listen on '$LISTEN_ADDR:$LISTEN_PORT': $@";

    my $msg_buf;
    while ($sock->recv($msg_buf, $MAX_PACKET_SIZE)) {
        my($port, $host) = Socket::sockaddr_in($sock->peername);
        print "Packet from $host:$port\n";
        my $req = uRPC::Message::Request->new(Raw => $msg_buf);
        print "received: " . $req->payload() . "\n";
        my $res;
        if ($req->rpc() == 0) {
            $res = $self->rpc_register($req);
        } else {
            $res = $self->rpc_echo($req);
        }
        $sock->send($res->serialize());
    }
}

1;
