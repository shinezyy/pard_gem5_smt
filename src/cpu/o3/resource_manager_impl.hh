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
ResourceManager<Impl>::setRename(Rename *_rename)
{
    rename = _rename;
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
            printf("new IQ denominator is %d\n", denominator);
        }

        if(config.HasMember("InstQueuePortion")) {
            portion[0] = config["InstQueuePortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            std::cout << "Instruction queue partition policy not updated\n";
        }
    }

    printf("IQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        printf("IQ portion[%d]: %d\n", i, portion[i]);
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
            printf("new LSQ denominator is %d\n", denominator);
        }

        if(config.HasMember("LQPortion")) {
            portion[0] = config["LQPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            std::cout << "LQ partition policy not updated\n";
        }
    }

    printf("LSQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        printf("LQ portion[%d]: %d\n", i, portion[i]);
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
            printf("new LSQ denominator is %d\n", denominator);
        }

        if(config.HasMember("SQPortion")) {
            portion[0] = config["SQPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            std::cout << "SQ partition policy not updated\n";
        }
    }

    printf("LSQ Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        printf("SQ portion[%d]: %d\n", i, portion[i]);
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
            printf("new ROB denominator is %d\n", denominator);
        }

        if(config.HasMember("ROBPortion")) {
            portion[0] = config["ROBPortion"].GetInt();
            assert(portion[0] <= denominator);
            portion[1] = denominator - portion[0];
        } else {
            std::cout << "ROB partition policy not updated\n";
        }
    }

    printf("ROB Denominator is %d\n", denominator);

    for (int i = 0; i < sizeof(portion) / sizeof(portion[0]); i++) {
        printf("ROB portion[%d]: %d\n", i, portion[i]);
    }

    rob->reassignPortion(portion, 2, denominator);
}

template<class Impl>
void
ResourceManager<Impl>::reconfigIssuePrio()
{
    int prio[] = {5, 0};
    if(configUpdated && config.HasMember("IssuePriority")) {
        auto prio_array = config["IssuePriority"].GetArray();
        std::cout << "Priority array:\n";
        for (int i = 0; i < 2; i++) {
            prio[i] = prio_array[i].GetInt();
            std::cout << prio[i] <<", ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Use default issue priority\n";
    }
    std::cout << "Using issue policy (0: prio): " << instQueue->issuePolicy << "\n";
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
