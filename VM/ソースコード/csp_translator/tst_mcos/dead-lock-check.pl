#!/usr/bin/perl -w

my %channel;

while (<>) {
    chomp;

    my $type;
    my $ch;

    if (/\d+:\s*((IN|OUT)_\d+):\s*wait/) {
	$ch = $1;
	$type = 1;
    } elsif (/\d+:\s*((IN|OUT)_\d+):\s*end/) {
	$ch = $1;
	$type = 0;
    } elsif (/\d+:\s*((IN|OUT)_\d+):\s*send/) {
	$ch = $1;
	$type = 2;
    } else {
	next;
    }
    unless (defined $channel{$ch}) {
	$channel{$ch} = [0, "", []];
    }
    if ($type == 0) {
	$channel{$ch}->[0] = 0;
	$channel{$ch}->[1] = "";
	$channel{$ch}->[2] = [];
    } elsif ($type == 1) {
	$channel{$ch}->[0] = 1;
	$channel{$ch}->[1] = $_;
    } else {
	push (@{$channel{$ch}->[2]}, $_);
    }
}

foreach my $ch (keys %channel) {
    next if $channel{$ch}->[0] == 0;
    print $channel{$ch}->[1], "\n";
    foreach my $s (@{$channel{$ch}->[2]}) {
	print "  ", $s, "\n";
    }
}
