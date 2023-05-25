#include <cmath>
#include <cstdlib>
#include <vector>
#include "ns3/core-module.h"
#include "ns3/lte-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"

using namespace ns3;

std::vector<Vector> calculateStationsPosiotions(double cellRadius) {
    std::vector<Vector> positions;
    positions.push_back(Vector(0.0, 0.0, 0.0));
    positions.push_back(Vector(2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(-cellRadius, -cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(cellRadius, -cellRadius * sqrt(3), 0.0));

    return positions;
}

std::vector<Vector> calculateUesPosiotions(double cellRadius, int numberOfUes) {
    std::vector<Vector> positions;
    std::vector<Vector> enbPositions = calculateStationsPosiotions(cellRadius);

    for(int i = 0; i < numberOfUes; i++) {
        int enbIndex = i % 7;
        double enbX = enbPositions[enbIndex][0];
        double enbY = enbPositions[enbIndex][1];
        double x = rand() % cellRadius + (enbX - cellRadius);
        double y = rand() % cellRadius + (enbY - cellRadius);
        positions.push_back(Vector(x, y, 0));
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

int main(int argc, char *argv[])
{
    //parameters
    uint32_t NUMBER_OF_UES = 7;
    uint32_t NUMBER_OF_STATIONS = 7;
    double cellRadius = 60.0;
    
    //create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    
    //set basic LTE attributes
    Config::SetDefault("ns3::LteUePowerControl::Pcmin", DoubleValue(23.0));
    Config::SetDefault("ns3::LteUePowerControl::Pcmax", DoubleValue(23.0));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(50));
    lteHelper->SetEnbDeviceAttribute("Mtu", UintegerValue(1500));  
    lteHelper->SetUeDeviceAttribute("Mtu", UintegerValue(1500));

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

    for(int i = 0; i < NUMBER_OF_STATIONS; i++) {
        enbDevs.Add(lteHelper->InstallEnbDevice(enbNodes.Get(i)));
    }

    ueDevs = lteHelper->InstallUeDevice(ueNodes);

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
    for(int i=0; i<UES_NUMBER; i++){
        lteHelper->Attach(ueDevs.Get (i), enbDevs.Get(findNearestStationIndexForUe(uesPositions[i], stationsPositions)));  
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
        ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(8.192)));
        ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
        clientApps.Add (ulClient.Install (ueNodes.Get(u)));
    }

    serverApps.Start (Seconds (0.01));
    clientApps.Start (Seconds (0.02));

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    clientApps.Stop(MilliSeconds(1000));
    serverApps.Stop(MilliSeconds(1000));
    Simulator::Stop(MilliSeconds(1000));

    lteHelper->EnablePhyTraces ();
    lteHelper->EnableMacTraces ();
    lteHelper->EnableRlcTraces ();
    lteHelper->EnableUlPhyTraces ();
    lteHelper->EnableUlRxPhyTraces ();
    lteHelper->EnableUlTxPhyTraces ();
    
    Simulator::Run();

    flowMonitor->SerializeToXmlFile("Results.xml", false, true);

    Simulator::Destroy();
    return 0;
}