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

#include "putchunks.hpp"

namespace ns3 {
namespace ndn {

namespace chunks = ::ndn::chunks;

class PutChunksApp : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("PutChunksApp")
      .SetParent<Application>()
      .AddConstructor<PutChunksApp>()
      .AddAttribute("Prefix", "Prefix for interests", StringValue("/"),
                    MakeNameAccessor(&PutChunksApp::m_prefix), MakeNameChecker())
      .AddAttribute("InputFile", "The name of the input file", StringValue(""),
                    MakeStringAccessor(&PutChunksApp::m_inputFile), MakeStringChecker());

    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    m_instance.reset(new chunks::PutChunksSim);
    m_instance->setPrefix(m_prefix);
    m_instance->setInputFile(m_inputFile);
    m_instance->setOptions();
    m_instance->run();
  }

  virtual void
  StopApplication()
  {
    m_instance.reset();
  }

private:
  std::unique_ptr<chunks::PutChunksSim> m_instance;
  Name m_prefix;
  std::string m_inputFile;
};

} // namespace ndn
} // namespace ns3
