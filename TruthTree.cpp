#include "TruthTree.hpp"

TruthTree::TruthTree(string formula) {

	root = mainTree.insert(mainTree.begin(),
			string(toPrefix_C((char*) formula.c_str())));

}

TruthTree::TruthTree(char *formula) {

	root = mainTree.insert(mainTree.begin(), string(toPrefix_C(formula)));

}

void TruthTree::decompose() {



}

ostream& operator<<(ostream& os, const TruthTree& truthTree) {

	tree<string>::iterator it = truthTree.mainTree.begin();
	while (it != truthTree.mainTree.end()) {
		os << *(it) << "	";
		++it;
	}
	return os;
}
