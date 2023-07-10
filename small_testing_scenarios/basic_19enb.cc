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

void installMobility(Vector stationPosition, double cellRadius, std::string bounds, MobilityHelper ueMobility, NodeContainer ueNodes)
{
    std::string x = std::to_string(stationPosition.x);
    std::string y = std::to_string(stationPosition.y);
    std::string cellRadiusString = std::to_string(1);
    ueMobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X", StringValue(x),
                                    "Y", StringValue(y),
                                    "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusString + "]"));
    // ueMobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
    //                             "Mode", StringValue("Time"),
    //                             "Time", StringValue("2s"),
    //                             "Speed", StringValue("ns3::ConstantRandomVariable[Constant=1.0]"),
    //                             "Bounds", StringValue(bounds));
    ueMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ueMobility.Install(ueNodes);
}

std::vector<Vector> calculateStationsPosiotions(double cellRadius)
{
    std::vector<Vector> positions;
    positions.push_back(Vector(0.0, 0.0, 0.0));
    positions.push_back(Vector(2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-2 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(-cellRadius, -cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(cellRadius, -cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(4 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(3 * cellRadius, cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(2 * cellRadius, 2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(0.0, 2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(-2 * cellRadius, 2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(-3 * cellRadius, sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(-4 * cellRadius, 0.0, 0.0));
    positions.push_back(Vector(-3 * cellRadius, -cellRadius * sqrt(3), 0.0));
    positions.push_back(Vector(-2 * cellRadius, -2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(0.0, -2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(2 * cellRadius, -2 * sqrt(3) * cellRadius, 0.0));
    positions.push_back(Vector(3 * cellRadius, -cellRadius * sqrt(3), 0.0));

    NS_LOG_UNCOND("Enb's positions table:");
    NS_LOG_UNCOND("--------------------------------------------------------");

    for (int i = 0; i < positions.size(); i++)
    {
        Vector enb = positions[i];
        double x = enb.x;
        double y = enb.y;
        double z = enb.z;

        NS_LOG_UNCOND("| Enb" + std::to_string(i) + " | x: " + std::to_string(x) + " | y: " + std::to_string(y) + " | z: " + std::to_string(z) + " |");
        NS_LOG_UNCOND("--------------------------------------------------------");
    }

    NS_LOG_UNCOND(" ");

    return positions;
}

int main(int argc, char *argv[])
{
    // parameters
    int numberOfUEs = 19;
    double cellRadius = 10;
    NS_LOG_UNCOND("Number of UEs: " + std::to_string(numberOfUEs) + "; Cell Radius: " + std::to_string(cellRadius));

    // create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    lteHelper->SetPathlossModelType(TypeId::LookupByName("ns3::LogDistancePropagationLossModel"));
    lteHelper->SetPathlossModelAttribute("Exponent", DoubleValue(3.9));
    lteHelper->SetPathlossModelAttribute("ReferenceLoss", DoubleValue(38.57)); // ref. loss in dB at 1m for 2.025GHz
    lteHelper->SetPathlossModelAttribute("ReferenceDistance", DoubleValue(1));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(100));

    // create EPC helper
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);

    // create a remote host
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);

    // create internet stack helper
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    // set basic internet parameters
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.001)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    // interface 0 is localhost, 1 is the p2p device
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    // nodes containers for eNBs and UEs
    NodeContainer enbNodes;
    enbNodes.Create(19);
    NodeContainer ueNodes;
    ueNodes.Create(numberOfUEs);

    // lists of devices positions
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();
    // Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator>();

    // putting values of coordinates to simulation position array
    std::vector<Vector> stationsPositions = calculateStationsPosiotions(cellRadius);
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            enbPositionAlloc->Add(stationsPositions[i]);
        }
    }

    // set mobility parameters

    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(enbPositionAlloc);
    enbMobility.Install(enbNodes);

    MobilityHelper ueMobility;
    std::vector<NodeContainer> ueMobilityVector;
    NodeContainer ueNodes0;
    NodeContainer ueNodes1;
    NodeContainer ueNodes2;
    NodeContainer ueNodes3;
    NodeContainer ueNodes4;
    NodeContainer ueNodes5;
    NodeContainer ueNodes6;
    NodeContainer ueNodes7;
    NodeContainer ueNodes8;
    NodeContainer ueNodes9;
    NodeContainer ueNodes10;
    NodeContainer ueNodes11;
    NodeContainer ueNodes12;
    NodeContainer ueNodes13;
    NodeContainer ueNodes14;
    NodeContainer ueNodes15;
    NodeContainer ueNodes16;
    NodeContainer ueNodes17;
    NodeContainer ueNodes18;

    for (int i = 0; i < numberOfUEs; i++)
    {
        if (i % 19 == 0)
        {
            ueNodes0.Add(ueNodes.Get(i));
        }
        if (i % 19 == 1)
        {
            ueNodes1.Add(ueNodes.Get(i));
        }
        if (i % 19 == 2)
        {
            ueNodes2.Add(ueNodes.Get(i));
        }
        if (i % 19 == 3)
        {
            ueNodes3.Add(ueNodes.Get(i));
        }
        if (i % 19 == 4)
        {
            ueNodes4.Add(ueNodes.Get(i));
        }
        if (i % 19 == 5)
        {
            ueNodes5.Add(ueNodes.Get(i));
        }
        if (i % 19 == 6)
        {
            ueNodes6.Add(ueNodes.Get(i));
        }
        if (i % 19 == 7)
        {
            ueNodes7.Add(ueNodes.Get(i));
        }
        if (i % 19 == 8)
        {
            ueNodes8.Add(ueNodes.Get(i));
        }
        if (i % 19 == 9)
        {
            ueNodes9.Add(ueNodes.Get(i));
        }
        if (i % 19 == 10)
        {
            ueNodes10.Add(ueNodes.Get(i));
        }
        if (i % 19 == 11)
        {
            ueNodes11.Add(ueNodes.Get(i));
        }
        if (i % 19 == 12)
        {
            ueNodes12.Add(ueNodes.Get(i));
        }
        if (i % 19 == 13)
        {
            ueNodes13.Add(ueNodes.Get(i));
        }
        if (i % 19 == 14)
        {
            ueNodes14.Add(ueNodes.Get(i));
        }
        if (i % 19 == 15)
        {
            ueNodes15.Add(ueNodes.Get(i));
        }
        if (i % 19 == 16)
        {
            ueNodes16.Add(ueNodes.Get(i));
        }
        if (i % 19 == 17)
        {
            ueNodes17.Add(ueNodes.Get(i));
        }
        if (i % 19 == 18)
        {
            ueNodes18.Add(ueNodes.Get(i));
        }
    }
    ueMobilityVector.push_back(ueNodes0);
    ueMobilityVector.push_back(ueNodes1);
    ueMobilityVector.push_back(ueNodes2);
    ueMobilityVector.push_back(ueNodes3);
    ueMobilityVector.push_back(ueNodes4);
    ueMobilityVector.push_back(ueNodes5);
    ueMobilityVector.push_back(ueNodes6);
    ueMobilityVector.push_back(ueNodes7);
    ueMobilityVector.push_back(ueNodes8);
    ueMobilityVector.push_back(ueNodes9);
    ueMobilityVector.push_back(ueNodes10);
    ueMobilityVector.push_back(ueNodes11);
    ueMobilityVector.push_back(ueNodes12);
    ueMobilityVector.push_back(ueNodes13);
    ueMobilityVector.push_back(ueNodes14);
    ueMobilityVector.push_back(ueNodes15);
    ueMobilityVector.push_back(ueNodes16);
    ueMobilityVector.push_back(ueNodes17);
    ueMobilityVector.push_back(ueNodes18);

    std::string bounds = std::to_string(-5 * cellRadius) + "|" + std::to_string(5 * cellRadius) + "|" + std::to_string(-5 * cellRadius) + "|" + std::to_string(5 * cellRadius);

    for (int i = 0; i < 19; i++)
    {
        Vector stationPosition = stationsPositions[i];
        installMobility(stationPosition, cellRadius, bounds, ueMobility, ueMobilityVector[i]);
    }

    // instal devices
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;

    for (int i = 0; i < 19; i++)
    {
        enbDevs.Add(lteHelper->InstallEnbDevice(enbNodes.Get(i)));
    }

    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    // install the IP stack on the UEs
    internet.Install(ueNodes);
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs));

    // assign IP address to UEs, and install applications
    for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes.Get(u);
        // set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    // attach UEs to eNBs
    for (int i = 0; i < numberOfUEs; i++)
    {
        lteHelper->Attach(ueDevs.Get(i));
    }

    // install application
    uint16_t ulPort = 2000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;
    for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
    {
        ++ulPort;
        PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), ulPort));
        serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

        UdpClientHelper ulClient(remoteHostAddr, ulPort);
        ulClient.SetAttribute("Interval", TimeValue(MilliSeconds(8.192)));
        ulClient.SetAttribute("MaxPackets", UintegerValue(1000000));
        clientApps.Add(ulClient.Install(ueNodes.Get(u)));
    }

    serverApps.Start(MilliSeconds(1000));
    clientApps.Start(MilliSeconds(1000));

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    serverApps.Stop(MilliSeconds(2000));
    clientApps.Stop(MilliSeconds(2000));
    Simulator::Stop(MilliSeconds(4000));

    lteHelper->EnablePhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();
    lteHelper->EnableUlPhyTraces();
    lteHelper->EnableUlRxPhyTraces();
    lteHelper->EnableUlTxPhyTraces();

    Simulator::Run();

    flowMonitor->SerializeToXmlFile("Results.xml", false, true);
    AsciiTraceHelper ascii;
    MobilityHelper::EnableAsciiAll(ascii.CreateFileStream("mobility-trace-example.mob"));

    Simulator::Destroy();
    return 0;
}