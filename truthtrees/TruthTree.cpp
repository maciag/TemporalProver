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
	if (!currentNode->isEliminated() && !currentNode->getAllChecked()) {
		StringFormula::decomposeType currentType = currentNode->decomposeNext(
				childNodes);
		switch (currentType) {
		case StringFormula::decomposeType::branching:
			mainTree.append_child(currentNode, childNodes[0]);
			mainTree.append_child(currentNode, childNodes[1]);
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::branching_stacking:
			mainTree.append_child(currentNode, childNodes[0]);
			mainTree.append_child(currentNode, childNodes[1]);
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::branching_substacking:
			mainTree.append_child(currentNode, childNodes[0]);
			mainTree.append_child(currentNode, childNodes[1]);
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::stacking:
			mainTree.append_child(currentNode, childNodes[0]);
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::single:
			mainTree.append_child(currentNode, childNodes[0]);
			currentNode->setAllChecked(true);
			//return true;
			break;

		case StringFormula::decomposeType::next: {
			tree<FormulaNode>::iterator node;
			node = mainTree.parent(currentNode);

			while (node != mainTree.begin()) {
				if ((*node) == childNodes[0]) {
					currentNode->setAllChecked(true);
					mainTree.append_child(currentNode, childNodes[0]);
					currentNode++;
					currentNode->setFeedbackPath(node);
					currentNode->setAllChecked(true);
					break;
				}
				node = mainTree.parent(node);;
			}

			if (currentNode->getFeedbackPath() == NULL) {
				mainTree.append_child(currentNode, childNodes[0]);
				currentNode->setAllChecked(true);
			}

			break;
		}
		case StringFormula::decomposeType::none:
			currentNode->setAllChecked(true);
			/*if (!currentNode->isEliminated()
			 && !currentNode->hasNextSet().isEmpty()) {
			 FormulaNode nextFormulaNode = currentNode->hasNextSet();
			 tree<FormulaNode>::iterator node = currentNode;

			 while (node != mainTree.begin()) {
			 if ((*node) == nextFormulaNode) {
			 currentNode->setFeedbackPath(node);
			 break;
			 }
			 node--;
			 }

			 if (currentNode->getFeedbackPath() == NULL) {
			 mainTree.append_child(currentNode, nextFormulaNode);
			 }

			 }*/
			break;
		default:
			return false;
			break;
		}
	}
	if (currentNode != mainTree.end()) {
		currentNode++;
		if (currentNode == mainTree.end())
			return false;
		else {
			//decomposeStep();
			return true;
		}
	} else
		return false;

	return false;

}

void TruthTree::eliminateNodes() {
	tree<FormulaNode>::pre_order_iterator it(mainTree.begin());

	while (it != mainTree.end()) {

		if (!it->isEliminated() && it.number_of_children() != 0) {

			tree<FormulaNode>::sibling_iterator sibIt = mainTree.child(it, 0);
			bool allEliminated = true;

			for (int i = 0; i < mainTree.number_of_siblings(sibIt); i++) {
				if (!sibIt->isEliminated()) {
					allEliminated = false;
					break;
				}
			}

			if (allEliminated)
				it->setEliminated();

		}

		if (!it->isEliminated()) {

			vector<StringFormula> toSatisfy = it->toSatisfy();
			for (int i = 0; i < toSatisfy.size(); i++) {
				tree<FormulaNode>::iterator matchIt = findFormula(toSatisfy[i]);
				if (matchIt == mainTree.end() || !existsPath(it, matchIt)) {
					it->setEliminated();
					break;
				}
			}

		}
		it++;

	}

}

tree<FormulaNode>::iterator TruthTree::findFormula(StringFormula formula) {
	tree<FormulaNode>::iterator it(root);

	while (it != mainTree.end()) {
		if (it->contains(formula) && !it->isEliminated())
			return it;
		it++;
	}

	return it;
}

bool TruthTree::existsPath(tree<FormulaNode>::iterator it1, tree<FormulaNode>::iterator it2) {
	tree<FormulaNode>::iterator currentIt(it1);
	while (currentIt != mainTree.end()) {


		if (it1 == it2)
			return false;

		if (currentIt == it2)
			return true;

		if (currentIt->getFeedbackPath() != NULL) {
			if (existsPath(currentIt->getFeedbackPath(), it2))
				return true;
		}

		currentIt++;
	}
	return false;
}

bool TruthTree::getResult() {
	return !root->isEliminated();
}

string TruthTree::toFormattedString() {
	tree<FormulaNode>::iterator it = mainTree.begin();
	string formattedString;
	while (it != mainTree.end()) {
		formattedString.append((*it).toFormattedString() + "\n");
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
