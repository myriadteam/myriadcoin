// Copyright (c) 2015-2017 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <pow.h>
#include <random.h>
#include <util.h>
#include <test/test_bitcoin.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)

/* Test calculation of next difficulty target with no constraints applying */
BOOST_AUTO_TEST_CASE(get_next_work)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const int algo = ALGO_SHA256D;

    int nActualTimespan = 600;
    CBlockIndex pindexFirst;
    pindexFirst.nHeight = 2000000;
    pindexFirst.nTime = 1420070400;  // Block #(not real)
    pindexFirst.nBits = 0x1d00ffff;
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2000010;
    pindexLast.nTime = 1420071000;  // Block #(not real)
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV1(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d00f5c1);
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d00f5c1);
    // Check MIP3-longblocks intervals
    nActualTimespan = 3010;  // Target is 3000s
    pindexFirst.nHeight = 2903020;  // nPowTargetSpacingV2
    pindexLast.nHeight = 2903030;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d0100d9);
    nActualTimespan = 6050;  // Target is 6000s
    pindexFirst.nHeight = 2903040;  // nPowTargetSpacingV3a
    pindexLast.nHeight = 2903050;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010221);
    nActualTimespan = 12020;  // Target is 12000s
    pindexFirst.nHeight = 3386880;  // nPowTargetSpacingV3b
    pindexLast.nHeight = 3386890;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d01006c);
    nActualTimespan = 24070;  // Target is 24000s
    pindexFirst.nHeight = 3628800;  // nPowTargetSpacingV3c
    pindexLast.nHeight = 3628810;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d0100be);
    nActualTimespan = 24100;  // Target is 24000s
    pindexFirst.nHeight = 3749760;  // nPowTargetSpacingV3c
    pindexLast.nHeight = 3749770;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010110);
}

/* Test the constraint on the upper bound for next work */
BOOST_AUTO_TEST_CASE(get_next_work_pow_limit)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const int algo = ALGO_SHA256D;

    int nActualTimespan = 6000;
    CBlockIndex pindexFirst;
    pindexFirst.nHeight = 2000000;
    pindexFirst.nTime = 1420070400;  // Block #(not real)
    pindexFirst.nBits = 0x1d00ffff;
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2000010;
    pindexLast.nTime = 1420076400;  // Block #(not real)
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV1(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010a3c);
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010a3c);
}

/* Test the constraint on the lower bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_lower_limit_actual)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const int algo = ALGO_SHA256D;

    int nActualTimespan = 10;
    CBlockIndex pindexFirst;
    pindexFirst.nHeight = 2000000;
    pindexFirst.nTime = 1420070400;  // Block #(not real)
    pindexFirst.nBits = 0x1d00ffff;
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2000010;
    pindexLast.nTime = 1420070410;  // Block #(not real)
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV1(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d00f5c1);
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d00f5c1);
}

/* Test the constraint on the upper bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_upper_limit_actual)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const int algo = ALGO_SHA256D;

    int nActualTimespan = 1123200;
    CBlockIndex pindexFirst;
    pindexFirst.nHeight = 2000000;
    pindexFirst.nTime = 1420070400;  // Block #(not real)
    pindexFirst.nBits = 0x1d00ffff;
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2000010;
    pindexLast.nTime = 1421193600;  // Block #(not real)
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV1(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010a3c);
    BOOST_CHECK_EQUAL(CalculateNextWorkRequiredV2(&pindexLast, &pindexFirst, chainParams->GetConsensus(), algo, nActualTimespan, pindexLast.nHeight+1), 0x1d010a3c);
}

BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : nullptr;
        blocks[i].nHeight = i;
        blocks[i].nTime = 1269211443 + i * chainParams->GetConsensus().nPowTargetSpacing;
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p2 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p3 = &blocks[InsecureRandRange(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, chainParams->GetConsensus());
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

BOOST_AUTO_TEST_SUITE_END()
