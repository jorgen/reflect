#include "astvisitor.h"
#include "configuration.h"
#include <clang/AST/DeclCXX.h>
#include <clang/Lex/Lexer.h>

ClassVisitor::ClassVisitor(const Configuration &conf, clang::ASTContext *context)
	: clang::RecursiveASTVisitor<ClassVisitor>()
	, _conf(conf)
	, _context(context)
{
}

bool ClassVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl *declaration)
{
	clang::FullSourceLoc fullLocation = _context->getFullLoc(declaration->getLocStart());
	clang::PresumedLoc loc = _context->getSourceManager().getPresumedLoc(fullLocation);
	if (loc.isInvalid())
		return true;
	std::string filename = loc.getFilename();
	llvm::SmallString<512> fileNameV;
	fileNameV.append(filename.begin(), filename.end());
	_context->getSourceManager().getFileManager().makeAbsolutePath(fileNameV);
	filename = fileNameV.data();
	if (filename.compare(0, _conf.srcroot.size(), _conf.srcroot))
		return true;

	fprintf(stderr, "cxx record %s\n\t%s\n", declaration->getQualifiedNameAsString().c_str(), filename.c_str());
	for (auto ctr : declaration->ctors()) {
	}
	for (auto fields : declaration->fields()) {
		fprintf(stderr, "field: \n");
		fields->dump();
	}
//	for (auto attr : declaration->attrs()) {
//		fprintf(stderr, "attr: \n");
//		attr->();
//	}

	for (auto method : declaration->methods()) {
		clang::CXXMethodDecl *func = method;
//		if (!func->is)
//			continue;

		fprintf(stderr, "method: \n");
		method->dump();
	}
	return true;
}

class ClassPrinter : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
	ClassPrinter(const Configuration &conf, clang::ASTContext &context)
		: _conf(conf)
		, _context(context)
	{}

	virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result) {
		const clang::CXXMethodDecl *method = result.Nodes.getStmtAs<const clang::CXXMethodDecl>("cxxFunction");
		if (!method)
			return;

		if (!method->getAttr<clang::OverrideAttr>())
			fprintf(stderr, "match\n");
	}

	const Configuration &_conf;
	clang::ASTContext &_context;
};

AstConsumer::AstConsumer(const Configuration &conf)
	: clang::ASTConsumer()
	, _conf(conf)
{

}

void AstConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
//	ClassVisitor visitor(_conf, &context);
//	visitor.TraverseDecl(context.getTranslationUnitDecl());
	clang::ast_matchers::MatchFinder finder;
	ClassPrinter printer(_conf, context);

	clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::cxxMethodDecl(clang::ast_matchers::isOverride()).bind("cxxFunction");
	finder.addMatcher(matcher, &printer);
	finder.matchAST(context);
	clang::CharSourceRange range = clang::CharSourceRange::getTokenRange(context.getTranslationUnitDecl()->getSourceRange());
	clang::CharSourceRange lexerRange = clang::Lexer::makeFileCharRange(range,context.getSourceManager(), context.LangOpts);

}
