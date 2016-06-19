// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include "uint256.h"

namespace Consensus {
/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    int64_t nPowTargetSpacingV1;
    int64_t nPowTargetSpacingV2;
    int64_t nPowTargetTimespan;
    int64_t nAveragingInterval;
    int64_t nBlockTimeWarpPreventStart1;
    int64_t nBlockTimeWarpPreventStart2;
    int64_t nBlockTimeWarpPreventStart3;
    int64_t Phase2Timespan_Start;
    int64_t nBlockDiffAdjustV2;
    int64_t nMaxAdjustDown;
    int64_t nMaxAdjustUpV1;
    int64_t nMaxAdjustUpV2;
    int64_t nBlockSequentialAlgoRuleStart1;
    int64_t nBlockSequentialAlgoRuleStart2;
    int64_t nBlockSequentialAlgoRuleStart3;
    int nBlockSequentialAlgoMaxCount1;
    int nBlockSequentialAlgoMaxCount2;
    int nBlockSequentialAlgoMaxCount3;
    int64_t nBlockAlgoWorkWeightStart;
    int64_t nBlockAlgoNormalisedWorkStart;
    int64_t nBlockAlgoNormalisedWorkDecayStart1;
    int64_t nBlockAlgoNormalisedWorkDecayStart2;
    int64_t nGeoAvgWork_Start;
    int64_t nAlgoSwitchBlock1;
    
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacingV1; }
    
    /** Auxpow parameters */
    int16_t nAuxpowChainId;
    bool fStrictChainId;
    int nStartAuxPow;
};
} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
