#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL Rx stats block
my $filenameRxOutput1 = "UlRxPhyStats.csv" or die;
open(FRxO1, '>', $filenameRxOutput1) or die $!;

my $fileRx = "UlRxPhyStats.txt" or die;
open(my $dataRx, '<', $fileRx) or die;

while (my $lineRx = <$dataRx>) 
{
	chomp $lineRx;
    
    # Split the line and store it
    # inside the words array
    my @wordsRx = split(/	/, $lineRx);
    my $newLineRx = join(';', @wordsRx);
    print FRxO1 $newLineRx;
    print FRxO1 "\n";
}

my $filenameRxOutput = "outputRx.csv" or die;
open(FRxO, '>', $filenameRxOutput) or die $!;
my @m = (1..$numberOfImsi);
for(@m){
    my $fileUlRxStats = "UlRxPhyStats.csv" or die;
    open(my $dataUlRxStats, '<', $fileUlRxStats) or die;
    print FRxO "imsi: ","$_","\n";
    print FRxO "%time;cellId;IMSI;RNTI;layer;mcs;size;rv;ndi;correct;ccId","\n";

    while (my $lineUlRxStats = <$dataUlRxStats>) 
    {
        chomp $lineUlRxStats;
    
        # Split the line and store it
        # inside the words array
        my @wordsUlRxStats = split ";", $lineUlRxStats;  

        if ($wordsUlRxStats[2] == "$_")
        {
            print FRxO $lineUlRxStats;
            print FRxO "\n";
        }
    }
}