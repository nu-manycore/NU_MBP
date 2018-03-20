#!/usr/bin/env perl

use strict;
use warnings;
use Getopt::Std;

my $hz = 60 * 1000 * 1000;
my $max_load = 0x1fffffff;

my $base_rate = 0.001;

my %opts;

getopts ('b:c:l:', \%opts)
    || die "usage: $0 [-b base_rate] [-c hz] [-l max_load] [dumpfile]\n";

$base_rate = $opts{b} if defined $opts{b};
$hz = $opts{c} if defined $opts{c};
$max_load = $opts{l} if defined $opts{l};

my %tasks;

my $last_task;
my $last_count = 0;
my $current_counter = 0;
while (<>) {
    chomp;

    if (m/(\d+):(\d+):.*(Core(\d)_(\d+)_(\d))_Task/) {
	my ($e, $l, $name, $rate) = (int($1), int($2), $3, int($5));
	if (!defined $last_task || $last_task ne $name) {
	    $last_task = $name;
	    $last_count = 0;
	    $current_counter = 0;
	}
	$current_counter += $max_load + 1 if $e < $last_count;
	$last_count = $e;
	$current_counter += $max_load + 1 if $l < $last_count;
	$last_count = $l;

	my $r = $tasks{$name};
	unless (defined $r) {
	    $r = [$rate, []];
	    $tasks{$name} = $r;
	}
	push @{$r->[1]}, [$e + $current_counter, $l + $current_counter, 0, 0];
    }
}

my $data = [];

foreach my $k (sort { $a cmp $b; } keys (%tasks)) {
    my ($rate, $v) = @{$tasks{$k}};
    push @{$data}, [$k, $rate, 0, $v];
}

my $run = 1;

my $last_time = 0;
$last_count = 0;
while ($run) {
    $run = 0;
    my $min_ent = -1;
    my $min_ent_id = -1;
    my $min_lv = -1;
    my $min_lv_id = -1;
    for (my $i = 0; $i <= $#{$data}; $i++) {
	my $c = $data->[$i]->[2];
	my $r = $data->[$i]->[3];

	# もうない
	next if $c > $#{$r};

	my $ent;
	$ent = $r->[$c]->[0] if $r->[$c]->[2] == 0;
	my $lv;
	$lv = $r->[$c]->[1] if $r->[$c]->[3] == 0;
	if (defined $ent) {
	    if ($min_ent == -1 || $ent < $min_ent) {
		$min_ent = $ent;
		$min_ent_id = $i;
	    }
	}
	if (defined $lv) {
	    if ($min_lv == -1 || $lv < $min_lv) {
		$min_lv = $lv;
		$min_lv_id = $i;
	    }
	}
	$run = 1;
    }

    last if $run == 0;

    # print "defined ent:$min_ent_id:$min_ent\n" if defined $min_ent;
    # print "defined lv:$min_lv_id:$min_lv\n" if defined $min_lv;

    my $mode;
    if ($min_ent != -1 && $min_lv != -1) {
	if ($min_ent <= $min_lv) {
	    # print "select ent: $min_ent\n";
	    $mode = 0;
	} else {
	    # print "select lv: $min_lv\n";
	    $mode = 1;
	}
    } elsif ($min_ent != -1) {
	# print "select ent: $min_ent\n";
	$mode = 0;
    } elsif ($min_lv != -1) {
	# print "select lv: $min_lv\n";
	$mode = 1;
    } else {
	die "ERROR";
    }

    my $current_count = $mode ? $min_lv : $min_ent;
    my $ind = $mode ? $min_lv_id : $min_ent_id;

    my $elapsed;
    $elapsed = $current_count - $last_count;

    $last_time += $elapsed / $hz;

    $last_count = $current_count;

    my $d = $data->[$ind];
    my $name = $d->[0];
    my $rate = $d->[1];
    my $cnt = $d->[2];
    my $ttime = $d->[3]->[$cnt];

    my $end = '';
    if ($cnt == $#{$d->[3]}) {
	my $x = $cnt + 1;
	$end = " [[-- END(${x}) --]]";
    }

    if ($mode) {
	my $delta = ($ttime->[1] - $ttime->[0]) / $hz;
	printf "- %11.6f %11.6f %s%s\n", $last_time, $delta, $name, $end;
	$d->[2] = $cnt + 1;
	$ttime->[3] = 1;
    } else {
	my $elapsed;
	my $delta;
	if ($cnt == 0) {
	    $elapsed = 0;
	    $delta = 0;
	} else {
	    $elapsed = ($d->[3]->[$cnt]->[0]
			- $d->[3]->[$cnt - 1]->[0]) / $hz;
	    $delta = ($elapsed / $base_rate - $rate) / $rate * 100;
	}

	printf "+ %11.6f %11.6f %s (%8.3f%%)%s\n",
		$last_time, $elapsed, $name, $delta, $end;
	$ttime->[2] = 1;
    }
}

exit 0;

__END__

=pod

=encoding utf8

=head1 NAME

tracecheck.pl -- 出力された実行トレースを読み込み、時系列に表示する

=head1 SYNOPSIS

tracecheck.pl [-b base_rate] [-c hz] [-l max_load] [dumpfile]

=head1 DESCRIPTION

本コマンドは、
B<abrexyaml2pthread.pl>が生成した実行トレース用ヘッダファイルを用いて、
取得・表示した実行トレースの情報を読み込み、
それらを時系列に並べ、タスクの周期と実行時刻の差異を表示します。

=head1 OPTIONS

=over 4

=item B<-b> I<base_rate>

タスクの周期のベースレートを秒で指定します。Z<>E<0x0A>
デフォルトはB<0.001(1ms)>です。

実行トレースの表示では、タスクは、次のような識別子となっています。

B<モデル名>_CoreZ<>B<コア番号>_Z<>B<周期>_Z<>B<オフセット>_TASK

コマンドでは、この識別子から抽出した、B<周期>、B<オフセット>を
I<base_rate>を用いて、実時間の周期とオフセットを変換します。

=item B<-c> I<hz>

実行トレースの計時に用いたB<MAIN_HW_COUNTER_CORE0>の周波数を指定します。
デフォルトはB<60000000(60MHz)>です。

実行トレースの時刻はTOPPERS/ATK2のB<GetCounterValue()>で取得した、
B<MAIN_HW_COUNTER_CORE0>
の値を使用します。
このB<MAIN_HW_COUNTER_CORE0>の周波数を指定してください。

=item B<-l> I<max_load>

実行トレースの計時に用いたB<MAIN_HW_COUNTER_CORE0>
の最大カウント値を指定します。
デフォルトではB<536870911(0x1fffffff)>です。

実行トレースの時刻はB<MAIN_HW_COUNTER_CORE0>の値ですが、
カウンタは、最大カウント値の次はB<0>に戻ります。
このため、B<0>に戻ったカウント値の時刻を補正するために、
最大カウント値をコマンドに与える必要があります。

=item F<dumpfile>

入力ファイルを指定します。省略時は標準入力になります。

実行トレースの情報が出力されているファイルを指定してください。

=back

=head1 表示の説明

=over 4

=item 周期動作開始行

行頭がB<+>の場合、周期動作を開始した事を示します。

I<S<+    1.000455    1.000297 Core1_1000_0 (   0.030%)>>

続くフィールドの意味は次の通りです。

=over 4

=item 1. B<1.000455>

このフィールドは、開始した時刻です。
単位は秒です。

=item 2. B<1.000297>

このフィールドは前回開始した時刻からの差異です。
単位は秒で、この期待値は周期です。

=item 3. B<Core1_1000_0>

このフィールドは、タスクの情報です。
この例では、コア番号1、周期1000、開始オフセット0のタスクを示します。

=item 4. B<S<(   0.030%)>>

このフィールドは、周期からのずれを示します。
単位はパーセンテージです。

=back

=item 周期動作終了行

行頭がB<->の場合、周期動作を終了した事を示します。

I<S<-    1.000466    0.000011 Core1_1000_0>>

続くフィールドの意味は次の通りです。

=over 4

=item 1. B<1.000466>

このフィールドは、終了した時刻です。
単位は秒です。

=item 2. B<0.000011>

このフィールドは、開始した時刻からの差異です。
この回の周期動作に要した時間で、単位は秒で、期待値は、周期未満です。

=item 3. B<Core1_1000_0>

このフィールドは、タスクの情報です。
この例では、コア番号1、周期1000、開始オフセット0のタスクを示します。

=back

=item レコード数

タスク毎の実行トレースの最終レコードに対しては、
行末に以下のように表示します。

I<S<[[-- END(8)--]]>>

=over 4

=item B<8>

この例では、B<8>個のレコードを表示した事を示します。

=back

=back

=cut
