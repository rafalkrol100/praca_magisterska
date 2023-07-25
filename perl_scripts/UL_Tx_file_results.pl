#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL Tx stats block 
my $filenameTxOutput1 = "UlTxPhyStats.csv" or die;
open(FTxO1, '>', $filenameTxOutput1) or die $!;

my $fileTx = "UlTxPhyStats.txt" or die;
open(my $dataTx, '<', $fileTx) or die;

while (my $lineTx = <$dataTx>) 
{
	chomp $lineTx;
    
    # Split the line and store it
    # inside the words array
    my @wordsTx = split(/	/, $lineTx);
    my $newLineTx = join(';', @wordsTx);
    print FTxO1 $newLineTx;
    print FTxO1 "\n";
}

my $filenameTxOutput = "outputTx.csv" or die;
open(FTxO, '>', $filenameTxOutput) or die $!;
my @n = (1..$numberOfImsi);
for(@n){
    my $fileUlTxStats = "UlTxPhyStats.csv" or die;
    open(my $dataUlTxStats, '<', $fileUlTxStats) or die;
    print FTxO "imsi: ","$_","\n";
    print FTxO "%time;cellId;IMSI;RNTI;layer;mcs;size;rv;ndi;correct;ccId","\n";

    while (my $lineUlTxStats = <$dataUlTxStats>) 
    {
        chomp $lineUlTxStats;
    
        # Split the line and store it
        # inside the words array
        my @wordsUlTxStats = split ";", $lineUlTxStats;  

        if ($wordsUlTxStats[2] == "$_")
        {
            print FTxO $lineUlTxStats;
            print FTxO "\n";
        }
    }
}
