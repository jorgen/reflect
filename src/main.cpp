#include <iostream>

#include "optionparser.h"
#include "arg.h"
#include "configuration.h"

#include <llvm/Support/Host.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Parse/Parser.h>
#include <clang/Parse/ParseAST.h>

#include "astvisitor.h"
#include "preprocessorhandler.h"

#include <stdio.h>
int main(int argc, char *argv[])
{
    const Configuration conf = Arg::getConfig(argc, argv);

    clang::CompilerInstance ci;
    ci.getLangOpts().Bool = 1;
    ci.getLangOpts().CPlusPlus = 1;
    ci.getLangOpts().CPlusPlus11 = 1;
    ci.getLangOpts().WChar = 1;
    ci.getLangOpts().EncodeExtendedBlockSig = 1;
    ci.getLangOpts().ImplicitInt = 0;
    ci.createDiagnostics();

    std::shared_ptr<clang::TargetOptions> pto = std::make_shared<clang::TargetOptions>();
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo *pti = clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
    ci.setTarget(pti);

    if (conf.sysroot.size()) {
        ci.getHeaderSearchOpts().Sysroot = conf.sysroot;
    }

    ci.getHeaderSearchOpts().UseLibcxx = true;

    for(auto include_dir : conf.include_dirs) {
        ci.getHeaderSearchOpts().AddPath(llvm::StringRef(include_dir), clang::frontend::Angled,false,true);
    }

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);
    ci.getPreprocessorOpts().UsePredefines = true;
    ci.getPreprocessor().SetSuppressIncludeNotFoundError(true);
    ci.getPreprocessor().getBuiltinInfo().initializeBuiltins(ci.getPreprocessor().getIdentifierTable(), ci.getPreprocessor().getLangOpts());
    for (int i = 0; i < conf.defines.size(); i++) {
        ci.getPreprocessorOpts().addMacroDef(conf.defines.at(i));
    }
    ci.setASTConsumer(llvm::make_unique<AstConsumer>(conf));
    ci.getPreprocessor().addPPCallbacks(llvm::make_unique<PreProcessorHandler>(conf, ci.getSourceManager()));

    ci.createASTContext();

    fprintf(conf.out_file, "#pragma once\n");
    fprintf(conf.out_file, "#include <JR/parse.h>\n");
    fprintf(conf.out_file, "\n");

    const clang::FileEntry *pFile = ci.getFileManager().getFile(conf.file.c_str());
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                             &ci.getPreprocessor());
    clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext(), false, clang::TU_Complete, nullptr, true);
    int num_errors = ci.getDiagnosticClient().getNumErrors();
    ci.getDiagnosticClient().EndSourceFile();

    if (conf.tmp_file_name.size()) {
        fclose(conf.out_file);
        if (num_errors) {
            remove(conf.tmp_file_name.c_str());
            remove(conf.out_file_name.c_str());
        } else {
            rename(conf.tmp_file_name.c_str(), conf.out_file_name.c_str());
        }
    }

    return num_errors;
}
