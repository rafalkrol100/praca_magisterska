use strict;
use warnings;

use Data::Dump;

my $numberOfUsers = $ARGV[0];
my $cellRadius = $ARGV[1];
my @resultsArray = ();

my $fileNameOutput =  join("", "comp_", "$numberOfUsers", "_", "$cellRadius", ".csv") or die;
open(FO, '>', $fileNameOutput) or die $!;

my @i = (1..25);
for(@i){
    my $fileNameInput = join("", "outputSINR_", "$numberOfUsers", "_", "$cellRadius", "_", $_, ".csv") or die;
    
    open(my $data, '<', $fileNameInput) or die;

    my $containsValue = 0;
    my $iterator = 1;
    my @row = ();
    while(my $line = <$data>)
    {
        chomp $line;
        if ($containsValue == 0)
        {
            if ($line eq "0")
            {
                $containsValue = 1;
            }
        } else
        {
            push(@row, $line);
        }
        $iterator = $iterator + 1;
    }
    # print @row;
    push(@resultsArray, \@row);
}
# dd @resultsArray[0];

# dd $resultsArray[0];
# my @rowToJoin = @{$resultsArray[0]};
# my $joined = join(';', @rowToJoin);
# print $joined;

my @j = (1..25);
for(@j){
    my @rowToJoin = @{$resultsArray[$_ - 1]};
    my $joined = join(';', @rowToJoin);
    print FO $joined, "\n";
}

# my @word = ();
# my @j = (1..20);
# for(@j){
#     push(@word, @{$resultsArray[$_]});
#     my $joined = join(';', @word);
#     print FO $joined, "\n";
# }



