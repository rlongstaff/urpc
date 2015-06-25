#!/usr/bin/perl

use warnings;
use strict;

use uRPC::Server::Echo;

my $server = uRPC::Server::Echo->new(
    ListenAddr => '127.0.0.1',
    ListenPort => 31415,
);

$server->run();

