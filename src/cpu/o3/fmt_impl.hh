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
    for (ThreadID tid = 0; tid < numThreads; tid++) {

        table[tid][0].baseSlots = 0;
        table[tid][0].waitSlots = 0;
        table[tid][0].missSlots = 0;
        table[tid][0].initTimeStamp = 0;
    }
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
    BranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.baseSlots++;
}

    template<class Impl>
void FMT<Impl>::incWaitSlot(DynInstPtr &inst, ThreadID tid)
{
    BranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.waitSlots++;
}

    template<class Impl>
void FMT<Impl>::incMissSlot(DynInstPtr &inst, ThreadID tid)
{
    BranchEntryIterator it = table[tid].rbegin();
    for (; it->first > inst->seqNum; it++);
    it->second.missSlots++;
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


#endif  // __CPU_O3_FMT_IMPL_HH__
