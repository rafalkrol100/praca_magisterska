use strict;
   
my $fileTrue = "UlSinrStatsTrue.csv" or die;
open(my $dataTrue, '<', $fileTrue) or die;

my $fileFalse = "UlSinrStatsFalse.csv" or die;
open(my $dataFalse, '<', $fileFalse) or die;

my $filename = "UlSinrStats4.csv" or die;
open(FH, '>', $filename) or die $!;

print FH "TRUE\n";
print FH "\%time;cellId;IMSI;RNTI;sinrLinear;componentCarrierId";
print FH "\n";

while (my $lineTrue = <$dataTrue>) 
{
    chomp $lineTrue;
    
   
    # Split the line and store it
    # inside the words array
    my @wordsTrue = split ";", $lineTrue;  

    if ($wordsTrue[2] == "4")
    {
        print FH $lineTrue;
        print FH "\n";
    }
}

print FH "FALSE\n";
print FH "\%time;cellId;IMSI;RNTI;sinrLinear;componentCarrierId";
print FH "\n";

while (my $lineFalse = <$dataFalse>) 
{
    chomp $lineFalse;
    
   
    # Split the line and store it
    # inside the words array
    my @wordsFalse = split ";", $lineFalse;  

    if ($wordsFalse[2] == "4")
    {
        print FH $lineFalse;
        print FH "\n";
    }
}