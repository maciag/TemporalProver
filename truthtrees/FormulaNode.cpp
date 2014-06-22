#include "FormulaNode.hpp"

FormulaNode::FormulaNode(vector<StringFormula> formulas) :
		allChecked(false) {
	isChecked = vector<bool>(this->formulas.size(), false);
	this->formulas.reserve(10);
	isChecked.reserve(10);
	for (int i = 0; i < formulas.size(); i++)
		this->formulas.push_back(formulas[i]);
}

FormulaNode::FormulaNode(vector<string> rawFormulas) :
		allChecked(false) {
	isChecked = vector<bool>(this->formulas.size(), false);
	this->formulas.reserve(10);
	isChecked.reserve(10);
	for (int i = 0; i < rawFormulas.size(); i++)
		this->formulas.push_back(StringFormula(rawFormulas[i]));
}

FormulaNode::FormulaNode() :
		allChecked(false) {
	formulas.reserve(10);
	isChecked.reserve(10);
}

StringFormula::decomposeType FormulaNode::decomposeNext(
		vector<FormulaNode> &childNodes) {

	if (!allChecked) {

		int stackIdx = -1, singleIdx = -1, stackBranchIdx = -1,
				substackBranchIdx = -1, branchIdx = -1;
		if (formulas.size() < 1)
			return StringFormula::decomposeType::none;
		for (int i = 0; i < formulas.size(); i++) {
			if (!isChecked[i]) {
				if (formulas[i].getType()
						== StringFormula::decomposeType::stacking) {
					stackIdx = i;
				} else if (formulas[i].getType()
						== StringFormula::decomposeType::single) {
					singleIdx = i;
				} else if (formulas[i].getType()
						== StringFormula::decomposeType::branching_stacking) {
					stackBranchIdx = i;
				} else if (formulas[i].getType()
						== StringFormula::decomposeType::branching_substacking) {
					substackBranchIdx = i;
				} else if (formulas[i].getType()
						== StringFormula::decomposeType::branching) {
					branchIdx = i;
				} else {
					isChecked[i] = true;
				}
			}
		}

		if (stackIdx >= 0) {
			vector<StringFormula> subformulas;
			formulas[stackIdx].decompose(subformulas);
			isChecked[stackIdx] = true;
			for (int i = 0; i < 2; i++)
				appendFormula(subformulas[i]);
			return StringFormula::decomposeType::stacking;
		} else if (singleIdx >= 0) {
			vector<StringFormula> subformulas;
			formulas[singleIdx].decompose(subformulas);
			isChecked[singleIdx] = true;
			appendFormula(subformulas[0]);
			return StringFormula::decomposeType::single;
		} else if (stackBranchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[stackBranchIdx].decompose(subformulas);
			isChecked[stackBranchIdx] = true;
			for (int i = 0; i < 2; i++) {
				childNodes[i].appendFormula(subformulas[2 * i]);
				childNodes[i].appendFormula(subformulas[2 * i + 1]);
			}
			return StringFormula::decomposeType::branching_stacking;
		} else if (substackBranchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[substackBranchIdx].decompose(subformulas);
			isChecked[substackBranchIdx] = true;
			childNodes[0].appendFormula(subformulas[0]);
			childNodes[1].appendFormula(subformulas[1]);
			childNodes[1].appendFormula(subformulas[2]);
		} else if (branchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[branchIdx].decompose(subformulas);
			isChecked[branchIdx] = true;
			for (int i = 0; i < 2; i++) {
				childNodes[i].appendFormula(subformulas[i]);
			}
			return StringFormula::decomposeType::branching;
		}
	}
	return StringFormula::decomposeType::none;
}

FormulaNode::~FormulaNode() {

}

void FormulaNode::appendFormula(StringFormula formula) {
	formulas.push_back(formula);
	isChecked.push_back(false);
	allChecked = false;
}

void FormulaNode::appendFormula(string rawFormula) {
	formulas.push_back(StringFormula(rawFormula));
	isChecked.push_back(false);
	allChecked = false;
}

bool FormulaNode::isAllChecked() {
	return allChecked;
}

string FormulaNode::toFormattedString() {
	string formattedString;
	for (int i = 0; i < formulas.size(); i++) {
		formattedString.append(formulas[i].getRaw() + "; ");
	}
	return formattedString;
}

ostream& operator<<(ostream& os, const FormulaNode& formulaNode) {

	for (int i = 0; i < formulaNode.formulas.size(); i++) {
		os << formulaNode.formulas[i] << "; ";
	}
	return os;
}

