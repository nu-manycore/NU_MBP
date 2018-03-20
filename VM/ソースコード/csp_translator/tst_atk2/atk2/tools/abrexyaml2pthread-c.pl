#!/usr/bin/env perl
# -*- coding: utf-8-unix; -*-

use strict;
use warnings;
use utf8;

use FileHandle;
use POSIX;
use Getopt::Std;
use YAML qw(LoadFile Load);
use YAML::Dumper;
use File::Basename;

sub usage {
    die "usage: $0 [-t trace.h] [-s tick size] [abrex.yaml]\n",
    "  -n: no output Os_Lcfg.[ch]\n",
    "  -t: output header file[default testout_trace.h]\n",
    "  -s: trace ticks size[default 64]\n";
}

my %opts;

my $abrex_Ecuc;
my $abrex_Os;

my $prefix = "OS_WRAPPER__";

my $cfg_h = "Os_Lcfg.h";
my $cfg_c = "Os_Lcfg.c";
my $trace_h;
my $trace_ticks = 64;

getopts ('nt:s:', \%opts) || usage ();

$trace_h = $opts{t} if defined $opts{t};
$trace_ticks = $opts{s} if defined $opts{s};

my ($ostype) = POSIX::uname ();

my $any_task_type = "OS_WRAPPER(_ANY_TASK_TYPE)";

my $task_type = "OS_WRAPPER(_TASK_TYPE)";
my $core_task_type = "OS_WRAPPER(_CORE_TASK_TYPE)";
my $alarm_type = "OS_WRAPPER(_ALARM_TYPE)";
my $appmode_type = "OS_WRAPPER(_APPMODE_TYPE)";
my $osapp_type = "OS_WRAPPER(_OSAPP_TYPE)";
my $counter_type = "OS_WRAPPER(_COUNTER_TYPE)";

my $task_resource = "OS_WRAPPER(_task_resource)";
my $core_assign = "OS_WRAPPER(_core_assign)";
my $alarm_list = "OS_WRAPPER(_alarm_list)";
my $appmode_list = "OS_WRAPPER(_appmode_list)";
my $counter_list = "OS_WRAPPER(_counter_list)";

my $task_function_prefix = "${prefix}_taskmain_";
my $alarm_notify_prefix = "${prefix}_alarm_notify_";

my $hashref;			# YAML file

my $numberofcores;		# Number of Cores

my %task;			# YAML task list
my %event;			# YAML event list
my %alarm;			# YAML alarm list
my %appmode;			# YAML appmode list
my %osap;			# YAML osap list
my %osisr;			# YAML osisr list
my %oscounter;			# YAML oscounter list;

my @gen_event;			# generated event list
my @gen_task;			# generated task list
my @gen_alarm;			# generated alaram list
my @gen_counter;		# generate counter list;

my %gen_hevent;			# generated event hash
my %gen_htask;			# generated task hash
my %gen_halarm;			# generated alarm hash
my %gen_hcounter;		# generated counter hash

my @core_info;			# core assigned task

my $oshooks;

my @hw_def;
my %hw_hdef;

# H/W Counter

my $timer_clock_hz = {
    name => 'TIMER_CLOCK_HZ',
    comm => 'Timer Tick',
    def => 60000000,
};

push @hw_def, $timer_clock_hz;
$hw_hdef{$timer_clock_hz->{name}} = $timer_clock_hz;

my @hw_counter = (
    {
	name => 'MAIN_HW_COUNTER_CORE0',
	id => 0,
	timer_clock => $timer_clock_hz->{name},
	nsec_mul => 50,		# C[Tick] * 50 / 3 => C'[ns]
	nsec_div => 3,
	max_load => 0x1fffffff,
	min_cycle => 4000,
	ticks_per_base => 10,
	hw_type => 1,
    },
    {
	name => 'MAIN_HW_COUNTER_CORE1',
	id => 1,
	timer_clock => $timer_clock_hz->{name},
	nsec_mul => 50,		# C[Tick] * 50 / 3 => C'[ns]
	nsec_div => 3,
	max_load => 0x1fffffff,
	min_cycle => 4000,
	ticks_per_base => 10,
	hw_type => 1,
    }
    );

push @gen_counter, @hw_counter;
foreach my $c (@hw_counter) { $gen_hcounter{$c->{name}} = $c; }

sub getdir {
    my ($hash, $ent) = @_;

    my @path = split ('/', $ent);
    shift @path;

    my $last;
    my $x = $hash;
    foreach my $e (@path) {
	$x = $x->{$e};
	die "Unknown $e: $ent" unless defined $x;
	$last = $e;
    }
    ($last, $x);
}

my $any_this = "OS_WRAPPER(__any_ctx)";
my $any_this_type = "${any_task_type}";
my $any_this_decl = "${any_this_type} *${any_this}";

sub gen_config_header {
    my $cfg_h_fh = new FileHandle $cfg_h, 'w';
    die "Couldn't open Os.h: $!" unless defined $cfg_h_fh;

    my $n;

    my $hdef = $cfg_h;
    $hdef =~ s/\W/_/g;

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_h_fh->print (<<"EOF"
#ifndef ${hdef}
#define ${hdef}

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#ifndef OS_WRAPPER
#define OS_WRAPPER(X) ${prefix}##X
#endif

#define ${prefix}__MKSTR(X)\t#X
#define ${prefix}_MKSTR(X)\t${prefix}__MKSTR(X)

#define TASK(N)\tvoid ${task_function_prefix}##N(TASK__DECL)
#define TASK_FNAME(N)\t${task_function_prefix}##N
#define ALARM_FUNC(N)\tvoid ${alarm_notify_prefix}##N(union sigval sv)
#define ALARM_FNAME(N)\t${alarm_notify_prefix}##N

#define TASK__TYPE\t${any_task_type}
#define TASK__CTX\t${any_this}
#define TASK__DECL\tTASK__TYPE \*TASK__CTX
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    # ヘッダファイル
    my $osinc = $abrex_Os->{OsInclude};
    if (defined $osinc) {
	my $fn = $osinc->{OsIncludeFileName};
	if (defined $fn) {
	    $cfg_h_fh->print ("\n");
	    if (ref $fn eq 'ARRAY') {
		foreach my $f (@{$fn}) {
		    $cfg_h_fh->print ("#include \"${f}\"\n");
		}
	    } else {
		$cfg_h_fh->print ("#include \"${fn}\"\n");
	    }
	}
    }

    if (defined $oshooks) {
	$cfg_h_fh->print ("\n");
	while (my ($k, $v) = each %{$oshooks}) {
	    next unless $v eq 'true';
	    $k =~ m/^Os(.*)/;
	    my $h = $k;
	    $h =~ tr/a-z/A-Z/;
	    $cfg_h_fh->print ("#define CFG_USE_", $h, ' 1', "\n");
	}
    }

    # H/W define
    foreach my $h (@hw_def) {
	my $n = $h->{name};
	my $c = $h->{comm};
	my $d = $h->{def};
	$cfg_h_fh->print ("\n/* ${c} */\n#define ${n}\t${d}\n");
    }

    my $tnum_task = $#gen_task + 1;
    my $tnum_alarm = $#gen_alarm + 1;
    my $tnum_hwcounter = $#hw_counter + 1;
    my $tnum_counter = $#gen_counter + 1;

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_h_fh->print (<<"EOF"

/* Master Core */
#define OS_CORE_ID_MASTER\t0

#define TNUM_HWCORE\t${numberofcores}
#define TNUM_TASK\t${tnum_task}
#define TNUM_ALARM\t${tnum_alarm}
#define TNUM_HWCOUNTER\t${tnum_hwcounter}
#define TNUM_COUNTER\t${tnum_counter}
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    $n = 0;
    $cfg_h_fh->print ("\n", '/* AppMode */', "\n");
    foreach my $k (sort keys %appmode) {
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_h_fh->print (<<"EOF"
#define THIS__APPMODE_${n}\t${k}
#define ${k}\t${n}
#define StartupHook_${k}(VOID)\t\\
  OS_WRAPPER(StartupHook_${k})(TASK__DECL)
#define ShutdownHook_${k}(E)\t\\
  OS_WRAPPER(ShutdownHook_${k})(TASK__DECL,E)
#define ErrorHook_${k}(E)\t\\
  OS_WRAPPER(ErrorHook_${k})(TASK__DECL,E)
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
	$n++;
    }

    $cfg_h_fh->print ("\n", '/* TaskID */', "\n");
    foreach my $v (@gen_task) {
	$cfg_h_fh->print ('#define ', $v->{name}, "\t", $v->{id}, "\n");
    }

    $cfg_h_fh->print ("\n", '/* EventID */', "\n");
    foreach my $v (@gen_event) {
	$cfg_h_fh->print ('#define ', $v->{name}, "\t(1U<<", $v->{id}, ")\n");
    }

    $cfg_h_fh->print ("\n", '/* AlarmID */', "\n");
    foreach my $v (@gen_alarm) {
	$cfg_h_fh->print ('#define ', $v->{name}, "\t", $v->{id}, "\n");
    }

    $cfg_h_fh->print ("\n/* CounterID */\n");
    foreach my $v (@gen_counter) {
	$cfg_h_fh->print ('#define ', $v->{name}, "\t", $v->{id}, "\n");
    }

    foreach my $v (@gen_counter) {
	my $n = $v->{name};
	my $maxload = $v->{max_load};
	my $ticksperbase = $v->{ticks_per_base};
	my $mincycle = $v->{min_cycle};
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_h_fh->print (<<"EOF"

#define OSMAXALLOWEDVALUE_${n}\t${maxload}
#define OSTICKSPERBASE_${n}\t${ticksperbase}
#define OSMINCYCLE_${n}\t${mincycle}
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_h_fh->print (<<"EOF"

/* structure type id */
typedef enum \{
  OS_WRAPPER(_STRUCTID_TASK),
  OS_WRAPPER(_STRUCTID_CORE)
\} OS_WRAPPER(_structId);

/* Any Task Type */
typedef struct \{
  const OS_WRAPPER(_structId) sid;
\} ${any_task_type};

/* define task functions */
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $t (@gen_task) {
	$cfg_h_fh->print ("TASK(", $t->{name}, ");\n");
    }

    foreach my $k (sort keys %appmode) {
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_h_fh->print (<<"EOF"

/* Application hooks */
void __attribute__ ((weak))
  OS_WRAPPER(StartupHook_${k})(TASK__DECL);
void __attribute__ ((weak))
  OS_WRAPPER(ShutdownHook_${k})(TASK__DECL, StatusType Error);
void __attribute__ ((weak))
  OS_WRAPPER(ErrorHook_${k})(TASK__DECL, StatusType Error);
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    $cfg_h_fh->print ("\n#endif /* ${hdef} */\n");
    $cfg_h_fh->close;
}


sub gen_config_source {
    my $cfg_c_fh = new FileHandle $cfg_c, 'w';
    die "Couldn't open Os_Lcfg.c: $!" unless defined $cfg_c_fh;

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_c_fh->print (<<"EOF"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "Os.h"
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $t (@gen_task) {
	my $n = $t->{name};
	my $am = $t->{appmode};

	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"

/* AppMode ${n} */
static int OS_WRAPPER(_task_${n}_appmode)[] = \{
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

	foreach my $a (@{$am}) {
	    $cfg_c_fh->print ("\t${a},\n");
	}
	$cfg_c_fh->print ("};\n");
    }

    # タスクリストの作成
    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_c_fh->print (<<"EOF"

/* Task Resources */
${task_type} ${task_resource}[TNUM_TASK] = \{
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $t (@gen_task) {
	my $n = $t->{name};
	my $c = $t->{core};
	my $v = $t->{config};
	my $e = $v->{OsTaskEventRef};
	my $mask = 0;
	foreach my $m (@{$e}) {
	    my ($en, $ev) = getdir ($hashref, $m);
	    my $ge = $gen_hevent{$en};
	    $mask |= (1 << $ge->{id});
	}

	my $napp = $#{$t->{appmode}} + 1;
	my $coreid = $t->{core};
	my $pri = $t->{priority};
	my $func = $t->{name};
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
  [${n}] = \{
    .sid = OS_WRAPPER(_STRUCTID_TASK),
    .name = "${n}",
    .tid = ${n},
    .appmode = OS_WRAPPER(_task_${n}_appmode),
    .napp = ${napp}, .coreid = ${coreid},
    .pri = ${pri},
    .mask = $mask, .event = 0, .running = 0,
    .func = TASK_FNAME(${func}),
  \},
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    $cfg_c_fh->print ("};\n");

    # AppMode の生成

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_c_fh->print (<<"EOF"

/* AppMode Type */
${appmode_type} ${appmode_list}[] = \{
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    foreach my $k (sort keys %appmode) {
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
  \{
    .name = "${k}",
    .apid = ${k},
    .startuphook = OS_WRAPPER(StartupHook_${k}),
    .errorhook = OS_WRAPPER(ErrorHook_${k}),
    .shutdownhook = OS_WRAPPER(ShutdownHook_${k}),
  \},
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    $cfg_c_fh->print ("};\n");

    # コアのAppModeのリスト作成
    for (my $c = 0; $c < $numberofcores; $c++) {
	my $core = $core_info[$c];
	next unless defined $core;
	my $av = $core->{appmode};
	next if $#{$av} == -1;

	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"

/* Core#${c} OsApp List */
static ${osapp_type} OS_WRAPPER(_core${c}_osapp_list)[] = \{
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
	foreach my $a (@{$av}) {
	    my $n = $a->{name};
	    my $erh = $a->{err_hook};
	    my $sdh = $a->{shd_hook};
	    my $sth = $a->{stu_hook};
	    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	    $cfg_c_fh->print (<<"EOF"
  \{
    .appmode = \&${appmode_list}\[${n}\],
    .startup = ${sth}, .error = ${erh}, .shutdown = ${sdh},
  \}
EOF
		);
	    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
	}

	$cfg_c_fh->print ("};\n");
    }

    # タスクのコア割り当て
    for (my $c = 0; $c < $numberofcores; $c++) {
	my $tl = $core_info[$c]->{task};

	next unless defined $tl;

	$cfg_c_fh->print (
	    "\nstatic ${task_type} \*OS_WRAPPER(_core${c}_tasklist)[] = {\n");

	foreach my $t (@{$tl}) {
	    $cfg_c_fh->print (
		'  &', ${task_resource}, '[', $t->{id}, '],', "\n");
	}

	$cfg_c_fh->print ("};\n");
    }

    $cfg_c_fh->print ("\n${core_task_type} ${core_assign}[] = {\n");
    for (my $c = 0; $c < $numberofcores; $c++) {
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
  \{
    .sid = OS_WRAPPER(_STRUCTID_CORE),
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

	my $tl = $core_info[$c]->{task};
	if (defined $tl) {
	    my $ntask = $#{$tl} + 1;
	    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	    $cfg_c_fh->print (<<"EOF"
    .tasklist = OS_WRAPPER(_core${c}_tasklist), .ntask = ${ntask},
EOF
		);
	    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
	} else {
	    $cfg_c_fh->print ("    .tasklist = NULL, .ntask = 0,\n");
	}

	my $av = $core_info[$c]->{appmode};
	if (defined $av and $#{$av} > -1) {
	    my $nosapp = $#{$av} + 1;
	    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	    $cfg_c_fh->print (<<"EOF"
    .osapplist = OS_WRAPPER(_core${c}_osapp_list), .nosapp = ${nosapp},
EOF
		);
	    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
	} else {
	    $cfg_c_fh->print ("    .osapplist = NULL, .nosapp = 0,\n");
	}

	my $herror = 0;
	my $hposttask = 0;
	my $hpretask = 0;
	my $hprotection = 0;
	my $hshutdown = 0;
	my $hstartup = 0;
	my $hapstartup = 0;
	my $hapshutdown = 0;
	my $haperror = 0;
	if (defined $oshooks) {
	    while (my ($k, $v) = each %{$oshooks}) {
		$herror = 1 if $k eq 'OsErrorHook' and $v eq 'true';
		$hposttask = 1 if $k eq 'OsPostTaskHook' and $v eq 'true';
		$hpretask = 1 if $k eq 'OsPreTaskHook' and $v eq 'true';
		$hprotection = 1 if $k eq 'OsProtectionHook' and $v eq 'true';
		$hshutdown = 1 if $k eq 'OsShutdownHook' and $v eq 'true';
		$hstartup = 1 if $k eq 'OsStartupHook' and $v eq 'true';
	    }
	}

	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
    .err = ${herror}, .post = ${hposttask}, .pre = ${hpretask}, .prot = ${hprotection}, .shtd = ${hshutdown}, .stup = ${hstartup},
    .enable = 0,
  \},
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }
    $cfg_c_fh->print ("};\n");

    # カウンタの生成

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_c_fh->print (<<"EOF"

/* Counter */
${counter_type} ${counter_list}[] = \{
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $c (@gen_counter) {
	my $name = $c->{name};
	my $timer_clock = $c->{timer_clock};
	my $nsec_mul = $c->{nsec_mul};
	my $nsec_div = $c->{nsec_div};
	my $min_cycle = $c->{min_cycle};
	my $ticks_per_base = $c->{ticks_per_base};
	my $hw_type = $c->{hw_type};
	my $max_load = $c->{max_load};
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
  \{
    .name = "${name}",
    .cid = ${name},
    .timer_clock = ${timer_clock},
    .nsec_mul = ${nsec_mul},
    .nsec_div = ${nsec_div},
    .max_load = ${max_load},
    .min_cycle = ${min_cycle},
    .ticks_per_base = ${ticks_per_base},
    .hw_type = ${hw_type},
  \},
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    $cfg_c_fh->print ("};\n");

    # タイマ関数の生成
    $cfg_c_fh->print ("\n/* Timer function */");
    foreach my $a (@gen_alarm) {
	my $an = $a->{name};
	my $v = $a->{config};
	my $act = $v->{OsAlarmAction};
	die "OsAlarmAction undefined at ", $an unless defined $act;
	my $se = $act->{OsAlarmSetEvent};
	die "OsAlarmSetEvent undefined at ", $an unless defined $se;
	my $t = $se->{OsAlarmSetEventTaskRef};
	die "OsAlarmSetEventTaskRef undefined at ", $an unless defined $t;
	my $e = $se->{OsAlarmSetEventRef};
	die "OsAlarmSetEventRef undefined at ", $an unless defined $e;

	my ($tn, $tv) = getdir ($hashref, $t);
	my ($en, $ev) = getdir ($hashref, $e);

	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"

static ALARM_FUNC($an)
\{
  ${any_task_type} \*ctx = (${any_task_type}*)sv.sival_ptr;
  OS_WRAPPER(SetEvent)(ctx,\n\t${tn}, ${en});
\}
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    # アラームリストの作成

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $cfg_c_fh->print (<<"EOF"

/* Alaram List */
${alarm_type} ${alarm_list}[] = \{
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $a (@gen_alarm) {
	my $an = $a->{name};
	my $cref = $a->{config}->{OsAlarmCounterRef};
	die "OsAlarmCounterRef undefined at ", $an unless defined $cref;
	my $cname = basename ($cref);
	die "Counter: $cref not found" unless defined $gen_hcounter{$cname};
	my $se = $a->{config}->{OsAlarmAction}->{OsAlarmSetEvent};
	my $t = $se->{OsAlarmSetEventTaskRef};
	my $e = $se->{OsAlarmSetEventRef};

	my $name = $a->{name};
	my $aid = $a->{id};
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$cfg_c_fh->print (<<"EOF"
  \{
    .name = "${name}",
    .aid = ${aid},
    .timer_event = \{
      .sigev_notify = SIGEV_THREAD,
      .sigev_notify_function = ALARM_FNAME(${an}),
      .sigev_value = \{
\t.sival_ptr = (void \*)\&${task_resource}\[${aid}\],
      \},
    \},
    .counter = \&${counter_list}\[${cname}\],
  \},
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    $cfg_c_fh->print ("};\n");
    $cfg_c_fh->close;
}

sub gen_trace_header {
    my $trace_h_fh = new FileHandle $trace_h, 'w';
    die "Couldn't open $trace_h: $!" unless defined $trace_h_fh;

    my $hdef = basename($trace_h);
    $hdef =~ s/\W/_/g;

    my $tticks = "OS_WRAPPER(_tticks)";
    my $trace_stop = "OS_WRAPPER(_trace_stop)";
    my $auto_stop = "OS_WRAPPER(_auto_stop)";
    my $ttick_counters = "OS_WRAPPER(_ttick_counts)";
    my $m_ttick_size = "${prefix}_TTICK_SIZE";
    my $m_enable_auto_stop = "${prefix}_ENABLE_TTICK_AUTO_STOP";
    my $m_not_def = "${prefix}_NOT_DEFINED";
    my $m_enable_task_tick = "${prefix}_ENABLE_TASK_TICK";

    my $task_resource = "OS_WRAPPER(_task_resource)";

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $trace_h_fh->print (<<"EOF"
#ifndef ${hdef}
#define ${hdef}

#ifndef ${m_enable_task_tick}
#define ${m_enable_task_tick}\t1
#endif

#if ${m_enable_task_tick}

#ifndef OS_WRAPPER
#define ${m_not_def}\t1
#define OS_WRAPPER(X) ${prefix}##X
#ifndef TASK__DECL
#define TASK__DECL\tvoid
#endif
#ifndef TASK__CTX
#define TASK__CTX
#endif
#endif

#ifndef ${m_ttick_size}
#define ${m_ttick_size}\t${trace_ticks}
#endif

#ifndef ${m_enable_auto_stop}
#ifdef ${m_not_def}
#define ${m_enable_auto_stop}\t1
#else
#define ${m_enable_auto_stop}\t0
#endif
#endif

static const int $auto_stop = ${m_enable_auto_stop};

static int ${trace_stop} = 0;

static TickType ${tticks}[TNUM_TASK][${m_ttick_size}][2];

static unsigned int ${ttick_counters}[TNUM_TASK];

#ifdef ${m_not_def}
static const struct \{
  const char *name;
\} ${task_resource}[TNUM_TASK] = \{
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $t (@gen_task) {
	my $n = $t->{name};
	$trace_h_fh->print ('  { "', $n, '", },', "\n");
    };

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $trace_h_fh->print (<<"EOF"
\};
#endif

int
OS_WRAPPER(_check_task_ticks_done) (void)
\{
  size_t i;
  for (i = 0; i < TNUM_TASK; i++) \{
    if (${ttick_counters}\[i\] != ${m_ttick_size})
      return 0;
  \}
  return 1;
\}

void
OS_WRAPPER(_dump_task_ticks) (TASK__DECL)
\{
  size_t i;
  for (i = 0; i < TNUM_TASK; i++) \{
    size_t j;
    unsigned int c = ${ttick_counters}\[i\];
    for (j = 0; j < c; j++) \{
      syslog (LOG_INFO, "%u:%u:%s",
\t${tticks}\[i\]\[j\]\[0\],
\t${tticks}\[i\]\[j\]\[1\],
\t${task_resource}\[i\].name);
    \}
  \}
\}

#define ${prefix}_TASK_LOOP_TOP(ID)\t\\
  do \{\\
    unsigned int c;\\
    TickType t;\\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &t);\\
    c = ${ttick_counters}\[ID\];\\
    if (c < ${m_ttick_size})\\
      ${tticks}\[ID\]\[c\]\[0\] = t;\\
  \} while (0)

#define ${prefix}_TASK_BEFORE_SLEEP(ID)\t\\
  do \{\\
    unsigned int c;\\
    TickType t;\\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &t);\\
    c = ${ttick_counters}\[ID\];\\
    if (c < ${m_ttick_size}) \{\\
      ${tticks}\[ID\]\[c\]\[1\] = t;\\
      ${ttick_counters}\[ID\] = ++c;\\
      if (${auto_stop} \\
\t  && ${trace_stop} == 0\\
\t  && OS_WRAPPER(_check_task_ticks_done)())\\
\t${trace_stop} = 1;\\
    \}\\
    if (${trace_stop} == 1\\
\t&& GetCoreID() == OS_CORE_ID_MASTER) \{\\
      CH__MEM_BARRIER();\\
      if (${trace_stop} == 1) \{\\
\t${trace_stop} = 2;\\
\tOS_WRAPPER(_dump_task_ticks)(TASK__CTX);\\
      \}\\
    \}\\
  \} while (0)
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####

    foreach my $v (@gen_task) {
	my $name= $v->{name};
	my $id = $v->{id};
	#### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
	$trace_h_fh->print (<<"EOF"

/* ${name} */
#define TASK_${name}_DEFINITIONS\t/* empty */
#define TASK_${name}_LOOP_TOP\t\\
\t${prefix}_TASK_LOOP_TOP(${id});
#define TASK_${name}_BEFORE_SLEEP\t\\
\t${prefix}_TASK_BEFORE_SLEEP(${id});
EOF
	    );
	#### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
    }

    #### <=<=<=<=<=<=<=<= BEGIN Here-Document <=<=<=<=<=<=<=<= ####
    $trace_h_fh->print (<<"EOF"

#endif /* ${m_enable_task_tick} */
#endif /* ${hdef} */
EOF
	);
    #### =>=>=>=>=>=>=>=> END Here-Document =>=>=>=>=>=>=>=> ####
}

sub yaml_merge {
    my ($dst, $src) = @_;

    while (my ($k, $sv) = each %{$src}) {
	my $dv = $dst->{$k};
	if (defined $dv) {
	    if (ref $sv eq 'HASH') {
		die "dst $k is not HASH\n" if ref $dv ne 'HASH';
		yaml_merge ($dv, $sv);
	    } else {
		die "dst $k is HASH\n" if ref $dv eq 'HASH';
		# ARRAYでなければARRAYにして多重にする
		unless (ref $dv eq 'ARRAY') {
		    my @t = ($dv);
		    $dv = \@t;
		    $dst->{$k} = $dv;
		}
		if (ref $sv eq 'ARRAY') {
		    push (@{$dv}, @{$sv});
		} else {
		    push (@{$dv}, $sv);
		}
	    }
	} else {
	    # コピー
	    $dst->{$k} = $sv;
	}
    }
}

if ($#ARGV > -1) {
    foreach my $f (@ARGV) {
	my ($h) = LoadFile ($f);
	die "Couldn't load $f" unless defined $h;

	if (defined $hashref) {
	    yaml_merge ($hashref, $h);
	} else {
	    $hashref = $h;
	}
    }
} else {
    my $input;
    read (STDIN, $input, 2*1024*1024*1024);
    my ($hashref) = Load ($input);
    die "Couldn't read " unless defined $hashref;
}

my $dumper = new YAML::Dumper;
$dumper->dump ($hashref);

$abrex_Ecuc = $hashref->{Ecuc};

die "Ecuc not defined" unless $abrex_Ecuc;

$abrex_Os = $abrex_Ecuc->{Os};

die "Ecuc/Os not defined" unless $abrex_Os;

while (my ($k, $v) = each %{$abrex_Os}) {
    if ($k eq 'OsOS') {
	$numberofcores = $v->{OsNumberOfCores};
	die "OsNumberOfCores undefined in ${k}" unless defined $numberofcores;
	$oshooks = $v->{OsHooks};
	next;
    }

    next if $k eq 'OsInclude';

    my $dr = $v->{DefinitionRef};
    unless (defined $dr) {
	warn "$k: not have DefinitionRef";
	next;
    }
    if ($dr eq 'OsAppMode') {
	$appmode{$k} = $v;
    } elsif ($dr eq 'OsEvent') {
	$event{$k} = $v;
    } elsif ($dr eq 'OsTask') {
	$task{$k} = $v;
    } elsif ($dr eq 'OsAlarm') {
	$alarm{$k} = $v;
    } elsif ($dr eq 'OsApplication') {
	$osap{$k} = $v;
    } elsif ($dr eq 'OsIsr') {
	$osisr{$k} = $v;
    } elsif ($dr eq 'OsCounter') {
	$oscounter{$k} = $v;
    }
}

die "OsTask not defined" if scalar(keys %task) == 0;
die "OsEvent not defined" if scalar(keys %event) == 0;
die "OsAlarm not defined" if scalar(keys %alarm) == 0;

my $n = 0;
foreach my $t (sort keys %task) {
    my $conf = $task{$t};
    my $v = { name => $t, id => $n, config => $conf };
    my $otp = $conf->{OsTaskPriority};
    die "OsTaskPriority undefined in $t" unless defined $otp;
    if ($ostype eq 'SunOS') {
	$v->{priority} = "(15-${otp})";
    } else {
	$v->{priority} = 0;
    }
    my $autostart = $conf->{OsTaskAutostart};
    die "OsTaskAutostart undefined in $t" unless defined $autostart;
    my $appmoderef = $autostart->{OsTaskAppModeRef};
    die "OsTaskAppModeRef undefined in $t" unless defined $appmoderef;
    $v->{appmode} = [];
    foreach my $ref (@{$appmoderef}) {
	my ($an, $av) = getdir ($hashref, $ref);
	push @{$v->{appmode}}, $an;
    }
    push @gen_task, $v;
    $gen_htask{$t} = $v;
    $n++;
}

$n = 0;
foreach my $e (sort keys %event) {
    my $v = { name => $e, id => $n, config => $event{$e} };
    push @gen_event, $v;
    $gen_hevent{$e} = $v;
    $n++;
}

$n = 0;
foreach my $a (sort keys %alarm) {
    my $v = { name => $a, id => $n, config => $alarm{$a} };
    push @gen_alarm, $v;
    $gen_halarm{$a} = $v;
    $n++;
}

$n = $#gen_counter + 1;
foreach my $c (sort keys %oscounter) {
    my $config = $oscounter{$c};
    my $v = {
	name => $c, id => $n,

	timer_clock => 1000,	# 1ms
	nsec_mul => 1000,	# X[Tick] * 1000 => X'[ns]
	nsec_div => 0,		# disable divisor

	max_load => $config->{OsCounterMaxAllowedValue},
	min_cycle => $config->{OsCounterMinCycle},
	ticks_per_base => $config->{OsCounterTicksPerBase},
	hw_type => $config->{OsCounterType} eq 'SOFTWARE' ? 0 : 1,
    };
    push @gen_counter, $v;
    $gen_hcounter{$c} = $v;
    $n++;
}

# コア割り当ての対応
while (my ($k, $v) = each %osap) {
    my $cv = $v->{OsApplicationCoreAssignment};
    die "OsApplicationCoreAssignment undefined at $k" unless defined $cv;

    my $c;
    if ($cv =~ /^0/) {
	$c = oct ($cv);
    } else {
	$c = int ($cv);
    }
    die "OsApplicationCoreAssignment(${cv}) invalid" if $c >= $numberofcores;

    my $t = $v->{OsAppTaskRef};
    die "OsAppTaskRef undefined at $k" unless defined $t;

    $core_info[$c] = {} unless defined $core_info[$c];
    $core_info[$c]->{task} = [] unless defined $core_info[$c]->{task};
    $core_info[$c]->{osapp} = {} unless defined $core_info[$c]->{osapp};
    $core_info[$c]->{appmode} = [] unless defined $core_info[$c]->{appmode};
    $core_info[$c]->{apphash} = {} unless defined $core_info[$c]->{apphash};

    my $ct = $core_info[$c]->{task};
    my $ca = $core_info[$c]->{appmode};
    my $ch = $core_info[$c]->{apphash};
    my $co = $core_info[$c]->{osapp};

    $co->{$k} = $v;

    my $err_hook = 0;
    my $shd_hook = 0;
    my $stu_hook = 0;
    my $hs = $v->{OsApplicationHooks};
    if (defined $hs) {
	my $erh = $hs->{OsAppErrorHook};
	my $sdh = $hs->{OsAppShutdownHook};
	my $sth = $hs->{OsAppStartupHook};
	$err_hook = 1 if defined $erh and $erh eq 'true';
	$shd_hook = 1 if defined $sdh and $sdh eq 'true';
	$stu_hook = 1 if defined $sth and $sth eq 'true';
    }

    foreach my $kt (@{$t}) {
	my ($tn, $tv) = getdir ($hashref, $kt);
	my $gv = $gen_htask{$tn};
	die "Task $tn not geneted at $c" unless defined $gv;
	if (defined $gv->{core}) {
	    warn 'Task ', $tn, ' already core#', $gv->{core}, ' assigned';
	} else {
	    $gv->{core} = $c;
	    push @{$ct}, $gv;
	}
	foreach my $am (@{$gv->{appmode}}) {
	    my $x = $ch->{$am};
	    if (defined $x) {
		$x->{err_hook} = 1 if $err_hook;
		$x->{shd_hook} = 1 if $shd_hook;
		$x->{stu_hook} = 1 if $stu_hook;
	    } else {
		$x = {};
		$x->{name} = $am;
		$x->{err_hook} = $err_hook;
		$x->{shd_hook} = $shd_hook;
		$x->{stu_hook} = $stu_hook;
		push @{$ca}, $x;
		$ch->{$am} = $x;
	    }
	}
    }
}

die "No Core assigned task" if $#core_info == -1;

gen_config_header unless defined $opts{n};
gen_config_source unless defined $opts{n};
gen_trace_header if defined $trace_h;

exit 0;

__END__

=pod

=encoding utf8

=head1 NAME

abrexyaml2pthread-c.pl -- TOPPERS/ATK2のpthreadエミュレーション用コード生成

=head1 SYNOPSIS

abrexyaml2pthread-c.pl [-n] [-t trace.h] [-s tick size] [abrex.yaml...]

=head1 DESCRIPTION

B<abrexyaml2pthread-c.pl> は B<TOPPERS/ATK2> の B<abrex>
のYAMLファイルを入力し、
pthread環境でエミュレーションするために必要なファイルを生成します。

以下の機能があります。

=over 4

=item 1. TOPPERS/ATK2 同様に I<Os_Lcfg.c> と I<Os_Lcfg.h> を生成する

=item 2. デバッグ/動作確認用の実行トレース用のヘッダファイルを生成する

=back

=head1 OPTIONS

=over 4

=item B<-n>

このオプションは I<Os_Lcfg.c> と I<Os_Lcfg.h> の生成を制御します。

このオプションを指定した場合、
I<Os_Lcfg.c> と I<Os_Lcfg.h> を出力しません。

オプションを指定しない場合、
 I<Os_Lcfg.c> と I<Os_Lcfg.h> を出力します。

これは主に B<-t> オプションと併用します。

TOPPERS/ATK2のabrexは、AUTOSARの設定ファイルを生成するため、
モデルのYAMLファイル以外に、
他のYAMLファイルを入力する必要があります。
一方、B<-t> オプションで生成する、
実行トレース用のヘッダファイル生成には、
モデル以外のYAMLファイルは不要です。

このように I<Os_Lcfg.c> と I<Os_Lcfg.h> の生成と、
I<trace.h> の生成を分けて行うのが適当な場合に
B<-n> オプションを使用します。

=item B<-t> F<trace.h>

このオプションは実行トレース用のヘッダーファイルの生成を制御します。

このオプションを指定した場合、
実行トレース用のヘッダーファイルを生成します。

なお前述の通り、入力するabrexのYAMLファイルに差異があり、
I<Os_Lcfg.c> と I<Os_Lcfg.h> と同時に生成する事が適当でない場合、
B<-n> オプションと併用するのが適当です。

このオプションを指定しない場合、
実行トレース用のヘッダーファイルを生成しません。

=item B<-s> I<tick size>

このオプションは、
実行トレースとしてタスクの周期動作の開始と終了時刻を記録する数を指定します。

このオプションを省略した場合は B<64> 個になります。

pthreadによるエミュレーションでなく、実機動作の場合は、
実際に確保可能なサイズを考えて指定する必要があります。
配列のサイズは以下の通りです。

    B<NUM_TASK> x 記録する数 x 2 x 4(byte)

=item F<abrex.yaml>

処理するabrexのYAMLファイルを指定します。

このパラメタは複数指定可能です。

=back

=head1 実行トレース用ヘッダーファイル

この生成ファイルは、CSPトランスレータが生成した
TOPPERS/ATK2のコードに対し、
タスクの周期動作の開始と終了の時刻を
各タスク毎の配列に記録する機能を提供するヘッダファイルです。

=head2 Cマクロ

=over 4

=item B<OS_WRAPPER___ENABLE_TASK_TICK>

タスクの周期動作を記録する機能を使用するかどうか指定します。

B<OS_WRAPPER___ENABLE_TASK_TICK> が B<1> の場合、機能を使用します。

B<OS_WRAPPER___ENABLE_TASK_TICK> が B<0> の場合、機能を使用せん。

デフォルトでは B<1> で、機能を使用します。

なお B<OS_WRAPPER___ENABLE_TASK_TICK> は、コンパイル時に、
B<-DOS_WRAPPER___ENABLE_TASK_TICK=0>
のようにすることで、機能を無効化できます。

=item B<OS_WRAPPER___ENABLE_TTICK_AUTO_STOP>

タスクの周期動作の開始と終了時刻を記録は、
記録する配列に空きがなくなるまで行います。
(空きがなければ記録されません)

B<OS_WRAPPER___ENABLE_TTICK_AUTO_STOP> が B<1> の場合、
全てのタスクについて、
記録する配列の空きがなくなると自動停止するようになります。

B<OS_WRAPPER___ENABLE_TTICK_AUTO_STOP> が B<0> の場合、
自動停止しません。

自動停止した場合、マスタのコアが、記録した情報を出力します。
(B<OS_WRAPPER(_dump_task_info)()>を参照)

なお B<OS_WRAPPER___ENABLE_TTICK_AUTO_STOP> は、コンパイル時に、
B<-DOS_WRAPPER___ENABLE_TTICK_AUTO_STOP=1>
のようにすることでも変更可能です。

=over 4

=item B<**注意**>

pthreadエミュレーションの環境では、
指定時間動作して停止させる事ができるため、
デフォルトでは自動停止は無効となり、
終了のタイミングで記録した情報を出力しますが、
実機では停止させるタイミングがないため、
配列に空きがなくなったタイミングで記録を停止し、
記録した情報を出力するために用いる機能です

そのため実機では、
B<-DOS_WRAPPER___ENABLE_TTICK_AUTO_STOP=0>
のような、機能の無効化を行わない事を推奨します。

=back

=back

=head2 CSPトランスレータの対応マクロ

=over 4

=item B<TASK_>I<TASK_ID>B<_DEFINITIONS>

CSPトランスレータの生成コードでは、
タスクに対して任意の宣言・定義を配置するためのマクロです。

現在、このマクロに対しては何も設定を行いません。

=item B<TASK_>I<TASK_ID>B<_LOOP_TOP>

CSPトランスレータの生成コードでは、
タスクの周期動作のループの先頭部に置かれるマクロです。

本機能では、このマクロに、
タスク毎の周期動作の開始時刻を取得・記録する処理を設定します。

=item B<TASK_>I<TASK_ID>B<_BEFORE_SLEEP>

CSPトランスレータの生成コードでは、
タスクの周期動作のループの最後尾に置かれるマクロです。

本機能では、このマクロに、
タスク毎の周期動作の終了時刻を取得・記録する処理を設定します。

記録の自動停止が有効な場合、
このマクロの中で自動停止するかどうかの判定と、
自動停止した場合の記録した情報の出力を行います。

=back

=head2 定義される関数

生成されたヘッダファイルでは関数が定義されます。
これらの関数は、特に利用することを意識するはありません。

実機では原則として、機能は全てCSPトランスレータの生成コード中の
B<TASK_>I<TASK_ID>B<_LOOP_TOP>
および
B<TASK_>I<TASK_ID>B<_BEFORE_SLEEP>
マクロ内に隠蔽されるため、特にこれら関数の知識は不要です。

一方、pthreadエミュレーション環境では、
記録の自動停止が行われる場合とそうでない場合、
機能が有効か無効かで動作を変えるため、
F<os_wrapper.c> で B<weak> シンボルとして宣言され、
そのシンボルがリンクされているかどうかで
B<main()> の動作を変えます。

これらの関数の実際の使われかたは F<os_wrapper.c> の
B<OS_WRAPPER(_check_and_dump)()>
および
B<OS_WRAPPER(_dump_task_info)()>
と
B<main()>
を参照してください。

=over 4

=item B<OS_WRAPPER(_check_task_ticks_done)>

周期動作の開始時刻・終了時刻を記録した配列が、
全てのタスクについて、空きがない状況かを調べます。

B<1> が返った場合は、記録する配列に空きがない事を示し、
B<0> が返った場合は空きがある事を示します。

=item B<OS_WRAPPER(_dump_task_ticks)>

タスク毎に周期動作の開始時刻、終了時刻を、
TOPPERS/ATK2 のAPI B<syslog()> を使って表示します。

フォーマットは、以下の通りです。

  開始時刻:終了時刻:タスク名

このフォーマットの出力は B<tracecheck.pl> のスクリプトで、
パースして実行状態を表示させることができます。

=back

=cut
