#include "TruthTree.hpp"

TruthTree::TruthTree(string formula) {

	initParserOperators();
	root = mainTree.insert(mainTree.begin(), FormulaNode());
	vector<string> subformulas = StringFormula::splitString(formula, ";");
	for (int i = 0; i < subformulas.size(); i++)
		root->appendFormula(subformulas[i]);
	currentNode = root;

}

TruthTree::TruthTree(char *formula) {

	initParserOperators();
	root = mainTree.insert(mainTree.begin(), FormulaNode());
	root->appendFormula(string(toPrefix_C(formula)));
	currentNode = root;

}

TruthTree::TruthTree(StringFormula formula) {

	initParserOperators();
	root = mainTree.insert(mainTree.begin(), FormulaNode());
	root->appendFormula(formula);
	currentNode = root;

}

TruthTree::TruthTree(FormulaNode formulaNode) {

	initParserOperators();
	root = mainTree.insert(mainTree.begin(), formulaNode);
	currentNode = root;

}

void TruthTree::initParserOperators() {

	clearOperators_C();

	for (std::map<token, string>::iterator it = symbolMap.begin();
			it != symbolMap.end(); ++it) {

		int precendence;
		bool unary;

		switch (it->first) {

		case token::talw:
			precendence = 5;
			unary = true;
			break;

		case token::tand:
			precendence = 3;
			unary = false;
			break;

		case token::teq:
			precendence = 1;
			unary = false;
			break;

		case token::tfin:
			precendence = 5;
			unary = true;
			break;

		case token::timp:
			precendence = 2;
			unary = false;
			break;

		case token::tneg:
			precendence = 5;
			unary = true;
			break;

		case token::tnext:
			precendence = 5;
			unary = true;
			break;

		case token::tor:
			precendence = 3;
			unary = false;
			break;

		case token::tunt:
			precendence = 4;
			unary = false;
			break;

		case token::txor:
			precendence = 3;
			unary = false;
			break;

		}
		string symbol = it->second;
		char *cstr = new char[symbol.length() + 1];
		strcpy(cstr, symbol.c_str());
		setOperator_C(cstr, precendence, unary);
		delete[] cstr;
	}

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
			eliminateNode(mainTree.child(currentNode, 0));
			eliminateNode(mainTree.child(currentNode, 1));
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::branching_stacking:
			mainTree.append_child(currentNode, childNodes[0]);
			mainTree.append_child(currentNode, childNodes[1]);
			eliminateNode(mainTree.child(currentNode, 0));
			eliminateNode(mainTree.child(currentNode, 1));
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::branching_substacking:
			mainTree.append_child(currentNode, childNodes[0]);
			mainTree.append_child(currentNode, childNodes[1]);
			eliminateNode(mainTree.child(currentNode, 0));
			eliminateNode(mainTree.child(currentNode, 1));
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::stacking:
			mainTree.append_child(currentNode, childNodes[0]);
			eliminateNode(mainTree.child(currentNode, 0));
			currentNode->setAllChecked(true);
			//return true;
			break;
		case StringFormula::decomposeType::single:
			mainTree.append_child(currentNode, childNodes[0]);
			eliminateNode(mainTree.child(currentNode, 0));
			currentNode->setAllChecked(true);
			//return true;
			break;

		case StringFormula::decomposeType::next: {
			tree<FormulaNode>::iterator node;
			node = mainTree.parent(currentNode);

			while (mainTree.is_valid(node)) {
				if ((*node) == childNodes[0]) {
					currentNode->setAllChecked(true);
					currentNode->setFeedbackPath(node);
					//mainTree.append_child(currentNode, childNodes[0]);
					//currentNode++;

					//currentNode->setAllChecked(true);
					break;
				}
				node = mainTree.parent(node);
			}

			if (currentNode->getFeedbackPath() == NULL) {
				mainTree.append_child(currentNode, childNodes[0]);
				eliminateNode(mainTree.child(currentNode, 0));
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

	bool eliminated = false;

	while (it != mainTree.end()) {

		if (!it->isEliminated() && it.number_of_children() != 0) {

			//tree<FormulaNode>::sibling_iterator sibIt = mainTree.child(it, 0);
			bool allEliminated = true;

			for (int i = 0; i < mainTree.number_of_children(it); i++) {
				if (!(mainTree.child(it, i)->isEliminated())) {
					allEliminated = false;
					break;
				}
			}

			if (allEliminated) {
				it->setEliminated();
				eliminated = true;
			}
		}

		if (!it->isEliminated()) {

			vector<StringFormula> toSatisfy = it->toSatisfy();
			for (int i = 0; i < toSatisfy.size(); i++) {
				tree<FormulaNode>::iterator matchIt = findFormula(toSatisfy[i]);
				if (!mainTree.is_valid(matchIt) || !existsPath(it, matchIt)) {
					it->setEliminated();
					eliminated = true;
					break;
				}
			}

		}
		it++;

	}

	if (eliminated)
		eliminateNodes();
}

void TruthTree::eliminateNode(tree<FormulaNode>::pre_order_iterator node) {
	if (!node->isEliminated() && node.number_of_children() != 0) {

		//tree<FormulaNode>::sibling_iterator sibIt = mainTree.child(it, 0);
		bool allEliminated = true;

		for (int i = 0; i < mainTree.number_of_children(node); i++) {
			if (!(mainTree.child(node, i)->isEliminated())) {
				allEliminated = false;
				break;
			}
		}

		if (allEliminated) {
			node->setEliminated();
		}
	}

	if (!node->isEliminated()) {

		vector<StringFormula> toSatisfy = node->toSatisfy();
		for (int i = 0; i < toSatisfy.size(); i++) {
			tree<FormulaNode>::iterator matchIt = findFormula(toSatisfy[i]);
			if (!mainTree.is_valid(matchIt) || !existsPath(node, matchIt)) {
				node->setEliminated();
				break;
			}
		}

	}
}

tree<FormulaNode>::iterator TruthTree::findFormula(StringFormula formula) {
	tree<FormulaNode>::iterator it(root);

	while (mainTree.is_valid(it)) {
		if (it->contains(formula) && !it->isEliminated())
			return it;
		it++;
	}

	return it;
}

bool TruthTree::existsPath(tree<FormulaNode>::iterator it1,
		tree<FormulaNode>::iterator it2) {
	tree<FormulaNode>::iterator currentIt(it1);

	while (mainTree.is_valid(currentIt)) {

		if (it1 == it2)
			return false;

		if (currentIt == it2)
			return true;

		currentIt++;
	}

	if (currentIt->getFeedbackPath() != NULL) {
		return existsPath(currentIt->getFeedbackPath(), it2);
	}

	return false;
}

bool TruthTree::getResult() {
	return !root->isEliminated();
}

string TruthTree::nodeToFormattedString(tree<FormulaNode>::iterator it) {

	if (it == mainTree.end())
		return "";

	string nodeString = it->toFormattedString();

	//if(mainTree.number_of_children(it) == 0)
	//	return nodeString;

	if (mainTree.number_of_children(it) == 0) {
		nodeString.append("\t\t");
	}

	else if (mainTree.number_of_children(it) == 1) {
		nodeString.append("\t");
		nodeString.append(nodeToFormattedString(mainTree.child(it, 0)));
		nodeString.append("\t");
	}

	else if (mainTree.number_of_children(it) == 2) {
		nodeString.append("\t");
		nodeString.append(nodeToFormattedString(mainTree.child(it, 0)));
		nodeString.append("\t");
		nodeString.append(nodeToFormattedString(mainTree.child(it, 1)));
	}

	return nodeString;

}

string TruthTree::toFormattedString() {
	/*tree<FormulaNode>::iterator it = mainTree.end();
	 string formattedString;
	 while (it != mainTree.begin()) {
	 formattedString.append((*it).toFormattedString() + "t");
	 if (mainTree.number_of_siblings(it) == 1)
	 formattedString.append("t");
	 --it;
	 }*/
	return nodeToFormattedString(root);
}

bool TruthTree::changeOperator(token op, string symbol) {

	map<token, string>::const_iterator val = symbolMap.find(op);

	if (val == symbolMap.end())
		return false;

	string oldSymbol = symbolMap[op];
	symbolMap[op] = symbol;

	tokenMap.erase(oldSymbol);
	tokenMap[symbol] = op;

	int precendence;
	bool unary;

	switch (op) {

	case token::talw:
		precendence = 5;
		unary = true;
		break;

	case token::tand:
		precendence = 3;
		unary = false;
		break;

	case token::teq:
		precendence = 1;
		unary = false;
		break;

	case token::tfin:
		precendence = 5;
		unary = true;
		break;

	case token::timp:
		precendence = 2;
		unary = false;
		break;

	case token::tneg:
		precendence = 5;
		unary = true;
		break;

	case token::tnext:
		precendence = 5;
		unary = true;
		break;

	case token::tor:
		precendence = 3;
		unary = false;
		break;

	case token::tunt:
		precendence = 4;
		unary = false;
		break;

	case token::txor:
		precendence = 3;
		unary = false;
		break;

	}

	char *cstr = new char[oldSymbol.length() + 1];
	strcpy(cstr, oldSymbol.c_str());
	unsetOperator_C(cstr);
	delete[] cstr;

	cstr = new char[symbol.length() + 1];
	strcpy(cstr, symbol.c_str());
	setOperator_C(cstr, precendence, unary);
	delete[] cstr;

	return true;
}

void TruthTree::resetOperators() {

	tokenMap.clear();
	symbolMap.clear();
	clearOperators_C();

	tokenMap["!"] = token::tneg;
	tokenMap["&"] = token::tand;
	tokenMap["|"] = token::tor;
	tokenMap["^"] = token::txor;
	tokenMap[">"] = token::timp;
	tokenMap["="] = token::teq;
	tokenMap["X"] = token::tnext;
	tokenMap["F"] = token::tfin;
	tokenMap["U"] = token::tunt;
	tokenMap["G"] = token::talw;

	symbolMap[token::tneg] = "!";
	symbolMap[token::tand] = "&";
	symbolMap[token::tor] = "|";
	symbolMap[token::txor] = "^";
	symbolMap[token::timp] = ">";
	symbolMap[token::teq] = "=";
	symbolMap[token::tnext] = "X";
	symbolMap[token::tfin] = "F";
	symbolMap[token::tunt] = "U";
	symbolMap[token::talw] = "G";

	initParserOperators();

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
