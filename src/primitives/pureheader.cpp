// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/pureheader.h>

#include <hash.h>
#include <crypto/hashgroestl.h>
#include <crypto/hashqubit.h>
#include <crypto/hashskein.h>
#include <crypto/scrypt/scrypt.h>
#include <crypto/yescrypt/yescrypt.h>
#include <utilstrencodings.h>

uint256 CPureBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CPureBlockHeader::GetPoWHash(int algo, const Consensus::Params& consensusParams) const
{
    switch (algo)
    {
        case ALGO_SHA256D:
            return GetHash();
        case ALGO_SCRYPT:
        {
            uint256 thash;
            scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
            return thash;
        }
        case ALGO_GROESTL:
            return HashGroestl(BEGIN(nVersion), END(nNonce));
        case ALGO_SKEIN:
            return HashSkein(BEGIN(nVersion), END(nNonce));
        case ALGO_QUBIT:
            return HashQubit(BEGIN(nVersion), END(nNonce));
        case ALGO_YESCRYPT:
        {
            uint256 thash;
            yescrypt_hash(BEGIN(nVersion), BEGIN(thash));
            return thash;
        }
    }
    return GetHash();
}

void CPureBlockHeader::SetBaseVersion(int32_t nBaseVersion, int32_t nChainId)
{
    //assert(nBaseVersion >= 1 && nBaseVersion < VERSION_AUXPOW);
    assert(!IsAuxpow());
    nVersion = nBaseVersion | (nChainId * VERSION_CHAIN_START);
}

int GetAlgo(int nVersion)
{
    switch (nVersion & BLOCK_VERSION_ALGO)
    {
        case 0:
            return ALGO_SHA256D;
        case BLOCK_VERSION_SCRYPT:
            return ALGO_SCRYPT;
        case BLOCK_VERSION_GROESTL:
            return ALGO_GROESTL;
        case BLOCK_VERSION_SKEIN:
            return ALGO_SKEIN;
        case BLOCK_VERSION_QUBIT:
            return ALGO_QUBIT;
        case BLOCK_VERSION_YESCRYPT:
            return ALGO_YESCRYPT;
    }
    return ALGO_SHA256D;
}
