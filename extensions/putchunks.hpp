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

#include "tools/chunks/putchunks/producer.hpp"

#include <fstream>

namespace ndn {
namespace chunks {

namespace putChunksSim = ::ndn::chunks;

class PutChunksSim
{
public:
  PutChunksSim()
  {
    m_signingStr = "";
  }

  int
  setOptions();

  void
  setPrefix (Name prefix)
  {
    m_uri = prefix;
  }

  void
  setInputFile (std::string inputFile)
  {
    m_inputFile = inputFile; 
  }

  void
  run()
  {
    try {
      m_istream.open(m_inputFile);
      m_producer = new Producer(m_uri, m_face, m_keyChain, m_istream, m_options);
      m_producer->run();
    }
    catch (const std::exception& e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
    }
  }

private:
  Face m_face;
  Name m_uri;
  Producer* m_producer;
  Producer::Options m_options;
  KeyChain m_keyChain;
  std::string m_signingStr;
  std::string m_inputFile;
  std::ifstream m_istream;
};

} // namespace chunks
} // namespace ndn
