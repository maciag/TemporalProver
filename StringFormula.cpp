#include "StringFormula.hpp"

map<string, token> StringFormula::tokenMap =
		{ { string("!"), token::tneg }, { string("&"), token::tand }, { string(
				"|"), token::tor }, { string("^"), token::txor }, { string(">"),
				token::timp }, { string("="), token::teq } };

StringFormula::StringFormula(string content) :
		rawContent(content) {

	symbolArray.reserve(100);
	tokenArray.reserve(100);

	if (!validate_C((char*) rawContent.c_str()))
		throw runtime_error("Nieprawidłowa formuła");

	inflixContent = string(toPrefix_C((char*) rawContent.c_str()));

	symbolArray = splitString(inflixContent, " ");
	for (int i = 0; i < symbolArray.size(); i++)
		tokenArray.push_back(parseSymbol(symbolArray[i]));

}

StringFormula::StringFormula(vector<string> symbolArray) :
		symbolArray(symbolArray) {
	symbolArray.reserve(100);
	tokenArray.reserve(100);

	for (int i = 0; i < symbolArray.size(); i++) {
		tokenArray.push_back(parseSymbol(symbolArray[i]));
		inflixContent.append(symbolArray[i] + " ");
	}

	inflixContent.pop_back();

	//TODO Kontrola konsystentności stworzonej formuły
	//TODO Wyznaczenie klasycznej postaci infiksowej
	rawContent = string("***");

}

vector<string> StringFormula::splitString(const string& s, const string& delim,
		const bool keep_empty) {
	vector<string> result;
	if (delim.empty()) {
		result.push_back(s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);
		if (keep_empty || !temp.empty()) {
			result.push_back(temp);
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return result;
}

token StringFormula::parseSymbol(string symbol) {

	bool isAllNum = true;
	for (int i = 0; i < symbol.size(); i++)
		if (!isalnum(symbol[i]))
			isAllNum = false;

	if (isAllNum)
		return token::tvar;

	map<string, token>::const_iterator val = tokenMap.find(symbol);
	if (val == tokenMap.end())
		throw runtime_error("Corrupted input formula " + rawContent);

	return val->second;
}

StringFormula::~StringFormula() {

}

token StringFormula::getToken(int idx) {
	return tokenArray[idx];
}

string StringFormula::getSymbol(int idx) {
	return symbolArray[idx];
}

string StringFormula::getRaw() {
	return rawContent;
}

vector<token> StringFormula::getTokenArray() {
	return tokenArray;
}

vector<string> StringFormula::getSymbolArray() {
	return symbolArray;
}

bool StringFormula::isBranching() {
	if (tokenArray.size() < 3)
		return false;

	if (tokenArray.front() == token::tor || tokenArray.front() == token::timp
			|| tokenArray.front() == token::teq)
		return true;

	if (tokenArray.size() >= 4) {
		if (tokenArray.front() == token::tneg) {
			if (tokenArray[1] == token::tand || tokenArray[1] == token::teq)
				return true;
		} else
			return false;
	}

	return false;
}

bool StringFormula::isAtomic() {
	if (tokenArray.front() == token::tvar)
		return true;
	if (tokenArray[0] == token::tneg && tokenArray[1] == token::tvar)
		return true;
	return false;
}

StringFormula::decomposeType StringFormula::decompose(
		vector<StringFormula> &elements) {
	if (isAtomic())
		throw runtime_error("Dekompozycja niemożliwa - formuła atomiczna");

	elements.clear();

	int idx = 0, counter = 1;
	bool isNegated = false;
	if (tokenArray.front() == token::tneg) {
		idx++;
		isNegated = true;
	}
	while (counter != 0) {
		idx++;
		if (tokenArray[idx] == token::tvar)
			counter--;
		else {
			if (tokenArray[idx] != token::tneg)
				counter++;
		}
	}

	int begin = 1, end = idx + 1;
	if (isNegated) {
		if (tokenArray[begin] == token::tneg) {
			elements.push_back(subFormula(begin + 1, end));
			return decomposeType::single;
		}
		begin++;
	}
	StringFormula first = subFormula(begin, end), second = subFormula(end,
			symbolArray.size());

	switch (tokenArray.front()) {
	case token::tneg:

		switch (tokenArray[1]) {
		case token::tand:
			first.negate();
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return decomposeType::branching;
			break;

		case token::tor:
			first.negate();
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return decomposeType::stacking;
			break;

		case token::timp:
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return decomposeType::stacking;
			break;

		case token::teq:
			StringFormula negFirst = first, negSecond = second;
			negFirst.negate();
			negSecond.negate();
			elements.push_back(first);
			elements.push_back(negSecond);
			elements.push_back(negFirst);
			elements.push_back(second);
			return decomposeType::branching_stacking;
			break;
		}
		break;

	case token::tand:
		elements.push_back(first);
		elements.push_back(second);
		return decomposeType::stacking;
		break;

	case token::tor:
		elements.push_back(first);
		elements.push_back(second);
		return decomposeType::branching;
		break;

	case token::timp:
		first.negate();
		elements.push_back(first);
		elements.push_back(second);
		return decomposeType::branching;
		break;

	case token::teq:
		StringFormula negFirst = first, negSecond = second;
		negFirst.negate();
		negSecond.negate();
		elements.push_back(first);
		elements.push_back(second);
		elements.push_back(negFirst);
		elements.push_back(negSecond);
		return decomposeType::branching;
		break;

	}

}

void StringFormula::negate() {
	symbolArray.insert(symbolArray.begin(), "!");
	tokenArray.insert(tokenArray.begin(), token::tneg);
	inflixContent.insert(0, string("! "));
//TODO Konwersja postaci surowej notacji infiksowej (rawContent)

}

StringFormula StringFormula::subFormula(int begin, int end) {

	if (begin < 0 || end > symbolArray.size())
		throw runtime_error("Subformuła - nieprawidłowy zakres");

	vector<string> trimmedSymbolArray(symbolArray.begin() + begin,
			symbolArray.begin() + end);

	return StringFormula(trimmedSymbolArray);
}

ostream& operator<<(ostream& os, const StringFormula& stringFormula) {

	os << stringFormula.rawContent << "   " << stringFormula.inflixContent
			<< "  ";
	for (int i = 0; i < stringFormula.tokenArray.size(); i++) {
		switch (stringFormula.tokenArray[i]) {
		case token::tneg:
			os << " " << "tneg";
			break;
		case token::tand:
			os << " " << "tand";
			break;
		case token::tor:
			os << " " << "tor";
			break;
		case token::txor:
			os << " " << "txor";
			break;
		case token::timp:
			os << " " << "timp";
			break;
		case token::teq:
			os << " " << "teq";
			break;
		case token::tvar:
			os << " " << "tvar";
			break;
		}
	}
	return os;
}
