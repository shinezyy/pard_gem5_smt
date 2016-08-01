#ifndef __CPU_O3_RM_HH__
#define __CPU_O3_RM_HH__

#include <map>
#include <rapidjson/document.h>

#include "config/the_isa.hh"

struct DerivO3CPUParams;

template<class Impl>
class ResourceManager {

    typedef typename Impl::CPUPol CPUPol;
    typedef typename Impl::O3CPU O3CPU;
    typedef typename CPUPol::IQ IQ;
    typedef typename CPUPol::ROB ROB;
    typedef typename CPUPol::LSQ LSQ;
    typedef typename CPUPol::Rename Rename;
    typedef typename CPUPol::Fetch Fetch;

    private:

        IQ *instQueue;

        ROB *rob;

        LSQ *lsq;

        O3CPU *cpu;

        Rename *rename;

        Fetch *fetch;

        // TODO Document is a typedef, so we cannot pre-declare it...
        rapidjson::Document config;

        bool configUpdated;

    public:

        ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params);

        std::string name() const;

        void setIQ(IQ *_instQueue);

        void setROB(ROB *_rob);

        void setLSQ(LSQ *_lsq);

        void setRename(Rename *_rename);

        void setFetch(Fetch *_fetch);

        void reserveIQ();

        void reserveROB();

        void reserveLQ();

        void reserveSQ();

        void reserveRename();

        void reserveDecode();

        void reconfigIssuePrio();

        bool readConfig();
};

#endif
