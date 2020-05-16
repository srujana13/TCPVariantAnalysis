#include <string>
#include <fstream>
#include <cmath>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("TcpBulkSendExample");

// Function to calculate standard deviation
float calculate_sd(float a, float b, float c)
{
  float mean = (a+b+c)/3;
  float summation = pow(a-mean,2) + pow(b-mean, 2) + pow(c-mean, 2);
  float sd = sqrt(summation/3);
  return sd;
}

int
main(int argc, char const *argv[]) {

    // Creating nodes 
    NodeContainer c; 
    c.Create(6);

    // Creating containers for each link of nodes 
    NodeContainer n0n4 = NodeContainer (c.Get (0), c.Get (4));
    NodeContainer n1n4 = NodeContainer (c.Get (1), c.Get (4));
    NodeContainer n2n5 = NodeContainer (c.Get (2), c.Get (5));
    NodeContainer n3n5 = NodeContainer (c.Get (3), c.Get (5));
    NodeContainer n4n5 = NodeContainer (c.Get (4), c.Get (5));

    // installing the internet stack 
    InternetStackHelper internet;
    internet.Install (c);

    // Adding channel attributes 
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));

    NetDeviceContainer d0d4 = p2p.Install (n0n4);
    NetDeviceContainer d1d4 = p2p.Install (n1n4);
    NetDeviceContainer d4d5 = p2p.Install (n4n5);
    NetDeviceContainer d2d5 = p2p.Install (n2n5);
    NetDeviceContainer d3d5 = p2p.Install (n3n5);

    // Setting Ip Addresses 
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i4 = ipv4.Assign (d0d4);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);

    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);

    ipv4.SetBase ("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i5 = ipv4.Assign (d2d5);

    ipv4.SetBase ("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer i3i5 = ipv4.Assign (d3d5);


    // Enabling global routing for easy identification of packages 
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    // using port 80 for bulk sender 
    uint16_t port_1 = 80;

    float time_in_secs = 0.0;

// Setting type ids for easy modification 
TypeId TcpCubicId = TypeId::LookupByName ("ns3::TcpBic");
TypeId TcpDctcpId = TypeId::LookupByName("ns3::TcpDctcp");


std::stringstream nodeId_1, nodeId_2;
nodeId_1 << n0n4.Get (0)->GetId ();
nodeId_2 << n1n4.Get(0) -> GetId();

std::string sender1 = "/NodeList/" + nodeId_1.str () + "/$ns3::TcpL4Protocol/SocketType";
std::string sender2= "/NodeList/" + nodeId_2.str () + "/$ns3::TcpL4Protocol/SocketType";

// Experiment-1

// Setting Tcp Variant to Cubic 
Config::Set (sender1, TypeIdValue (TcpCubicId));

for(int i =0; i < 3; i++)
{
    BulkSendHelper S1_source ("ns3::TcpSocketFactory",
                          InetSocketAddress (i2i5.GetAddress (0), port_1));
   // Set the amount of data to send in bytes.  Zero is unlimited.
   S1_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
   ApplicationContainer S1_App = S1_source.Install (n0n4.Get (0));
   S1_App.Start (Seconds (time_in_secs));
   S1_App.Stop (Seconds (time_in_secs + 10.0));
   PacketSinkHelper D1_sink ("ns3::TcpSocketFactory",
                          InetSocketAddress (Ipv4Address::GetAny (), port_1));
   ApplicationContainer D1_App = D1_sink.Install (n2n5.Get (0));
   D1_App.Start (Seconds (time_in_secs));
   D1_App.Stop (Seconds (time_in_secs + 10.0));

time_in_secs += 15;

}

// Experiment-2

// Setting Tcp Variant to Cubic 
Config::Set (sender2, TypeIdValue (TcpCubicId));

for (int i = 0; i < 3; i++)
{
BulkSendHelper S1_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i2i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S1_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S1_App = S1_source.Install (n0n4.Get (0));
S1_App.Start (Seconds (time_in_secs));
S1_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D1_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));
ApplicationContainer D1_App = D1_sink.Install (n2n5.Get (0));
D1_App.Start (Seconds (time_in_secs));
D1_App.Stop (Seconds (time_in_secs + 15.0));

BulkSendHelper S2_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i3i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S2_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S2_App = S2_source.Install (n1n4.Get (0));
S2_App.Start (Seconds (time_in_secs));
S2_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D2_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));

ApplicationContainer D2_App = D2_sink.Install (n3n5.Get (0));
D2_App.Start (Seconds (time_in_secs));
D2_App.Stop (Seconds (time_in_secs + 15.0));

time_in_secs += 15;
}

//Experiment-3

// Setting Tcp Variant to DCTCP 
Config::Set (sender1, TypeIdValue (TcpDctcpId));

for(int i =0; i < 3; i++)
{
    BulkSendHelper S1_source ("ns3::TcpSocketFactory",
                          InetSocketAddress (i2i5.GetAddress (0), port_1));
   // Set the amount of data to send in bytes.  Zero is unlimited.
   S1_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
   ApplicationContainer S1_App = S1_source.Install (n0n4.Get (0));
   S1_App.Start (Seconds (time_in_secs));
   S1_App.Stop (Seconds (time_in_secs + 10.0));
   PacketSinkHelper D1_sink ("ns3::TcpSocketFactory",
                          InetSocketAddress (Ipv4Address::GetAny (), port_1));
   ApplicationContainer D1_App = D1_sink.Install (n2n5.Get (0));
   D1_App.Start (Seconds (time_in_secs));
   D1_App.Stop (Seconds (time_in_secs + 10.0));

time_in_secs += 15;

}

//Experiment-4

// Setting Tcp Variant to DCTCP 
Config::Set (sender1, TypeIdValue (TcpDctcpId));
Config::Set (sender2, TypeIdValue (TcpDctcpId));

for (int i = 0; i < 3; i++)
{
BulkSendHelper S1_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i2i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S1_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S1_App = S1_source.Install (n0n4.Get (0));
S1_App.Start (Seconds (time_in_secs));
S1_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D1_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));
ApplicationContainer D1_App = D1_sink.Install (n2n5.Get (0));
D1_App.Start (Seconds (time_in_secs));
D1_App.Stop (Seconds (time_in_secs + 15.0));

BulkSendHelper S2_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i3i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S2_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S2_App = S2_source.Install (n1n4.Get (0));
S2_App.Start (Seconds (time_in_secs));
S2_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D2_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));

ApplicationContainer D2_App = D2_sink.Install (n3n5.Get (0));
D2_App.Start (Seconds (time_in_secs));
D2_App.Stop (Seconds (time_in_secs + 15.0));

time_in_secs += 15;
}

//Experiment-5

// Setting Tcp Variant to Cubic 
Config::Set (sender1, TypeIdValue (TcpCubicId));
// Setting Tcp Variant to DCTCP 
Config::Set (sender2, TypeIdValue (TcpDctcpId));

for (int i = 0; i < 3; i++)
{
BulkSendHelper S1_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i2i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S1_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S1_App = S1_source.Install (n0n4.Get (0));
S1_App.Start (Seconds (time_in_secs));
S1_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D1_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));
ApplicationContainer D1_App = D1_sink.Install (n2n5.Get (0));
D1_App.Start (Seconds (time_in_secs));
D1_App.Stop (Seconds (time_in_secs + 15.0));

BulkSendHelper S2_source ("ns3::TcpSocketFactory",
                      InetSocketAddress (i3i5.GetAddress (0), port_1));
// Set the amount of data to send in bytes.  Zero is unlimited.
S2_source.SetAttribute ("MaxBytes", UintegerValue (500*1024*1024));
ApplicationContainer S2_App = S2_source.Install (n1n4.Get (0));
S2_App.Start (Seconds (time_in_secs));
S2_App.Stop (Seconds (time_in_secs + 15.0));

PacketSinkHelper D2_sink ("ns3::TcpSocketFactory",
                      InetSocketAddress (Ipv4Address::GetAny (), port_1));

ApplicationContainer D2_App = D2_sink.Install (n3n5.Get (0));
D2_App.Start (Seconds (time_in_secs));
D2_App.Stop (Seconds (time_in_secs + 15.0));

time_in_secs += 15;
}


   Ptr<FlowMonitor> flowmon;
      FlowMonitorHelper flowmonHelper;
      flowmon = flowmonHelper.InstallAll ();

p2p.EnablePcapAll("pcap_file", true);

    NS_LOG_INFO ("Run Simulation.");
      Simulator::Stop (Seconds (time_in_secs));
      Simulator::Run ();
      Simulator::Destroy ();

      flowmon->CheckForLostPackets ();
      Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = flowmon->GetFlowStats ();
  int flow_count = 0;
  float throughput[25];
  float afct[25];
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    if(t.sourceAddress == "10.1.1.1" || t.sourceAddress == "10.1.2.1")
    {
          std::cout << "Flow " << ++flow_count  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
            throughput[flow_count] = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024;
            afct[flow_count] = (i->second.timeLastTxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds());
          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
          std::cout<< " Average Flow Completion time "<< (i->second.timeLastTxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) << " Seconds \n";
        }
      }


float th_means[9];
th_means[1] = (throughput[1]  + throughput[2]  + throughput[3]) / 3; //th1_s1
th_means[2] = (throughput[4]  + throughput[6]  + throughput[8]) / 3; //th2_s1
th_means[3] = (throughput[5]  + throughput[7]  + throughput[9]) / 3; //th2_s2
th_means[4] = (throughput[10] + throughput[11] + throughput[12]) / 3; //th3_s1
th_means[5] = (throughput[13] + throughput[15] + throughput[17]) / 3; //th4_s1
th_means[6] = (throughput[14] + throughput[16] + throughput[18]) / 3; //th4_s2
th_means[7] = (throughput[19] + throughput[21] + throughput[23]) / 3; //th5_s1
th_means[8] = (throughput[20] + throughput[22] + throughput[24]) / 3; //th5_s2

float th_sds[9];
th_sds[1] = calculate_sd(throughput[1],  throughput[2] , throughput[3]);
th_sds[2] = calculate_sd(throughput[4] , throughput[6] , throughput[8]); //th2_s1
th_sds[3] = calculate_sd(throughput[5] , throughput[7] , throughput[9]); //th2_s2
th_sds[4] = calculate_sd(throughput[10], throughput[11], throughput[12]); //th3_s1
th_sds[5] = calculate_sd(throughput[13], throughput[15], throughput[17]); //th4_s1
th_sds[6] = calculate_sd(throughput[14], throughput[16], throughput[18]); //th4_s2
th_sds[7] = calculate_sd(throughput[19], throughput[21], throughput[23]); //th5_s1
th_sds[8] = calculate_sd(throughput[20], throughput[22], throughput[24]); //th5_s2

float afct_means[9];
afct_means[1] = (afct[1]  + afct[2]  + afct[3]) / 3;
afct_means[2] = (afct[4]  + afct[6]  + afct[8]) / 3;
afct_means[3] = (afct[5]  + afct[7]  + afct[9]) / 3;
afct_means[4] = (afct[10] + afct[11] + afct[12]) / 3;
afct_means[5] = (afct[13] + afct[15] + afct[17]) / 3;
afct_means[6] = (afct[14] + afct[16] + afct[18]) / 3;
afct_means[7] = (afct[19] + afct[21] + afct[23]) / 3;
afct_means[8] = (afct[20] + afct[22] + afct[24]) / 3;

float afct_sds[9];
afct_sds[1] = calculate_sd(afct[1] , afct[2] , afct[3]);
afct_sds[2] = calculate_sd(afct[4] , afct[6] , afct[8]);
afct_sds[3] = calculate_sd(afct[5] , afct[7] , afct[9]);
afct_sds[4] = calculate_sd(afct[10], afct[11], afct[12]);
afct_sds[5] = calculate_sd(afct[13], afct[15], afct[17]);
afct_sds[6] = calculate_sd(afct[14], afct[16], afct[18]);
afct_sds[7] = calculate_sd(afct[19], afct[21], afct[23]);
afct_sds[8] = calculate_sd(afct[20], afct[22], afct[24]);

// Setting up the output csv file 
std::ofstream outputFile("tcp_srachak.csv");
outputFile<<"exp,r1_s1,r2_s1,r3_s1,avg_s1,std_s1,unit_s1,r1_s2,r2_s2,r3_s2,avg_s2,std_s2,unit_s2,";
outputFile<<"\n";

outputFile<<"th_1,"<<throughput[1]<<","<<throughput[2]<<","<<throughput[3]<<","<<th_means[1]<<","<<th_sds[1]<<","<<"Mbps,"<<",,,,,,\n";
outputFile<<"th_2,"<<throughput[4]<<","<<throughput[6]<<","<<throughput[8]<<","<<th_means[2]<<","<<th_sds[2]<<","<<"Mbps,"<<throughput[5]<<","<<throughput[7]<<","<<throughput[9]<<","<<th_means[3]<<","<<th_sds[3]<<",Mbps,\n";
outputFile<<"th_3,"<<throughput[10]<<","<<throughput[11]<<","<<throughput[12]<<","<<th_means[4]<<","<<th_sds[4]<<","<<"Mbps,"<<",,,,,,\n";
outputFile<<"th_4,"<<throughput[13]<<","<<throughput[15]<<","<<throughput[17]<<","<<th_means[5]<<","<<th_sds[5]<<","<<"Mbps,"<<throughput[14]<<","<<throughput[16]<<","<<throughput[18]<<","<<th_means[6]<<","<<th_sds[6]<<",Mbps,\n";
outputFile<<"th_5,"<<throughput[19]<<","<<throughput[21]<<","<<throughput[23]<<","<<th_means[7]<<","<<th_sds[7]<<","<<"Mbps,"<<throughput[20]<<","<<throughput[22]<<","<<throughput[24]<<","<<th_means[8]<<","<<th_sds[8]<<",Mbps,\n";

outputFile<<"afct_1,"<<afct[1]<<","<< afct[2]<<","<< afct[3]<<","<< afct_means[1]<<","<<afct_sds[1]<<","<<"Seconds,"<<",,,,,,\n";
outputFile<<"afct_2,"<<afct[4]<<","<< afct[6]<<","<< afct[8]<<","<< afct_means[2]<<","<<afct_sds[2]<<","<<"Seconds,"<<afct[5]<<","<<afct[7]<<","<<afct[9]<<","<<afct_means[3]<<","<<afct_sds[3]<<",Seconds,\n";
outputFile<<"afct_3,"<<afct[10]<<","<<afct[11]<<","<<afct[12]<<","<<afct_means[4]<<","<<afct_sds[4]<<","<<"Seconds,"<<",,,,,,\n";
outputFile<<"afct_4,"<<afct[13]<<","<<afct[15]<<","<<afct[17]<<","<<afct_means[5]<<","<<afct_sds[5]<<","<<"Seconds,"<<afct[14]<<","<<afct[16]<<","<<afct[18]<<","<<afct_means[6]<<","<<afct_sds[6]<<",Seconds,\n";
outputFile<<"afct_5,"<<afct[19]<<","<<afct[21]<<","<<afct[23]<<","<<afct_means[7]<<","<<afct_sds[7]<<","<<"Seconds,"<<afct[20]<<","<<afct[22]<<","<<afct[24]<<","<<afct_means[8]<<","<<afct_sds[8]<<",Seconds,\n";

  return 0;
}
