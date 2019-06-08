#!/usr/bin/env perl
# Haiku OS: we don't care!

use strict;

my $in = 'cmake-config.h.in';
my $out = "$in.$$";

my ($fin, $fout);
open $fin, "< $in" or die "can't open $in: $!";
open $fout, "> $out" or die "can't create $out: $!";

my $zipconf_defines = read_zipconf_defines();

my $in_defines = 0;
while (my $line = <$fin>) {
	if ($in_defines) {
		if ($line =~ m,/* END DEFINES,) {
			$in_defines = 0;
		}
		else {
			next;
		}
	}
	print $fout $line;
	if ($line =~ m,/\* BEGIN DEFINES,) {
		$in_defines = 1;
		add_defines($fout, $zipconf_defines);
	}
}

close $fin;
close $fout;

rename($out, $in);

sub add_defines {
	my ($fout, $zipconf_defines) = @_;

	my $fin;
	open $fin, "< CMakeLists.txt" or die "can't open CMakeLists.txt: $!";

	while (my $line = <$fin>) {
		my ($key, $value);

		if ($line =~ m/CHECK_TYPE_SIZE\(.* (\S*)\)/) {
			$key = $1;
			$value = "\${$1}";
		}
		elsif ($line =~ m/CHECK_\S*\(.* (\S*)\)/) {
			$key = $1;
		}

		if (defined($key) && !defined($zipconf_defines->{$key})) {
			print $fout "#cmakedefine $key" . ($value ? " $value" : "") . "\n";
		}
	}

	close $fin;
}

sub read_zipconf_defines {
	my %zipconf_defines = ();

	my $fin;
	open $fin, "< cmake-zipconf.h.in" or die "can't open cmake-zipconf.h.in: $!";

	while (my $line = <$fin>) {
		if ($line =~ m/#cmakedefine\s+(\S+)/) {
			$zipconf_defines{$1} = 1;
		}
	}

	close $fin;

	return \%zipconf_defines;
}
