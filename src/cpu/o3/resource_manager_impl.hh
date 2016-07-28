#ifndef __CPU_O3_RM_IMPL_HH__
#define __CPU_O3_RM_IMPL_HH__

#include <string>
#include <cstdio>
#include <rapidjson/filereadstream.h>

#include "config/the_isa.hh"
#include "params/DerivO3CPU.hh"
#include "cpu/o3/resource_manager.hh"

template<class Impl>
ResourceManager<Impl>::ResourceManager(O3CPU *_cpu, DerivO3CPUParams *params)
    : cpu(_cpu) {}

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
    int denominator = 1024;

    if(readConfig()) {
        if(config.HasMember("InstQueuePortionDenominator")) {
            denominator = config["InstQueuePortionDenominator"].GetInt();
            printf("new denominator is %d\n", denominator);
        }

        if(config.HasMember("InstQueuePortion")) {
            portion[0] = config["InstQueuePortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        }
    }

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        printf("portion[%d]: %d\n", i, portion[i]);
    }

    instQueue->reassignPortion(portion, 2, denominator);
}

template<class Impl>
bool
ResourceManager<Impl>::readConfig()
{
    const char *json = "config.json";
    FILE *fp = fopen(json, "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found\n", json);
        return false;
    }

    char readBuffer[0xffff];
    rapidjson::FileReadStream input(fp, readBuffer, sizeof(readBuffer));
    config.ParseStream(input);
    fclose(fp);

    if (config.HasParseError()) {
        fprintf(stderr, "config.json syntax error\n");
        assert(!config.HasParseError());
    }

    return true;
}

#endif
