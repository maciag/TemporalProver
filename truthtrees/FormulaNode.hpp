#ifndef FORMULANODE_HPP_
#define FORMULANODE_HPP_

#include "StringFormula.hpp"
#include "../include/tree.hh"

class FormulaNode {

	vector<StringFormula> formulas;
	vector<bool> isChecked;
	bool allChecked;
	bool eliminated;
	tree<FormulaNode>::iterator feedbackPath;
	void eraseFormula(int idx);
	vector<StringFormula> nextSet();

public:
	FormulaNode(vector<StringFormula> formulas);
	FormulaNode(vector<string> rawFormulas);
	FormulaNode();
	~FormulaNode();
	void appendFormula(string rawFormula);
	void appendFormula(StringFormula formula);
	bool getAllChecked();
	void setAllChecked(bool value);
	bool isEliminated();
	void setEliminated();
	string toFormattedString();
	bool isEmpty();
	void setFeedbackPath(tree<FormulaNode>::iterator newFeedbackPath);
	tree<FormulaNode>::iterator getFeedbackPath();
	vector<StringFormula> toSatisfy();
	bool contains(StringFormula formula);

	StringFormula::decomposeType decomposeNext(vector<FormulaNode> &childNodes);
	friend ostream& operator<<(ostream& os, const FormulaNode& formulaNode);
	bool operator==(const FormulaNode &other) const;
};

#endif /* FORMULANODE_HPP_ */
