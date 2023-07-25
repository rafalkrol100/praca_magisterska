#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL RLC stats block 
my $filenameRLCOutput1 = "UlRlcStats.csv" or die;
open(FRO1, '>', $filenameRLCOutput1) or die $!;

my $fileRLC = "UlRlcStats.txt" or die;
open(my $dataRLC, '<', $fileRLC) or die;

while (my $lineRLC = <$dataRLC>) 
{
	chomp $lineRLC;
    
    my @wordsRLC = split(/	/, $lineRLC);
    my $newLineRLC = join(';', @wordsRLC);
    my @wordsRLC1 = split(/\./, $newLineRLC);
    my $newLineRLC1 = join(',', @wordsRLC1);
    print FRO1 $newLineRLC1;
    print FRO1 "\n";
}

my $filenameRLCOutput = "outputRLC.csv" or die;
open(FRO, '>', $filenameRLCOutput) or die $!;
my @l = (1..$numberOfImsi);
for(@l){
    my $fileUlRLCStats = "UlRlcStats.csv" or die;
    open(my $dataUlRLCStats, '<', $fileUlRLCStats) or die;
    print FRO "imsi: ","$_","\n";
    print FRO "%start;end;CellId;IMSI;RNTI;LCID;nTxPDUs;TxBytes;nRxPDUs;RxBytes;delay;stdDev;min;max;PduSize;stdDev;min;max","\n";

    while (my $lineUlRLCStats = <$dataUlRLCStats>) 
    {
        chomp $lineUlRLCStats;
    
        my @wordsUlRLCStats = split ";", $lineUlRLCStats;  

        if ($wordsUlRLCStats[3] == "$_")
        {
            print FRO $lineUlRLCStats;
            print FRO "\n";
        }
    }
}