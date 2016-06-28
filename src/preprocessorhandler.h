#ifndef PREPROCESSORHANDLER_H
#define PREPROCESSORHANDLER_H

#include <clang/Lex/PPCallbacks.h>

#include "configuration.h"

class PreProcessorHandler : public clang::PPCallbacks
{
public:
	PreProcessorHandler(const Configuration &conf, clang::SourceManager &sourceManager);

	void InclusionDirective(clang::SourceLocation HashLocation,
							const clang::Token & /*include_token*/,
							llvm::StringRef FileNameRef, bool IsAngled,
							clang::CharSourceRange FileNameRange,
							const clang::FileEntry * /*IncludedFile*/,
							llvm::StringRef /*SearchPath*/, llvm::StringRef /*RelativePath*/,
							const clang::Module * /*ImportedModule*/) override;
private:
	const Configuration &_config;
	clang::SourceManager &_manager;
};

#endif // PREPROCESSORHANDLER_H
