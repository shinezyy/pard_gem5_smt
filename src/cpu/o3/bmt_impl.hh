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
    numROBEntries(params->numROBEntries)
{
}


    template<class Impl>
void BMT<Impl>::init(DerivO3CPUParams *params)
{
    table.resize(params->LQEntries);
    bmt_ptr = 0;
}


    template<class Impl>
void BMT<Impl>::allocEntry(DynInstPtr& inst)
{
    StaticInstPtr& stInst = inst->staticInst;
    BME *ent = &table[bmt_ptr];

    ent->llid = inst->InstSeqNum % numROBEntries;
    for (int i = 0; i < stInst->numDestRegs(); i++) {
        ent->orbv[stInst->destRegIdx(i)] = 1;
    }
    ent->dic = 1; // or 0 ?
    ent->dlp = inst;
    ent->tid = inst->threadNumber;
}


    template<class Impl>
void BMT<Impl>::update(DynInstPtr& inst)
{
    StaticInstPtr& stInst = inst->staticInst;

    for (std::vector<BME>::iterator it = table.begin();
            it != table.end(); it++) {

        for (int i = 0; i < stInst->numDestRegs(); i++) {
            RegIndex srcReg = stInst->SrcRegIdx(i);

            if (it->orbv[srcReg] == 1) {

                for (int d = 0; d < stInst->numDestRegs(); d++) {

                    it->orbv[stInst->renamedDestRegIdx(d)] = 1;
                }
                break;
            }
        }

        // no dependency
        if (i == stInst->numDestRegs()) {

            for (int d = 0; d < stInst->numDestRegs(); d++) {

                it->orbv[stInst->renamedDestRegIdx(d)] = 0;
            }
        }
    }
}


template<class Impl>
int BMT<Impl>::computeMLP(ThreadID tid) {

}


#endif  // __CPU_O3_FETCH_IMPL_HH__
