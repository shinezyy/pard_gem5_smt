#ifndef __CPU_O3_RM_IMPL_HH__
#define __CPU_O3_RM_IMPL_HH__

#include <fstream>
#include <string>
#include "config/the_isa.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/resource_manager.hh"

template<class Impl>
ResourceManager<Impl>::ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params)
    : cpu(_cpu){}

template<class Impl>
std::string
ResourceManager<Impl>::name() const
{
    return cpu->name() + ".resourceManager";
}

template<class Impl>
void
ResourceManager<Impl>::setInstQueue(IQ *_instQueue)
{
    instQueue = _instQueue;
}

template<class Impl>
void
ResourceManager<Impl>::preserveInstQueue()
{
    int portion[] = {512, 512};

    if(readConfig()) {
        if(config.find("InstQueuePortion") != config.end()) {
            portion[0] = config["InstQueuePortion"];
            assert(portion[0] <= Denominator);
            portion[1] = 1 - portion[0];
        }
    }

    instQueue->reassignPortion(portion, 2);
}

template<class Impl>
bool
ResourceManager<Impl>::readConfig()
{
    std::fstream fin;
    fin.open("config.txt", std::fstream::in);

    if (!fin.is_open()) {
        std::cerr << "Failed to open config file\n";
        return false;
    }

    std::cout << "Read config file in!\n";
    while (!fin.eof()) {
        std::string parameter_name;
        int parameter_val;
        fin >> parameter_name >> parameter_val;
        config[parameter_name] = parameter_val;
    }

    fin.close();
    return true;
}

#endif
