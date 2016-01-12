#!/usr/bin/perl

use warnings;
use strict;

use Socket qw(sockaddr_in);
use IO::Socket::INET;
use uRPC::Message::Request;
use uRPC::Message::Response;

my $DEFAULT_MAX_PACKET_SIZE = 64;

run();

sub run {
    my $sock = IO::Socket::INET->new(
        LocalAddr => '127.0.0.1',
        LocalPort => 31415,
        Proto => 'udp',
        ReusePort => 1,
    ) or die "Could not listen $@";

    my $msg_buf;
    while ($sock->recv($msg_buf, $DEFAULT_MAX_PACKET_SIZE)) {
        my($port, $host) = Socket::sockaddr_in($sock->peername);
        print "Packet from $host:$port\n";
        print "received: $msg_buf\n";
    }
}

1;
