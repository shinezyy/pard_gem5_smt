SYMBOL(IqEntry),                 /// Instruction is in the IQ
SYMBOL(RobEntry),                /// Instruction is in the ROB
SYMBOL(LsqEntry),                /// Instruction is in the LSQ
SYMBOL(Completed),               /// Instruction has completed
SYMBOL(ResultReady),             /// Instruction has its result
SYMBOL(CanIssue),                /// Instruction can issue and execute
SYMBOL(Issued),                  /// Instruction has issued
SYMBOL(Executed),                /// Instruction has executed
SYMBOL(CanCommit),               /// Instruction can commit
SYMBOL(AtCommit),                /// Instruction has reached commit
SYMBOL(Committed),               /// Instruction has committed
SYMBOL(Squashed),                /// Instruction is squashed
SYMBOL(SquashedInIQ),            /// Instruction is squashed in the IQ
SYMBOL(SquashedInLSQ),           /// Instruction is squashed in the LSQ
SYMBOL(SquashedInROB),           /// Instruction is squashed in the ROB
SYMBOL(RecoverInst),             /// Is a recover instruction
SYMBOL(BlockingInst),            /// Is a blocking instruction
SYMBOL(ThreadsyncWait),          /// Is a thread synchronization instruction
SYMBOL(SerializeBefore),         /// Needs to serialize on
                                 /// instructions ahead of it
SYMBOL(SerializeAfter),          /// Needs to serialize instructions behind it
SYMBOL(SerializeHandled),        /// Serialization has been handled
SYMBOL(NumStatus)
