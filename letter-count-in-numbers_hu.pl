#!/usr/bin/env perl
# Homework :-) to https://www.youtube.com/watch?v=LYKn0yUTIU4 "Four has Four Letters" 

use utf8;
use Data::Dumper;

%HU_base = qw{
	0 nulla
	1 egy
	2 kettő
	3 három
	4 négy
	5 öt
	6 hat
	7 hét
	8 nyolc
	9 kilenc
	10 tíz
	20 húsz
	30 harminc
	100 száz
	1000 ezer
};

$ket = length "két";  # két- / kettő-


sub group_prefix_length
{
	my ($units, $div) = @_;
	
	if($units == 0)
	{
		return undef;
	}
	if($units == 1 and $div ~~ [100, 1000])
	{
		return 0;
	}
	if($units == 2)
	{
		return $ket;
	}
	return lettercount($units);
}

sub is_integer
{
	my ($x) = @_;
	return 0 if $x =~ /[^0-9-]/;
	return 1;
}

sub lettercount
{
	my ($x, $first_recursion) = @_;
	my $y;
	
	if(exists $HU_base{$x})
	{
		$y = length $HU_base{$x};
	}
	elsif($x > 10 and $x < 30)
	{
		$infix = 2; # tiz-en-egy, husz-on-egy
		$y = lettercount(10 * int $x / 10) + $infix + lettercount($x % 10);
	}
	elsif($x < 100)
	{
		if($x % 10 == 0)
		{
			$suffix = 3; # negy-ven, öt-ven, hat-van, het-ven, nyolc-van, kilenc-ven
			$y = lettercount(int $x/10) + $suffix;
		}
		else
		{
			$y = lettercount(10 * int $x/10) + lettercount($x % 10);
		}
	}
	elsif(is_integer(my $log1000 = (log $x) / (log 1000)))
	{
		@prefix = qw/m b tr quadr quint sext sept oct non/;
		$infix = "illi";
		@suffix = qw/ó árd/;
		
		$y = length $prefix[int $log1000 / 2] . $infix . $suffix[$log1000 % 2];
		
		if($first_recursion)
		{
			$y += lettercount(1);
		}
	}
	else
	{
		$y = 0;
		my $reminder = $x;
		for my $group_divisor (1000_000, 1000, 100)
		{
			my $units = int $reminder / $group_divisor;
			my $pl = group_prefix_length($units, $group_divisor);
			#print "div=$group_divisor u=$units pl=$pl";
			if(defined $pl)
			{
				$reminder -= $units * $group_divisor;
				#print "$y += $pl + lettercount($group_divisor)";
				$y += $pl + lettercount($group_divisor);
			}
		}
		if($reminder > 0)
		{
			$y += lettercount($reminder);
		}
	}
	return $y;
}


$\ = "\n";

if(@ARGV)
{
	print lettercount($ARGV[0], 1);
}
else
{
	print "digraph {";
	
	while(<>)
	{
		chomp;
		print $_, '->', lettercount($_, 1);
	}
	
	print "}";
}
