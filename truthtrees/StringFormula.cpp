#include "StringFormula.hpp"

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

	evalDecompositionType();

}

StringFormula::StringFormula(vector<string> symbolArray) :
		symbolArray(symbolArray) {
	symbolArray.reserve(100);
	tokenArray.reserve(100);

	for (int i = 0; i < symbolArray.size(); i++) {
		tokenArray.push_back(parseSymbol(symbolArray[i]));
		inflixContent.append(symbolArray[i] + " ");
	}

	if (inflixContent.size() > 0)
		inflixContent.erase(inflixContent.size() - 1);

	//TODO Kontrola konsystentności stworzonej formuły
	//TODO Wyznaczenie klasycznej postaci infiksowej
	rawContent = string("***");

	evalDecompositionType();

}

StringFormula::StringFormula() {
	symbolArray.reserve(100);
	tokenArray.reserve(100);
	type = decomposeType::none;
}

void StringFormula::evalDecompositionType() {

	// Przypadki, dla których nie jest wykonywana dalsza dekompozycja
	// Pusta formuła
	if (tokenArray.size() == 0) {
		type = decomposeType::none;
		return;
	}
	// Formuła składająca się tylko ze zmiennej
	if (tokenArray[0] == token::tvar) {
		type = decomposeType::none;
		return;
	}
	// Formuła składająca się tylko z zaporzeczonej zmiennej
	if (tokenArray[0] == token::tneg && tokenArray[1] == token::tvar) {
		type = decomposeType::none;
		return;
	}

	switch (tokenArray[0]) {
	case token::tneg: {
		switch (tokenArray[1]) {
		case token::tand:
			type = decomposeType::branching;
			break;

		case token::tor:
			type = decomposeType::stacking;
			break;

		case token::timp:
			type = decomposeType::stacking;
			break;

		case token::teq:
			type = decomposeType::branching_stacking;
			break;

		case token::tneg:
			type = decomposeType::single;
			break;

		case token::tnext:
			type = decomposeType::single;

		case token::tfin:
			type = decomposeType::stacking;

		case token::tunt:
			type = decomposeType::stacking;

		case token::talw:
			type = decomposeType::single;

		default:
			type = decomposeType::none;
		}
		break;
	}
	case token::tand:
		type = decomposeType::stacking;
		break;

	case token::tor:
		type = decomposeType::branching;
		break;

	case token::timp:
		type = decomposeType::branching;
		break;

	case token::teq:
		type = decomposeType::branching_stacking;
		break;

	case token::tfin:
		type = decomposeType::branching;
		break;

	case token::tunt:
		type = decomposeType::branching_substacking;
		break;

	case token::talw:
		type = decomposeType::stacking;
		break;

	case token::tnext:
		type = decomposeType::next;
		break;

	default:
		type = decomposeType::none;

	}

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
		if (!isalnum(symbol[i]) || isupper((int)symbol[i]))
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
	return inflixContent;
}

vector<token> StringFormula::getTokenArray() {
	return tokenArray;
}

vector<string> StringFormula::getSymbolArray() {
	return symbolArray;
}

bool StringFormula::decompose(vector<StringFormula> &elements) {
	if (type == decomposeType::none || type == decomposeType::error)
		return false;

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
			if (tokenArray[idx] != token::tneg
					&& tokenArray[idx] != token::tnext
					&& tokenArray[idx] != token::tfin
					&& tokenArray[idx] != token::talw)
				counter++;
		}
	}

	// Obsługa dekompozycji typu single oraz dekompozycji tworzących dwie formuły potomne z jednej wejściowej (operatory temporalne)

	int begin = 1, end = idx + 1;
	if (isNegated) {
		// Podwójna negacja - !!f = f
		if (tokenArray[begin] == token::tneg) {
			elements.push_back(subFormula(begin + 1, end));
			return true;
		}
		// Negacja operatora next - !Xf = X!f
		else if (tokenArray[begin] == token::tnext) {
			StringFormula newFormula = subFormula(begin + 1, end);
			newFormula.pushOperatorFront(token::tneg);
			newFormula.pushOperatorFront(token::tnext);
			return true;
		}
		// Negacja operatora finally - !Ff = {!f, !XFf}
		else if (tokenArray[begin] == token::tfin) {
			StringFormula newFormulaFirst = subFormula(begin + 1, end),
					newFormulaSecond = newFormulaFirst;
			newFormulaFirst.negate();
			newFormulaSecond.pushOperatorFront(token::tfin);
			newFormulaSecond.pushOperatorFront(token::tnext);
			newFormulaSecond.negate();
			elements.push_back(newFormulaFirst);
			elements.push_back(newFormulaSecond);
			return true;
		}
		// Negacja operatora globally - !Gf = F!p
		else if (tokenArray[begin] == token::talw) {
			StringFormula newFormula = subFormula(begin + 1, end);
			newFormula.negate();
			newFormula.pushOperatorFront(token::tfin);
			elements.push_back(newFormula);
			return true;
		}
		begin++;
	} else {
		// Operator finally - Ff = {f, XFf}
		if (tokenArray.front() == token::tfin) {
			StringFormula newFormulaFirst = subFormula(begin, end),
					newFormulaSecond = newFormulaFirst;
			newFormulaSecond.pushOperatorFront(token::tfin);
			newFormulaSecond.pushOperatorFront(token::tnext);
			elements.push_back(newFormulaFirst);
			elements.push_back(newFormulaSecond);
			return true;
		}
		// Operator globally - Gf = {f, XGf}
		if (tokenArray.front() == token::talw) {
			StringFormula newFormula = subFormula(begin, end);
			elements.push_back(newFormula);
			newFormula.pushOperatorFront(token::talw);
			newFormula.pushOperatorFront(token::tnext);
			elements.push_back(newFormula);
		}
	}
	// Obsługa dekompozycji typu stacking, branching i branching-stacking

	StringFormula first = subFormula(begin, end);
	StringFormula second = subFormula(end, symbolArray.size());

	switch (tokenArray.front()) {
	// Zanegowane operatory
	case token::tneg:
		switch (tokenArray[1]) {
		case token::tand:
			first.negate();
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return true;
			break;

		case token::tor:
			first.negate();
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return true;
			break;

		case token::timp:
			second.negate();
			elements.push_back(first);
			elements.push_back(second);
			return true;
			break;

		case token::teq: {
			StringFormula negFirst = first, negSecond = second;
			negFirst.negate();
			negSecond.negate();
			elements.push_back(first);
			elements.push_back(negSecond);
			elements.push_back(negFirst);
			elements.push_back(second);
			return true;
			break;
		}

		case token::tunt: {
			StringFormula newFormula = compose(first, token::tunt, second);
			newFormula.pushOperatorFront(token::tnext);
			newFormula.negate();
			first.negate();
			newFormula = compose(first, token::tor, newFormula);
			second.negate();
			elements.push_back(second);
			elements.push_back(newFormula);
			return true;
			break;
		}

		}
		break;

	case token::tand:
		elements.push_back(first);
		elements.push_back(second);
		return true;
		break;

	case token::tor:
		elements.push_back(first);
		elements.push_back(second);
		return true;
		break;

	case token::timp:
		first.negate();
		elements.push_back(first);
		elements.push_back(second);
		return true;
		break;

	case token::teq: {
		StringFormula negFirst = first, negSecond = second;
		negFirst.negate();
		negSecond.negate();
		elements.push_back(first);
		elements.push_back(second);
		elements.push_back(negFirst);
		elements.push_back(negSecond);
		return true;
		break;
	}

	case token::tunt: {
		StringFormula newFormula = compose(first, token::tunt, second);
		newFormula.pushOperatorFront(token::tnext);
		elements.push_back(second);
		elements.push_back(first);
		elements.push_back(newFormula);
		break;
	}

	default:
		return false;

	}

	return false;

}

void StringFormula::negate() {
	symbolArray.insert(symbolArray.begin(), "!");
	tokenArray.insert(tokenArray.begin(), token::tneg);
	inflixContent.insert(0, string("! "));
	evalDecompositionType();
//TODO Konwersja postaci surowej notacji infiksowej (rawContent)

}

void StringFormula::pushOperatorFront(token op) {

	map<token, string>::const_iterator val = symbolMap.find(op);
	if (val == symbolMap.end())
		throw runtime_error("Corrupted input operator");

	if (op != token::tneg && op != token::tnext && op != token::talw
			&& op != token::tfin)
		throw runtime_error(
				"Wrong non-unary operator type: " + val->second
						+ " - will cause formula corruption");

	tokenArray.insert(tokenArray.begin(), op);
	symbolArray.insert(symbolArray.begin(), val->second);
	inflixContent.insert(0, val->second + " ");
	evalDecompositionType();
}

StringFormula StringFormula::subFormula(int begin, int end) {

	if (begin < 0 || end > symbolArray.size())
		throw runtime_error("Subformuła - nieprawidłowy zakres");

	vector<string> trimmedSymbolArray(symbolArray.begin() + begin,
			symbolArray.begin() + end);

	return StringFormula(trimmedSymbolArray);
}

bool StringFormula::isPreposition() {
	return tokenArray[0] == token::tvar;
}

bool StringFormula::isPrepositionNegation(StringFormula other) {
	return tokenArray[1] == token::tvar && tokenArray[0] == token::tneg
			&& other.tokenArray[0] == token::tvar
			&& other.symbolArray[0] == symbolArray[1];
}

bool StringFormula::isEmpty() {
	if (symbolArray.size() > 0)
		return false;
	else
		return true;
}

ostream& operator<<(ostream& os, const StringFormula& stringFormula) {

	/*os << stringFormula.rawContent << "   " << stringFormula.inflixContent
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
	 */
	os << stringFormula.inflixContent;

	return os;
}

bool StringFormula::operator==(const StringFormula &other) const {

	if (symbolArray.size() != other.symbolArray.size())
		return false;
	for (int i = 0; i < symbolArray.size(); i++)
		if (symbolArray[i] != other.symbolArray[i])
			return false;
	return true;
}

bool StringFormula::operator!=(const StringFormula &other) const {

	return !(*this == other);

}

StringFormula StringFormula::nextFormula() {

	if (tokenArray.size() > 0 && tokenArray[0] == token::tnext) {
		vector<string> newSymbolArray(symbolArray);
		newSymbolArray.erase(newSymbolArray.begin());
		return StringFormula(newSymbolArray);
	}
	return StringFormula();
}

StringFormula::decomposeType StringFormula::getType() {
	return type;
}

StringFormula StringFormula::needSatisfaction() {

	if (tokenArray[0] == token::tfin) {
		vector<string> newSymbolArray(symbolArray);
		newSymbolArray.erase(newSymbolArray.begin());
		//std::cout << StringFormula(newSymbolArray) << endl;
		return StringFormula(newSymbolArray);
	}
	if (tokenArray[1] == token::tunt) {
		int idx = 0, counter = 1;

		while (counter != 0) {
			idx++;
			if (tokenArray[idx] == token::tvar)
				counter--;
			else {
				if (tokenArray[idx] != token::tneg
						&& tokenArray[idx] != token::tnext
						&& tokenArray[idx] != token::tfin
						&& tokenArray[idx] != token::talw)
					counter++;
			}
		}

		int begin = 1, end = idx + 1;
		//std::cout << subFormula(begin, end) << endl;
		return subFormula(begin, end);
	}

	return StringFormula();

}

StringFormula StringFormula::compose(StringFormula left, token op,
		StringFormula right) {

	map<token, string>::const_iterator val = symbolMap.find(op);
	if (val == symbolMap.end())
		throw runtime_error("Corrupted input operator");

	if (op != token::tand && op != token::tor && op != token::timp
			&& op != token::teq && op != token::tunt)
		throw runtime_error(
				"Wrong non-binary operator type: " + val->second
						+ " - will cause formula corruption");

	vector<string> newSymbolArray;
	newSymbolArray.push_back(val->second);
	for (int i = 0; i < left.symbolArray.size(); i++)
		newSymbolArray.push_back(left.symbolArray[i]);
	for (int i = 0; i < right.symbolArray.size(); i++)
		newSymbolArray.push_back(right.symbolArray[i]);
	return StringFormula(newSymbolArray);

}
