#!/usr/bin/env perl

use strict;

my $usage = "div.pl <data>\n";
my $data = shift or die $usage;

my $i = 0;
my %result;

#read the data by row
open DATA, $data or die "Can't open `$data': $!\n";
while (<DATA>){
    chomp;
    next if /^$/;
    $_ =~ s/s//g;
    my @line = split;
    
    my $index = 0;
    my $imd = $line[0];
    for my $val (@line){
        $val = $imd / $val;
        $line[$index] = $val;
        $index++;
    }
    
    $result{$i} = [@line];
    $i ++;
}

# output the result by column
for (my $i = 1; $i < 10; $i++){
    my $n = $i + 1;
    print "$n\t";
    for my $np (sort {$a<=>$b} keys %result) {
        my @line_array = @{$result{$np}};
        my $val = sprintf '%.4f', $line_array[$i];
        print "$val\t";
    }
    print "\n";
}
