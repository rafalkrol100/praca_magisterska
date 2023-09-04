cd /mnt/c/Users/Rafal/Desktop/magister/results/CSVsbasic

for numberOfUEsPerCell in 15 20 25 30 35 40
do
    for cellRadius in 60 80 100 120
    do
        perl bulk_results.pl $numberOfUEsPerCell $cellRadius
    done
done