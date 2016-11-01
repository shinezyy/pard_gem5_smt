#ifndef __CPU_O3_BMT_IMPL_HH__
#define __CPU_O3_BMT_IMPL_HH__


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
        for (int t = 0; t < numThreads; t++) {
            table.push_back(dummy);
        }
    }
}

#if 0
// Need to be rebase..
    template<class Impl>
void BMT<Impl>::allocEntry(DynInstPtr& inst)
{
    const StaticInstPtr& stInst = inst->staticInst;
    BME *ent = nullptr;
    int i;

    for (i = 0; i < table.size(); i++) {
        if (table[i].numDefRegs == 0) {
            ent = &table[i];
            break;
        }
    }

    if (i == table.size()) {

        for (int d = 0; d < table.size(); d++) {
            for (int r = 0; r < numLQEntries; r++) {
                if (table[d].orbv[r]) {
                    printf(" *");
                } else {
                    printf(" o");
                }
            }
            printf("\n");
        }

        panic("No BMT entry left\n");

    } else {
        DPRINTF(BMT, "Allocate BMT entry [%d]\n", i);
    }

    ent->llid = inst->seqNum % numROBEntries;

    for (int i = 0; i < stInst->numDestRegs(); i++) {

        ent->orbv[stInst->destRegIdx(i)] = 1;
    }

    ent->dic = 1; // or 0 ?
    ent->numDefRegs = stInst->numDestRegs();
    ent->tid = inst->threadNumber;
}


    template<class Impl>
void BMT<Impl>::merge(DynInstPtr& inst)
{
    BME *inh = nullptr;
    const StaticInstPtr& stInst = inst->staticInst;

    for (i = 0; i < stInst->numDestRegs(); i++) {

        for (typename std::vector<BME>::iterator it = table.begin();
                it != table.end(); it++) {

            RegIndex srcReg = stInst->srcRegIdx(i);
            if (it->orbv[srcReg] == 1) {

                if (inh == nullptr) {
                    inh = &*it;
                } else {

                    // Transfer defined register
                    for (int d = 0; d < numLQEntries; d++) {
                        if (it->orbv[d]) {
                            it->orbv[d] = 0;
                            inh->orbv[d] = 1;
                            inh->numDefRegs += 1;
                        }
                    }
                }
            }
        }
    }
}


    template<class Impl>
void BMT<Impl>::update(DynInstPtr& inst)
{
    const StaticInstPtr& stInst = inst->staticInst;

    /* Stupid code ?
       if (stInst->numSrcRegs() == 0 && !inst->isLoad()) {
       return;
       }
     */

    bool hasDep = false;

    vector<BME *> defSet;

    for (typename std::vector<BME>::iterator it = table.begin();
            it != table.end(); it++) {

        int i;
        for (i = 0; i < stInst->numDestRegs(); i++) {
            RegIndex srcReg = stInst->srcRegIdx(i);

            if (it->orbv[srcReg] == 1) {

                // Whether two family joint...
                bool joint = false;
                for (int j = 0; j < defSet.size(); j++) {
                    if (defSet[j]->orbv[srcReg] == 1) {
                        joint = true;
                    }
                }

                for (int d = 0; d < stInst->numDestRegs(); d++) {

                    // A new register defined by this instruction family
                    if (it->orbv[stInst->destRegIdx(d)] == 0 ) {


                        it->numDefRegs += 1;
                        it->orbv[stInst->destRegIdx(d)] = 1;
                        defSet.push_back(&*it);
                    }
                }

                it->dic += 1;
                hasDep = true;
                break;
            }
        }

        // no dependency on current row, clear destRegs
        if (i == stInst->numDestRegs()) {

            for (int d = 0; d < stInst->numDestRegs(); d++) {
                it->orbv[stInst->destRegIdx(d)] = 0;
                it->numDefRegs -= 1;
            }


            if (it->numDefRegs == 0) {
                DPRINTF(BMT, "Deallocate BMT entry [%d]", i);
            }
        }
    }

    if (!hasDep && inst->isLoad()) {
        allocEntry(inst);
    }
}

#endif


template<class Impl>
int BMT<Impl>::computeMLP(ThreadID tid) {

    int i, counter = 0;

    for (i = 0; i < table[tid].size(); i++) {
        if (table[tid][i].numDefRegs != 0) {
            counter += 1;
        }
    }

    return counter;
}


#endif  // __CPU_O3_BMT_IMPL_HH__
