#include "exception_codegen.h"
#include "exception_types.h"
#include <llvm-c/Core.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations for statement/expression codegen
extern LLVMValueRef llvm_gen_statement(void* stmt, LLVMModuleRef module, 
                                       LLVMBuilderRef builder, LLVMValueRef function);
extern LLVMValueRef llvm_gen_expression(void* expr, LLVMModuleRef module,
                                        LLVMBuilderRef builder, LLVMValueRef function);

// ============================================================================
// LLVM Exception Runtime Support
// ============================================================================

// Declare exception runtime functions
static void declare_exception_runtime(LLVMModuleRef module) {
    // __melp_throw(exception_ptr)
    LLVMTypeRef throw_params[] = {LLVMPointerType(LLVMInt8Type(), 0)};
    LLVMTypeRef throw_type = LLVMFunctionType(LLVMVoidType(), throw_params, 1, 0);
    LLVMAddFunction(module, "__melp_throw", throw_type);
    
    // __melp_begin_catch(exception_ptr) -> exception_ptr
    LLVMTypeRef begin_catch_params[] = {LLVMPointerType(LLVMInt8Type(), 0)};
    LLVMTypeRef begin_catch_type = LLVMFunctionType(
        LLVMPointerType(LLVMInt8Type(), 0), begin_catch_params, 1, 0);
    LLVMAddFunction(module, "__melp_begin_catch", begin_catch_type);
    
    // __melp_end_catch()
    LLVMTypeRef end_catch_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
    LLVMAddFunction(module, "__melp_end_catch", end_catch_type);
    
    // __melp_personality_v0 (personality function)
    LLVMTypeRef personality_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 1);
    LLVMAddFunction(module, "__melp_personality_v0", personality_type);
}

// ============================================================================
// Try Block Codegen
// ============================================================================

LLVMValueRef llvm_gen_try_block(TryBlock* try_block, LLVMModuleRef module,
                                 LLVMBuilderRef builder, LLVMValueRef function) {
    if (!try_block) return NULL;
    
    declare_exception_runtime(module);
    
    // Create basic blocks
    LLVMBasicBlockRef try_bb = LLVMAppendBasicBlock(function, "try");
    LLVMBasicBlockRef landingpad_bb = LLVMAppendBasicBlock(function, "landingpad");
    LLVMBasicBlockRef finally_bb = try_block->finally_block ? 
        LLVMAppendBasicBlock(function, "finally") : NULL;
    LLVMBasicBlockRef continue_bb = LLVMAppendBasicBlock(function, "try_continue");
    
    // Jump to try block
    LLVMBuildBr(builder, try_bb);
    LLVMPositionBuilderAtEnd(builder, try_bb);
    
    // Generate try body
    for (int i = 0; i < try_block->body_count; i++) {
        llvm_gen_statement(try_block->body[i], module, builder, function);
    }
    
    // If no exception: jump to finally or continue
    if (finally_bb) {
        LLVMBuildBr(builder, finally_bb);
    } else {
        LLVMBuildBr(builder, continue_bb);
    }
    
    // ========================================================================
    // Landingpad (Exception Handler)
    // ========================================================================
    LLVMPositionBuilderAtEnd(builder, landingpad_bb);
    
    // Create landingpad instruction
    // landingpad { i8*, i32 } personality @__melp_personality_v0
    LLVMTypeRef exception_struct_types[] = {
        LLVMPointerType(LLVMInt8Type(), 0),  // exception pointer
        LLVMInt32Type()                       // selector
    };
    LLVMTypeRef exception_struct = LLVMStructType(exception_struct_types, 2, 0);
    
    LLVMValueRef landingpad = LLVMBuildLandingPad(builder, exception_struct, 
                                                   LLVMGetNamedFunction(module, "__melp_personality_v0"),
                                                   0, "exception");
    
    // Extract exception pointer
    LLVMValueRef exception_ptr = LLVMBuildExtractValue(builder, landingpad, 0, "exc_ptr");
    LLVMValueRef selector = LLVMBuildExtractValue(builder, landingpad, 1, "selector");
    
    // ========================================================================
    // Catch Clause Handling
    // ========================================================================
    CatchClause* clause = try_block->catch_clauses;
    LLVMBasicBlockRef next_catch_bb = NULL;
    
    while (clause) {
        // Create basic block for this catch
        LLVMBasicBlockRef catch_bb = LLVMAppendBasicBlock(function, "catch");
        next_catch_bb = clause->next ? LLVMAppendBasicBlock(function, "next_catch") : NULL;
        
        // Add catch clause to landingpad
        // TODO: Type info for exception matching
        LLVMAddClause(landingpad, LLVMConstNull(LLVMPointerType(LLVMInt8Type(), 0)));
        
        // Position at catch block
        LLVMPositionBuilderAtEnd(builder, catch_bb);
        
        // Call __melp_begin_catch
        LLVMValueRef begin_catch = LLVMGetNamedFunction(module, "__melp_begin_catch");
        LLVMValueRef caught_exc = LLVMBuildCall(builder, begin_catch, &exception_ptr, 1, "caught");
        
        // Generate catch body
        for (int i = 0; i < clause->body_count; i++) {
            llvm_gen_statement(clause->body[i], module, builder, function);
        }
        
        // Call __melp_end_catch
        LLVMValueRef end_catch = LLVMGetNamedFunction(module, "__melp_end_catch");
        LLVMBuildCall(builder, end_catch, NULL, 0, "");
        
        // Jump to finally or continue
        if (finally_bb) {
            LLVMBuildBr(builder, finally_bb);
        } else {
            LLVMBuildBr(builder, continue_bb);
        }
        
        clause = clause->next;
    }
    
    // If no catch matched: rethrow
    if (!try_block->catch_clauses) {
        LLVMBuildResume(builder, landingpad);
    }
    
    // ========================================================================
    // Finally Block
    // ========================================================================
    if (finally_bb) {
        LLVMPositionBuilderAtEnd(builder, finally_bb);
        
        // Generate finally body
        for (int i = 0; i < try_block->finally_block->body_count; i++) {
            llvm_gen_statement(try_block->finally_block->body[i], module, builder, function);
        }
        
        // Jump to continue
        LLVMBuildBr(builder, continue_bb);
    }
    
    // ========================================================================
    // Continue
    // ========================================================================
    LLVMPositionBuilderAtEnd(builder, continue_bb);
    
    return NULL;  // Try blocks don't return values
}

// ============================================================================
// Throw Statement Codegen
// ============================================================================

LLVMValueRef llvm_gen_throw_statement(ThrowStatement* throw_stmt, LLVMModuleRef module,
                                      LLVMBuilderRef builder, LLVMValueRef function) {
    if (!throw_stmt) return NULL;
    
    declare_exception_runtime(module);
    
    // Generate exception expression
    LLVMValueRef exception_value = llvm_gen_expression(throw_stmt->exception_expr, 
                                                        module, builder, function);
    
    // Cast to i8* for __melp_throw
    LLVMValueRef exception_ptr = LLVMBuildBitCast(builder, exception_value,
                                                   LLVMPointerType(LLVMInt8Type(), 0),
                                                   "exc_ptr");
    
    // Call __melp_throw(exception_ptr)
    LLVMValueRef throw_fn = LLVMGetNamedFunction(module, "__melp_throw");
    LLVMBuildCall(builder, throw_fn, &exception_ptr, 1, "");
    
    // Throw is terminator: insert unreachable
    LLVMBuildUnreachable(builder);
    
    return NULL;
}
