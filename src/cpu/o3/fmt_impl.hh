#ifndef __CPU_O3_FMT_IMPL_HH__
#define __CPU_O3_FMT_IMPL_HH__

#define __STDC_FORMAT_MACROS // for PRIu64 macro
#include <cinttypes>

#include "cpu/o3/comm.hh"
#include "debug/FMT.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/fmt.hh"


    template<class Impl>
FMT<Impl>::FMT(O3CPU *cpu_ptr, DerivO3CPUParams *params)
    : cpu(cpu_ptr),
    numThreads(params->numThreads)
{
    std::map<InstSeqNum, BranchEntry> entry;
    for (ThreadID tid = 0; tid < numThreads; tid++) {
        table.push_back(entry);
    }

    for (ThreadID tid = 0; tid < numThreads; tid++) {
        BranchEntry dummy;
        bzero((void *)&dummy, sizeof(BranchEntry));
        table[tid].insert(std::make_pair(0, dummy));

        DPRINTF(FMT, "Initiate branch entries for thread %d\n", tid);
        DPRINTF(FMT, "Size of table[%d] is %d\n", tid, table[tid].size());
    }
}


    template<class Impl>
void FMT<Impl>::regStats()
{
    using namespace Stats;

    numBaseSlots
        .init(cpu->numThreads)
        .name(name() + ".numBaseSlots")
        .desc("Number of slots dispatching instruction")
        .flags(display);

    numMissSlots
        .init(cpu->numThreads)
        .name(name() + ".numMissSlots")
        .desc("Number of slots wasted because of stall")
        .flags(display);

    numWaitSlots
        .init(cpu->numThreads)
        .name(name() + ".numWaitSlots")
        .desc("Number of slots wasted because of waiting another thread")
        .flags(display);
}

    template<class Impl>
void FMT<Impl>::setStage(Fetch *_fetch, Decode *_decode, IEW *_iew)
{
    fetch = _fetch;
    decode = _decode;
    iew = _iew;
}

#if 0
    template<class Impl>
void FMT<Impl>::init(std::vector<DynInstPtr> &v_bran, uint64_t timeStamp)
{
    ThreadID tid = 0;
    for (auto&& it : v_bran) {
        addBranch(it, tid++, timeStamp);
    }
}
#endif

    template<class Impl>
void FMT<Impl>::addBranch(DynInstPtr &bran, ThreadID tid, uint64_t timeStamp)
{
    BranchEntry *be = &table[tid][bran->seqNum];
    bzero((void *)be, sizeof(uint64_t)*4);
    be->initTimeStamp = timeStamp;
}

    template<class Impl>
void FMT<Impl>::incBaseSlot(DynInstPtr &inst, ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());
    rBranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.baseSlots++;
    numBaseSlots[tid]++;
}

    template<class Impl>
void FMT<Impl>::incWaitSlot(DynInstPtr &inst, ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());
    rBranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.waitSlots++;
    numWaitSlots[tid]++;
}

    template<class Impl>
void FMT<Impl>::incMissSlot(DynInstPtr &inst, ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());

    rBranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.missSlots++;
    numMissSlots[tid]++;
}

    template<class Impl>
void FMT<Impl>::resolveBranch(bool right, DynInstPtr &bran, ThreadID tid)
{
    if (right) {
        globalBase[tid] += table[tid][bran->seqNum].baseSlots;
        globalMiss[tid] += table[tid][bran->seqNum].missSlots;
        globalWait[tid] += table[tid][bran->seqNum].waitSlots;

    } else {

        globalMiss[tid] += table[tid][bran->seqNum].baseSlots;
        globalMiss[tid] += table[tid][bran->seqNum].missSlots;
        globalMiss[tid] += table[tid][bran->seqNum].waitSlots;
    }
}

    template<class Impl>
void FMT<Impl>::incBaseSlot(ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());
    rBranchEntryIterator it = table[tid].rbegin();
    it->second.baseSlots++;
    numBaseSlots[tid]++;
}

    template<class Impl>
void FMT<Impl>::incMissSlot(ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());
    rBranchEntryIterator it = table[tid].rbegin();
    it->second.missSlots++;
    numMissSlots[tid]++;
}

    template<class Impl>
void FMT<Impl>::incWaitSlot(ThreadID tid)
{
    assert(table[tid].rbegin() != table[tid].rend());
    rBranchEntryIterator it = table[tid].rbegin();
    it->second.waitSlots++;
    numWaitSlots[tid]++;
}


#endif  // __CPU_O3_FMT_IMPL_HH__
