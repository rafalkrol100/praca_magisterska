#include <cmath>
#include <cstdlib>
#include <vector>

using namespace ns3;


vector[] calculateStationsPosiotions(double cellRadius) {
    Vector positions[7];
    positions[0] = Vector(0.0, 0.0, 0.0)
    positions[1] = Vector(2 * cellRadius, 0.0, 0.0)
    positions[2] = Vector(cellRadius, cellRadius * sqrt(3), 0.0)
    positions[3] = Vector(-cellRadius, cellRadius * sqrt(3), 0.0)
    positions[4] = Vector(-2 * cellRadius, 0.0, 0.0)
    positions[5] = Vector(-cellRadius, -cellRadius * sqrt(3), 0.0)
    positions[6] = Vector(cellRadius, -cellRadius * sqrt(3), 0.0)

    return positions;
}

vector[] calculateUesPosiotions(double cellRadius, uint32_t numberOfUes) {
    //zamiast losowania miejsc z calej puli komórek robimy osobno dla kazdej z offsetem
    vector positions[numberOfUes];
    vector enbPositions = calculateStationsPosiotions(cellRadius);

    for(int i = 0; i < numberOfUes; i++) {
        int enbIndex = i % 7;
        Vector enbX = enbPositions[enbIndex];
        Vector enbY = enbPositions[enbIndex];
        double x = rand() % cellRadius + (enbX - cellRadius);
        double y = rand() % cellRadius + (enbY - cellRadius);
        positions[i] = Vector(x, y, 0);
    }

    return positions;
}

uint32_t findNearestStationIndexForUe(vector uePosition, vector[] stationsPositions) {
    //obliczenie najblizszej komórki
    int shortestDistance = 9999999;
    int indexOfNearestStation = 0;
    for(int i = 0; i < stationsPositions.size(); i++) {
        int distance;
        int xDist;
        int yDist;

        xDist = uePosition[0] - stationsPositions[i][0];
        yDist = uePosition[1] - stationsPositions[i][1];

        distance = sqrt(xDist*xDist + yDist*yDist);
        if (distance < shortestDistance) {
            shortestDistance = distance;
            indexOfNearestStation = i;
        }
    }
    return indexOfNearestStation;
}

uint32_t findBestStationIndexForUe(vector uePosition, vector[] stationsPositions) {
    //algorytm kuby z pracy
    for(int i = 0; i < stationsPositions.size(); i++) {
        ...
    }
    return 0;
}

int main(int argc, char *argv[])
{
    //geographical parameters for devices
    uint32_t NUMBER_OF_UES = 1;
    uint32_t NUMBER_OF_STATIONS = 7;
    double cellRadius = 60.0;
    uint32_t numberOfAntennasForSingleStation = 6;
    bool isAlgorithmUsed = false;





    //create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    
    //set basic LTE attributes
    Config::SetDefault("ns3::LteUePowerControl::Pcmin", DoubleValue(23.0));
    Config::SetDefault("ns3::LteUePowerControl::Pcmax", DoubleValue(23.0));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(50));
    lteHelper->SetEnbDeviceAttribute("Mtu", UintegerValue(1500));  
    lteHelper->SetUeDeviceAttribute("Mtu", UintegerValue(1500));
    lteHelper->SetAttribute("FadingModel", StringValue("ns3::TraceFadingLossModel"));

    //ustawienie 6 sektorowych anten + Strict Frequency Reuse

    //create EPC helper
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>(); 
    lteHelper->SetEpcHelper(epcHelper);

    //create a remote host
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);

    //create internet stack helper
    InternetStackHelper internet;  
    internet.Install(remoteHostContainer);
    NS_LOG_UNCOND("RRHs turned on");
    
    //set basic internet parameters
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.001)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    //interface 0 is localhost, 1 is the p2p device
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4> ());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    NS_LOG_UNCOND("Internet created");
    
    //nodes containers for eNBs and UEs
    NodeContainer enbNodes;
    enbNodes.Create (NUMBER_OF_STATIONS);
    NodeContainer ueNodes;
    ueNodes.Create (NUMBER_OF_UES);

    //lists of devices positions
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator> ();
    Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator> ();

    //putting values of coordinates to simulation position array
    Vector[] stationsPositions = calculateStationsPosiotions(cellRadius);
    for(int i = 0; i < NUMBER_OF_STATIONS; i++){
        enbPositionAlloc -> Add(stationsPositions[i]);
    }
    
    Vector[] uesPositions = calculateUesPosiotions(cellRadius, NUMBER_OF_UES);
    for(int i = 0; i < NUMBER_OF_UES; i++){
        uePositionAlloc -> Add(uesPositions[i]);
    }

    //set mobility parameters
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(enbPositionAlloc);
    enbMobility.Install(enbNodes);

    MobilityHelper ueMobility;
    ueMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ueMobility.SetPositionAllocator(uePositionAlloc);
    ueMobility.Install(ueNodes);

    //instal devices
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;

    //set up strict frequency reuse model
    lteHelper->SetFfrAlgorithmAttribute ("RsrqThreshold", UintegerValue (32));
    lteHelper->SetFfrAlgorithmAttribute ("CenterPowerOffset",
                                            UintegerValue (LteRrcSap::PdschConfigDedicated::dB_6));
    lteHelper->SetFfrAlgorithmAttribute ("EdgePowerOffset",
                                            UintegerValue (LteRrcSap::PdschConfigDedicated::dB3));
    lteHelper->SetFfrAlgorithmAttribute ("CenterAreaTpc", UintegerValue (0));
    lteHelper->SetFfrAlgorithmAttribute ("EdgeAreaTpc", UintegerValue (3));
 
    //ns3::LteFrStrictAlgorithm works with Absolute Mode Uplink Power Control
    Config::SetDefault ("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue (false));

    for(int i = 0; i < NUMBER_OF_STATIONS; i++) {
        lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
        lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (0 + i*60));
        lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (60));
        lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0));

        enbDevs.Add(lteHelper->InstallEnbDevice(enbNodes.Get(i)));
    }

    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    NS_LOG_UNCOND("UEs and eNBs prepared");

    //install the IP stack on the UEs
    internet.Install(ueNodes);
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs));

    //assign IP address to UEs, and install applications
    for(uint32_t u = 0; u < ueNodes.GetN(); ++u) {
        Ptr<Node> ueNode = ueNodes.Get(u);
        //set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4> ());
        ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    //attach UEs to eNBs
    if (isAlgorithmUsed) {
        for(int i=0; i<UES_NUMBER; i++){
            lteHelper->Attach(ueDevs.Get (i), enbDevs.Get(findBestStationIndexForUe(uesPositions[i], stationsPositions)));  
        }
    } else {
        for(int i=0; i<UES_NUMBER; i++){
            lteHelper->Attach(ueDevs.Get (i), enbDevs.Get(findNearestStationIndexForUe(uesPositions[i], stationsPositions)));  
        }
    }
    

    //install application
    uint16_t dlPort = 1100;
    uint16_t ulPort = 2000;
    uint16_t otherPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        ++ulPort;
        PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
        serverApps.Add (ulPacketSinkHelper.Install (remoteHost));

        UdpClientHelper ulClient (remoteHostAddr, ulPort);
        ulClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
        ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
        clientApps.Add (ulClient.Install (ueNodes.Get(u)));
    }

    NS_LOG_UNCOND("Application installed");

    serverApps.Start (Seconds (0.01));
    clientApps.Start (Seconds (0.02));

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    clientApps.Stop(simTime);
    serverApps.Stop(simTime);
    Simulator::Stop(simTime);

    lteHelper->EnablePhyTraces ();
    lteHelper->EnableMacTraces ();
    lteHelper->EnableRlcTraces ();
    lteHelper->EnableUlPhyTraces ();
    lteHelper->EnableUlRxPhyTraces ();
    lteHelper->EnableUlTxPhyTraces ();
    
    //Insert RLC Performance Calculator
    std::string dlOutFname = "DlRlcStats";
    dlOutFname.append (tag.str ());
    std::string ulOutFname = "UlRlcStats";
    ulOutFname.append (tag.str ());
    
    Simulator::Run ();

    flowMonitor->SerializeToXmlFile("Results_200_40.xml", false, true);

    Simulator::Destroy ();
    return 0;

}