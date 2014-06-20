#include "GUI.hpp"

bool QmlBridge::validate(QString formula)
{
	char* cstr = formula.toUtf8().data();
	return validate_C(cstr);
}

QString QmlBridge::toPrefix(QString formula)
{
	char* cstr = formula.toUtf8().data();
	return QString::fromUtf8(toPrefix_C(cstr));
}

QString QmlBridge::toInfix(QString formula)
{
	char* cstr = formula.toUtf8().data();
	return QString::fromUtf8(toInfix_C(cstr));
}

int QmlBridge::getOperatorArgCount(QString formula)
{
	char* cstr = formula.toUtf8().data();
	return getOperatorArgCount_C(cstr);
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
	return QString::fromUtf8(getErrorToken_C());
}

int QmlBridge::getErrorPosition()
{
	return getErrorPosition_C();
}

void initGUI()
{
	int argc = 0;
	char **argv = NULL;
	
	QApplication *app = new QApplication(argc, argv);
	
	QmlBridge::declareQML();
	QmlBridge *bridge = new QmlBridge();
	
	QQmlApplicationEngine *engine = new QQmlApplicationEngine("gui/Main.qml");
	engine->rootContext()->setContextProperty("cppBridge", bridge);
	
	app->exec();
}