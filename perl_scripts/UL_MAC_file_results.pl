#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL MAC stats block
my $filenameMACOutput1 = "UlMacStats.csv" or die;
open(FMO1, '>', $filenameMACOutput1) or die $!;

my $fileMAC = "UlMacStats.txt" or die;
open(my $dataMAC, '<', $fileMAC) or die;

while (my $lineMAC = <$dataMAC>) 
{
	chomp $lineMAC;
    
    my @wordsMAC = split(/	/, $lineMAC);
    my $newLineMAC = join(';', @wordsMAC);
    my @wordsMAC1 = split(/\./, $newLineMAC);
    my $newLineMAC1 = join(',', @wordsMAC1);
    print FMO1 $newLineMAC1;
    print FMO1 "\n";
}

my $filenameMACOutput = "outputMAC.csv" or die;
open(FMO, '>', $filenameMACOutput) or die $!;
my @k = (1..$numberOfImsi);
for(@k){
    my $fileUlMACStats = "UlMacStats.csv" or die;
    open(my $dataUlMACStats, '<', $fileUlMACStats) or die;
    print FMO "imsi: ","$_","\n";
    print FMO "%time;cellId;IMSI;frame;sframe;RNTI;mcs;size;ccId","\n";

    while (my $lineUlMACStats = <$dataUlMACStats>) 
    {
        chomp $lineUlMACStats;
    
        my @wordsUlMACStats = split ";", $lineUlMACStats;  

        if ($wordsUlMACStats[2] == "$_")
        {
            print FMO $lineUlMACStats;
            print FMO "\n";
        }
    }
}