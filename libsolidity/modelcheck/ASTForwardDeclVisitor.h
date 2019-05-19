/**
 * @date 2019
 * First-pass visitor for converting Solidity AST's to models in C.
 */

#pragma once

#include <libsolidity/ast/ASTVisitor.h>
#include <ostream>

namespace dev
{
namespace solidity
{
namespace modelcheck
{

/**
 * Interprets the AST in terms of its C model, and prints forward declarations
 * for each of structures.
 */
class ASTForwardDeclVisitor : public ASTConstVisitor
{
public:
    // Constructs a printer for all forward decl's required by ast's c model.
    ASTForwardDeclVisitor(
        ASTNode const& _ast
    );

    // Prints each for declaration once, in some order.
    void print(std::ostream& _stream);

	bool visit(ContractDefinition const& _node) override;
	bool visit(StructDefinition const& _node) override;
	bool visit(FunctionDefinition const& _node) override;
	bool visit(ModifierDefinition const& _node) override;
	bool visit(Mapping const& _node) override;

	void endVisit(ContractDefinition const&) override;
	void endVisit(StructDefinition const&) override;
	void endVisit(FunctionDefinition const&) override;
	void endVisit(ModifierDefinition const&) override;
	void endVisit(Mapping const&) override;

private:
	ASTNode const* m_ast;
	std::ostream* m_ostream = nullptr;
};

}
}
}
