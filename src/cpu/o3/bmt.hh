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
    //typedef typename Impl::StaticInstPtr StaticInstPtr;
    typedef typename CPUPol::Fetch Fetch;
    typedef typename CPUPol::Decode Decode;
    typedef typename CPUPol::IEW IEW;
    typedef typename TheISA::RegIndex RegIndex;


    public:

    struct BME { // Backend Miss Entry

        // Long-latency Load ID
        InstSeqNum llid;

        // Output Register Bit Vector
        uint8_t orbv[64];

        // dependent instruction counter
        int dic;

        /* number of architectual register defined by this Load and its
         * dependent instructions
         */
        int numDefRegs;

        // Pointer to the Long-latency Load
        // DynInstPtr& dlp;

        ThreadID tid;
    };


    private:

    O3CPU *cpu;

    std::vector<BME> table;

    ThreadID numThreads;

    int numROBEntries;

    int numLQEntries;


    public:

    BMT(O3CPU *cpu_ptr, DerivO3CPUParams *params);

    // allocate a BME for the loading inst, and initiate the entry
    void allocEntry(DynInstPtr& inst);

    // update the orbv and dic when an instruction commit
    void update(DynInstPtr& inst);

    void init(DerivO3CPUParams *params);

    int computeMLP(ThreadID tid);
};

#endif // __CPU_O3_VOC_HH__
