// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <primitives/pureheader.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <arith_uint256.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

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
        consensus.nFork2MinBlock = 2772000; // minimum block height where fork 2 takes effect (algo switch, GetGeometricPrevWork2)

        consensus.MIP2Height = 2328480;

        // Legbit parameters (used for v0.11 -> v0.14 compatability)
        consensus.nLegbitStart = 2306304;
        consensus.nLegbitStop = 2586528;

        // MIP3 parameters
        consensus.nPowTargetSpacingV3a = 2 * 60; // new target time for block spacing across all algorithms (2min)
        consensus.nPowTargetSpacingV3b = 4 * 60; // new target time for block spacing across all algorithms (4min)
        consensus.nPowTargetSpacingV3c = 8 * 60; // new target time for block spacing across all algorithms (8min)
        consensus.nLongblocks_StartV1a = 2903040; // Start of longblocks, original 3rd block halving
        consensus.nLongblocks_StartV1b = 3386880; // Start of longblocks, original 4th block halving
        consensus.nLongblocks_StartV1c = 3628800; // Start of longblocks, original 5th block halving
        consensus.nSubsidyHalvingIntervalV2a = 483840; // Longblocks Subsidy halving after adjust block time (2min)
        consensus.nSubsidyHalvingIntervalV2b = 241920; // Longblocks Subsidy halving after adjust block time (4min)
        consensus.nSubsidyHalvingIntervalV2c = 120960; // Longblocks Subsidy halving after adjust block time (8min)

        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1507420800; // October 8th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1538956800; // October 8th, 2018

        // Deployment of MIP2 (Reserve algorithm ids)
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 1516320000; // Jan 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 1548979200; // Feb 1st, 2019

        // Deployment of MIP3 (longblocks)
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nStartTime = 1525132800; // May 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nTimeout = 1556668800; // May 1st, 2019

        // Deployment of Argon2d (Argon2d4096 mining replacing Skein)
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nStartTime = 1550188800; // February 15th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nTimeout = 1581724800; // February 15th, 2020

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 967680;
        consensus.BIP16Exception = uint256S("0xcb41589c918fba1beccca8bc6b34b2b928b4f9888595d7664afd6ec60a576291");
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xcb41589c918fba1beccca8bc6b34b2b928b4f9888595d7664afd6ec60a576291");
        consensus.BIP65Height = 1764000; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
        consensus.BIP66Height = 1764000; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
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

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0ce40000000000000000000000000000000000000000167e92f47c43f03e9eb4"); //2772278

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0e7606672e9651e8b2e04b8463b824c7149300ac1b79610d79150996bad34b18"); //2772278

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
        m_assumed_blockchain_size = 240;
        m_assumed_chain_state_size = 3;

        genesis = CreateGenesisBlock(1393164995, 2092903596, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000ffde4c020b5938441a0ea3d314bf619eff0b38f32f78f7583cffa1ea485"));
        assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seed1.myriadcoin.org");
        vSeeds.emplace_back("seed2.myriadcoin.org");
        vSeeds.emplace_back("seed3.myriadcoin.org");
        vSeeds.emplace_back("seed4.myriadcoin.org");
        vSeeds.emplace_back("seed5.myriadcoin.org");
        vSeeds.emplace_back("seed6.myriadcoin.org");
        vSeeds.emplace_back("seed7.myriadcoin.org");
        vSeeds.emplace_back("seed8.myriadcoin.org");
        vSeeds.emplace_back("myriadseed1.cryptapus.org"); // cryptapus

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,9);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "my";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {      0, uint256S("0x00000ffde4c020b5938441a0ea3d314bf619eff0b38f32f78f7583cffa1ea485")},
                {   9646, uint256S("0x0000000000000b0f1372211861f226a3ec06a27d0a5bf36e4244a982da077e8f")},
                {  27255, uint256S("0x00000000000005112a0debf53703eb3dc4ec2d8d68599c90db71423ea14489b7")},
                {  70623, uint256S("0x00000000000004767ff6e509d00772af5c4bedaa82c38c1e95c33adbf5ff84f5")},
                { 112567, uint256S("0x000000000000018c0621bf32ab33d3ca871509f406f08be6dd20facea747b099")},
                { 141845, uint256S("0x00000000000000f62d14d55c2bc3ec0ba94e4f2b3868bbe7be9cb5b681fcc0fb")},
                { 149540, uint256S("0x000000000000061b8f5b8653fe13b5e453347d9386d43d09445ee5e26a8222bb")},
                { 348178, uint256S("0x0000000000000a410c6aff939087769e757132098fa0b0ce89f11d68f935077d")},
                { 431747, uint256S("0x000000000000065616abeccd681f7b4d6d7bed06deef0e1a6e70c905edae3692")},
                { 490168, uint256S("0x000000000000155ce9845e501617f152127e32c2c468edc643809f9988d30327")},
                { 557719, uint256S("0x0000000000000851f690d42129b34408697c88316879debd413049959517d7c6")},
                { 740001, uint256S("0x00000000000002c7aaa3ad688eb5af7d2779f4aa1d9a1b9a5c3cfb4ccad56caa")},
                { 740576, uint256S("0x00000000000043dc047538e724f17a388146f22dd2ecf66f3c88253799912f4d")},
                { 766081, uint256S("0x00000000000001e604a7acc2dd539ff8bd51224a7aac8647736d1af10160427d")},
                {1007712, uint256S("0x00000000000013e0fb1ec8ba2ce0b04b1547d9ae7d9c9e5a2691aeca87559a12")},
                {1077647, uint256S("0x00000000000038b85361a79dd4a04b1f01ec2cc60f4176185363aa4522aab12d")},
                {1177283, uint256S("0x000000000000359b1fb371d92db45de64b449ea72863a51fa78fea6b7ff3989f")},
                {1336174, uint256S("0x00000000000001eafb1051fda69eb842577a0e48e4f0682bd7f33677373d0807")},
                {1402167, uint256S("0x8215cd9feb2db9bf74514361a9d4b4440f054101079df8bb6bf629065db3ab29")},
                {1449500, uint256S("0xef19939e314f239999b9959ac908b20a43b165719d1c35dcfe38ba2566d20765")},
                {1681927, uint256S("0x9be88de17bccfa3ceb43d158652189af2c3038460dcf91732320a73557fe6a2e")},
                {1764003, uint256S("0xf969487c0ba2426bf767a5cb83c6bb7f3f9a19cccdcfc50cf5db0b39032aae55")},
                {1863106, uint256S("0xc2a2012bac1de0e0039efb958e3465390eb7dcd439d83be077dc1c1006d0ebd6")},
                {2210002, uint256S("0x9e16b567c393c46e40b4b4c4a21283b998aec17b3c7edf8319b3860e4403e4bf")},
                {2587043, uint256S("0x6bebe78892419acf8f47fa34ac08417036c244a244d209dbb5113edfda7ae480")},
                {2772278, uint256S("0x0e7606672e9651e8b2e04b8463b824c7149300ac1b79610d79150996bad34b18")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0e7606672e9651e8b2e04b8463b824c7149300ac1b79610d79150996bad34b18
            /* nTime    */ 1555832230,
            /* nTxCount */ 6343318,
            /* dTxRate  */ 0.0172
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

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
        consensus.nFork2MinBlock = 32256; // minimum block height where fork 2 takes effect (algo switch, GetGeometricPrevWork2)

        consensus.MIP2Height = 1;

        // Legbit parameters (used for v0.11 -> v0.14 compatability)
        consensus.nLegbitStart = 8064;
        consensus.nLegbitStop = 26208;

        // MIP3 parameters
        consensus.nPowTargetSpacingV3a = 2 * 60; // new target time for block spacing across all algorithms (2min)
        consensus.nPowTargetSpacingV3b = 4 * 60; // new target time for block spacing across all algorithms (4min)
        consensus.nPowTargetSpacingV3c = 8 * 60; // new target time for block spacing across all algorithms (8min)
        consensus.nLongblocks_StartV1a = 2903040; // Start of longblocks, original 3rd block halving
        consensus.nLongblocks_StartV1b = 3386880; // Start of longblocks, original 4th block halving
        consensus.nLongblocks_StartV1c = 3628800; // Start of longblocks, original 5th block halving
        consensus.nSubsidyHalvingIntervalV2a = 483840; // Longblocks Subsidy halving after adjust block time (2min)
        consensus.nSubsidyHalvingIntervalV2b = 241920; // Longblocks Subsidy halving after adjust block time (4min)
        consensus.nSubsidyHalvingIntervalV2c = 120960; // Longblocks Subsidy halving after adjust block time (8min)

        // Notice for BIP9 bit 2: Please see 'legbit' notes in validation.cpp prior to future use.
        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1504224000; // September 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1535760000; // September 1st, 2018

        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 1516320000; // Jan 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 1548979200; // Feb 1st, 2019

        // Deployment of MIP3 (longblocks)
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nStartTime = 1525132800; // May 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nTimeout = 1556668800; // May 1st, 2019

        // Deployment of Argon2d (Argon2d4096 miniing replacing Skein)
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nStartTime = 1546300800; // January 1st, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nTimeout = 1577836800; // January 1st, 2020

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 967680;
        consensus.BIP16Exception = uint256S("0x0000d23adc28e33bc05f4bee57c873ae0aab584a6a436e75ac0ed40396f6d86b");
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x0000d23adc28e33bc05f4bee57c873ae0aab584a6a436e75ac0ed40396f6d86b");
        consensus.BIP65Height = 641; // ff983c72147a81ac5b8ebfc68b62b39358cac4b8eb5518242e87f499b71c6a51
        consensus.BIP66Height = 641; // ff983c72147a81ac5b8ebfc68b62b39358cac4b8eb5518242e87f499b71c6a51
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

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5"); // 26607

        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xa4;
        nDefaultPort = 20888;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 30;
        m_assumed_chain_state_size = 2;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis = CreateGenesisBlock(1392876393, 416875379, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testseed1.myriadcoin.org");
        vSeeds.emplace_back("myriadtestseed1.cryptapus.org"); // cryptapus

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,188);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tm";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                {     0, uint256S("0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788")},
                {   800, uint256S("00000071942cef6d87635a92f106d5b1935b1314538af80922c766487afd8b22")},
                { 26607, uint256S("000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5
            /* nTime    */ 1549032928,
            /* nTxCount */ 26673,
            /* dTxRate  */ 0.00166
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";

        /*** Myriadcoin Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 0;
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
        consensus.nFork2MinBlock = 32256; // minimum block height where fork 2 takes effect (algo switch, GetGeometricPrevWork2)

        consensus.MIP2Height = 1;

        // Legbit parameters (used for v0.11 -> v0.14 compatability)
        consensus.nLegbitStart = 0;
        consensus.nLegbitStop = 0;

        // MIP3 parameters
        consensus.nPowTargetSpacingV3a = 2 * 60; // new target time for block spacing across all algorithms (2min)
        consensus.nPowTargetSpacingV3b = 4 * 60; // new target time for block spacing across all algorithms (4min)
        consensus.nPowTargetSpacingV3c = 8 * 60; // new target time for block spacing across all algorithms (8min)
        consensus.nLongblocks_StartV1a = 450; // Start of longblocks, original 3rd block halving
        consensus.nLongblocks_StartV1b = 525; // Start of longblocks, original 4th block halving
        consensus.nLongblocks_StartV1c = 562; // Start of longblocks, original 5th block halving
        consensus.nSubsidyHalvingIntervalV2a = 75; // Longblocks Subsidy halving after adjust block time (2min)
        consensus.nSubsidyHalvingIntervalV2b = 37; // Longblocks Subsidy halving after adjust block time (4min)
        consensus.nSubsidyHalvingIntervalV2c = 18; // Longblocks Subsidy halving after adjust block time (8min)

        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 999999999999ULL;

        // Deployment of MIP2 (Reserve algorithm ids)
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 999999999999ULL;

        // Deployment of MIP3 (longblocks)
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LONGBLOCKS].nTimeout = 999999999999ULL;

        // Deployment of Argon2d (Argon2d4096 miniing replacing Skein)
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ARGON2D].nTimeout = 999999999999ULL;

        /*** Upstream Chainparams ***/

        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

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
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateVersionBitsParametersFromArgs(args);

        genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"));
        assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "bcrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateVersionBitsParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateVersionBitsParametersFromArgs(const ArgsManager& args)
{
    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
