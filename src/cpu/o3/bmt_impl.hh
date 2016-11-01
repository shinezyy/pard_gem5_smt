#ifndef __CPU_O3_FMT_IMPL_HH__
#define __CPU_O3_FMT_IMPL_HH__


#include "cpu/o3/comm.hh"
#include "debug/BMT.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/bmt.hh"


    template<class Impl>
    BMT<Impl>::BMT(O3CPU *cpu_ptr, DerivO3CPUParams *params)
: cpu(cpu_ptr),
    numThreads(params->numThreads),
    numROBEntries(params->numROBEntries),
    numLQEntries(params->LQEntries)
{
}


    template<class Impl>
void BMT<Impl>::init(DerivO3CPUParams *params)
{
    for (unsigned i = 0; i < numLQEntries; i++) {
        BME dummy;
        bzero((void *)&dummy, sizeof(BME));
        table.push_back(dummy);
    }
}


    template<class Impl>
void BMT<Impl>::allocEntry(DynInstPtr& inst)
{
    const StaticInstPtr& stInst = inst->staticInst;
    BME *ent;
    int i;

    for (i = 0; i < table.size(); i++) {
        if (table[i].numDefRegs == 0) {
            ent = &table[i];
            break;
        }
    }

    if (i == table.size()) {
        panic("No BMT entry left\n");
    }

    ent->llid = inst->seqNum % numROBEntries;

    for (int i = 0; i < stInst->numDestRegs(); i++) {

        ent->orbv[stInst->destRegIdx(i)] = 1;
    }

    ent->dic = 1; // or 0 ?
    ent->numDefRegs = 1;
    ent->tid = inst->threadNumber;
}


    template<class Impl>
void BMT<Impl>::update(DynInstPtr& inst)
{
    const StaticInstPtr& stInst = inst->staticInst;

    for (typename std::vector<BME>::iterator it = table.begin();
            it != table.end(); it++) {

        int i;
        for (i = 0; i < stInst->numDestRegs(); i++) {
            RegIndex srcReg = stInst->srcRegIdx(i);

            if (it->orbv[srcReg] == 1) {

                for (int d = 0; d < stInst->numDestRegs(); d++) {

                    it->orbv[stInst->destRegIdx(d)] = 1;
                }

                it->numDefRegs += stInst->numDestRegs();
                it->dic += 1;
                break;
            }
        }

        // no dependency
        if (i == stInst->numDestRegs()) {

            for (int d = 0; d < stInst->numDestRegs(); d++) {

                it->orbv[stInst->destRegIdx(d)] = 0;
            }
            it->numDefRegs -= stInst->numDestRegs();
        }
    }
}


template<class Impl>
int BMT<Impl>::computeMLP(ThreadID tid) {

    int i, counter = 0;

    for (i = 0; i < table.size(); i++) {
        if (table[i].numDefRegs == 1) {
            counter += 1;
        }
    }

    return counter;
}


#endif  // __CPU_O3_FETCH_IMPL_HH__
