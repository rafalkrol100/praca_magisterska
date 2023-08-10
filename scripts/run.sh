#!/bin/bash

for numberOfUEsPerCell in 15
do
	for cellRadius in 60
	do
		for radiusMultiplier in 0.95
		do
			for rngRun in 1
			do
				./ns3 run "comp_scenario.cc --numberOfUEsPerCell=$numberOfUEsPerCell --cellRadius=$cellRadius --radiusMultiplier=$radiusMultiplier --RngRun=$rngRun"
				mkdir results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv UlSinrStats.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv Results.xml results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv ue_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv enb_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
			done
		done
	done
done

for numberOfUEsPerCell in 20
do
	for cellRadius in 60 80
	do
		for radiusMultiplier in 0.95
		do
			for rngRun in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
			do
				./ns3 run "comp_scenario.cc --numberOfUEsPerCell=$numberOfUEsPerCell --cellRadius=$cellRadius --radiusMultiplier=$radiusMultiplier --RngRun=$rngRun"
				mkdir results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv UlSinrStats.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv Results.xml results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv ue_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv enb_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
			done
		done
	done
done

for numberOfUEsPerCell in 15 20 25 30 35
do
	for cellRadius in 100 120
	do
		for radiusMultiplier in 0.95
		do
			for rngRun in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
			do
				./ns3 run "comp_scenario.cc --numberOfUEsPerCell=$numberOfUEsPerCell --cellRadius=$cellRadius --radiusMultiplier=$radiusMultiplier --RngRun=$rngRun"
				mkdir results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv UlSinrStats.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv Results.xml results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv ue_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv enb_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
			done
		done
	done
done

for numberOfUEsPerCell in 40
do
	for cellRadius in 60 80
	do
		for radiusMultiplier in 0.95
		do
			for rngRun in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
			do
				./ns3 run "comp_scenario.cc --numberOfUEsPerCell=$numberOfUEsPerCell --cellRadius=$cellRadius --radiusMultiplier=$radiusMultiplier --RngRun=$rngRun"
				mkdir results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv UlSinrStats.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv Results.xml results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
				mv ue_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
                mv enb_coordinates.txt results/COMP_scenario_NOC_"$numberOfUEsPerCell"_CR_"$cellRadius"_RM_"$radiusMultiplier"_RNG_"$rngRun"
			done
		done
	done
done

