// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "arith_uint256.h"
#include "tinyformat.h"
#include "chainparams.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"

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
        consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of
        consensus.fPowAllowMinDifficultyBlocks = false;
        
        // Allow AuxPow blocks from this height
        consensus.nStartAuxPow = 1402000;
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
        consensus.nBlockSequentialAlgoRuleStart3 = 2000000; // block where sequential algo rule starts ** TODO CHANGE LATER **
        consensus.nBlockSequentialAlgoMaxCount1 = 6; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo
        
        consensus.nBlockAlgoWorkWeightStart = 142000; // block where algo work weighting starts
        consensus.nBlockAlgoNormalisedWorkStart = 740000; // block where algo combined weight starts
        consensus.nBlockAlgoNormalisedWorkDecayStart1 = 866000; // block where weight decay starts
        consensus.nBlockAlgoNormalisedWorkDecayStart2 = 932000; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 1400000;
        consensus.nAlgoSwitchBlock1 = 100000000;
        consensus.nMajorityEnableAlgoSwitch1 = 600; // enable algo switch from qubit to yescrypt with 60% mining consensus
        
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0x45;
        pchMessageStart[2] = 0x76;
        pchMessageStart[3] = 0xee;
        vAlertPubKey = ParseHex("04a82e43bebee0af77bb6d4f830c5b2095b7479a480e91bbbf3547fb261c5e6d1be2c27e3c57503f501480f5027371ec62b2be1b6f00fc746e4b3777259e7f6a78");
        nDefaultPort = 10888;
        nMinerThreads = 0;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

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
        const char* pszTimestamp = "2014-02-23 FT - G20 aims to add $2tn to global economy";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1000 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04e941763c7750969e751bee1ffbe96a651a0feb131db046546c219ea40bff40b95077dc9ba1c05af991588772d8daabbda57386c068fb9bc7477c5e28702d5eb9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 2;
        genesis.nTime    = 1393164995;
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 2092903596;

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

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,9);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x00000ffde4c020b5938441a0ea3d314bf619eff0b38f32f78f7583cffa1ea485"))
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
        (1681927, uint256S("0x9be88de17bccfa3ceb43d158652189af2c3038460dcf91732320a73557fe6a2e")),
            1465113607, // * UNIX timestamp of last checkpoint block
            4409414,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2880     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        
        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A; 
        consensus.fStrictChainId = false;
        
        consensus.nBlockTimeWarpPreventStart1 = 740500; // block where time warp 1 prevention starts
        consensus.nBlockTimeWarpPreventStart2 = 766000; // block where time warp 2 prevention starts
        consensus.nBlockTimeWarpPreventStart3 = 1048320; // block where time warp 3 prevention starts
        consensus.Phase2Timespan_Start = 150; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 150; // block where difficulty adjust V2 starts
        
        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUpV1 = 2; // 2% adjustment up
        consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up     
        
        consensus.nBlockSequentialAlgoRuleStart1 = 200; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 250; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart3 = 300; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 6; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo
        
        consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 150;
        consensus.nAlgoSwitchBlock1 = 100;
        
        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xa4;
        vAlertPubKey = ParseHex("044adf046e6bc86fb83ef92f261fa3feff9176bd029c5ad4afb5c52ac21f9851f2b2eb861cdbf2c09e0cb97dbf75c6ca5ff6c5df88cfb244c72dba1d44b5a47655");
        nDefaultPort = 20888;
        nMinerThreads = 0;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1392876393;
        genesis.nNonce = 416875379;
        consensus.hashGenesisBlock = genesis.GetHash();
        
        assert(consensus.hashGenesisBlock == uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testseed1.myriadcoin.org", "testseed1.myriadcoin.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,188);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788")),
            1392876393,
            1,
            10
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 1);
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x0f;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0x5a;
        nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 4;
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        
        assert(consensus.hashGenesisBlock == uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
