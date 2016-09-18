#include "preprocessorhandler.h"

#include "llvm/ADT/StringRef.h"
#include "clang/Basic/SourceLocation.h"

PreProcessorHandler::PreProcessorHandler(const Configuration &conf, clang::SourceManager &sourceManager)
	: _config(conf)
	, _manager(sourceManager)
{

}

//void PreProcessorHandler::InclusionDirective(clang::SourceLocation HashLocation,
//                                             const clang::Token & include_token,
//                                             llvm::StringRef FileNameRef, bool IsAngled,
//                                             clang::CharSourceRange FileNameRange,
//                                             const clang::FileEntry * IncludedFile,
//                                             llvm::StringRef /*SearchPath*/, llvm::StringRef RelativePath,
//                                             const clang::Module * ImportedModule)
//{
//
//    const clang::FileEntry * file = _manager.getFileEntryForID(_manager.getFileID(HashLocation));
//    //fprintf(stderr, "file is in dir: %s\n", file->getDir()->getName());
//    //fprintf(stderr, "include found in file %s\n", _manager.getFilename(HashLocation).data());
////	fprintf(stderr, "hashLocation dump begin\n");
////	HashLocation.dump(_manager);
//
////	fprintf(stderr, "hashLocation dump end\n");
////	end.dump(_manager);
////	begin.print(this->)
////	fprintf(stderr, "%s %s\n", FileNameRef.str().c_str(), FileNameRange.)
//
//}
