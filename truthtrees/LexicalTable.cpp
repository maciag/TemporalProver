#include "LexicalTable.hpp"

map<string, token> tokenMap = { { string("!"), tneg }, {
        string("&"), tand }, { string("|"), tor }, { string("^"),
        txor }, { string(">"), timp },
        { string("="), teq }, { string("X"), tnext }, { string(
                "F"), tfin }, { string("U"), tunt }, { string(
                "G"), talw } };

map<token, string> symbolMap = { { tneg, string("!") }, {
        tand, string("&") }, { tor, string("|") }, { txor,
        string("^") }, { timp, string(">") },
        { teq, string("=") }, { tnext, string("X") }, {
                tfin, string("F") }, { tunt, string("U") }, {
                talw, string("G") } };
