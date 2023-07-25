#!/usr/bin/perl
use strict;
my $numberOfImsi = 38;
my $numberOfCellId = 19*6;

# UL Interference stats block
my $filenameInterferenceOutput1 = "UlInterferenceStats.csv" or die;
open(FIO1, '>', $filenameInterferenceOutput1) or die $!;

my $fileInterference = "UlInterferenceStats.txt" or die;
open(my $dataInterference, '<', $fileInterference) or die;

while (my $lineInterference = <$dataInterference>) 
{
	chomp $lineInterference;
    
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
    
        my @wordsUlInterferenceStats = split ";", $lineUlInterferenceStats;  

        if ($wordsUlInterferenceStats[1] == "$_")
        {
            print FIO $lineUlInterferenceStats;
            print FIO "\n";
        }
    }
}