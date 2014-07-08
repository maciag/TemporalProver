#include "FormulaNode.hpp"

FormulaNode::FormulaNode(vector<StringFormula> formulas) :
		allChecked(false), eliminated(false), feedbackPath(NULL) {
	isChecked = vector<bool>(this->formulas.size(), false);
	this->formulas.reserve(10);
	isChecked.reserve(10);
	for (int i = 0; i < formulas.size(); i++)
		this->formulas.push_back(formulas[i]);
}

FormulaNode::FormulaNode(vector<string> rawFormulas) :
		allChecked(false), eliminated(false), feedbackPath(NULL) {
	isChecked = vector<bool>(this->formulas.size(), false);
	this->formulas.reserve(10);
	isChecked.reserve(10);
	for (int i = 0; i < rawFormulas.size(); i++)
		this->formulas.push_back(StringFormula(rawFormulas[i]));
}

FormulaNode::FormulaNode() :
		allChecked(false), eliminated(false), feedbackPath(NULL) {
	formulas.reserve(10);
	isChecked.reserve(10);
}

StringFormula::decomposeType FormulaNode::decomposeNext(
		vector<FormulaNode> &childNodes) {

	if (!allChecked && !eliminated) {

		// Sprawdzanie spełnialności, decyzja o elminiacji w przypadku zawierania zmiennej p i jej negacji !p
		for (int i = 0; i < formulas.size(); i++) {
			if (formulas[i].isPreposition())
				for (int j = i; j < formulas.size(); j++) {
					if (formulas[j].isPrepositionNegation(formulas[i])) {
						eliminated = true;
						return StringFormula::decomposeType::none;
						break;
					}
				}
		}

		int stackIdx = -1, singleIdx = -1, stackBranchIdx = -1,
				substackBranchIdx = -1, branchIdx = -1, nextIdx = -1;
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
				} else if (formulas[i].getType()
						== StringFormula::decomposeType::next) {
					nextIdx = i;
				} else {
					//isChecked[i] = true;
				}
			}
		}

		if (stackIdx >= 0) {
			childNodes.clear();
			childNodes.resize(1);
			vector<StringFormula> subformulas;
			formulas[stackIdx].decompose(subformulas);
			for (int i = 0; i < formulas.size(); i++)
				if (!isChecked[i]) {
					if (i == stackIdx) {
						childNodes[0].appendFormula(subformulas[0]);
						childNodes[0].appendFormula(subformulas[1]);
					} else
						childNodes[0].appendFormula(formulas[i]);
				}
			isChecked[stackIdx] = true;
			return StringFormula::decomposeType::stacking;
		} else if (singleIdx >= 0) {
			childNodes.clear();
			childNodes.resize(1);
			vector<StringFormula> subformulas;
			formulas[singleIdx].decompose(subformulas);
			for (int i = 0; i < formulas.size(); i++)
				if (!isChecked[i]) {
					if (i == singleIdx) {
						childNodes[0].appendFormula(subformulas[0]);
					} else
						childNodes[0].appendFormula(formulas[i]);
				}
			isChecked[singleIdx] = true;
			return StringFormula::decomposeType::single;
		} else if (stackBranchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[stackBranchIdx].decompose(subformulas);
			for (int i = 0; i < formulas.size(); i++)
				if (!isChecked[i]) {
					if (i == stackBranchIdx) {
						childNodes[0].appendFormula(subformulas[0]);
						childNodes[0].appendFormula(subformulas[1]);
						childNodes[1].appendFormula(subformulas[2]);
						childNodes[1].appendFormula(subformulas[3]);
					} else {
						childNodes[0].appendFormula(formulas[i]);
						childNodes[1].appendFormula(formulas[i]);
					}
				}
			isChecked[stackBranchIdx] = true;
			return StringFormula::decomposeType::branching_stacking;
		} else if (substackBranchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[substackBranchIdx].decompose(subformulas);
			for (int i = 0; i < formulas.size(); i++)
				if (!isChecked[i]) {
					if (i == substackBranchIdx) {
						childNodes[0].appendFormula(subformulas[0]);
						childNodes[1].appendFormula(subformulas[1]);
						childNodes[1].appendFormula(subformulas[2]);
					} else {
						childNodes[0].appendFormula(formulas[i]);
						childNodes[1].appendFormula(formulas[i]);
					}
				}
			isChecked[substackBranchIdx] = true;
			return StringFormula::decomposeType::branching_substacking;
		} else if (branchIdx >= 0) {
			childNodes.clear();
			childNodes.resize(2);
			vector<StringFormula> subformulas;
			formulas[branchIdx].decompose(subformulas);
			for (int i = 0; i < formulas.size(); i++)
				if (!isChecked[i]) {
					if (i == branchIdx) {
						childNodes[0].appendFormula(subformulas[0]);
						childNodes[1].appendFormula(subformulas[1]);
					} else {
						childNodes[0].appendFormula(formulas[i]);
						childNodes[1].appendFormula(formulas[i]);
					}
				}
			isChecked[branchIdx] = true;
			return StringFormula::decomposeType::branching;
		} else if (nextIdx >= 0) {
			childNodes.clear();
			childNodes.resize(1);
			vector<StringFormula> subformulas = nextSet();
			for (int i = 0; i < subformulas.size(); i++) {
				childNodes[0].appendFormula(subformulas[i]);
			}
			isChecked[nextIdx] = true;
			return StringFormula::decomposeType::next;
		}
	}

	return StringFormula::decomposeType::none;
}

FormulaNode::~FormulaNode() {

}

bool FormulaNode::isEliminated() {
	return eliminated;
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

bool FormulaNode::getAllChecked() {
	return allChecked;
}

void FormulaNode::setAllChecked(bool value) {
	allChecked = value;
}

string FormulaNode::toFormattedString() {
	string formattedString;
	if (formulas.size() > 0)
			formattedString.append(formulas[0].getRaw());
	for (int i = 1; i < formulas.size(); i++) {
		formattedString.append("; " + formulas[i].getRaw());
	}
	return formattedString;
}

bool FormulaNode::isEmpty() {
	if (formulas.size() > 0)
		return false;
	else
		return true;
}

vector<StringFormula> FormulaNode::nextSet() {

	vector<StringFormula> nextNode;

	for (int i = 0; i < formulas.size(); i++) {
		StringFormula newNextFormula = formulas[i].nextFormula();
		if (!newNextFormula.isEmpty()) {
			nextNode.push_back(newNextFormula);
		}
	}

	return nextNode;

}

void FormulaNode::setEliminated() {
	eliminated = true;
}

void FormulaNode::setFeedbackPath(tree<FormulaNode>::iterator newFeedbackPath) {
	feedbackPath = newFeedbackPath;
}

tree<FormulaNode>::iterator FormulaNode::getFeedbackPath() {
	return feedbackPath;
}

void FormulaNode::eraseFormula(int idx) {
	formulas.erase(formulas.begin() + idx);
}

vector<StringFormula> FormulaNode::toSatisfy() {
	vector<StringFormula> formulasVect;
	for (int i = 0; i < formulas.size(); i++) {
		if (!formulas[i].needSatisfaction().isEmpty())
			formulasVect.push_back(formulas[i].needSatisfaction());
	}
	return formulasVect;
}

bool FormulaNode::contains(StringFormula formula) {
	for (int i = 0; i < formulas.size(); i++)
		if (formulas[i] == formula)
			return true;
	return false;
}

ostream& operator<<(ostream& os, const FormulaNode& formulaNode) {

	if (formulaNode.formulas.size() > 0)
		os << formulaNode.formulas[0];
	for (int i = 1; i < formulaNode.formulas.size(); i++) {
		os << "; " << formulaNode.formulas[i];
	}
	os << "    eliminated: " << formulaNode.eliminated << "    all checked: " << formulaNode.allChecked;
	return os;
}

bool FormulaNode::operator==(const FormulaNode &other) const {

	if (formulas.size() != other.formulas.size())
		return false;
	bool hasEqual;
	for (int i = 0; i < formulas.size(); i++)
	{
		hasEqual = false;
		for (int j = 0; j < other.formulas.size(); j++)
		{
			if (formulas[i] == other.formulas[j]) {
				hasEqual = true;
				break;
			}
		}
		if (!hasEqual)
			return false;
	}
	return true;
}
