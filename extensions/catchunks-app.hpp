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

#include "ns3/ndnSIM-module.h"
#include "ns3/integer.h"
#include "ns3/string.h"

#include "catchunks.hpp"

namespace ns3 {
namespace ndn {

namespace chunks = ::ndn::chunks;

class CatChunksApp : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("CatChunksApp")
      .SetParent<Application>()
      .AddConstructor<CatChunksApp>()
      .AddAttribute("Prefix", "Prefix for interests", StringValue("/"),
                    MakeNameAccessor(&CatChunksApp::m_prefix), MakeNameChecker())
      .AddAttribute("PipelineType", "Type of pipeline (aimd/fixed)", StringValue("aimd"),
                    MakeStringAccessor(&CatChunksApp::m_pipelineType), MakeStringChecker())
      .AddAttribute("Verbose", "Enable/Disable verbose mode", IntegerValue(0),
                    MakeIntegerAccessor(&CatChunksApp::m_verbose), MakeIntegerChecker<int32_t>());


    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    m_instance.reset(new chunks::CatChunksSim);
    m_instance->setPrefix(m_prefix);
    m_instance->setPipelineType(m_pipelineType);
    if (m_verbose > 0)
      m_instance->setVerbose();
    m_instance->setOptions();
    m_instance->run();
  }

  virtual void
  StopApplication()
  {
    m_instance.reset();
  }

private:
  std::unique_ptr<chunks::CatChunksSim> m_instance;
  Name m_prefix;
  std::string m_pipelineType;
  int32_t m_verbose;
};

} // namepace ndn
} // namespace ns3
