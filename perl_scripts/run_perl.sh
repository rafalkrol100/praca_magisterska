# cd /mnt/c/Users/Rafal/Desktop/magister/results/CoMP

# for numberOfUEsPerCell in 15 20 25 30 35 40
# do
#     for cellRadius in 60 80 100 120
#     do
#         for rngRun in 21 22 23 24 25
#         do
#             cd $numberOfUEsPerCell/$cellRadius/$rngRun
#             perl /home/rafal/bake/source/ns-3.37/SINR_file_results.pl $numberOfUEsPerCell
#             cp outputSINR.csv /mnt/c/Users/Rafal/Desktop/magister/results/CSVs/outputSINR_$numberOfUEsPerCell\_$cellRadius\_$rngRun.csv
#             cd ../../../
#         done
#     done
# done

cd /mnt/c/Users/Rafal/Desktop/magister/results/Basic

for numberOfUEsPerCell in 30
do
    for cellRadius in 60 80 100 120
    do
        for rngRun in 21 22 23 24 25
        do
            cd $numberOfUEsPerCell/$cellRadius/$rngRun
            perl /home/rafal/bake/source/ns-3.37/SINR_file_results.pl $numberOfUEsPerCell
            cp outputSINR.csv /mnt/c/Users/Rafal/Desktop/magister/results/CSVsbasic/outputSINR_$numberOfUEsPerCell\_$cellRadius\_$rngRun.csv
            cd ../../../
        done
    done
done