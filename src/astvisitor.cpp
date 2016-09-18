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

        if (!declaration->isCompleteDefinition())
                return true;

        const std::string &qualifiedName = declaration->getQualifiedNameAsString();
        fprintf(_conf.out_file, "template<>\n");
        fprintf(_conf.out_file, "%s JR::parseData<%s>(const char *data, size_t size, bool &parsed)\n", qualifiedName.c_str(), qualifiedName.c_str());
        fprintf(_conf.out_file, "{\n");
        fprintf(_conf.out_file, "    parsed = false;\n");
        fprintf(_conf.out_file, "    %s ret_obj;\n", qualifiedName.c_str());
        fprintf(_conf.out_file, "    JT::Tokenizer tokenizer;\n");
        fprintf(_conf.out_file, "    tokenizer.addData(data, size);\n");
        fprintf(_conf.out_file, "    JT::Token token;\n");
        fprintf(_conf.out_file, "    tokenizer.nextToken(&token);\n");
        fprintf(_conf.out_file, "    if (token.value_type != JT::Token::ObjectStart)\n");
        fprintf(_conf.out_file, "        return ret_obj;\n");
        fprintf(_conf.out_file, "    tokenizer.nextToken(&token);\n");
        fprintf(_conf.out_file, "    while(token.value_type != JT::Token::ObjectEnd)\n");
        fprintf(_conf.out_file, "    {\n");
	for (auto field : declaration->fields()) {
            fprintf(_conf.out_file, "        if (memcmp(\"%s\", token.name.data, std::min(sizeof(\"%s\"), token.name.size)))\n", field->getNameAsString().c_str(), field->getNameAsString().c_str());
            fprintf(_conf.out_file, "        {\n");
            fprintf(_conf.out_file, "            ret_obj.%s = std::string(token.value.data, token.value.size);\n", field->getNameAsString().c_str());
            fprintf(_conf.out_file, "        } else\n");

	}
        fprintf(_conf.out_file, "        {\n");
        fprintf(_conf.out_file, "            fprintf(stderr, \"Not recognised property %%s with value %%s when parsing JSON for structure %s\\n\", std::string(token.name.data, token.name.size).c_str(), std::string(token.value.data, token.value.size).c_str());\n", qualifiedName.c_str());
        fprintf(_conf.out_file, "        }\n");
        fprintf(_conf.out_file, "        tokenizer.nextToken(&token);\n");
        fprintf(_conf.out_file, "    }\n");
        fprintf(_conf.out_file, "    parsed = true;\n");
        fprintf(_conf.out_file, "    return ret_obj;\n");
        fprintf(_conf.out_file, "}\n");

        for(auto method : declaration->methods()) {
//            clang::QualType qualfield = field->getType().getLocalUnqualifiedType().getTypePtr()->getPointeeType();
//            if (qualfield.isNull())
//                continue;
//            fprintf(stderr, "field2 %s and is record%d\n", qualfield.getAsString().c_str(), qualfield.getTypePtr()->isRecordType());;
//                const clang::RecordType *subRecordType  = qualfield->getAsStructureType();
//                if (subRecordType) {
//                    fprintf(stderr, "has subfield\n");
//                }
//            clang::CXXConstructorDecl *constuctor = clang::dyn_cast<clang::CXXConstructorDecl>(method);
//            if (constuctor)
//                fprintf(stderr, "constructor %s\n", method->getNameInfo().getAsString().c_str());
//            clang::CXXDestructorDecl *destructor = clang::dyn_cast<clang::CXXDestructorDecl>(method);
//            if (destructor)
//                fprintf(stderr, "destructor %s\n", method->getNameInfo().getAsString().c_str());
//            else
//                fprintf(stderr, "method: %s\n", method->getNameInfo().getAsString().c_str());
        }
	return true;
}

AstConsumer::AstConsumer(const Configuration &conf)
	: clang::ASTConsumer()
	, _conf(conf)
{

}

void AstConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
	ClassVisitor visitor(_conf, &context);
	visitor.TraverseDecl(context.getTranslationUnitDecl());
	//clang::CharSourceRange lexerRange = clang::Lexer::makeFileCharRange(range,context.getSourceManager(), context.LangOpts);

}
