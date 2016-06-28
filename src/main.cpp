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
#include <clang/Basic/Diagnostic.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Parse/Parser.h>
#include <clang/Parse/ParseAST.h>

#include "astvisitor.h"
#include "preprocessorhandler.h"

int main(int argc, char *argv[])
{
	fprintf(stderr, "argv %s\n", *argv);
	const Configuration conf = Arg::getConfig(argc, argv);



    clang::CompilerInstance ci;
    clang::DiagnosticOptions diagnosticOptions;
    ci.getLangOpts().CPlusPlus = 1;
	ci.getLangOpts().CPlusPlus11 = 1;
    ci.createDiagnostics();

    std::shared_ptr<clang::TargetOptions> pto = std::make_shared<clang::TargetOptions>();
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo *pti = clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
    ci.setTarget(pti);

	if (conf.sysroot.size()) {
		ci.getHeaderSearchOpts().Sysroot = conf.sysroot;
    }

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);
    ci.getPreprocessorOpts().UsePredefines = false;
	for (int i = 0; i < conf.defines.size(); i++) {
		ci.getPreprocessorOpts().addMacroDef(conf.defines.at(i));
	}
	ci.setASTConsumer(llvm::make_unique<AstConsumer>(conf));
	ci.getPreprocessor().addPPCallbacks(llvm::make_unique<PreProcessorHandler>(conf, ci.getSourceManager()));

    ci.createASTContext();

	for (size_t i = 0; i < conf.files.size(); i++) {
		const clang::FileEntry *pFile = ci.getFileManager().getFile(conf.files.at(i).c_str());
        ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
        ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                                 &ci.getPreprocessor());
        clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext());
        ci.getDiagnosticClient().EndSourceFile();
    }


    return 0;
}
