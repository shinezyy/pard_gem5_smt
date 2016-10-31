#ifndef __CPU_O3_BMT_HH__
#define __CPU_O3_BMT_HH__


#include <vector>

#include "config/the_isa.hh"

struct DerivO3CPUParams;

template <class Impl>
class BMT {

    typedef typename Impl::CPUPol CPUPol;
    typedef typename Impl::O3CPU O3CPU;
    typedef typename Impl::DynInst DynInst;
    typedef typename Impl::DynInstPtr DynInstPtr;
    typedef typename Impl::StaticInstPtr StaticInstPtr;
    typedef typename CPUPol::Fetch Fetch;
    typedef typename CPUPol::Decode Decode;
    typedef typename CPUPol::IEW IEW;

    public:

    struct BME { // Backend Miss Entry

        // Long-latency Load ID
        InstSeqNum llid;

        // Output Register Bit Vector
        uint8_t orbv[256];

        int dic;

        // Pointer to the Long-latency Load
        DynInstPtr& dlp;

        ThreadID tid;
    };

    uint32_t bmt_ptr; // Used for allocation

    std::vector<BME> table;

    ThreadID numThreads;
    int numROBEntries;

    // allocate a BME for the loading inst, and initiate the entry
    void allocEntry(DynInstPtr& inst);

    // update the orbv and dic when an instruction commit
    void update(DynInstPtr& inst);

    void init(DerivO3CPUParams *params);

    int computeMPL(ThreadID tid);
};

#endif // __CPU_O3_VOC_HH__
