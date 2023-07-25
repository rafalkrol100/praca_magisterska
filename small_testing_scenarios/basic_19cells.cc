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
#include "ns3/config-store.h"

using namespace ns3;
int main(int argc, char *argv[])
{
    // parameters
    int numberOfUEsPerCell = 2;
    double cellRadius = 15;
    NS_LOG_UNCOND("basic_19cells.cc | Number of UEs per cell: " + std::to_string(numberOfUEsPerCell) + "; Cell Radius: " + std::to_string(cellRadius));

    // create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    lteHelper->SetPathlossModelType(TypeId::LookupByName("ns3::LogDistancePropagationLossModel"));
    lteHelper->SetPathlossModelAttribute("Exponent", DoubleValue(3.9));
    lteHelper->SetPathlossModelAttribute("ReferenceLoss", DoubleValue(38.57)); // ref. loss in dB at 1m for 2.025GHz
    lteHelper->SetPathlossModelAttribute("ReferenceDistance", DoubleValue(1));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(75));

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
    ueNodes.Create(19 * numberOfUEsPerCell);

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
    NodeContainer ueNodes19;

    for (int i = 0; i < (19 * numberOfUEsPerCell); i++)
    {
        if (i % 19 == 0)
        {
            ueNodes1.Add(ueNodes.Get(i));
        }
        if (i % 19 == 1)
        {
            ueNodes2.Add(ueNodes.Get(i));
        }
        if (i % 19 == 2)
        {
            ueNodes3.Add(ueNodes.Get(i));
        }
        if (i % 19 == 3)
        {
            ueNodes4.Add(ueNodes.Get(i));
        }
        if (i % 19 == 4)
        {
            ueNodes5.Add(ueNodes.Get(i));
        }
        if (i % 19 == 5)
        {
            ueNodes6.Add(ueNodes.Get(i));
        }
        if (i % 19 == 6)
        {
            ueNodes7.Add(ueNodes.Get(i));
        }
        if (i % 19 == 7)
        {
            ueNodes8.Add(ueNodes.Get(i));
        }
        if (i % 19 == 8)
        {
            ueNodes9.Add(ueNodes.Get(i));
        }
        if (i % 19 == 9)
        {
            ueNodes10.Add(ueNodes.Get(i));
        }
        if (i % 19 == 10)
        {
            ueNodes11.Add(ueNodes.Get(i));
        }
        if (i % 19 == 11)
        {
            ueNodes12.Add(ueNodes.Get(i));
        }
        if (i % 19 == 12)
        {
            ueNodes13.Add(ueNodes.Get(i));
        }
        if (i % 19 == 13)
        {
            ueNodes14.Add(ueNodes.Get(i));
        }
        if (i % 19 == 14)
        {
            ueNodes15.Add(ueNodes.Get(i));
        }
        if (i % 19 == 15)
        {
            ueNodes16.Add(ueNodes.Get(i));
        }
        if (i % 19 == 16)
        {
            ueNodes17.Add(ueNodes.Get(i));
        }
        if (i % 19 == 17)
        {
            ueNodes18.Add(ueNodes.Get(i));
        }
        if (i % 19 == 18)
        {
            ueNodes19.Add(ueNodes.Get(i));
        }
    }

    // lists of devices positions
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();
    // Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator>();

    // putting values of coordinates to simulation position array
    enbPositionAlloc->Add(Vector(0.0, 0.0, 0.0));
    enbPositionAlloc->Add(Vector(2 * cellRadius, 0.0, 0.0));
    enbPositionAlloc->Add(Vector(cellRadius, cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(-cellRadius, cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(-2 * cellRadius, 0.0, 0.0));
    enbPositionAlloc->Add(Vector(-cellRadius, -cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(cellRadius, -cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(4 * cellRadius, 0.0, 0.0));
    enbPositionAlloc->Add(Vector(3 * cellRadius, cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(2 * cellRadius, 2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(0.0, 2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(-2 * cellRadius, 2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(-3 * cellRadius, sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(-4 * cellRadius, 0.0, 0.0));
    enbPositionAlloc->Add(Vector(-3 * cellRadius, -cellRadius * sqrt(3), 0.0));
    enbPositionAlloc->Add(Vector(-2 * cellRadius, -2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(0.0, -2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(2 * cellRadius, -2 * sqrt(3) * cellRadius, 0.0));
    enbPositionAlloc->Add(Vector(3 * cellRadius, -cellRadius * sqrt(3), 0.0));

    // set mobility parameters
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(enbPositionAlloc);
    enbMobility.Install(enbNodes);

    std::ofstream MyFile("enb_coordinates.txt");
    for (int i = 0; i < 19; i++)
    {
        MyFile << enbNodes.Get(i)->GetObject<MobilityModel>()->GetPosition() << std::endl;
    }

    MyFile.close();

    MobilityHelper ueMobility1;
    ueMobility1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility2;
    ueMobility2.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility3;
    ueMobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility4;
    ueMobility4.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility5;
    ueMobility5.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility6;
    ueMobility6.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility7;
    ueMobility7.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility8;
    ueMobility8.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility9;
    ueMobility9.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility10;
    ueMobility10.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility11;
    ueMobility11.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility12;
    ueMobility12.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility13;
    ueMobility13.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility14;
    ueMobility14.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility15;
    ueMobility15.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility16;
    ueMobility16.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility17;
    ueMobility17.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility18;
    ueMobility18.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    MobilityHelper ueMobility19;
    ueMobility19.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    // ueMobility.SetPositionAllocator(uePositionAlloc);
    std::string cellRadiusStringMultiplied = std::to_string(cellRadius*0.3);
    std::string cellRadiusString = std::to_string(cellRadius);
    std::string cellRadiusString2 = std::to_string(2 * cellRadius);
    std::string cellRadiusString3 = std::to_string(3 * cellRadius);
    std::string cellRadiusString4 = std::to_string(4 * cellRadius);
    std::string cellRadiusStringSqrt3 = std::to_string(cellRadius * sqrt(3));
    std::string cellRadiusString2Sqrt3 = std::to_string(2 * cellRadius * sqrt(3));
    std::string cellRadiusStringMinus = std::to_string(-cellRadius);
    std::string cellRadiusString2Minus = std::to_string(-2 * cellRadius);
    std::string cellRadiusString3Minus = std::to_string(-3 * cellRadius);
    std::string cellRadiusString4Minus = std::to_string(-4 * cellRadius);
    std::string cellRadiusStringSqrt3Minus = std::to_string(-cellRadius * sqrt(3));
    std::string cellRadiusString2Sqrt3Minus = std::to_string(-2 * cellRadius * sqrt(3));

    ueMobility1.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue("0.0"),
                                     "Y", StringValue("0.0"),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility2.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString2),
                                     "Y", StringValue("0.0"),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility3.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString),
                                     "Y", StringValue(cellRadiusStringSqrt3),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility4.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusStringMinus),
                                     "Y", StringValue(cellRadiusStringSqrt3),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility5.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString2Minus),
                                     "Y", StringValue("0.0"),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility6.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusStringMinus),
                                     "Y", StringValue(cellRadiusStringSqrt3Minus),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility7.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString),
                                     "Y", StringValue(cellRadiusStringSqrt3Minus),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility8.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString4),
                                     "Y", StringValue("0.0"),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility9.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X", StringValue(cellRadiusString3),
                                     "Y", StringValue(cellRadiusStringSqrt3),
                                     "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility10.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString2),
                                      "Y", StringValue(cellRadiusString2Sqrt3),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility11.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue("0.0"),
                                      "Y", StringValue(cellRadiusString2Sqrt3),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility12.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString2Minus),
                                      "Y", StringValue(cellRadiusString2Sqrt3),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility13.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString3Minus),
                                      "Y", StringValue(cellRadiusStringSqrt3),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility14.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString4Minus),
                                      "Y", StringValue("0.0"),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility15.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString3Minus),
                                      "Y", StringValue(cellRadiusStringSqrt3Minus),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility16.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString2Minus),
                                      "Y", StringValue(cellRadiusString2Sqrt3Minus),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility17.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue("0.0"),
                                      "Y", StringValue(cellRadiusString2Sqrt3Minus),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility18.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString2),
                                      "Y", StringValue(cellRadiusString2Sqrt3Minus),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility19.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                      "X", StringValue(cellRadiusString3),
                                      "Y", StringValue(cellRadiusStringSqrt3Minus),
                                      "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusStringMultiplied + "]"));

    ueMobility1.Install(ueNodes1);
    ueMobility2.Install(ueNodes2);
    ueMobility3.Install(ueNodes3);
    ueMobility4.Install(ueNodes4);
    ueMobility5.Install(ueNodes5);
    ueMobility6.Install(ueNodes6);
    ueMobility7.Install(ueNodes7);
    ueMobility8.Install(ueNodes8);
    ueMobility9.Install(ueNodes9);
    ueMobility10.Install(ueNodes10);
    ueMobility11.Install(ueNodes11);
    ueMobility12.Install(ueNodes12);
    ueMobility13.Install(ueNodes13);
    ueMobility14.Install(ueNodes14);
    ueMobility15.Install(ueNodes15);
    ueMobility16.Install(ueNodes16);
    ueMobility17.Install(ueNodes17);
    ueMobility18.Install(ueNodes18);
    ueMobility19.Install(ueNodes19);

    std::ofstream MyFile1("ue_coordinates.txt");
    for (int i = 0; i < 19 * numberOfUEsPerCell; i++)
    {
        MyFile1 << ueNodes.Get(i)->GetObject<MobilityModel>()->GetPosition() << std::endl;
    }
    MyFile1.close();

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
    for (int i = 0; i < (numberOfUEsPerCell * 19); i++)
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

    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();
    lteHelper->EnableUlPhyTraces();
    lteHelper->EnableUlRxPhyTraces();
    lteHelper->EnableUlTxPhyTraces();

    Simulator::Run();

    flowMonitor->SerializeToXmlFile("Results.xml", false, true);

    Simulator::Destroy();
    return 0;
}