#ifndef EvaLLVM_hpp
#define EvaLLVM_hpp

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

class EvaLLVM {
public:
    EvaLLVM() {
        moduleInit();
        setupExternFunctions();
    }

    void exec(const std::string& progarm) {

        compile(/* ast */);

        module->print(llvm::outs(), nullptr);

        std::cout << '\n';

        saveModuleToFile("./out.ll");
    }

private:
    void compile(/* ast */) {
        fn = createFunction("main", llvm::FunctionType::get(builder->getInt32Ty(), false));
 
        gen(/* ast */);

        builder->CreateRet(builder->getInt32(0));
    }
    
    llvm::Value* gen(/* exp */) {
        // return builder->getInt32(42);

        // strings:
        auto str = builder->CreateGlobalStringPtr("Hello, world\n");

        // call to printf:
        auto printfFn = module->getFunction("printf");

        // args:
        std::vector<llvm::Value*> args{str};

        return builder->CreateCall(printfFn, args);
    }

    void setupExternFunctions() {
        // i8* to substitute for char*, void*, etc
        auto bytePtrTy = builder->getInt8Ty()->getPointerTo();

        module->getOrInsertFunction("printf", 
            llvm::FunctionType::get(
                /* return type */ builder->getInt32Ty(),
                /* format args */ bytePtrTy,
                /* vararg */ true
            ));
    }

    llvm::Function* createFunction(const std::string& fnName, llvm::FunctionType* fnType) {
        auto fn = module->getFunction(fnName);

        if (fn == nullptr)
            fn = createFunctionProto(fnName, fnType);

        createFunctionBlock(fn);
        return fn;
    }

    llvm::Function* createFunctionProto(const std::string& fnName, llvm::FunctionType* fnType) {
        auto fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);
        verifyFunction(*fn);

        return fn;
    }

    void createFunctionBlock(llvm::Function* fn) {
        auto entry = createBB("entry", fn);
        builder->SetInsertPoint(entry);
    }

    llvm::BasicBlock* createBB(std::string name, llvm::Function* fn = nullptr) {
        return llvm::BasicBlock::Create(*ctx, name, fn);
    }

    void saveModuleToFile(const std::string& fileName) {
        std::error_code errorCode;
        llvm::raw_fd_ostream outLL(fileName, errorCode);
        module->print(outLL, nullptr);
    }

    void moduleInit() {
        ctx = std::make_unique<llvm::LLVMContext>();
        module = std::make_unique<llvm::Module>("EvaLLVM", *ctx);

        // Create an IRBuilder with the LLVMContext
        builder = std::make_unique<llvm::IRBuilder<>>(*ctx);
    }

    llvm::Function* fn;

    std::unique_ptr<llvm::LLVMContext> ctx;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
};

#endif