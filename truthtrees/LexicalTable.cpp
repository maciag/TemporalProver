#include "LexicalTable.hpp"

map<string, token> tokenMap = { { string("!"), token::tneg }, {
		string("&"), token::tand }, { string("|"), token::tor }, { string("^"),
		token::txor }, { string(">"), token::timp },
		{ string("="), token::teq }, { string("X"), token::tnext }, { string(
				"F"), token::tfin }, { string("U"), token::tunt }, { string(
				"G"), token::talw } };

map<token, string> symbolMap = { { token::tneg, string("!") }, {
		token::tand, string("&") }, { token::tor, string("|") }, { token::txor,
		string("^") }, { token::timp, string(">") },
		{ token::teq, string("=") }, { token::tnext, string("X") }, {
				token::tfin, string("F") }, { token::tunt, string("U") }, {
				token::talw, string("G") } };
