#ifndef __CPU_O3_FETCH_HH__
#define __CPU_O3_FETCH_HH__

template <class Impl>
class FMT
{
  typedef typename Impl::CPUPol CPUPol;
  typedef typename Impl::DynInst DynInst;
  typedef typename Impl::DynInstPtr DynInstPtr;
  typedef typename Impl::DynInstPtr DynInstPtr;

  public:


  FMT(O3CPU *cpu_ptr, DerivO3CPUParams *params);

  void init(DerivO3CPUParams *params);

  void addBranch(DynInstPtr bran, ThreadID tid);

  /* when dispatching an instruction, what should also be done is
   * to increase other threads' waiting slot
   */
  void incBaseSlot(DynInstPtr bran, ThreadID tid);

  /* If prediction is right:
   * add timestamp difference counts to global dispatching count;
   * else:
   * add timestamp difference to global branch misprediction count,
   * and count slots after the branch instruction as miss event slots.
   */
  void resolveBranch(bool right, DynInstPtr bran, ThreadID tid);

}

















#endif // __CPU_O3_FETCH_HH__
