
use warnings;
use strict;

use Socket qw(sockaddr_in);
use IO::Socket::INET;
use uRPC::Message::Request;
use uRPC::Message::Response;

package uRPC::Server::Echo;
use parent 'uRPC::Server';

sub handle_rpc {
    my $self = shift;
    my ($req) = @_;

    return $self->rpc_echo($req);
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

1;
