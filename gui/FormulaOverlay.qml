import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Overlay
{
	id: formulaOverlay;
	title: "Edycja formuły";
	
	dialogButtons: [
	ToolButton
	{
		iconSource: "img/load.svg";
		tooltip: "Wczytaj z pliku";
		onClicked: {  }
	},
	
	ToolButton
	{
		iconSource: "img/confirm.svg";
		tooltip: "OK";
		onClicked: {  }
	}
	]
	
	TextArea
	{
		anchors.fill: parent;
	}
	
	Toast
	{
		id: toast;
		width: 250;
	}
}