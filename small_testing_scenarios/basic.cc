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

int main(int argc, char *argv[])
{
    // parameters
    int numberOfUEs = 20;
    double cellRadius = 5;
    NS_LOG_UNCOND("Number of UEs: " + std::to_string(numberOfUEs) + "; Cell Radius: " + std::to_string(cellRadius));


    // create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    lteHelper->SetPathlossModelType(TypeId::LookupByName("ns3::LogDistancePropagationLossModel"));
    lteHelper->SetPathlossModelAttribute("Exponent", DoubleValue(3.9));
    lteHelper->SetPathlossModelAttribute("ReferenceLoss", DoubleValue(38.57)); // ref. loss in dB at 1m for 2.025GHz
    lteHelper->SetPathlossModelAttribute("ReferenceDistance", DoubleValue(1));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(6));

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
    enbNodes.Create(1);
    NodeContainer ueNodes;
    ueNodes.Create(numberOfUEs);

    // lists of devices positions
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();
    //Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator>();

    // putting values of coordinates to simulation position array
    for (int i = 0; i < 1; i++)
    {
        enbPositionAlloc->Add(Vector(0.0, 0.0, 0.0));
    }

    // set mobility parameters
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(enbPositionAlloc);
    enbMobility.Install(enbNodes);

    MobilityHelper ueMobility;
    ueMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    //ueMobility.SetPositionAllocator(uePositionAlloc);
    std::string cellRadiusString = std::to_string(cellRadius);
    ueMobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X", StringValue("0.0"),
                                    "Y", StringValue("0.0"),
                                    "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusString + "]"));

    ueMobility.Install(ueNodes);

    // instal devices
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;

    for (int i = 0; i < 1; i++)
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