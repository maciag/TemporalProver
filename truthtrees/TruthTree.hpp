#include "../include/tree.hh"
#include "../functions/prefix.h"
#include <string>
#include <iostream>

using namespace std;

class TruthTree {

	tree<string> mainTree;
	tree<string>::iterator root;

public:

	TruthTree(string formula);
	TruthTree(char *formula);

	void decompose();

	friend ostream& operator<<(ostream& os, const TruthTree& truthTree);

};
