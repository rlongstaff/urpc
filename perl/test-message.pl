#!/usr/bin/perl

use warnings;
use strict;

use uRPC::Message::Request;
use uRPC::Message::Response;

main();

sub main {
    test_serialize();
    test_deserialize();
    test_serialize_deserialize();
}

sub test_serialize {
    my $req = uRPC::Message::Request->new();
    $req->request_id(255);
    $req->rpc(1);
    $req->payload('a');
    my $buf = join(' ', unpack('H4 H2 H2 H2 H4 H2 H*', $req->serialize()));
    my $expected = 'ff00 01 00 00 0100 4e 61';
    if ($buf eq $expected) {
        print "PASS\n";
        return 1;
    }
    print "FAIL!\n";
    print "got      : $buf\n";
    print "expected : $expected\n";
    return undef;
}

sub test_deserialize {
    my $resp = uRPC::Message::Response->new();
    my $raw = 'ff0001000001004e61';
    my $buf = pack('H*', $raw);
    $resp->deserialize($buf);

    unless ($resp->is_error()) {
        print "PASS\n";
        return 1;
    }
    print "FAIL!\n";
    return undef;
}

sub test_serialize_deserialize {
    my $req = uRPC::Message::Request->new();
    $req->request_id(255);
    $req->rpc(1);
    $req->payload('a');

    my $resp = uRPC::Message::Request->new();
    $resp->deserialize($req->serialize);
    unless ($resp->is_error()) {
        print "PASS\n";
        return 1;
    }
    print "FAIL!\n";
    return undef;
}
