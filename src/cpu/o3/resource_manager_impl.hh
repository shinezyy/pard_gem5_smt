#ifndef __CPU_O3_RM_IMPL_HH__
#define __CPU_O3_RM_IMPL_HH__

#include "config/the_isa.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/resource_manager.hh"

template<class Impl>
ResourceManager<Impl>::ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params)
    : cpu(_cpu){}

template<class Impl>
std::string
ResourceManager<Impl>::name() const
{
    return cpu->name() + ".resource manager";
}

template<class Impl>
void
ResourceManager<Impl>::setInstQueue(IQ *_instQueue)
{
    instQueue = _instQueue;
}

template<class Impl>
void
ResourceManager<Impl>::preserveInstQueue()
{
    int portion[] = {768, 256};
    instQueue->reassignPortion(portion, 2);
}

#endif
