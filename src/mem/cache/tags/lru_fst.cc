/*
 * Copyright (c) 2012-2013 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2003-2005,2014 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Erik Hallnor
 */

/**
 * @file
 * Definitions of a LRUFst tag store.
 */

#include "debug/CacheRepl.hh"
#include "mem/cache/tags/lru_fst.hh"
#include "mem/cache/base.hh"


LRUFst::LRUFst(const Params *p)
    : BaseSetAssoc(p)
{
    assert(numSets < MAX_NUM_SETS);
    for (int i = 0; i < numSets; i++) {
        for (int j = 0; j < assoc; j++) {
            sets[i].blks[j]->threadID = -1;
        }
    }

    // Init bits to 0, indicating no interference.
    for (int i = 0; i < numSets; i++) {
        interferedSetPerThread[i][0] =
        interferedSetPerThread[i][1] = 0;
    }
}

CacheBlk*
LRUFst::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);

    if (blk != NULL) {
        // move this block to head of the MRU list
        sets[blk->set].moveToHead(blk);
        // Check the coherence of threadID assigned when inserted.
        // ??? The different thread can hit on the same line!?
        // assert(curThreadID == blk->threadID);
        assert(blk->threadID >= 0);  // invalid ones with -1
        DPRINTF(CacheRepl, "set %x: moving blk %x (%s) to MRU\n",
                blk->set, regenerateBlkAddr(blk->tag, blk->set),
                is_secure ? "s" : "ns");
    }
    else if (interferedSetPerThread[extractSet(addr)][curThreadID] == 1) {
        // We get a miss and the bit vector tells us some ways belongs to this
        // thread have been evicted by the other thread.
        interferedSetPerThread[extractSet(addr)][curThreadID] = 0;
        // TODO Update the CPU's state to indicate that it got interfered in cache.

        // TODO Why not using a counter instead of a bit, which can
        // record how many ways has been evicted by others,
        // indicating how badly this thread is interfered?
    }

    return blk;
}

CacheBlk*
LRUFst::findVictim(Addr addr)
{
    assert(curThreadID >= 0);
    int set = extractSet(addr);
    // grab a replacement candidate
    BlkType *blk = sets[set].blks[assoc - 1];

    if (blk->isValid()) {
        DPRINTF(CacheRepl, "set %x: selecting blk %x for replacement\n",
                set, regenerateBlkAddr(blk->tag, set));
        assert(blk->threadID >= 0);
        if (blk->threadID != curThreadID) {
            /* TODO Mark the set array of the blk->threadID
               as it got interfered */
            interferedSetPerThread[set][blk->threadID] = 1;
        }
    }

    blk->threadID = curThreadID;
    return blk;
}

void
LRUFst::insertBlock(PacketPtr pkt, BlkType *blk)
{
    // insertBlock is called in the specific thread context.
    // so we can determine that this block belongs to thread
    // <curThreadID>
    BaseSetAssoc::insertBlock(pkt, blk);

    assert(blk->threadID >= 0);
    int set = extractSet(pkt->getAddr());
    sets[set].moveToHead(blk);
}

void
LRUFst::invalidate(CacheBlk *blk)
{
    BaseSetAssoc::invalidate(blk);

    // should be evicted before valid blocks
    int set = blk->set;
    sets[set].moveToTail(blk);

    // Reset block belonging status
    assert(blk->threadID >= 0);
    blk->threadID = -1;
}

LRUFst*
LRUFstParams::create()
{
    return new LRUFst(this);
}
