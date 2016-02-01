#ifndef ASTVISITOR_H
#define ASTVISITOR_H
#include "configuration.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

struct Configuration;

class ClassVisitor
		: public clang::RecursiveASTVisitor<ClassVisitor> {
public:
	ClassVisitor(const Configuration &conf, clang::ASTContext *context);
	bool VisitCXXRecordDecl(clang::CXXRecordDecl *declaration);
private:
	const Configuration &_conf;
	clang::ASTContext *_context;
};

class AstConsumer : public clang::ASTConsumer {
public:
	AstConsumer(const Configuration &conf);
	virtual void HandleTranslationUnit(clang::ASTContext &context);
private:
	const Configuration &_conf;
};
#endif //ASTVISITOR_H

