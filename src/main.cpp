#include <llvm/Support/Host.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/TargetInfo.h>

int main()
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    using clang::DiagnosticOptions;
    using clang::TextDiagnosticPrinter;

    CompilerInstance ci;
    ci.createDiagnostics();

    std::shared_ptr<TargetOptions>	pto	=	std::make_shared<TargetOptions>();
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
    ci.setTarget(pti);

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);
    return 0;
}
