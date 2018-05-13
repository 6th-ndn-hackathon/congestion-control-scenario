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

#include "catchunks.hpp"
#include "tools/chunks/catchunks/discover-version-fixed.hpp"
#include "tools/chunks/catchunks/discover-version-iterative.hpp"
#include "tools/chunks/catchunks/pipeline-interests-fixed-window.hpp"
#include <fstream>

namespace ndn {
namespace chunks {

int
CatChunksSim::setOptions()
{
  if (m_discoverType == "fixed" && (m_uri.empty() || !m_uri[-1].isVersion())) {
    std::cerr << "ERROR: The specified name must contain a version component when using "
                 "fixed version discovery" << std::endl;
    return 2;
  }

  if (m_maxPipelineSize < 1 || m_maxPipelineSize > 1024) {
    std::cerr << "ERROR: pipeline size must be between 1 and 1024" << std::endl;
    return 2;
  }

  if (m_options.maxRetriesOnTimeoutOrNack < -1 || m_options.maxRetriesOnTimeoutOrNack > 1024) {
    std::cerr << "ERROR: retries value must be between -1 and 1024" << std::endl;
    return 2;
  }

  if (m_maxRetriesAfterVersionFound < 0 || m_maxRetriesAfterVersionFound > 1024) {
    std::cerr << "ERROR: retries iterative value must be between 0 and 1024" << std::endl;
    return 2;
  }

  if (m_discoverTimeoutMs < 0) {
    std::cerr << "ERROR: timeout cannot be negative" << std::endl;
    return 2;
  }

  m_options.interestLifetime = time::milliseconds(m_lifetime);

  if (m_options.isQuiet && m_options.isVerbose) {
    std::cerr << "ERROR: cannot be quiet and verbose at the same time" << std::endl;
    return 2;
  }
 
  if (m_discoverType == "fixed") {
    m_discover = make_unique<DiscoverVersionFixed>(m_uri, m_face, m_options);
  }
  else if (m_discoverType == "iterative") {
    DiscoverVersionIterative::Options optionsIterative(m_options);
    optionsIterative.maxRetriesAfterVersionFound = m_maxRetriesAfterVersionFound;
    optionsIterative.discoveryTimeout = time::milliseconds(m_discoverTimeoutMs);
    m_discover = make_unique<DiscoverVersionIterative>(m_uri, m_face, optionsIterative);
  }
  else {
    std::cerr << "ERROR: discover version type not valid" << std::endl;
    return 2;
  }

  //unique_ptr<PipelineInterests> pipeline;
  //unique_ptr<aimd::StatisticsCollector> statsCollector;
  //unique_ptr<aimd::RttEstimator> rttEstimator;
  std::ofstream statsFileCwnd;
  std::ofstream statsFileRtt;

  if (m_pipelineType == "fixed") {
    PipelineInterestsFixedWindow::Options optionsPipeline(m_options);
    optionsPipeline.maxPipelineSize = m_maxPipelineSize;
    m_pipeline = make_unique<PipelineInterestsFixedWindow>(m_face, optionsPipeline);
  }
  else if (m_pipelineType == "aimd") {
    aimd::RttEstimator::Options optionsRttEst;
    optionsRttEst.isVerbose = m_options.isVerbose;
    optionsRttEst.alpha = m_alpha;
    optionsRttEst.beta = m_beta;
    optionsRttEst.k = m_k;
    optionsRttEst.minRto = aimd::Milliseconds(m_minRto);
    optionsRttEst.maxRto = aimd::Milliseconds(m_maxRto);

    m_rttEstimator = make_unique<aimd::RttEstimator>(optionsRttEst);

    PipelineInterestsAimd::Options optionsPipeline(m_options);
    optionsPipeline.disableCwa = m_disableCwa;
    optionsPipeline.resetCwndToInit = m_resetCwndToInit;
    optionsPipeline.initCwnd = static_cast<double>(m_initCwnd);
    optionsPipeline.initSsthresh = static_cast<double>(m_initSsthresh);
    optionsPipeline.aiStep = m_aiStep;
    optionsPipeline.mdCoef = m_mdCoef;
    optionsPipeline.ignoreCongMarks = m_ignoreCongMarks;

    auto aimdPipeline = make_unique<PipelineInterestsAimd>(m_face, *m_rttEstimator, optionsPipeline);

    if (!m_cwndPath.empty() || !m_rttPath.empty()) {
      if (!m_cwndPath.empty()) {
        statsFileCwnd.open(m_cwndPath);
        if (statsFileCwnd.fail()) {
          std::cerr << "ERROR: failed to open " << m_cwndPath << std::endl;
          return 4;
        }
      }
      if (!m_rttPath.empty()) {
        statsFileRtt.open(m_rttPath);
        if (statsFileRtt.fail()) {
          std::cerr << "ERROR: failed to open " << m_rttPath << std::endl;
          return 4;
        }
      }
      m_statsCollector = make_unique<aimd::StatisticsCollector>(*aimdPipeline, *m_rttEstimator,
                                                              statsFileCwnd, statsFileRtt);
    }
    
    m_pipeline = std::move(aimdPipeline);
  }
  else {
    std::cerr << "ERROR: Interest pipeline type not valid" << std::endl;
    return 2;
  }
  
  return 0;
}

} // namesapce chunks
} // namespace ndn
