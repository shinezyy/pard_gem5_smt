#ifndef __CPU_O3_RM_IMPL_HH__
#define __CPU_O3_RM_IMPL_HH__

#include <string>
#include <cstdio>
#include <rapidjson/filereadstream.h>

#include "cpu/o3/comm.hh"
#include "debug/Pard.hh"
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
ResourceManager<Impl>::setRename(Rename *_rename)
{
    rename = _rename;
}

template<class Impl>
void
ResourceManager<Impl>::setFetch(Fetch *_fetch)
{
    fetch = _fetch;
}

template<class Impl>
void
ResourceManager<Impl>::setIQ(IQ *_instQueue)
{
    instQueue = _instQueue;
}

template<class Impl>
void
ResourceManager<Impl>::setROB(ROB *_rob)
{
    rob = _rob;
}

template<class Impl>
void
ResourceManager<Impl>::setLSQ(LSQ *_lsq)
{
    lsq = _lsq;
}

template<class Impl>
void
ResourceManager<Impl>::reserveIQ()
{
    int portion[] = {512, 512};
    int denominator = 1024;

    if(configUpdated) {
        if(config.HasMember("InstQueuePortionDenominator")) {
            denominator = config["InstQueuePortionDenominator"].GetInt();
            DPRINTF(Pard, "new IQ denominator is %d\n", denominator);
        }

        if(config.HasMember("InstQueuePortion")) {
            portion[0] = config["InstQueuePortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            DPRINTF(Pard, "Instruction queue partition policy not updated\n");
        }
    }

    DPRINTF(Pard, "IQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        DPRINTF(Pard, "IQ portion[%d]: %d\n", i, portion[i]);
    }

    instQueue->reassignPortion(portion, 2, denominator);
}

template<class Impl>
void
ResourceManager<Impl>::reserveLQ()
{
    int portion[] = {512, 512};
    int denominator = 1024;

    if(configUpdated) {
        if(config.HasMember("LSQPortionDenominator")) {
            denominator = config["LSQPortionDenominator"].GetInt();
            DPRINTF(Pard, "new LSQ denominator is %d\n", denominator);
        }

        if(config.HasMember("LQPortion")) {
            portion[0] = config["LQPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            DPRINTF(Pard, "LQ partition policy not updated\n");
        }
    }

    DPRINTF(Pard, "LSQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        DPRINTF(Pard, "LQ portion[%d]: %d\n", i, portion[i]);
    }

    lsq->reassignLQPortion(portion, 2, denominator);
}


template<class Impl>
void
ResourceManager<Impl>::reserveSQ()
{
    int portion[] = {512, 512};
    int denominator = 1024;

    if(configUpdated) {
        if(config.HasMember("LSQPortionDenominator")) {
            denominator = config["LSQPortionDenominator"].GetInt();
            DPRINTF(Pard, "new LSQ denominator is %d\n", denominator);
        }

        if(config.HasMember("SQPortion")) {
            portion[0] = config["SQPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            DPRINTF(Pard, "SQ partition policy not updated\n");
        }
    }

    DPRINTF(Pard, "LSQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        DPRINTF(Pard, "SQ portion[%d]: %d\n", i, portion[i]);
    }

    lsq->reassignSQPortion(portion, 2, denominator);
}


template<class Impl>
void
ResourceManager<Impl>::reserveROB()
{
    int portion[] = {512, 512};
    int denominator = 1024;

    if(configUpdated) {
        if(config.HasMember("ROBPortionDenominator")) {
            denominator = config["ROBPortionDenominator"].GetInt();
            DPRINTF(Pard, "new ROB denominator is %d\n", denominator);
        }

        if(config.HasMember("ROBPortion")) {
            portion[0] = config["ROBPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            DPRINTF(Pard, "ROB partition policy not updated\n");
        }
    }

    DPRINTF(Pard, "ROB Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        DPRINTF(Pard, "ROB portion[%d]: %d\n", i, portion[i]);
    }

    rob->reassignPortion(portion, 2, denominator);
}

template<class Impl>
void
ResourceManager<Impl>::reserveDecode()
{
    int portion[] = {512, 512};
    int denominator = 1024;

    if(configUpdated) {
        if(config.HasMember("DecodePortionDenominator")) {
            denominator = config["DecodePortionDenominator"].GetInt();
            DPRINTF(Pard, "new Decode denominator is %d\n", denominator);
        }

        if(config.HasMember("DecodePortion")) {
            portion[0] = config["DecodePortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            DPRINTF(Pard, "Decode partition policy not updated\n");
        }
    }

    DPRINTF(Pard, "Decode Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        DPRINTF(Pard, "Decode portion[%d]: %d\n", i, portion[i]);
    }

    fetch->reassignDecodeWidth(portion, 2, denominator);
}

template<class Impl>
void
ResourceManager<Impl>::reconfigIssuePrio()
{
    int prio[] = {5, 0};
    if(configUpdated && config.HasMember("IssuePriority")) {
        auto prio_array = config["IssuePriority"].GetArray();
        DPRINTF(Pard, "Priority array:\n");
        for (int i = 0; i < 2; i++) {
            prio[i] = prio_array[i].GetInt();
            DPRINTF(Pard, "%d, ", prio[i]);
        }
        DPRINTF(Pard, "\n");
    } else {
        DPRINTF(Pard, "Use default issue priority\n");
    }
    DPRINTF(Pard, "Using issue policy (0: prio): %d\n", instQueue->issuePolicy);
    instQueue->reassignIssuePrio(prio, 2);
}


template <class Impl>
void
ResourceManager<Impl>::reserveRename()
{
    ThreadID numThreads = cpu->numThreads;
    auto freeRegArray = new unsigned[numThreads];
    if (configUpdated && config.HasMember("FreeRegs")) {
        auto freeRegArrayAst = config["FreeRegs"].GetArray();
        // TODO Add support for default value, or auto evaluated
        // from freeList.size()
        for (ThreadID tid = 0; tid < numThreads; ++tid) {
            freeRegArray[tid] = freeRegArrayAst[tid].GetInt();
        }
    }
    else {
        for (ThreadID tid = 0; tid < numThreads; ++tid) {
            // This assignment has no semantic meaning, and seems RIDICULOUS,
            // but it will make the limit check always true.
            // So that we can close this feature without additional
            // condition judgement.
            freeRegArray[tid] = cpu->regFile.numIntPhysRegs();
        }
    }

    rename->setNrFreeRegs(freeRegArray, numThreads);
    delete[] freeRegArray;
}


template<class Impl>
bool
ResourceManager<Impl>::readConfig()
{
    const char *json = "config.json";
    FILE *fp = fopen(json, "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found\n", json);
        configUpdated = false;
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

    configUpdated = true;

    return true;
}


#endif
