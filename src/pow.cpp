// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include "bignum.h"

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params, int algo)
{
    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);

    // Genesis block
    if (pindexLast == NULL)
    {
        if(fDebug)
        {
            LogPrintf("pindexLast is null. returning nProofOfWorkLimit\n");
        }
        return nProofOfWorkLimit.GetCompact();
    }

    // find previous block with same algo
    const CBlockIndex* pindexPrev = GetLastBlockIndexForAlgo(pindexLast, algo);
    
    // Genesis block
    if (pindexPrev == NULL)
    {
        if(fDebug)
        {
            LogPrintf("pindexPrev is null. returning nProofOfWorkLimit\n");
        }
        return nProofOfWorkLimit.GetCompact();
    }
    
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
            return nProofOfWorkLimit.GetCompact(); // not nAveragingInterval blocks of this algo available

        // check block before first block for time warp
        const CBlockIndex* pindexFirstPrev = pindexFirst->pprev;
        if (pindexFirstPrev == NULL)
            return nProofOfWorkLimit.GetCompact();
        pindexFirstPrev = GetLastBlockIndexForAlgo(pindexFirstPrev, algo);
        if (pindexFirstPrev == NULL)
            return nProofOfWorkLimit.GetCompact();
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
            return nProofOfWorkLimit.GetCompact(); // not nAveragingInterval blocks of this algo available

        const CBlockIndex* pindexFirstPrev;
        for ( ;; )
        {
            // check blocks before first block for time warp
            pindexFirstPrev = pindexFirst->pprev;
            if (pindexFirstPrev == NULL)
                return nProofOfWorkLimit.GetCompact();
            pindexFirstPrev = GetLastBlockIndexForAlgo(pindexFirstPrev, algo);
            if (pindexFirstPrev == NULL)
                return nProofOfWorkLimit.GetCompact();
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
                if(fDebug)
                {
                    LogPrintf("pindexFirst is null. returning nProofOfWorkLimit\n");
                }
                return nProofOfWorkLimit.GetCompact();
            }
        }
    }

    int64_t nActualTimespan;
    
    if (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart3)
    {
        nActualTimespan = pindexPrev->GetMedianTimePast() - pindexFirst->GetMedianTimePast();
        if(fDebug)
        {
            LogPrintf("  nActualTimespan = %d before bounds   %d   %d\n", nActualTimespan, pindexPrev->GetMedianTimePast(), pindexFirst->GetMedianTimePast());
        }
    }
    else
    {
        nActualTimespan = pindexPrev->GetBlockTime() - pindexFirst->GetBlockTime();
        if(fDebug)
        {
            LogPrintf("  nActualTimespan = %d before bounds   %d   %d\n", nActualTimespan, pindexPrev->GetBlockTime(), pindexFirst->GetBlockTime());
        }
    }
    
    // Time warp mitigation: Don't adjust difficulty if time is negative
    if ( (pindexLast->nHeight >= params.nBlockTimeWarpPreventStart1) && (pindexLast->nHeight < params.nBlockTimeWarpPreventStart2) )
    {
        if (nActualTimespan < 0)
        {
            if(fDebug)
            {
                LogPrintf("  nActualTimespan negative %d\n", nActualTimespan);
                LogPrintf("  Keeping: %08x \n", pindexPrev->nBits);
            }
            return pindexPrev->nBits;
        }
    }

    if (pindexLast->nHeight >= params.Phase2Timespan_Start)
    {
        return CalculateNextWorkRequiredV2(pindexPrev, pindexFirst, params, algo, nActualTimespan);
    }
    else
    {
        return CalculateNextWorkRequiredV1(pindexPrev, pindexFirst, params, algo, nActualTimespan, pindexLast->nHeight);
    }
}

unsigned int CalculateNextWorkRequiredV1(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan, int nHeight)
{
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
    
    if(fDebug)
    {
        LogPrintf("  nActualTimespan = %d after bounds   %d   %d\n", nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    }
    
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
    if(fDebug)
    {
        LogPrintf("CalculateNextWorkRequiredV1(Algo=%d): RETARGET\n", algo);
        LogPrintf("CalculateNextWorkRequiredV1(Algo=%d): nTargetTimespan = %d    nActualTimespan = %d\n", algo, nAveragingTargetTimespan, nActualTimespan);
        LogPrintf("CalculateNextWorkRequiredV1(Algo=%d): Before: %08x  %s\n", algo, pindexPrev->nBits, bnOld.ToString());
        LogPrintf("CalculateNextWorkRequiredV1(Algo=%d): After:  %08x  %s\n", algo, bnNew.GetCompact(), bnNew.ToString());
    }

    return bnNew.GetCompact();
}


unsigned int CalculateNextWorkRequiredV2(const CBlockIndex* pindexPrev, const CBlockIndex* pindexFirst, const Consensus::Params& params, int algo, int64_t nActualTimespan)
{
    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);    
    
    int64_t nTargetSpacingPerAlgo = params.nPowTargetSpacingV2 * NUM_ALGOS; // 60 * 5 = 300s per algo
    int64_t nAveragingTargetTimespan = params.nAveragingInterval * nTargetSpacingPerAlgo; // 10 * 300 = 3000s, 50 minutes
    int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - params.nMaxAdjustUpV2) / 100;
    int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + params.nMaxAdjustDown) / 100;
    
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;
    
    if(fDebug)
    {
        LogPrintf("  nActualTimespan = %d after bounds   %d   %d\n", nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    }
    
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexPrev->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;
    if (bnNew > nProofOfWorkLimit)
        bnNew = nProofOfWorkLimit;
    
    /// debug print
    if(fDebug)
    {
        LogPrintf("CalculateNextWorkRequiredV2(Algo=%d): RETARGET\n", algo);
        LogPrintf("CalculateNextWorkRequiredV2(Algo=%d): nTargetTimespan = %d    nActualTimespan = %d\n", algo, nAveragingTargetTimespan, nActualTimespan);
        LogPrintf("CalculateNextWorkRequiredV2(Algo=%d): Before: %08x  %s\n", algo, pindexPrev->nBits, bnOld.ToString());
        LogPrintf("CalculateNextWorkRequiredV2(Algo=%d): After:  %08x  %s\n", algo, bnNew.GetCompact(), bnNew.ToString());
    }

    return bnNew.GetCompact();
}


bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return error("CheckProofOfWork(): nBits below minimum work");

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return error("CheckProofOfWork(): hash doesn't match nBits");

    return true;
}

arith_uint256 GetBlockProofBase(const CBlockIndex& block)
{
    arith_uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a arith_uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}

int GetAlgoWorkFactor(int algo)
{
    switch (algo)
    {
        case ALGO_SHA256D:
            return 1; 
        // work factor = absolute work ratio * optimisation factor
        case ALGO_SCRYPT:
            return 1024 * 4;
        case ALGO_GROESTL:
            return 64 * 8;
        case ALGO_SKEIN:
            return 4 * 6;
        case ALGO_QUBIT:
            return 128 * 8;
        default:
            return 1;
    }
}

arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& block, int algo)
{
    const CBlockIndex* pindex = &block;
    while (pindex != NULL)
    {
        if (pindex->GetAlgo() == algo)
        {
            return GetBlockProofBase(*pindex);
        }
        pindex = pindex->pprev;
    }
    return UintToArith256(Params().GetConsensus().powLimit);
}

arith_uint256 GetPrevWorkForAlgoWithDecay(const CBlockIndex& block, int algo)
{
    int nDistance = 0;
    arith_uint256 nWork;
    const CBlockIndex* pindex = &block;
    while (pindex != NULL)
    {
        if (nDistance > 32)
        {
            return UintToArith256(Params().GetConsensus().powLimit);
        }
        if (pindex->GetAlgo() == algo)
        {
            arith_uint256 nWork = GetBlockProofBase(*pindex);
            nWork *= (32 - nDistance);
            nWork /= 32;
            if (nWork < UintToArith256(Params().GetConsensus().powLimit))
                nWork = UintToArith256(Params().GetConsensus().powLimit);
            return nWork;
        }
        pindex = pindex->pprev;
        nDistance++;
    }
    return UintToArith256(Params().GetConsensus().powLimit);
}

arith_uint256 GetPrevWorkForAlgoWithDecay2(const CBlockIndex& block, int algo)
{
    int nDistance = 0;
    arith_uint256 nWork;
    const CBlockIndex* pindex = &block;
    while (pindex != NULL)
    {
        if (nDistance > 32)
        {
            return arith_uint256(0);
        }
        if (pindex->GetAlgo() == algo)
        {
            arith_uint256 nWork = GetBlockProofBase(*pindex);
            nWork *= (32 - nDistance);
            nWork /= 32;
            return nWork;
        }
        pindex = pindex->pprev;
        nDistance++;
    }
    return arith_uint256(0);
}
    
arith_uint256 GetPrevWorkForAlgoWithDecay3(const CBlockIndex& block, int algo)
{
    int nDistance = 0;
    arith_uint256 nWork;
    const CBlockIndex* pindex = &block;
    while (pindex != NULL)
    {
        if (nDistance > 100)
        {
            return arith_uint256(0);
        }
        if (pindex->GetAlgo() == algo)
        {
            arith_uint256 nWork = GetBlockProofBase(*pindex);
            nWork *= (100 - nDistance);
            nWork /= 100;
            return nWork;
        }
        pindex = pindex->pprev;
        nDistance++;
    }
    return arith_uint256(0);
}

arith_uint256 GetGeometricMeanPrevWork(const CBlockIndex& block)
{
    //arith_uint256 bnRes;
    arith_uint256 nBlockWork = GetBlockProofBase(block);
    CBigNum bnBlockWork = CBigNum(ArithToUint256(nBlockWork));
    int nAlgo = block.GetAlgo();
    
    for (int algo = 0; algo < NUM_ALGOS_IMPL; algo++)
    {
        if (algo != nAlgo)
        {
            arith_uint256 nBlockWorkAlt = GetPrevWorkForAlgoWithDecay3(block, algo);
            CBigNum bnBlockWorkAlt = CBigNum(ArithToUint256(nBlockWorkAlt));
            if (bnBlockWorkAlt != 0)
                bnBlockWork *= bnBlockWorkAlt;
        }
    }
    // Compute the geometric mean
    CBigNum bnRes = bnBlockWork.nthRoot(NUM_ALGOS);
    
    // Scale to roughly match the old work calculation
    bnRes <<= 8;
    
    //return bnRes;
    return UintToArith256(bnRes.getuint256());
}

arith_uint256 GetBlockProof(const CBlockIndex& block)
{
    const CChainParams& chainparams = Params();
    
    arith_uint256 bnTarget;
    int nHeight = block.nHeight;
    int nAlgo = block.GetAlgo();
    
    if (nHeight >= chainparams.GetConsensus().nGeoAvgWork_Start)
    {
        bnTarget = GetGeometricMeanPrevWork(block);
    }
    else if (nHeight >= chainparams.GetConsensus().nBlockAlgoNormalisedWorkStart)
    {
        arith_uint256 nBlockWork = GetBlockProofBase(block);
        for (int algo = 0; algo < NUM_ALGOS; algo++)
        {
            if (algo != nAlgo)
            {
                if (nHeight >= chainparams.GetConsensus().nBlockAlgoNormalisedWorkDecayStart2)
                {
                    nBlockWork += GetPrevWorkForAlgoWithDecay2(block, algo);
                }
                else
                {
                    if (nHeight >= chainparams.GetConsensus().nBlockAlgoNormalisedWorkDecayStart1)
                    {
                        nBlockWork += GetPrevWorkForAlgoWithDecay(block, algo);
                    }
                    else
                    {
                        nBlockWork += GetPrevWorkForAlgo(block, algo);
                    }
                }
            }
        }
        bnTarget = nBlockWork / NUM_ALGOS;
    }
    else if (nHeight >= chainparams.GetConsensus().nBlockAlgoWorkWeightStart)
    {
        bnTarget = GetBlockProofBase(block) * GetAlgoWorkFactor(nAlgo);
    }
    else
    {
        bnTarget = GetBlockProofBase(block);
    }
    return bnTarget;
}
   
bool CheckAuxPowProofOfWork(const CBlockHeader& block, const Consensus::Params& params)
{
    int algo = block.GetAlgo();
    /* Except for legacy blocks with full version 1, ensure that
       the chain ID is correct.  Legacy blocks are not allowed since
       the merge-mining start, which is checked in AcceptBlockHeader
       where the height is known.  */
    if (!block.nVersion.IsLegacy() && params.fStrictChainId && block.nVersion.GetChainId() != params.nAuxpowChainId)
        return error("%s : block does not have our chain ID"
                     " (got %d, expected %d, full nVersion %d)",
                     __func__,
                     block.nVersion.GetChainId(),
                     params.nAuxpowChainId,
                     block.nVersion.GetFullVersion());

    /* If there is no auxpow, just check the block hash.  */
    if (!block.auxpow) {
        if (block.nVersion.IsAuxpow())
            return error("%s : no auxpow on block with auxpow version",
                         __func__);

        if (!CheckProofOfWork(block.GetPoWHash(algo), block.nBits, params))
            return error("%s : non-AUX proof of work failed", __func__);

        return true;
    }

    /* We have auxpow.  Check it.  */

    if (!block.nVersion.IsAuxpow())
        return error("%s : auxpow on block with non-auxpow version", __func__);

    if (!block.auxpow->check(block.GetHash(), block.nVersion.GetChainId(), params))
        return error("%s : AUX POW is not valid", __func__);
    
    if(fDebug)
    {
        bool fNegative;
        bool fOverflow;
        arith_uint256 bnTarget;
        bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
        
        LogPrintf("DEBUG: proof-of-work submitted  \n  parent-PoWhash: %s\n  target: %s  bits: %08x \n",
        block.auxpow->getParentBlockPoWHash(algo).ToString().c_str(),
        bnTarget.ToString().c_str(),
        bnTarget.GetCompact());
    }
    
    if (!(algo == ALGO_SHA256D || algo == ALGO_SCRYPT) )
    {
        return error("%s : AUX POW is not allowed on this algo", __func__);
    }
    
    if (!CheckProofOfWork(block.auxpow->getParentBlockPoWHash(algo), block.nBits, params))
    {
        return error("%s : AUX proof of work failed", __func__);
    }

    return true;
}

int64_t GetBlockProofEquivalentTime(const CBlockIndex& to, const CBlockIndex& from, const CBlockIndex& tip, const Consensus::Params& params)
{
    arith_uint256 r;
    int sign = 1;
    if (to.nChainWork > from.nChainWork) {
        r = to.nChainWork - from.nChainWork;
    } else {
        r = from.nChainWork - to.nChainWork;
        sign = -1;
    }
    r = r * arith_uint256(params.nPowTargetSpacingV2) / GetBlockProof(tip);
    if (r.bits() > 63) {
        return sign * std::numeric_limits<int64_t>::max();
    }
    return sign * r.GetLow64();
}

const CBlockIndex* GetLastBlockIndex(const CBlockIndex* pindex, int algo)
{
    while (pindex && pindex->pprev && (pindex->GetAlgo() != algo))
        pindex = pindex->pprev;
    return pindex;
}

const CBlockIndex* GetLastBlockIndexForAlgo(const CBlockIndex* pindex, int algo)
{
    for (;;)
    {
        if (!pindex)
            return NULL;
        if (pindex->GetAlgo() == algo)
            return pindex;
        pindex = pindex->pprev;
    }
}


