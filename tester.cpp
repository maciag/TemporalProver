#include "functions/prefix.h"

#include <iostream>
#include <string>
using namespace std;

int main()
{
	cout << "Podaj formułę:" << endl;
	
	string line;
	getline(cin, line);
	char* infix = (char*) line.c_str();
	
	if(!validate_C(infix))
	{
		cout << "Formuła nie jest poprawna." << endl;
		return 0;
	}
	
	cout << "Zapis prefiksowy:" << endl;
	cout << toPrefix_C(infix) << endl;
	return 0;
}