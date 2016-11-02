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
            table[t].push_back(dummy);
        }
    }
}


    template<class Impl>
void BMT<Impl>::allocEntry(DynInstPtr &inst, ThreadID tid)
{
    for (unsigned i = 0; i < numLQEntries; i++) {
        if (table[tid][i].orbv == 0) {
            table[tid][i].orbv |= getDestRegs(inst);
        }
    }
}

    template<class Impl>
void BMT<Impl>::merge(DynInstPtr &inst)
{
    BME* first = nullptr;
    ThreadID tid = inst->threadNumber;
    uint64_t srcVec = getSrcRegs(inst);
    uint64_t destVec = getDestRegs(inst);
    uint64_t oldOrbv = 0;

    for (int i = 0; i < numLQEntries; i++) {

        if ((srcVec & table[tid][i].orbv) != 0) {

            if (first == nullptr) {
                first = &table[tid][i];
                oldOrbv = first->orbv;

            } else {
                first->orbv |= table[tid][i].orbv;
                table[tid][i].dic = 0;
            }
        }
    }
    first->orbv |= destVec;
    first->dic += count1(first->orbv ^ oldOrbv);
}


    template<class Impl>
void BMT<Impl>::update(DynInstPtr &inst)
{
    int depCount = 0;
    uint64_t srcVec = getSrcRegs(inst);
    ThreadID tid = inst->threadNumber;
    BME *dep = nullptr;

    // Tell out if inst is dependent on some families
    for (int i = 0; i < numLQEntries; i++) {
        if ((srcVec & table[tid][i].orbv) != 0) {
            depCount += 1;
            dep = &table[tid][i];
        }
    }

    if (depCount == 0) {
        if (inst->isLoad()) {
            allocEntry(inst, tid);

        } else {
            // override these output registers
            uint64_t dstVec = getDestRegs(inst);

            for (int i = 0; i < numLQEntries; i++) {
                table[tid][i].orbv &= ~dstVec;
            }
        }

    } else if (depCount == 1) {
        // update dep
        dep->orbv |= getDestRegs(inst);
        dep->dic += 1;

    } else {
        merge(inst); // merge two dependency tree
    }

}

    template<class Impl>
int BMT<Impl>::computeMLP(ThreadID tid)
{
    int i, counter = 0;

    for (i = 0; i < table[tid].size(); i++) {
        if (table[tid][i].orbv != 0) {
            counter += 1;
        }
    }

    return counter;
}

    template<class Impl>
uint64_t BMT<Impl>::getSrcRegs(DynInstPtr &inst)
{
    const StaticInstPtr& stInst = inst->staticInst;
    uint64_t vec = 0;
    for (int i = 0; i < stInst->numSrcRegs(); i++) {
        vec |= 1 << stInst->srcRegIdx(i);
    }
    return vec;
}

    template<class Impl>
uint64_t BMT<Impl>::getDestRegs(DynInstPtr &inst)
{
    const StaticInstPtr& stInst = inst->staticInst;
    uint64_t vec = 0;
    for (int i = 0; i < stInst->numDestRegs(); i++) {
        vec |= 1 << stInst->destRegIdx(i);
    }
    return vec;
}

#endif  // __CPU_O3_BMT_IMPL_HH__
