#include "GUI.hpp"

void initGUI()
{
	int argc = 0;
	char **argv = NULL;
	
	QApplication *app = new QApplication(argc, argv);
	
	new QQmlApplicationEngine("gui/Main.qml");
	app->exec();
}