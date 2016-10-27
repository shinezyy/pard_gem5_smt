#ifndef __CPU_O3_VOC_HH__
#define __CPU_O3_VOC_HH__


#include <cstdint>

#include "config/the_isa.hh"

struct DerivO3CPUParams;

template <class Impl>
class VOC {

    typedef typename Impl::CPUPol CPUPol;
    typedef typename Impl::O3CPU O3CPU;
    typedef typename Impl::DynInst DynInst;
    typedef typename Impl::DynInstPtr DynInstPtr;
    typedef typename CPUPol::Fetch Fetch;
    typedef typename CPUPol::Decode Decode;
    typedef typename CPUPol::IEW IEW;
    typedef typename CPUPol::Commit Commit;


    public:

    private:

    O3CPU *cpu;

    Fetch *fetch;

    Decode *decode;

    IEW *iew;

    Commit *commit;

    ThreadID numThreads;

    uint32_t counter[Impl::MaxThreads];

    uint32_t numROBEntries[Impl::MaxThreads];

    public:

    std::string name() const;

    VOC(O3CPU *cpu_ptr, DerivO3CPUParams *params);

    void setStage(Fetch *_fetch, Decode *_decode, IEW *_iew, Commit *_commit)
    {
        fetch = _fetch;
        decode = _decode;
        iew = _iew;
        commit = _commit;
    }

    void freeVrob(ThreadID tid, DynInstPtr& inst);

    void setVrobSize(ThreadID tid, uint32_t n);

};

#endif // __CPU_O3_VOC_HH__
