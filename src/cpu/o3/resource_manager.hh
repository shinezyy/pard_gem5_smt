#ifndef __CPU_O3_RM_HH__
#define __CPU_O3_RM_HH__

#include <map>
#include <rapidjson/document.h>

#define Denominator 1024

struct DerivO3CPUParams;

template<class Impl>
class ResourceManager {

    typedef typename Impl::CPUPol CPUPol;
    typedef typename Impl::O3CPU O3CPU;
    typedef typename CPUPol::IQ IQ;

    private:
        IQ *instQueue;

        O3CPU *cpu;

        // TODO Document is a typedef, so we cannot pre-declare it...
        rapidjson::Document config;

    public:
        ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params);

        std::string name() const;

        void setInstQueue(IQ *_instQueue);

        void preserveInstQueue();

        bool readConfig();
};

#endif
