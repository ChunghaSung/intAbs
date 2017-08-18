/**
 * Author: Markus Kusano
 *
 * Visit every instruction in the program and sort them into instructions
 * return floating point values and instructions return integer values
 */
#include "llvm/IR/InstVisitor.h"

class VarVisitor : public llvm::InstVisitor<VarVisitor> {
  private:
  public:
    // A structure to hold "names" (value pointers) of floating
    // point and integer variables.
    //
    // Note: Global ints and floats are also placed into these vectors. This
    // means there may be both int and pointer to int types (respectively for
    // floats).
    struct Vars {
      std::vector<llvm::Value*> ints;
      std::vector<llvm::Value*> floats;
    };

    // Combine the internal vectors of the two passed structures
    static Vars mergeVars(Vars v1, Vars v2);

    // This structure is filled out by all the visitors
    Vars vars;

  // Checks the return type of the instruction and modifies the internal vars
    // field accordingly
    void handleInstructionWithRet(llvm::Instruction &I);

    // Overridden visitor functions
    void visitBinaryOperator(llvm::BinaryOperator &I);
    void visitUnaryInstruction(llvm::UnaryInstruction &I);
    void visitCastInst(llvm::CastInst &I);
    void visitCmpInst(llvm::CmpInst &I);
    void visitPHINode(llvm::PHINode &I);
    void visitAtomicRMWInst(llvm::AtomicRMWInst &I);

#if 0
    void visitReturnInst(ReturnInst &I);
    void visitCallInst(CallInst &I);
    void visitInvokeInst(InvokeInst &I);
    void visitLoadInst(LoadInst &I);
    void visitStoreInst(StoreInst &I);
    void visitAllocaInst(AllocaInst &I);
    void visitBranchInst(BranchInst &I);
    void visitSwitchInst(SwitchInst &I);
    void visitIndirectBrInst(IndirectBrInst &I);
    void visitResumeInst(ResumeInst &I);
    void visitUnreachableInst(UnreachableInst &I);
    void visitICmpInst(ICmpInst &I);
    void visitFCmpInst(FCmpInst &I);
    void visitAllocaInst(AllocaInst &I);
    void visitStoreInst(StoreInst &I);
    void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I);
    void visitFenceInst(FenceInst &I);
    void visitGetElementPtrInst(GetElementPtrInst &I);
    void visitTruncInst(TruncInst &I);
    void visitZExtInst(ZExtInst &I);
    void visitSExtInst(SExtInst &I);
    void visitFPTruncInst(FPTruncInst &I);
    void visitFPExtInst(FPExtInst &I);
    void visitFPToUIInst(FPToUIInst &I);
    void visitFPToSIInst(FPToSIInst &I);
    void visitUIToFPInst(UIToFPInst &I);
    void visitSIToFPInst(SIToFPInst &I);
    void visitPtrToIntInst(PtrToIntInst &I);
    void visitIntToPtrInst(IntToPtrInst &I);
    void visitBitCastInst(BitCastInst &I);
    void visitAddrSpaceCastInst(AddrSpaceCastInst &I);
    void visitSelectInst(SelectInst &I);
    void visitVAArgInst(VAArgInst &I);
    void visitExtractElementInst(ExtractElementInst &I);
    void visitInsertElementInst(InsertElementInst &I);
    void visitShuffleVectorInst(ShuffleVectorInst &I);
    void visitExtractValueInst(ExtractValueInst &I);
    void visitInsertValueInst(InsertValueInst &I);
    void visitLandingPadInst(LandingPadInst &I);
    void visitDbgDeclareInst(DbgDeclareInst &I);
    void visitDbgValueInst(DbgValueInst &I);
    void visitDbgInfoIntrinsic(DbgInfoIntrinsic &I);
    void visitMemSetInst(MemSetInst &I);
    void visitMemCpyInst(MemCpyInst &I);
    void visitMemMoveInst(MemMoveInst &I);
    void visitMemTransferInst(MemTransferInst &I);
    void visitMemIntrinsic(MemIntrinsic &I);
    void visitVAStartInst(VAStartInst &I);
    void visitVAEndInst(VAEndInst &I);
    void visitVACopyInst(VACopyInst &I);
    void visitIntrinsicInst(IntrinsicInst &I);

    // Next level propagators: If the user does not overload a specific
    // instruction type, they can overload one of these to get the whole class
    // of instructions...
    //
    void visitCastInst(CastInst &I);
    void visitCmpInst(CmpInst &I);
    void visitTerminatorInst(TerminatorInst &I);
    void visitBinaryOperator(BinaryOperator &I);
    void visitUnaryInstruction(UnaryInstruction &I);
#endif
};
