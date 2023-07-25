#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL Sinr stats block   
my $filenameSINROutput1 = "UlSinrStats.csv" or die;
open(FSO1, '>', $filenameSINROutput1) or die $!;

my $fileSINR = "UlSinrStats.txt" or die;
open(my $dataSINR, '<', $fileSINR) or die;

while (my $lineSINR = <$dataSINR>) 
{
	chomp $lineSINR;
    
    # Split the line and store it
    # inside the words array
    my @wordsSINR = split(/	/, $lineSINR);
    my $newLineSINR = join(';', @wordsSINR);
    my @wordsSINR1 = split(/\./, $newLineSINR);
    my $newLineSINR1 = join(',', @wordsSINR1);
    print FSO1 $newLineSINR1;
    print FSO1 "\n";
}

#coordinates operations
my @coordinatesVector = (0.0);
my $fileUeCoordinates = "ue_coordinates.txt" or die;
open(my $dataUeCoordinates, '<', $fileUeCoordinates) or die;

while(my $lineUeCoordinates = <$dataUeCoordinates>)
{
    chomp $lineUeCoordinates;
    push(@coordinatesVector, $lineUeCoordinates);
}

my @enbCoordinatesVector;
my $fileEnbCoordinates = "enb_coordinates.txt" or die;
open(my $dataEnbCoordinates, '<', $fileEnbCoordinates) or die;

while(my $lineEnbCoordinates = <$dataEnbCoordinates>)
{
    chomp $lineEnbCoordinates;
    push(@enbCoordinatesVector, $lineEnbCoordinates);
}

my $filenameSINROutput = "outputSINR.csv" or die;
open(FSO, '>', $filenameSINROutput) or die $!;

my @sums = (0.0);
my @numberOfProbes = (0);
my @summaryRows = ("");
my @numbers = (0);

my @i = (1..$numberOfImsi);
for(@i){
    #push(@sums, 0.0);
    my $fileUlSinrStats = "UlSinrStats.csv" or die;
    open(my $dataUlSinrStats, '<', $fileUlSinrStats) or die;
    print FSO "imsi: ","$_","\n";
    print FSO "%time;cellId;IMSI;RNTI;sinrLinear;componentCarrierId","\n";

    while (my $lineUlSinrStats = <$dataUlSinrStats>) 
    {
        chomp $lineUlSinrStats;
    
        # Split the line and store it
        # inside the words array
        my @wordsUlSinrStats = split ";", $lineUlSinrStats; 

        if ($wordsUlSinrStats[2] == "$_")
        {
            print FSO $lineUlSinrStats;
            print FSO "\n";

            my @dottedSplit = split(/\,/, $wordsUlSinrStats[4]);
            my $dottedJoin = join('.', @dottedSplit);

            $sums[$_] = $sums[$_] + $dottedJoin;
            $numberOfProbes[$_] = $numberOfProbes[$_] + 1;
        }
    }
    my $mean = $sums[$_] / $numberOfProbes[$_];

    my @dottedSplit1 = split(/\./, $coordinatesVector[$_]);
    my $dottedJoin1 = join(',', @dottedSplit1);
    my @splittedUeCoordinates = split(/\:/, $dottedJoin1);
    print FSO "sum: ;","$sums[$_]","; no of probes: ;","$numberOfProbes[$_]","; mean: ;","$mean",";coordinates: ;","$coordinatesVector[$_]","; coordinatex:;","$splittedUeCoordinates[0]",";coordinatey:;","$splittedUeCoordinates[1]","\n";
    push(@summaryRows, ("imsi: ;$_;sum: ;","$sums[$_]","; no of probes: ;","$numberOfProbes[$_]","; mean: ;","$mean",";coordinates: ;","$coordinatesVector[$_]","; coordinatex:;","$splittedUeCoordinates[0]",";coordinatey:;","$splittedUeCoordinates[1]","\n"));
    push(@numbers, $mean);
}

print FSO "\n";
print FSO "\n";
print FSO "\n";
for(@enbCoordinatesVector)
{
    my @splittedEnb = split(/ /, $_);
    my @dottedSplitx1 = split(/\./, $splittedEnb[0]);
    my $dottedJoinx1 = join(',', @dottedSplitx1);
    my @dottedSplity1 = split(/\./, $splittedEnb[1]);
    my $dottedJoiny1 = join(',', @dottedSplity1);

    print FSO "$dottedJoinx1",";","$dottedJoiny1","\n";
}

for(@summaryRows)
{
    print FSO "$_";
}

my @sorted_numbers = sort {$a <=> $b} @numbers;
for(@sorted_numbers)
{
    my @dottedSplit = split(/\./, $_);
    my $dottedJoin = join(',', @dottedSplit);
    print FSO "$dottedJoin","\n";
}

# UL Interference stats block
my $filenameInterferenceOutput1 = "UlInterferenceStats.csv" or die;
open(FIO1, '>', $filenameInterferenceOutput1) or die $!;

my $fileInterference = "UlInterferenceStats.txt" or die;
open(my $dataInterference, '<', $fileInterference) or die;

while (my $lineInterference = <$dataInterference>) 
{
	chomp $lineInterference;
    
    # Split the line and store it
    # inside the words array
    my @wordsInterference = split(/	/, $lineInterference);
    my $newLineInterference = join(';', @wordsInterference);
    my @wordsInterference1 = split(/ /, $newLineInterference);
    my $newLineInterference1 = join(';', @wordsInterference1);
    my @wordsInterference2 = split(/\./, $newLineInterference1);
    my $newLineInterference2 = join(',', @wordsInterference2);
    print FIO1 $newLineInterference2;
    print FIO1 "\n";
} 

my $filenameInterferenceOutput = "outputInterference.csv" or die;
open(FIO, '>', $filenameInterferenceOutput) or die $!;
my @j = (1..$numberOfCellId);
for(@j){
    my $fileUlInterferenceStats = "UlInterferenceStats.csv" or die;
    open(my $dataUlInterferenceStats, '<', $fileUlInterferenceStats) or die;
    print FIO "cellId: ","$_","\n";
    print FIO "%time;cellId;Interference","\n";

    while (my $lineUlInterferenceStats = <$dataUlInterferenceStats>) 
    {
        chomp $lineUlInterferenceStats;
    
        # Split the line and store it
        # inside the words array
        my @wordsUlInterferenceStats = split ";", $lineUlInterferenceStats;  

        if ($wordsUlInterferenceStats[1] == "$_")
        {
            print FIO $lineUlInterferenceStats;
            print FIO "\n";
        }
    }
}

# UL MAC stats block
my $filenameMACOutput1 = "UlMacStats.csv" or die;
open(FMO1, '>', $filenameMACOutput1) or die $!;

my $fileMAC = "UlMacStats.txt" or die;
open(my $dataMAC, '<', $fileMAC) or die;

while (my $lineMAC = <$dataMAC>) 
{
	chomp $lineMAC;
    
    # Split the line and store it
    # inside the words array
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
    
        # Split the line and store it
        # inside the words array
        my @wordsUlMACStats = split ";", $lineUlMACStats;  

        if ($wordsUlMACStats[2] == "$_")
        {
            print FMO $lineUlMACStats;
            print FMO "\n";
        }
    }
}

# UL RLC stats block 
my $filenameRLCOutput1 = "UlRlcStats.csv" or die;
open(FRO1, '>', $filenameRLCOutput1) or die $!;

my $fileRLC = "UlRlcStats.txt" or die;
open(my $dataRLC, '<', $fileRLC) or die;

while (my $lineRLC = <$dataRLC>) 
{
	chomp $lineRLC;
    
    # Split the line and store it
    # inside the words array
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
    
        # Split the line and store it
        # inside the words array
        my @wordsUlRLCStats = split ";", $lineUlRLCStats;  

        if ($wordsUlRLCStats[3] == "$_")
        {
            print FRO $lineUlRLCStats;
            print FRO "\n";
        }
    }
}

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
