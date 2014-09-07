#include "GUI.hpp"

#include "../truthtrees/WorkerThread.hpp"

#include <iostream>
#include <sstream>
using namespace std;

bool QmlBridge::validate(QString formula)
{
	return validate_C(formula);
}

QString QmlBridge::toPrefix(QString formula)
{
	return toPrefix_C(formula);
}

QString QmlBridge::toInfix(QString formula)
{
	return toInfix_C(formula);
}

int QmlBridge::getOperatorArgCount(QString formula)
{
	return getOperatorArgCount_C(formula);
}

// Ustawienie operatora wykorzystywanego przez TruthTree (pośrednio)
// QString token - semantyczna definicja operatora - ciąg znaków opisujący znaczenie opratora, np. and, or, etc.
// QString symbol - syntaktyczna definicja operatora - ciąg znaków symbolizujący operator, np. &, |, etc.
// return - true w przypadku powodzenia, false w przypadku nierozpoznania parametru token - dozwolone wartości:
// and, or, xor, impl, eq, not, tempU, tempX, tempG, tempF
bool QmlBridge::setTruthTreeOperator(QString token, QString symbol)
{
	if (token == "and") {
		return TruthTree::changeOperator(tand,symbol.toStdString());
	} else if (token == "or") {
		return TruthTree::changeOperator(tor,symbol.toStdString());
	} else if (token == "xor") {
		return TruthTree::changeOperator(txor,symbol.toStdString());
	} else if (token == "impl") {
		return TruthTree::changeOperator(timp,symbol.toStdString());
	} else if (token == "eq") {
		return TruthTree::changeOperator(teq,symbol.toStdString());
	} else if (token == "not") {
		return TruthTree::changeOperator(tneg,symbol.toStdString());
	} else if (token == "tempU") {
		return TruthTree::changeOperator(tunt,symbol.toStdString());
	} else if (token == "tempX") {
		return TruthTree::changeOperator(tnext,symbol.toStdString());
	} else if (token == "tempG") {
		return TruthTree::changeOperator(talw,symbol.toStdString());
	} else if (token == "tempF") {
		return TruthTree::changeOperator(tfin,symbol.toStdString());
	}
	// Nie rozpoznano operatora
	return false;
}


QmlBridge::ErrorCode QmlBridge::getErrorCode()
{
	int errcode = getErrorCode_C();
	if(errcode == ERRCODE_OK)
		return OK;
	
	else if(errcode == ERRCODE_UNEXPECTED_END)
		return UnexpectedEnd;
	
	else if(errcode == ERRCODE_UNEXPECTED_VAR)
		return UnexpectedVar;
	
	else if(errcode == ERRCODE_UNEXPECTED_BIN_OPER)
		return UnexpectedBinOper;
	
	else if(errcode == ERRCODE_UNEXPECTED_UN_OPER)
		return UnexpectedUnOper;
	
	else if(errcode == ERRCODE_UNEXPECTED_LBRACK)
		return UnexpectedLeftBracket;
	
	else if(errcode == ERRCODE_UNEXPECTED_RBRACK)
		return UnexpectedRightBracket;
	
	else if(errcode == ERRCODE_INVALID_TOKEN)
		return InvalidToken;
	
	else if(errcode == ERRCODE_BRACKET_UNCLOSED)
		return BracketUnclosed;
	
	else if(errcode == ERRCODE_SPARE_RBRACK)
		return SpareRightBracket;
	
	else return UnknownError;
}

QString QmlBridge::getErrorToken()
{
	return getErrorToken_C();
}

int QmlBridge::getErrorPosition()
{
	return getErrorPosition_C();
}

void QmlBridge::setParserOperator(QString symbol, int precedence, bool unary)
{
	setOperator_C(symbol, precedence, unary);
}

void QmlBridge::clearParserOperators()
{
	clearOperators_C();
}

QString QmlBridge::loadFile(QUrl file)
{
	QFile f(file.toLocalFile());
	
	if(!f.open(QFile::ReadOnly | QFile::Text))
		return QString("");
	
	QTextStream in(&f);
	return in.readAll();
}

void QmlBridge::saveFile(QUrl file, QString content)
{
	QFile f(file.toLocalFile());
	
	if(!f.open(QFile::WriteOnly | QFile::Truncate))
		return;
	
	QTextStream out(&f);
	out << content;
	f.close();
}

void QmlBridge::startComputation(QString formula)
{
	workerThread = new WorkerThread(formula.toStdString());
	
	connect(workerThread, SIGNAL(allDone(QString, bool)), this, SLOT(threadFinished(QString, bool)));
	workerThread->startComputation();
}

void QmlBridge::abortComputation()
{
	workerThread->abortComputation();
}

void QmlBridge::threadFinished(QString resTree, bool result)
{
	// Obiekt główny w QML-u tworzę tylko jeden, więc już go nie szukam po nazwie.
	QMetaObject::invokeMethod(engine->rootObjects()[0], "handleResults", Q_ARG(QVariant, resTree), Q_ARG(QVariant, result));
}

void initGUI()
{
	int argc = 0;
    char **argv = NULL;
	
    QApplication *app = new QApplication(argc, argv);
	
	QmlBridge::declareQML();
	QmlBridge *bridge = new QmlBridge();
	
	bridge->engine = new QQmlApplicationEngine("gui/Main.qml");
	bridge->engine->rootContext()->setContextProperty("cppBridge", bridge);
	
	app->exec();
	
	delete app;
	delete bridge;
}
