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

static void
CourseChange(std::string foo, Ptr<const MobilityModel> mobility)
{
    Vector pos = mobility->GetPosition();
    Vector vel = mobility->GetVelocity();
    std::cout << Simulator::Now() << ", model=" << mobility << ", POS: x=" << pos.x << ", y=" << pos.y
              << ", z=" << pos.z << "; VEL: x=" << vel.x << ", y=" << vel.y
              << ", z=" << vel.z << std::endl;
}

void NotifyConnectionEstablishedUe(std::string context,
                                   uint64_t imsi,
                                   uint16_t cellid,
                                   uint16_t rnti)
{
    std::cout << context
              << " UE IMSI " << imsi
              << ": connected to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
}

void NotifyHandoverStartUe(std::string context,
                           uint64_t imsi,
                           uint16_t cellid,
                           uint16_t rnti,
                           uint16_t targetCellId)
{
    std::cout << context
              << " UE IMSI " << imsi
              << ": previously connected to CellId " << cellid
              << " with RNTI " << rnti
              << ", doing handover to CellId " << targetCellId
              << std::endl;
}

void NotifyHandoverEndOkUe(std::string context,
                           uint64_t imsi,
                           uint16_t cellid,
                           uint16_t rnti)
{
    std::cout << context
              << " UE IMSI " << imsi
              << ": successful handover to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
}

void NotifyConnectionEstablishedEnb(std::string context,
                                    uint64_t imsi,
                                    uint16_t cellid,
                                    uint16_t rnti)
{
    std::cout << context
              << " eNB CellId " << cellid
              << ": successful connection of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
}

void NotifyHandoverStartEnb(std::string context,
                            uint64_t imsi,
                            uint16_t cellid,
                            uint16_t rnti,
                            uint16_t targetCellId)
{
    std::cout << context
              << " eNB CellId " << cellid
              << ": start handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << " to CellId " << targetCellId
              << std::endl;
}

void NotifyHandoverEndOkEnb(std::string context,
                            uint64_t imsi,
                            uint16_t cellid,
                            uint16_t rnti)
{
    std::cout << context
              << " eNB CellId " << cellid
              << ": completed handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
}

std::vector<Vector> calculateStationsPosiotions(double cellRadius, int numberOfLayers)
{
    std::vector<Vector> positions;

    if (numberOfLayers == 1)
    {
        positions.push_back(Vector(0.0, 0.0, 0.0));
    }
    else if (numberOfLayers == 2)
    {
        positions.push_back(Vector(0.0, 0.0, 0.0));
        positions.push_back(Vector(2 * cellRadius, 0.0, 0.0));
        positions.push_back(Vector(cellRadius, cellRadius * sqrt(3), 0.0));
        positions.push_back(Vector(-cellRadius, cellRadius * sqrt(3), 0.0));
        positions.push_back(Vector(-2 * cellRadius, 0.0, 0.0));
        positions.push_back(Vector(-cellRadius, -cellRadius * sqrt(3), 0.0));
        positions.push_back(Vector(cellRadius, -cellRadius * sqrt(3), 0.0));
    }
    else if (numberOfLayers == 3)
    {
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
    }

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

uint32_t findNearestStationIndexForUe(Vector uePosition, std::vector<Vector> stationsPositions, int index)
{
    int shortestDistance = 9999999;
    int indexOfNearestStation = 0;

    for (int i = 0; i < stationsPositions.size(); i++)
    {
        double distance = ns3::CalculateDistance(uePosition, stationsPositions[i]);
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            indexOfNearestStation = i;
        }
    }
    double ueX = uePosition.x;
    double ueY = uePosition.y;
    double ueZ = uePosition.z;

    NS_LOG_UNCOND("Ue" + std::to_string(index) + " (" + std::to_string(ueX) + ", " + std::to_string(ueY) + ", " + std::to_string(ueZ) + ") " + "was attached to enb" + std::to_string(indexOfNearestStation) + ". Distance between them is: " + std::to_string(shortestDistance));
    NS_LOG_UNCOND("----------------------------------------------------------------------------------------");
    return indexOfNearestStation;
}

void installMobility(Vector stationPosition, double cellRadius, std::string bounds, MobilityHelper ueMobility, NodeContainer ueNodes)
{
    std::string x = std::to_string(stationPosition.x);
    std::string y = std::to_string(stationPosition.y);
    std::string cellRadiusString = std::to_string(cellRadius);
    ueMobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X", StringValue(x),
                                    "Y", StringValue(y),
                                    "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=" + cellRadiusString + "]"));
    ueMobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                                "Mode", StringValue("Time"),
                                "Time", StringValue("2s"),
                                "Speed", StringValue("ns3::ConstantRandomVariable[Constant=1.0]"),
                                "Bounds", StringValue(bounds));
    ueMobility.Install(ueNodes);
}

void handler()
{
}

int main(int argc, char *argv[])
{
    // parameters
    uint32_t NUMBER_OF_UES = 7;
    uint32_t NUMBER_OF_LAYERS = 3;
    uint32_t NUMBER_OF_STATIONS = 1 + (6 * (NUMBER_OF_LAYERS - 1)) + (uint32_t)(NUMBER_OF_LAYERS / 3) * 6;
    double cellRadius = 6000.0;
    uint32_t NUMBER_OF_SECTORS = 6;

    // create LTE helper
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    // set basic LTE attributes
    Config::SetDefault("ns3::LteUePowerControl::Pcmin", DoubleValue(23.0));
    Config::SetDefault("ns3::LteUePowerControl::Pcmax", DoubleValue(23.0));
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(50));
    lteHelper->SetEnbDeviceAttribute("Mtu", UintegerValue(1500));
    lteHelper->SetUeDeviceAttribute("Mtu", UintegerValue(1500));

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
    enbNodes.Create(NUMBER_OF_STATIONS * NUMBER_OF_SECTORS);
    NodeContainer ueNodes;
    ueNodes.Create(NUMBER_OF_UES);

    // lists of devices positions
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();

    // putting values of coordinates to simulation position array
    std::vector<Vector> stationsPositions = calculateStationsPosiotions(cellRadius, NUMBER_OF_LAYERS);
    for (int i = 0; i < NUMBER_OF_STATIONS; i++)
    {
        for (int j = 0; j < NUMBER_OF_SECTORS; j++)
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

    for (int i = 0; i < NUMBER_OF_UES; i++)
    {
        if (NUMBER_OF_LAYERS == 1)
        {
            ueNodes0.Add(ueNodes.Get(i));
        }
        if (NUMBER_OF_LAYERS == 2)
        {
            if (i % 7 == 0)
            {
                ueNodes0.Add(ueNodes.Get(i));
            }
            if (i % 7 == 1)
            {
                ueNodes1.Add(ueNodes.Get(i));
            }
            if (i % 7 == 2)
            {
                ueNodes2.Add(ueNodes.Get(i));
            }
            if (i % 7 == 3)
            {
                ueNodes3.Add(ueNodes.Get(i));
            }
            if (i % 7 == 4)
            {
                ueNodes4.Add(ueNodes.Get(i));
            }
            if (i % 7 == 5)
            {
                ueNodes5.Add(ueNodes.Get(i));
            }
            if (i % 7 == 6)
            {
                ueNodes6.Add(ueNodes.Get(i));
            }
        }
        if (NUMBER_OF_LAYERS == 3)
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
    }

    if (NUMBER_OF_LAYERS == 1)
    {
        ueMobilityVector.push_back(ueNodes0);
    }
    if (NUMBER_OF_LAYERS == 2)
    {
        ueMobilityVector.push_back(ueNodes0);
        ueMobilityVector.push_back(ueNodes1);
        ueMobilityVector.push_back(ueNodes2);
        ueMobilityVector.push_back(ueNodes3);
        ueMobilityVector.push_back(ueNodes4);
        ueMobilityVector.push_back(ueNodes5);
        ueMobilityVector.push_back(ueNodes6);
    }
    if (NUMBER_OF_LAYERS == 3)
    {
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
    }

    int modifier = 0;
    if (NUMBER_OF_LAYERS == 1)
    {
        modifier = 1;
    }
    else if (NUMBER_OF_LAYERS == 2)
    {
        modifier = 3;
    }
    else if (NUMBER_OF_LAYERS == 3)
    {
        modifier = 5;
    }

    std::string bounds = std::to_string(-modifier * cellRadius) + "|" + std::to_string(modifier * cellRadius) + "|" + std::to_string(-modifier * cellRadius) + "|" + std::to_string(modifier * cellRadius);

    for (int i = 0; i < NUMBER_OF_STATIONS; i++)
    {
        Vector stationPosition = stationsPositions[i];

        installMobility(stationPosition, cellRadius, bounds, ueMobility, ueMobilityVector[i]);
    }

    // instal devices
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;

    // set up 6 sector enbs
    NS_LOG_UNCOND("Cells configuration: ");
    NS_LOG_UNCOND("----------------------------------------------------------------------------------------");
    for (int i = 0; i < NUMBER_OF_STATIONS; i++)
    {
        NS_LOG_UNCOND("Enb" + std::to_string(i) + ": ");

        for (int j = 0; j < NUMBER_OF_SECTORS; j++)
        {
            NS_LOG_UNCOND("    Sector: " + std::to_string(j));
            // set up strict frequency reuse model
            lteHelper->SetFfrAlgorithmType("ns3::LteFrStrictAlgorithm");
            lteHelper->SetFfrAlgorithmAttribute("DlCommonSubBandwidth", UintegerValue(6));
            lteHelper->SetFfrAlgorithmAttribute("UlCommonSubBandwidth", UintegerValue(6));
            lteHelper->SetFfrAlgorithmAttribute("DlEdgeSubBandOffset", UintegerValue(6));
            lteHelper->SetFfrAlgorithmAttribute("DlEdgeSubBandwidth", UintegerValue(6));
            lteHelper->SetFfrAlgorithmAttribute("UlEdgeSubBandOffset", UintegerValue(6));
            lteHelper->SetFfrAlgorithmAttribute("UlEdgeSubBandwidth", UintegerValue(6));

            std::string antennaModel = "ns3::CosineAntennaModel";
            double orientation = 0 + j * (360/NUMBER_OF_SECTORS);
            double horizontalBeamwidth = 360/NUMBER_OF_SECTORS;
            double maxGain = 0.0;
            int enbNodeIndex = i * NUMBER_OF_SECTORS + j;

            NS_LOG_UNCOND("        Antenna Model: " + antennaModel);
            NS_LOG_UNCOND("        Orientation: " + std::to_string(orientation));
            NS_LOG_UNCOND("        Horizontal Beam Width: " + std::to_string(horizontalBeamwidth));
            NS_LOG_UNCOND("        Max Gain: " + std::to_string(maxGain));
            NS_LOG_UNCOND("        Enb Node Index: " + std::to_string(enbNodeIndex));

            lteHelper->SetEnbAntennaModelType(antennaModel);
            lteHelper->SetEnbAntennaModelAttribute("Orientation", DoubleValue(orientation));
            lteHelper->SetEnbAntennaModelAttribute("HorizontalBeamwidth", DoubleValue(horizontalBeamwidth));
            lteHelper->SetEnbAntennaModelAttribute("MaxGain", DoubleValue(maxGain));

            enbDevs.Add(lteHelper->InstallEnbDevice(enbNodes.Get(enbNodeIndex)));
            Ptr<Node> enb = enbNodes.Get(enbNodeIndex);
            Ptr<NetDevice> enbLteDev = enb->GetDevice(0);
            Ptr<LteEnbNetDevice> enbLteDevice = enbLteDev->GetObject<LteEnbNetDevice>();
            uint16_t enbCellId = enbLteDevice->GetCellId();
            NS_LOG_UNCOND("        Cell Id: " + std::to_string(enbCellId));
        }
        NS_LOG_UNCOND("----------------------------------------------------------------------------------------");
    }

    // ns3::LteFrStrictAlgorithm works with Absolute Mode Uplink Power Control
    Config::SetDefault("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue(false));

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
    // std::vector<Vector> uesPositions = calculateUesPosiotions(cellRadius, NUMBER_OF_UES);
    for (int i = 0; i < NUMBER_OF_UES; i++)
    {
        // lteHelper->Attach(ueDevs.Get (i), enbDevs.Get(findNearestStationIndexForUe(uesPositions[i], stationsPositions, i)*6));
        lteHelper->Attach(ueDevs.Get(i));
    }

    // install application
    uint16_t dlPort = 1100;
    uint16_t ulPort = 2000;
    uint16_t otherPort = 3000;
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

    serverApps.Start(Seconds(0));
    clientApps.Start(Seconds(5));

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    clientApps.Stop(MilliSeconds(1000));
    serverApps.Stop(MilliSeconds(5000));
    Simulator::Stop(MilliSeconds(5000));

    lteHelper->EnablePhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();
    lteHelper->EnableUlPhyTraces();
    lteHelper->EnableUlRxPhyTraces();
    lteHelper->EnableUlTxPhyTraces();

    // connect custom trace sinks for RRC connection establishment and handover notification
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished", MakeCallback (&NotifyConnectionEstablishedEnb));
    Config::Connect("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished", MakeCallback(&NotifyConnectionEstablishedUe));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart", MakeCallback (&NotifyHandoverStartEnb));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart", MakeCallback (&NotifyHandoverStartUe));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk", MakeCallback (&NotifyHandoverEndOkEnb));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk", MakeCallback (&NotifyHandoverEndOkUe));

    Config::Connect("/NodeList/*/$ns3::MobilityModel/CourseChange", MakeCallback(&CourseChange));

    AsciiTraceHelper ascii;
    MobilityHelper::EnableAsciiAll(ascii.CreateFileStream("mobility-trace-example.mob"));

    Simulator::Run();

    flowMonitor->SerializeToXmlFile("Results.xml", false, true);

    Simulator::Destroy();
    return 0;
}