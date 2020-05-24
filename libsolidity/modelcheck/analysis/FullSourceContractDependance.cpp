/**
 * @date 2020
 * A set of tools to analyze the dependance between contracts, their methods and
 * their structs.
 * 
 * Implements the testing focused (full source unit) specialization.
 */

#include <libsolidity/modelcheck/analysis/ContractDependance.h>

using namespace std;

namespace dev
{
namespace solidity
{
namespace modelcheck
{

// -------------------------------------------------------------------------- //

FullSourceContractDependance::FullSourceContractDependance(
    SourceUnit const& _srcs
): DependencyAnalyzer({})
{
    auto listview = ASTNode::filteredNodes<ContractDefinition>(_srcs.nodes());

    for (auto contract : listview)
    {
        m_contracts.insert(contract);
    }
}

// -------------------------------------------------------------------------- //

ContractDependance::FuncInterface
    FullSourceContractDependance::get_interfaces_for(
        ContractDefinition const*_contract
) const
{
    ContractDependance::FuncInterface interface;

    // Iterates through each base class of this contract.
    set<string> methods;
    for (auto base : _contract->annotation().linearizedBaseContracts)
    {
        // If the class is an interface, this is the end of analysis.
        if (base->isInterface()) break;

        // Otherwise, analyze each function.
        for (auto func : base->definedFunctions())
        {
            // Unimplemented functions are skipped.
            if (!func->isImplemented()) continue;

            // If this name has already been seen, this is a superclass.
            if (!methods.insert(func->name()).second) continue;

            interface.push_back(func);
        }
    }

    return interface;
}

// -------------------------------------------------------------------------- //

ContractDependance::FunctionSet
    FullSourceContractDependance::get_superchain_for(
        ContractDependance::FuncInterface,
        FunctionDefinition const* _func
) const
{
    ContractDependance::FunctionSet chain;

    auto const* supercall = _func;
    while (supercall != nullptr && supercall->isImplemented())
    {
        chain.insert(supercall);
        supercall = supercall->annotation().superFunction;
    }

    return chain;
}

// -------------------------------------------------------------------------- //

}
}
}
