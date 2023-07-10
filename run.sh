#!/bin/bash

for compEnabled in false true
do
	for cellRadius in 60 80 100 120
	do
		for UEs in 5 10 15 20
		do
			for rngRun in 1 2 3 4 5 6 7 8 9 10
			do
				./ns3 run "network_script_wo_omni --cellRadius=$cellRadius --NUMBER_OF_UES=$UEs --isCompEnabled=$compEnabled"
				mkdir scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv DlPdcpStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv DlRlcStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv DlRsrpSinrStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv mobility-trace-example.mob scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv Results.xml scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlInterferenceStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlMacStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlPdcpStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlRlcStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlRxPhyStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlSinrStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
				mv UlTxPhyStats.txt scenario_"$compEnabled"_"$cellRadius"_"$UEs"_"$rngRun"
			done
		done
	done
done