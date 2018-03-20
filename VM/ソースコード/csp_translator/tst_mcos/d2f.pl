#/usr/bin/perl -w

use FileHandle;
use Getopt::Std;

my %opts;

sub usage {
    die "usage: $0 file1 file2";
}

getopts('n:r:', \%opts) || usage;

usage if $#ARGV < 1;

my $fa = new FileHandle $ARGV[0];
die "$ARGV[0]: Couldn't open: $!" unless defined $fa;

my $fb = new FileHandle $ARGV[1];
die "$ARGV[1]: Couldn't open: $!" unless defined $fb;

#my $d = 0.0000000001;
#my $d = 0.001;
my $d = 0.005 / 100;

my $maxdiff;
my $rate = 1;
my $diffs = 0;

$maxdiff = $opts{n} + 0 if defined $opts{n};
$rate = $opts{r} + 0 if defined $opts{r};

my $n = 1;
my $r = 0;

while (1) {
    my $da = <$fa>;

    if ($rate > 1 && $r++) {
	$r = 0 if $r == $rate;
	next;
    }

    my $db = <$fb>;

    if (defined $da && defined $db) {
	chomp $da;
	chomp $db;

	$da =~ s/\r//;
	$db =~ s/\r//;

	my @aa = split(',', $da);
	my @ab = split(',', $db);

	if ($#aa != $#ab) {
	    printf ("%s:%d:%d[col] != %s:%d:%d[col]\n",
		    $ARGV[0], $n, $#aa + 1, $ARGV[1], $n, $#ab + 1);
	    exit (1);
	}
	my $m = 0;
	while ($#aa >= 0) {
	    my $va = shift (@aa);
	    my $vb = shift (@ab);

	    if ($va eq 'true') {
		$va = 1;
	    } elsif ($va eq 'false') {
		$va = 0;
	    }

	    $va += 0;
	    $vb += 0;

	    my $delta = $va - $vb;
	    my $div = $va;
	    $div = 1 if $div == 0;
	    $delta = abs ($delta) / $div;

	    unless ($delta <= $d) {
		$delta *= 100;
		print "$n:$m: $va, $vb, (${delta}%)\n";
		if (defined $maxdiff) {
		    if (++$diffs >= $maxdiff) {
			exit(1);
		    }
		}
	    }

	    $m++;
	}
    } else {
	if (defined $da && !defined $db) {
	    print "$ARGV[1]: EOF, $n\n";
	} elsif (!defined $da && defined $db) {
	    print "$ARGV[0]: EOF, $n\n";
	}
	last;
    }
    $n++;
}
