cd /mnt/c/Users/Rafal/Desktop/magister/results/summarizedResults

for numberOfUEsPerCell in 15
do
    for cellRadius in 60
    do        
        perl /home/rafal/bake/source/ns-3.37/one_row_adding.pl $numberOfUEsPerCell $cellRadius
    done
done