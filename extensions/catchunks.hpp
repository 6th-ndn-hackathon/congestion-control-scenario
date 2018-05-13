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

#include "tools/chunks/catchunks/consumer.hpp"
#include "tools/chunks/catchunks/pipeline-interests-aimd.hpp"
#include "tools/chunks/catchunks/aimd-statistics-collector.hpp"
#include "tools/chunks/catchunks/pipeline-interests.hpp"

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/validator-null.hpp>

namespace ndn {
namespace chunks {

namespace catChunksSim = ::ndn::chunks;

class CatChunksSim
{
public:
  CatChunksSim()
  {
    m_discoverType = "iterative";
    m_pipelineType = "aimd";
    m_maxPipelineSize = 1;
    m_maxRetriesAfterVersionFound = 0;
    m_maxRetriesOnTimeoutOrNack = 1000;
    m_discoverTimeoutMs = 300;

    m_lifetime = 4000.0; // -- in milliseconds
    m_disableCwa = false;
    m_resetCwndToInit = false;
    m_ignoreCongMarks = false;
    m_aiStep = 1.0;
    m_mdCoef = 0.5;
    m_alpha = 0.125;
    m_beta = 0.25;
    m_minRto = 200.0;
    m_maxRto = 4000.0;
    m_initCwnd = 1;
    m_initSsthresh = std::numeric_limits<int>::max();
    m_k = 4;

    m_rttPath = "";
    m_cwndPath = "";
    
    m_consumer = new Consumer(security::v2::getAcceptAllValidator());
  }

  void
  setPrefix(Name name)
  {
    m_uri = name;
  }

  void
  setVerbose()
  {
    m_options.isVerbose = true;
  }

  void
  setPipelineType(std::string p)
  {
    if (p == "aimd")
      m_pipelineType = "aimd";
    else if (p == "fixed")
      m_pipelineType = "fixed";
    else {
      std::cerr << "ERROR: discover version type not valid" << std::endl;
      NS_ASSERT(0);
    }
  }

  int 
  setOptions();

  void
  run()
  {
    BOOST_ASSERT(m_discover != nullptr);
    BOOST_ASSERT(m_pipeline != nullptr);
    m_consumer->run(std::move(m_discover), std::move(m_pipeline));
    m_face.processEvents();
  }

private:
  Face m_face;
  Consumer* m_consumer;
  chunks::Options m_options; 
  unique_ptr<DiscoverVersion> m_discover;
  unique_ptr<PipelineInterests> m_pipeline;
  shared_ptr<aimd::StatisticsCollector> m_statsCollector;
  shared_ptr<aimd::RttEstimator> m_rttEstimator;

  Name m_uri;
  std::string m_discoverType;
  std::string m_pipelineType;
  size_t m_maxPipelineSize;
  int m_maxRetriesAfterVersionFound;
  int m_maxRetriesOnTimeoutOrNack;
  int64_t m_discoverTimeoutMs;
  int64_t m_lifetime;

  bool m_disableCwa, m_resetCwndToInit, m_ignoreCongMarks;
  double m_aiStep, m_mdCoef, m_alpha, m_beta, m_minRto, m_maxRto;
  int m_initCwnd, m_initSsthresh, m_k;
 
  // -- log files -- // 
  std::string m_cwndPath, m_rttPath;
};

}
}
// namespace(chunks)
// namespace(ndn)
