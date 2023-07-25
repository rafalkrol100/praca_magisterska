#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# Obtain data from UlSinrStats.txt and push it to UlSinrStats.csv in proper format ##################################################
my $fileNameSINROutputCsv = "UlSinrStats.csv" or die;
open(FSO1, '>', $fileNameSINROutputCsv) or die $!;

my $fileNameSINRInputTxt = "UlSinrStats.txt" or die;
open(my $dataSINR, '<', $fileNameSINRInputTxt) or die;

while (my $lineSINR = <$dataSINR>) 
{
	chomp $lineSINR;
    
    #split and join operations to change " " -> ";" and "." -> ","
    my @wordsSINR = split(/	/, $lineSINR);
    my $newLineSINR = join(';', @wordsSINR);
    my @wordsSINR1 = split(/\./, $newLineSINR);
    my $newLineSINR1 = join(',', @wordsSINR1);

    print FSO1 $newLineSINR1;
    print FSO1 "\n";
}
#####################################################################################################################################

#Prepare coordinates vextors for Ues and eNBs. Ue vector starts with additional number 0.0 added at the beginning so ################
#vector can be later iterated with indexes starting from 1

#UEs
my @ueCoordinatesVector = (0.0);
my $fileUeCoordinates = "ue_coordinates.txt" or die;
open(my $dataUeCoordinates, '<', $fileUeCoordinates) or die;

while(my $lineUeCoordinates = <$dataUeCoordinates>)
{
    chomp $lineUeCoordinates;
    push(@ueCoordinatesVector, $lineUeCoordinates);
}

#ENbs
my @enbCoordinatesVector;
my $fileEnbCoordinates = "enb_coordinates.txt" or die;
open(my $dataEnbCoordinates, '<', $fileEnbCoordinates) or die;

while(my $lineEnbCoordinates = <$dataEnbCoordinates>)
{
    chomp $lineEnbCoordinates;
    push(@enbCoordinatesVector, $lineEnbCoordinates);
}

#####################################################################################################################################

#Prepare output file#################################################################################################################

my $fileNameSINRInputCsv = "outputSINR.csv" or die;
open(FSI, '>', $fileNameSINRInputCsv) or die $!;

#Create vectors for:
#   -sums of all sinr values per single IMSI
#   -number of sinr results per single IMSI
#   -rows containing string row configuration fot later use in displaying sumarized information at the bottom of the file
#   -numbers to sort means for plot purpose
my @sums = (0.0);
my @numberOfProbes = (0);
my @summaryRows = ("");
my @numbers = (0);

my @i = (1..$numberOfImsi);
for(@i){
    my $fileUlSinrStats = "UlSinrStats.csv" or die;
    open(my $dataUlSinrStats, '<', $fileUlSinrStats) or die;

    #Create SINR stat blocks for single IMSI
    print FSI "imsi: ","$_","\n";
    print FSI "%time;cellId;IMSI;RNTI;sinrLinear;componentCarrierId","\n";

    while (my $lineUlSinrStats = <$dataUlSinrStats>) 
    {
        chomp $lineUlSinrStats;
        my @wordsUlSinrStats = split ";", $lineUlSinrStats; 

        #if row which is currently analyzed is describing particular imsi (3rd column is IMSI number) print it in current stats block
        if ($wordsUlSinrStats[2] == "$_")
        {
            print FSI $lineUlSinrStats;
            print FSI "\n";

            #split and join to transform "," -> "." to obtain SINR (5th column) value in proper format
            my @dottedSplit = split(/\,/, $wordsUlSinrStats[4]);
            my $dottedJoin = join('.', @dottedSplit);

            #add value to sums and increase counter
            $sums[$_] = $sums[$_] + $dottedJoin;
            $numberOfProbes[$_] = $numberOfProbes[$_] + 1;
        }
    }
    my $mean = $sums[$_] / $numberOfProbes[$_];

    my @dottedSplit1 = split(/\./, $ueCoordinatesVector[$_]);
    my $dottedJoin1 = join(',', @dottedSplit1);
    my @splittedUeCoordinates = split(/\:/, $dottedJoin1);

    print FSI "sum:;","$sums[$_]",
        ";no of probes:;","$numberOfProbes[$_]",
        ";mean:;","$mean",
        ";coordinates:;","$ueCoordinatesVector[$_]",
        ";coordinatex:;","$splittedUeCoordinates[0]",
        ";coordinatey:;","$splittedUeCoordinates[1]",
        "\n";


    push(@summaryRows, ("imsi: ;$_",
        ";sum:;","$sums[$_]",
        ";no of probes:;","$numberOfProbes[$_]",
        ";mean:;","$mean",
        ";coordinates:;","$ueCoordinatesVector[$_]",
        ";coordinatex:;","$splittedUeCoordinates[0]",
        ";coordinatey:;","$splittedUeCoordinates[1]",
        "\n"));
    
    push(@numbers, $mean);
}

print FSI "\n";
print FSI "\n";
print FSI "\n";
#####################################################################################################################################

#Create data for enb coordinates block###############################################################################################
for(@enbCoordinatesVector)
{
    my @splittedEnb = split(/ /, $_);
    my @dottedSplitx1 = split(/\./, $splittedEnb[0]);
    my $dottedJoinx1 = join(',', @dottedSplitx1);
    my @dottedSplity1 = split(/\./, $splittedEnb[1]);
    my $dottedJoiny1 = join(',', @dottedSplity1);

    print FSI "$dottedJoinx1",";","$dottedJoiny1","\n";
}

#####################################################################################################################################

#Print summarized information########################################################################################################

for(@summaryRows)
{
    print FSI "$_";
}

my @sorted_numbers = sort {$a <=> $b} @numbers;
for(@sorted_numbers)
{
    my @dottedSplit = split(/\./, $_);
    my $dottedJoin = join(',', @dottedSplit);
    print FSI "$dottedJoin","\n";
}