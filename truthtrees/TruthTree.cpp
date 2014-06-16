#include "TruthTree.hpp"

TruthTree::TruthTree(string formula) {

	root = mainTree.insert(mainTree.begin(), FormulaNode());
	vector<string> subformulas = StringFormula::splitString(formula, ";");
	for (int i = 0; i < subformulas.size(); i++)
		root->appendFormula(subformulas[i]);
	currentNode = root;

}

TruthTree::TruthTree(char *formula) {

	root = mainTree.insert(mainTree.begin(), FormulaNode());
	root->appendFormula(string(toPrefix_C(formula)));
	currentNode = root;

}

TruthTree::TruthTree(StringFormula formula) {

	root = mainTree.insert(mainTree.begin(), FormulaNode());
	root->appendFormula(formula);
	currentNode = root;

}

TruthTree::TruthTree(FormulaNode formulaNode) {

	root = mainTree.insert(mainTree.begin(), formulaNode);
	currentNode = root;

}

void TruthTree::decomposeAll() {
	bool running = decomposeStep();
	while (running)
		running = decomposeStep();
}

bool TruthTree::decomposeStep() {

	vector<FormulaNode> childNodes;
	StringFormula::decomposeType currentType = currentNode->decomposeNext(
			childNodes);
	switch (currentType) {
	case StringFormula::decomposeType::branching:
		mainTree.append_child(currentNode, childNodes[0]);
		mainTree.append_child(currentNode, childNodes[1]);
		return true;
		break;
	case StringFormula::decomposeType::branching_stacking:
		mainTree.append_child(currentNode, childNodes[0]);
		mainTree.append_child(currentNode, childNodes[1]);
		return true;
		break;
	case StringFormula::stacking:
		return true;
		break;
	case StringFormula::decomposeType::single:
		return true;
		break;
	case StringFormula::decomposeType::none:
		if (currentNode != mainTree.end()) {
			currentNode++;
			if (currentNode == mainTree.end())
				return false;
			else {
				decomposeStep();
				return true;
			}
		} else
			return false;
		break;
	default:
		return false;
		break;
	}
	return false;

}

string TruthTree::toFormattedString() {
	tree<FormulaNode>::iterator it = mainTree.begin();
	string formattedString;
	while (it != mainTree.end()) {
		formattedString.append((*it).toFormattedString()+"\n");
		++it;
	}
	return formattedString;
}

ostream& operator<<(ostream& os, const TruthTree& truthTree) {

	tree<FormulaNode>::iterator it = truthTree.mainTree.begin();
	while (it != truthTree.mainTree.end()) {
		for (int i = 0; i < truthTree.mainTree.depth(it); i++)
			cout << "   ";
		os << *(it) << endl;
		++it;
	}
	return os;
}
