#ifndef EXCEPTION_CODEGEN_H
#define EXCEPTION_CODEGEN_H

#include "exception.h"
#include <llvm-c/Core.h>

// LLVM exception handling codegen
LLVMValueRef llvm_gen_try_block(TryBlock* try_block, LLVMModuleRef module, 
                                 LLVMBuilderRef builder, LLVMValueRef function);

LLVMValueRef llvm_gen_throw_statement(ThrowStatement* throw_stmt, LLVMModuleRef module,
                                      LLVMBuilderRef builder, LLVMValueRef function);

// Helper: Create landingpad for catch clauses
LLVMBasicBlockRef llvm_create_landingpad(CatchClause* catch_clauses, 
                                         LLVMModuleRef module,
                                         LLVMBuilderRef builder,
                                         LLVMValueRef function);

// Helper: Match exception type in landingpad
void llvm_gen_exception_type_check(CatchClause* clause, LLVMValueRef exception_ptr,
                                   LLVMModuleRef module, LLVMBuilderRef builder,
                                   LLVMBasicBlockRef catch_block, 
                                   LLVMBasicBlockRef next_block);

#endif // EXCEPTION_CODEGEN_H
