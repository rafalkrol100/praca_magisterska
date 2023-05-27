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

std::vector<Vector> calculateStationsPosiotions(double cellRadius, bool isLogEnabled) {
    std::vector<Vector> positions;
    positions.push_back(Vector(0.0, 0.0, 0.0));
    positions.push_back(Vector(2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(-cellRadius, -cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(cellRadius, -cellRadius * sqrt(3), 0.0));

    if(isLogEnabled) {
        NS_LOG_UNCOND("Enb's positions table:");
        NS_LOG_UNCOND("--------------------------------------------------------");
    }
    
    for(int i = 0; i < positions.size(); i++) {
        Vector enb = positions[i];
        double x = enb.x;
        double y = enb.y;
        double z = enb.z;
        
        if(isLogEnabled) {
            NS_LOG_UNCOND("| Enb" + std::to_string(i) + " | x: " + std::to_string(x) + " | y: " + std::to_string(y) + " | z: " + std::to_string(z) + " |");
            NS_LOG_UNCOND("--------------------------------------------------------");
        } 
    }
    
    if(isLogEnabled) {
        NS_LOG_UNCOND(" ");
    } 
    
    return positions;
}

std::vector<Vector> calculateUesPosiotions(double cellRadius, int numberOfUes) {
    std::vector<Vector> positions;
    std::vector<Vector> enbPositions = calculateStationsPosiotions(cellRadius, true);
    const long max_rand = 1000000L;
    srandom(time(NULL));

    NS_LOG_UNCOND("User equipment positions table:");
    NS_LOG_UNCOND("--------------------------------------------------------");

    for(int i = 0; i < numberOfUes; i++) {
        int enbIndex = i % 7;
        Vector enbPosition = enbPositions[enbIndex];
        double enbX = enbPosition.x;
        double enbY = enbPosition.y;
        //bound parameter is used to decrease randomizer bounds to assure that every position is inside of cell radius
        double bound = (cellRadius*sqrt(2))/2;

        //double random_double = lower_bound + (upper_bound - lower_bound) * (random() % max_rand) / max_rand;
        double x = (enbX - bound) + 2 * bound * (random() % max_rand) / max_rand;
        double y = (enbY - bound) + 2 * bound * (random() % max_rand) / max_rand;
        double z = 0.0;
        positions.push_back(Vector(x, y, z));

        NS_LOG_UNCOND("| Ue" + std::to_string(i) + " | x: " + std::to_string(x) + " | y: " + std::to_string(y) + " | z: " + std::to_string(z) + " |");
        NS_LOG_UNCOND("--------------------------------------------------------");
    }

    NS_LOG_UNCOND(" ");

    return positions;
}

uint32_t findNearestStationIndexForUe(Vector uePosition, std::vector<Vector> stationsPositions, int index) {
    int shortestDistance = 9999999;
    int indexOfNearestStation = 0;

    for(int i = 0; i < stationsPositions.size(); i++) {
        double distance = ns3::CalculateDistance(uePosition, stationsPositions[i]);
        if (distance < shortestDistance) {
            shortestDistance = distance;
            indexOfNearestStation = i;
        }
    }
    double ueX = uePosition.x;
    double ueY = uePosition.y;
    double ueZ = uePosition.z;

    NS_LOG_UNCOND("Ue" + std::to_string(index) + " (" + std::to_string(ueX) + ", " + std::to_string(ueY) + ", " + std::to_string(ueZ) + ") " + "was attached to enb" + std::to_string(indexOfNearestStation) + ". Distance between them is: " + std::to_string(shortestDistance));
    NS_LOG_UNCOND("--------------------------------------------------------");
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
    std::vector<Vector> stationsPositions = calculateStationsPosiotions(cellRadius, false);
    for(int i = 0; i < NUMBER_OF_STATIONS; i++){
        enbPositionAlloc -> Add(stationsPositions[i]);
    }
    
    std::vector<Vector> uesPositions = calculateUesPosiotions(cellRadius, NUMBER_OF_UES);
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
    NS_LOG_UNCOND("Nearest station finding results:");
    NS_LOG_UNCOND("----------------------------------------------------------------------------------------");
    for(int i = 0; i < NUMBER_OF_UES; i++){
        lteHelper->Attach(ueDevs.Get (i), enbDevs.Get(findNearestStationIndexForUe(uesPositions[i], stationsPositions, i)));  
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