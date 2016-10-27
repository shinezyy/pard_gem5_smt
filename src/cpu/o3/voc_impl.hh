#ifndef __CPU_O3_VOC_IMPL_HH__
#define __CPU_O3_VOC_IMPL_HH__


#include "cpu/o3/comm.hh"
#include "debug/VOC.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/voc.hh"

template<class Impl>
VOC<Impl>::VOC(O3CPU *cpu_ptr, DerivO3CPUParams *params):
    cpu(cpu_ptr),
    numThreads(params->numThreads)
{
}

template<class Impl>
void VOC<Impl>::freeVrob(ThreadID tid, DynInstPtr& inst)
{
    counter[tid] += inst->getWaitSlot();
}

#endif  // __CPU_O3_VOC_IMPL_HH__
