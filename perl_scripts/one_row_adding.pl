use strict;
use warnings;

use Data::Dump;

my $numberOfUsers = $ARGV[0];
my $cellRadius = $ARGV[1];
my @resultsArray = ();

my $fileNameOutput =  join("", "comp_", "$numberOfUsers", "_", "$cellRadius", "_one_row.csv") or die;
open(FO, '>', $fileNameOutput) or die $!;


my $fileNameInput = join("", "comp_", "$numberOfUsers", "_", "$cellRadius", ".csv") or die;

open(my $data, '<', $fileNameInput) or die;

while(my $line = <$data>)
{
    chomp $line;
    push(@resultsArray, $line);
}

my $joined = join(';', @resultsArray);
my @split = split(/\;/, $joined);



foreach (@split)
{
    print FO $_, "\n";
}





