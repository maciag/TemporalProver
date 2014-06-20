#include "truthtrees/TruthTree.hpp"
#include "truthtrees/StringFormula.hpp"
#include "functions/prefix.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;

void initGUI();  // Zdefiniowane w GUI.hpp

int main()
{
	resetOperators_C();
	initGUI();
	return 0;
}
