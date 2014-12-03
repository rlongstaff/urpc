#!/usr/bin/perl

use warnings;
use strict;

use uRPC::Server;

my $server = uRPC::Server->new();

$server->run();

