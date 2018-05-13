/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Chavoosh Ghasemi <chghasemi@cs.arizona.edu>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {
namespace ndn {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     10Mbps      +--------+     10Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer where the producer publishes the content
 * according to the input file.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     ./waf --run chunks-simple
 */

int
main(int argc, char *argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  p2p.Install(nodes.Get(1), nodes.Get(2));

  // Install NDN stack on all nodes
  StackHelper ndnHelper;
  ndnHelper.InstallAll();

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("CatChunksApp");
  consumerHelper.SetAttribute("Prefix", StringValue("/tmp"));
  consumerHelper.SetAttribute("PipelineType", StringValue("aimd"));
  consumerHelper.SetAttribute("Verbose", IntegerValue(1));
  consumerHelper.Install(nodes.Get(0)).Start(Seconds(5));

  std::string inputFile = "/home/vagrant/ndnSIM/my-scenarios/ndn-tools/COPYING.md";
  // Producer
  ndn::AppHelper producerHelper("PutChunksApp");
  producerHelper.SetAttribute("Prefix", StringValue("/tmp"));
  producerHelper.SetAttribute("InputFile", StringValue(inputFile));
  producerHelper.Install(nodes.Get(2)).Start(Seconds(0.1));

  ndnGlobalRoutingHelper.AddOrigins("/tmp", nodes.Get(2));

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(20.0));

  ns3::ndn::AppDelayTracer::InstallAll("app-delays-trace.txt"); 

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ndn
} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::ndn::main(argc, argv);
}
