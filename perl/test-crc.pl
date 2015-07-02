#!/usr/bin/perl

use warnings;
use strict;

use Digest::CRC qw(crc8);
use Time::HiRes qw(gettimeofday tv_interval);

my $MSG = 'abcdefghijklmnopqrstuvwxyz';
my $POLY = 0x07;
#my $POLY = 0x9c;

#printf("msg:  %s\nPOLY: %X\ncrc:  %X\n", $MSG, $POLY, crc8($MSG, 1, $POLY));
#print "CRC : " . my_crc8($MSG, length($MSG), $POLY) . "\n";
#print "DCRC: " . crc8($MSG) . "\n";

my %funcz = (
    'Digest::CRC' => \&crc8,
    'my_crc8'     => \&my_crc8,
    'crc8_str'    => \&crc8_str,
);

my $iter = 10_000;
my ($t0, $elapsed);

foreach my $f (sort keys %funcz) {
    $t0 = [gettimeofday];
    for (1 .. $iter) {
        &{$funcz{$f}}($MSG);
    }
    $elapsed = tv_interval($t0, [gettimeofday]);
    print "$f: $elapsed\n";
}
$t0 = [gettimeofday];
for (1 .. $iter) {
    #crc8_opt(join('', map{ord} split(//,$MSG)));
    crc8_opt($MSG);
}
$elapsed = tv_interval($t0, [gettimeofday]);
print "crc_opt: $elapsed\n";
print crc8_opt($MSG) . "\n";

sub my_crc8 {
    my ($msg) = @_;

    my $crc = 0;
    my $p = 0;
    my $len = length $msg;
    while ($p < $len) {
        my $byte = ord(substr($msg, $p++, 1));
        $crc ^= $byte;
        for my $i (0..7) {
            if ($crc & 0x80) {
                $crc = ($crc << 1) & 0xFF;
                $crc ^= $POLY;
            } else {
                $crc = ($crc << 1) & 0xFF;
            }
        }
    }
    return $crc;
}

sub crc8_str {
    my ($msg) = @_;
    return crc8_opt(join('', map {ord} split(//, $msg)));
}

sub crc8_opt {
    my ($msg) = @_;

    my $crc = 0;
    foreach my $byte (unpack('C*', $msg)) {
        $crc ^= $byte;
        for my $i (0..7) {
            if ($crc & 0x80) {
                $crc = ($crc << 1) & 0xFF;
                $crc ^= $POLY;
            } else {
                $crc = ($crc << 1) & 0xFF;
            }
        }
    }
    return $crc;
}
