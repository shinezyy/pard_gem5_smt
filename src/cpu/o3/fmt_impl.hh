#ifndef __CPU_O3_FMT_IMPL_HH__
#define __CPU_O3_FMT_IMPL_HH__


#include <map>

#include "cpu/o3/comm.hh"
#include "debug/FMT.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/fmt.hh"

template<class Impl>
FMT<Impl>::FMT(O3CPU *cpu_ptr, DerivO3CPUParams *params)
  : cpu(cpu_ptr),
  numThreads(params->numThreads)
{
}


template<class Impl>
void FMT<Impl>::setStage(Fetch *_fetch, Decode *_decode, IEW *_iew)
{
  fetch = _fetch;
  decode = _decode;
  iew = _iew;
}

template<class Impl>
void FMT<Impl>::init(DynInstPtr &bran, ThreadID tid, uint64_t timeStamp)
{
  addBranch(bran, tid, timeStamp);
}

template<class Impl>
void FMT<Impl>::addBranch(DynInstPtr &bran, ThreadID tid, uint64_t timeStamp)
{
  if (table[tid].find(bran) != table[tid].end()) {
    fatal("Reinsert existing instruction to FMT in thread %d, at"
        "time stamp %" PRId64 "\n", tid, timeStamp);
  }

  BranchEntry *be = &table[tid][bran];
  bzero((void *)be, sizeof(uint64_t)*4);
  be->initTimeStamp = timeStamp;
}


#endif  // __CPU_O3_FETCH_IMPL_HH__
