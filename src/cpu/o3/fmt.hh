#ifndef __CPU_O3_FMT_HH__
#define __CPU_O3_FMT_HH__

#include <cstdint>

#include "config/the_isa.hh"

struct DerivO3CPUParams;

template <class Impl>
class FMT {

  typedef typename Impl::CPUPol CPUPol;
  typedef typename Impl::O3CPU O3CPU;
  typedef typename Impl::DynInst DynInst;
  typedef typename Impl::DynInstPtr DynInstPtr;
  typedef typename CPUPol::Fetch Fetch;
  typedef typename CPUPol::Decode Decode;
  typedef typename CPUPol::IEW IEW;


  public:

  struct BranchEntry {

    uint64_t baseSlots;

    uint64_t waitSlots;

    uint64_t missSlots;

    uint64_t initTimeStamp;
  };


  private:

  O3CPU *cpu;

  Fetch *fetch;

  Decode *decode;

  IEW *iew;

  std::map<DynInstPtr, BranchEntry> table[Impl::MaxThreads];

  uint64_t globalBase[Impl::MaxThreads];

  uint64_t globalMiss[Impl::MaxThreads];

  uint64_t globalWait[Impl::MaxThreads];

  ThreadID numThreads;


  public:

  std::string name() const;

  FMT(O3CPU *cpu_ptr, DerivO3CPUParams *params);

  // Add the first instruction of each thread into the table
  void init(DynInstPtr &bran, ThreadID tid, uint64_t timeStamp);

  void setStage(Fetch *_fetch, Decode *_decode, IEW *_iew);

  void addBranch(DynInstPtr &bran, ThreadID tid, uint64_t timeStamp);

  /* when dispatching an instruction, what should also be done is
   * to increase other threads' waiting slot
   */
  void incBaseSlot(DynInstPtr &bran, ThreadID tid);

  /* If prediction is right:
   * add timestamp difference counts to global dispatching count;
   * else:
   * add timestamp difference to global branch misprediction count,
   * and count slots after the branch instruction as miss event slots.
   */
  void resolveBranch(bool right, DynInstPtr &bran, ThreadID tid);

};

#endif // __CPU_O3_FETCH_HH__
