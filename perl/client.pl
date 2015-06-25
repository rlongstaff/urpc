#!/usr/bin/perl

use warnings;
use strict;

use uRPC::Client;

my $client = uRPC::Client->new();

my $host = '127.0.0.1';
my $port = 31415;
$client->connect($host, $port);

my $response = $client->register();

if ($response->is_error()) {
    print "failed to register: " . $response->payload() . "\n";
}

my $session = $client->session_id();

if ($session) {
    print "Registered with session $session\n";
}

$response = $client->request(1, "foo");
if ($response) {
    print "Payload: " . $response->payload() . "\n";
}

