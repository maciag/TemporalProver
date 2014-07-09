#ifndef LEXICALTABLE_HPP_
#define LEXICALTABLE_HPP_

#include <map>
#include <string>

using namespace std;

enum token {

	tvar = 0,
	tneg,
	tand,
	tor,
	txor,
	timp,
	teq,
	tnext,
	tfin,
	tunt,
	talw

};

extern map<string, token> tokenMap;
extern map<token, string> symbolMap;

#endif /* LEXICALTABLE_HPP_ */
