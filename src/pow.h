// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POW_H
#define BITCOIN_POW_H

#include <consensus/params.h>

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class uint256;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo, const Consensus::Params&);
unsigned int CalculateNextWorkRequiredV1(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan, int nHeight);
unsigned int CalculateNextWorkRequiredV2(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan, int nHeight);

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, int algo, unsigned int nBits, const Consensus::Params&);

#endif // BITCOIN_POW_H
