// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "primitives/pureheader.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "2014-02-23 FT - G20 aims to add $2tn to global economy";
    const CScript genesisOutputScript = CScript() << ParseHex("04e941763c7750969e751bee1ffbe96a651a0feb131db046546c219ea40bff40b95077dc9ba1c05af991588772d8daabbda57386c068fb9bc7477c5e28702d5eb9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        /*** Myriadcoin Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 1402000; // Allow AuxPow blocks from this height
        consensus.nAuxpowChainId = 0x005A; 
        consensus.fStrictChainId = false;

        consensus.nBlockTimeWarpPreventStart1 = 740500; // block where time warp 1 prevention starts
        consensus.nBlockTimeWarpPreventStart2 = 766000; // block where time warp 2 prevention starts
        consensus.nBlockTimeWarpPreventStart3 = 1048320; // block where time warp 3 prevention starts
        consensus.Phase2Timespan_Start = 1401000; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 766000; // block where difficulty adjust V2 starts

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUpV1 = 2; // 2% adjustment up
        consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up     

        consensus.nBlockSequentialAlgoRuleStart1 = 740000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 766000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 6; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo

        consensus.nBlockAlgoWorkWeightStart = 142000; // block where algo work weighting starts
        consensus.nBlockAlgoNormalisedWorkStart = 740000; // block where algo combined weight starts
        consensus.nBlockAlgoNormalisedWorkDecayStart1 = 866000; // block where weight decay starts
        consensus.nBlockAlgoNormalisedWorkDecayStart2 = 932000; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 1400000;
        consensus.nFork1MinBlock = 1764000; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)

        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1507420800; // October 8th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1538956800; // October 8th, 2018

        // Deployment of MIP2 (Reserve algorithm ids)
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 1516320000; // Jan 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 1548979200; // Feb 1st, 2019

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xcb41589c918fba1beccca8bc6b34b2b928b4f9888595d7664afd6ec60a576291");
        consensus.BIP65Height = 1764000; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
        consensus.BIP66Height = 1764000; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
        consensus.MIP2Height = 2328480;
        consensus.MIP1Height = 0; // 0 = activated by BIP9
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1512086400; // December 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1543622400; // December 1st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1513296000; // December 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1544832000; // December 15th, 2018

        // Deployment of Equihash (MIP1)
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nStartTime = 1527811200; // June 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nTimeout = 1546732800; // June 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x9e16b567c393c46e40b4b4c4a21283b998aec17b3c7edf8319b3860e4403e4bf"); //2210002

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0x45;
        pchMessageStart[2] = 0x76;
        pchMessageStart[3] = 0xee;
        nDefaultPort = 10888;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1393164995, 2092903596, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        assert(consensus.hashGenesisBlock == uint256S("0x00000ffde4c020b5938441a0ea3d314bf619eff0b38f32f78f7583cffa1ea485"));
        assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        vSeeds.push_back(CDNSSeedData("seed1.myriadcoin.org", "seed1.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed2.myriadcoin.org", "seed2.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed3.myriadcoin.org", "seed3.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed4.myriadcoin.org", "seed4.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed5.myriadcoin.org", "seed5.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed6.myriadcoin.org", "seed6.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed7.myriadcoin.org", "seed7.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("seed8.myriadcoin.org", "seed8.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("myriadseed1.cryptap.us", "myriadseed1.cryptap.us")); // cryptapus

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,9);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        // Equihash Parameters
        const size_t N = 200, K = 9;
        // BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (      0, uint256S("0x00000ffde4c020b5938441a0ea3d314bf619eff0b38f32f78f7583cffa1ea485"))
            (   9646, uint256S("0x0000000000000b0f1372211861f226a3ec06a27d0a5bf36e4244a982da077e8f"))
            (  27255, uint256S("0x00000000000005112a0debf53703eb3dc4ec2d8d68599c90db71423ea14489b7"))
            (  70623, uint256S("0x00000000000004767ff6e509d00772af5c4bedaa82c38c1e95c33adbf5ff84f5"))
            ( 112567, uint256S("0x000000000000018c0621bf32ab33d3ca871509f406f08be6dd20facea747b099"))
            ( 141845, uint256S("0x00000000000000f62d14d55c2bc3ec0ba94e4f2b3868bbe7be9cb5b681fcc0fb"))
            ( 149540, uint256S("0x000000000000061b8f5b8653fe13b5e453347d9386d43d09445ee5e26a8222bb"))
            ( 348178, uint256S("0x0000000000000a410c6aff939087769e757132098fa0b0ce89f11d68f935077d"))
            ( 431747, uint256S("0x000000000000065616abeccd681f7b4d6d7bed06deef0e1a6e70c905edae3692"))
            ( 490168, uint256S("0x000000000000155ce9845e501617f152127e32c2c468edc643809f9988d30327"))
            ( 557719, uint256S("0x0000000000000851f690d42129b34408697c88316879debd413049959517d7c6"))
            ( 740001, uint256S("0x00000000000002c7aaa3ad688eb5af7d2779f4aa1d9a1b9a5c3cfb4ccad56caa"))
            ( 740576, uint256S("0x00000000000043dc047538e724f17a388146f22dd2ecf66f3c88253799912f4d"))
            ( 766081, uint256S("0x00000000000001e604a7acc2dd539ff8bd51224a7aac8647736d1af10160427d"))
            (1007712, uint256S("0x00000000000013e0fb1ec8ba2ce0b04b1547d9ae7d9c9e5a2691aeca87559a12"))
            (1077647, uint256S("0x00000000000038b85361a79dd4a04b1f01ec2cc60f4176185363aa4522aab12d"))
            (1177283, uint256S("0x000000000000359b1fb371d92db45de64b449ea72863a51fa78fea6b7ff3989f"))
            (1336174, uint256S("0x00000000000001eafb1051fda69eb842577a0e48e4f0682bd7f33677373d0807"))
            (1402167, uint256S("0x8215cd9feb2db9bf74514361a9d4b4440f054101079df8bb6bf629065db3ab29"))
            (1449500, uint256S("0xef19939e314f239999b9959ac908b20a43b165719d1c35dcfe38ba2566d20765"))
            (1681927, uint256S("0x9be88de17bccfa3ceb43d158652189af2c3038460dcf91732320a73557fe6a2e"))
            (1764003, uint256S("0xf969487c0ba2426bf767a5cb83c6bb7f3f9a19cccdcfc50cf5db0b39032aae55"))
            (1863106, uint256S("0xc2a2012bac1de0e0039efb958e3465390eb7dcd439d83be077dc1c1006d0ebd6"))
            (2210002, uint256S("0x9e16b567c393c46e40b4b4c4a21283b998aec17b3c7edf8319b3860e4403e4bf"))
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000000000000166d612d5595e2b1cd88d71d695fc580af64d8da8658c23 (height 2210002).
            1507861206, // * UNIX timestamp of last known number of transactions
            5243041,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02        // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

        /*** Myriadcoin Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A; 
        consensus.fStrictChainId = false;

        consensus.nBlockTimeWarpPreventStart1 = 1000; // block where time warp 1 prevention starts
        consensus.nBlockTimeWarpPreventStart2 = 1005; // block where time warp 2 prevention starts
        consensus.nBlockTimeWarpPreventStart3 = 1010; // block where time warp 3 prevention starts
        consensus.Phase2Timespan_Start = 150; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 150; // block where difficulty adjust V2 starts

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUpV1 = 2; // 2% adjustment up
        consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up     

        consensus.nBlockSequentialAlgoRuleStart1 = 200; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 250; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 6; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo

        consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 150;
        consensus.nFork1MinBlock = 601; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)

        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1504224000; // September 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1535760000; // September 1st, 2018

        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 1516320000; // Jan 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 1548979200; // Feb 1st, 2019

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788");
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.MIP2Height = 1;
        consensus.MIP1Height = 1; // allow from block 1
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1504224000; // September 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1535760000; // September 1st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1506816000; // October 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1538352000; // October 1st, 2018

        // Deployment of Equihash (MIP1)
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nStartTime = 1514764800; // Jan 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nTimeout = 1546300800; // Jan 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xff983c72147a81ac5b8ebfc68b62b39358cac4b8eb5518242e87f499b71c6a51"); // 1

        pchMessageStart[0] = 0x02;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xa4;
        nDefaultPort = 20888;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis = CreateGenesisBlock(1392876393, 416875379, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testseed1.myriadcoin.org", "testseed1.myriadcoin.org"));
        vSeeds.push_back(CDNSSeedData("myriadtestseed1.cryptap.us", "myriadtestseed1.cryptap.us")); // cryptapus

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,188);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        // Equihash Parameters
        const size_t N = 200, K = 9;
        // BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (   0, uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"))
        };

        chainTxData = ChainTxData{
            1392876393,
            0,
            0.02
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";

        /*** Myriadcoin Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A;
        consensus.fStrictChainId = false;

        consensus.nBlockTimeWarpPreventStart1 = 1000; // block where time warp 1 prevention starts
        consensus.nBlockTimeWarpPreventStart2 = 1005; // block where time warp 2 prevention starts
        consensus.nBlockTimeWarpPreventStart3 = 1010; // block where time warp 3 prevention starts
        consensus.Phase2Timespan_Start = 150; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 150; // block where difficulty adjust V2 starts

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUpV1 = 2; // 2% adjustment up
        consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up

        consensus.nBlockSequentialAlgoRuleStart1 = 200; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 250; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 6; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo

        consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 0;
        consensus.nFork1MinBlock = 601; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)

        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 999999999999ULL;

        // Deployment of MIP2 (Reserve algorithm ids)
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 0; 
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 999999999999ULL;

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.MIP2Height = 1;
        consensus.MIP1Height = 1; // allow from block 1
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"));
        assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
