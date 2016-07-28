#ifndef __CPU_O3_RM_HH__
#define __CPU_O3_RM_HH__

#include <map>
#include <rapidjson/document.h>

struct DerivO3CPUParams;

template<class Impl>
class ResourceManager {

    typedef typename Impl::CPUPol CPUPol;
    typedef typename Impl::O3CPU O3CPU;
    typedef typename CPUPol::IQ IQ;
    typedef typename CPUPol::ROB ROB;

    private:

        IQ *instQueue;

        ROB *rob;

        O3CPU *cpu;

        // TODO Document is a typedef, so we cannot pre-declare it...
        rapidjson::Document config;

        bool configUpdated;

    public:

        ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params);

        std::string name() const;

        void setIQ(IQ *_instQueue);

        void setROB(ROB *_rob);

        void reserveIQ();

        void reserveROB();

        void reconfigIssuePrio();

        bool readConfig();
};

#endif
