// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    if (params.fPowAllowMinDifficultyBlocks)
    {
        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2* 10 minutes
        // then allow mining of a min-difficulty block.
        // TODO Myriadcoin: enable this at the next hard fork for testnet
        /*
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
            return nProofOfWorkLimit;
        */
    }

    // find previous block with same algo
    const CBlockIndex* pindexPrev = GetLastBlockIndexForAlgo(pindexLast, algo);

    // Genesis block check again
    if (pindexPrev == NULL)
        return nProofOfWorkLimit;

    const CBlockIndex* pindexFirst = NULL;

    if( (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart1) && (pindexLast->nHeight < params.nBlockTimeWarpPreventStart2) )
    {
        // find first block in averaging interval
        // Go back by what we want to be nAveragingInterval blocks
        pindexFirst = pindexPrev;
        for (int i = 0; pindexFirst && i < params.nAveragingInterval - 1; i++)
        {
            pindexFirst = pindexFirst->pprev;
            pindexFirst = GetLastBlockIndexForAlgo(pindexFirst, algo);
        }
        if (pindexFirst == NULL)
            return nProofOfWorkLimit; // not nAveragingInterval blocks of this algo available

        // check block before first block for time warp
        const CBlockIndex* pindexFirstPrev = pindexFirst->pprev;
        if (pindexFirstPrev == NULL)
            return nProofOfWorkLimit;
        pindexFirstPrev = GetLastBlockIndexForAlgo(pindexFirstPrev, algo);
        if (pindexFirstPrev == NULL)
            return nProofOfWorkLimit;
        // take previous block if block times are out of order
        if (pindexFirstPrev->GetBlockTime() > pindexFirst->GetBlockTime())
        {
            LogPrintf("  First blocks out of order times, swapping:   %d   %d\n", pindexFirstPrev->GetBlockTime(), pindexFirst->GetBlockTime());
            pindexFirst = pindexFirstPrev;
        }
        
    }
    else if ( (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart2) && (pindexLast->nHeight < params.nBlockTimeWarpPreventStart3) )
    {
        // find first block in averaging interval
        // Go back by what we want to be nAveragingInterval blocks
        pindexFirst = pindexPrev;
        for (int i = 0; pindexFirst && i < params.nAveragingInterval - 1; i++)
        {
            pindexFirst = pindexFirst->pprev;
            pindexFirst = GetLastBlockIndexForAlgo(pindexFirst, algo);
        }
        if (pindexFirst == NULL)
            return nProofOfWorkLimit; // not nAveragingInterval blocks of this algo available

        const CBlockIndex* pindexFirstPrev;
        for ( ;; )
        {
            // check blocks before first block for time warp
            pindexFirstPrev = pindexFirst->pprev;
            if (pindexFirstPrev == NULL)
                return nProofOfWorkLimit;
            pindexFirstPrev = GetLastBlockIndexForAlgo(pindexFirstPrev, algo);
            if (pindexFirstPrev == NULL)
                return nProofOfWorkLimit;
            // take previous block if block times are out of order
            if (pindexFirstPrev->GetBlockTime() > pindexFirst->GetBlockTime())
            {
                LogPrintf("  First blocks out of order times, swapping:   %d   %d\n", pindexFirstPrev->GetBlockTime(), pindexFirst->GetBlockTime());
                pindexFirst = pindexFirstPrev;
            }
            else
                break;
        }        
    }
    else
    {
        // find first block in averaging interval
        // Go back by what we want to be nAveragingInterval blocks
        pindexFirst = pindexPrev;
        for (int i = 0; pindexFirst && i < params.nAveragingInterval - 1; i++)
        {
            pindexFirst = pindexFirst->pprev;
            pindexFirst = GetLastBlockIndexForAlgo(pindexFirst, algo);
            if (pindexFirst == NULL)
            {
                LogPrint(BCLog::ALL,"pindexFirst is null. returning nProofOfWorkLimit\n");
                return nProofOfWorkLimit;
            }
        }
    }

    int64_t nActualTimespan;
    
    if (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart3)
    {
        nActualTimespan = pindexPrev->GetMedianTimePast() - pindexFirst->GetMedianTimePast();
        LogPrint(BCLog::ALL,"  nActualTimespan = %d before bounds   %d   %d\n", nActualTimespan, pindexPrev->GetMedianTimePast(), pindexFirst->GetMedianTimePast());
    }
    else
    {
        nActualTimespan = pindexPrev->GetBlockTime() - pindexFirst->GetBlockTime();
        LogPrint(BCLog::ALL,"  nActualTimespan = %d before bounds   %d   %d\n", nActualTimespan, pindexPrev->GetBlockTime(), pindexFirst->GetBlockTime());
    }
    
    // Time warp mitigation: Don't adjust difficulty if time is negative
    if ( (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart1) && (pindexLast->nHeight < params.nBlockTimeWarpPreventStart2) )
    {
        if (nActualTimespan < 0)
        {
            LogPrint(BCLog::ALL,"  nActualTimespan negative %d\n", nActualTimespan);
            LogPrint(BCLog::ALL,"  Keeping: %08x \n", pindexPrev->nBits);
            return pindexPrev->nBits;
        }
    }

    if (pindexLast->nHeight >= params.Phase2Timespan_Start)
    {
        return CalculateNextWorkRequiredV2(pindexPrev, pindexFirst, params, algo, nActualTimespan, pindexLast->nHeight);
    }
    else
    {
        return CalculateNextWorkRequiredV1(pindexPrev, pindexFirst, params, algo, nActualTimespan, pindexLast->nHeight);
    }
}

unsigned int CalculateNextWorkRequiredV1(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan, int nHeight)
{
    if (params.fPowNoRetargeting)
        return pindexPrev->nBits;

    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);    
    
    int64_t nTargetSpacingPerAlgo = params.nPowTargetSpacingV1 * NUM_ALGOS; // 30 * 5 = 150s per algo
    int64_t nAveragingTargetTimespan = params.nAveragingInterval * nTargetSpacingPerAlgo; // 10 * 150 = 1500s, 25 minutes
    int64_t nMinActualTimespanV1 = nAveragingTargetTimespan * (100 - params.nMaxAdjustUpV1) / 100;
    int64_t nMinActualTimespanV2 = nAveragingTargetTimespan * (100 - params.nMaxAdjustUpV2) / 100;
    int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + params.nMaxAdjustDown) / 100;
    
    int64_t nMinActualTimespan;
    if (nHeight >= params.nBlockDiffAdjustV2)
    {
        nMinActualTimespan = nMinActualTimespanV2;
    }
    else
    {
        nMinActualTimespan = nMinActualTimespanV1;
    }
    
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;
    
    LogPrint(BCLog::ALL,"  nActualTimespan = %d after bounds   %d   %d\n", nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    
    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexPrev->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;
    if (bnNew > nProofOfWorkLimit)
        bnNew = nProofOfWorkLimit;
    
    /// debug print
    LogPrint(BCLog::ALL,"CalculateNextWorkRequiredV1(Algo=%d): RETARGET\n", algo);
    LogPrint(BCLog::ALL,"CalculateNextWorkRequiredV1(Algo=%d): nTargetTimespan = %d    nActualTimespan = %d\n", algo, nAveragingTargetTimespan, nActualTimespan);
    LogPrint(BCLog::ALL,"CalculateNextWorkRequiredV1(Algo=%d): Before: %08x  %s\n", algo, pindexPrev->nBits, bnOld.ToString());
    LogPrint(BCLog::ALL,"CalculateNextWorkRequiredV1(Algo=%d): After:  %08x  %s\n", algo, bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}


unsigned int CalculateNextWorkRequiredV2(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan, int nHeight)
{
    if (params.fPowNoRetargeting)
        return pindexPrev->nBits;

    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);    
    
    int64_t nTargetSpacingPerAlgo = params.nPowTargetSpacingV2 * NUM_ALGOS; // 60 * 5 = 300s per algo
    std::string sBlockTime = "V2";
    if (nHeight >= params.nLongblocks_StartV1c) {
        nTargetSpacingPerAlgo = params.nPowTargetSpacingV3c * NUM_ALGOS; // 8 * 60 * 5 = 2400s per algo
        sBlockTime = "V2_longblocks_8min";
    } else if (nHeight >= params.nLongblocks_StartV1b) {
        nTargetSpacingPerAlgo = params.nPowTargetSpacingV3b * NUM_ALGOS; // 4 * 60 * 5 = 1200s per algo
        sBlockTime = "V2_longblocks_4min";
    } else if (nHeight >= params.nLongblocks_StartV1a) {
        nTargetSpacingPerAlgo = params.nPowTargetSpacingV3a * NUM_ALGOS; // 2 * 60 * 5 = 600s per algo
        sBlockTime = "V2_longblocks_2min";
    }
    int64_t nAveragingTargetTimespan = params.nAveragingInterval * nTargetSpacingPerAlgo; // 10 blocks per algo
    int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - params.nMaxAdjustUpV2) / 100;
    int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + params.nMaxAdjustDown) / 100;
    
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;
    
    LogPrint(BCLog::ALL,"  nActualTimespan = %d after bounds   %d   %d\n", nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexPrev->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;
    if (bnNew > nProofOfWorkLimit)
        bnNew = nProofOfWorkLimit;
    
    /// debug print
    LogPrint(BCLog::ALL,"CalculateNextWorkRequired%s(Algo=%d): RETARGET\n", sBlockTime, algo);
    LogPrint(BCLog::ALL,"CalculateNextWorkRequired%s(Algo=%d): nTargetTimespan = %d    nActualTimespan = %d\n", sBlockTime, algo, nAveragingTargetTimespan, nActualTimespan);
    LogPrint(BCLog::ALL,"CalculateNextWorkRequired%s(Algo=%d): Before: %08x  %s\n", sBlockTime, algo, pindexPrev->nBits, bnOld.ToString());
    LogPrint(BCLog::ALL,"CalculateNextWorkRequired%s(Algo=%d): After:  %08x  %s\n", sBlockTime, algo, bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, int algo, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
