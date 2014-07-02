#ifndef GUI_HPP
#define GUI_HPP

#include <QApplication>
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

#include <../functions/prefix.h>
#include <../truthtrees/LexicalTable.hpp>

/**
 * Klasa służąca do wywoływania metod C++ z poziomu QML.
 */
class QmlBridge : public QObject
{
	Q_OBJECT
	
	public:
	enum ErrorCode
	{
		OK,
		UnexpectedEnd,
		UnexpectedVar,
		UnexpectedBinOper,
		UnexpectedUnOper,
		UnexpectedLeftBracket,
		UnexpectedRightBracket,
		InvalidToken,
		BracketUnclosed,
		SpareRightBracket,
		UnknownError  // Dla "else", gdyby był niepoprawny kod błędu. Nie powinno wystąpić.
	}
	Q_ENUMS(ErrorCode);
	
	static void declareQML()
	{
		qmlRegisterType<QmlBridge>("CppBridge", 1, 0, "ErrorCode");
	}
	
	Q_INVOKABLE bool validate(QString formula);
	Q_INVOKABLE QString toPrefix(QString formula);
	Q_INVOKABLE QString toInfix(QString formula);
	Q_INVOKABLE int getOperatorArgCount(QString formula);
	Q_INVOKABLE bool setTruthTreeOperator(QString token, QString symbol);
	
	Q_INVOKABLE ErrorCode getErrorCode();
	Q_INVOKABLE QString getErrorToken();
	Q_INVOKABLE int getErrorPosition();
	
	Q_INVOKABLE void setParserOperator(QString symbol, int precedence, bool unary);
	Q_INVOKABLE void clearParserOperators();
	
	Q_INVOKABLE QString loadFile(QUrl file);
	Q_INVOKABLE void saveFile(QUrl file, QString content);
};

// GUI
void initGUI();

#endif
