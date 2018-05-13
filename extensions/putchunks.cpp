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

#include "putchunks.hpp"

namespace ndn {
namespace chunks {

int
PutChunksSim::setOptions()
{
  if (m_options.maxSegmentSize < 1 || m_options.maxSegmentSize > MAX_NDN_PACKET_SIZE) {
    std::cerr << "ERROR: Maximum chunk size must be between 1 and " << MAX_NDN_PACKET_SIZE << std::endl;
    return 2;
  }
  m_options.signingInfo = security::SigningInfo(m_signingStr);

  if (m_options.isQuiet && m_options.isVerbose) {
    std::cerr << "ERROR: Cannot be quiet and verbose at the same time" << std::endl;
    return 2;
  }

  return 0;
}

} // namespace chunks
} // namespace ndn
