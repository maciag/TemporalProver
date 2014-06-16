import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle
{
	id: toast;
	width: 150;
	height: 28;
	color: "#404040";
	radius: 5;
	visible: false;
	
	property string text;
	
	anchors.horizontalCenter: parent.horizontalCenter;
	anchors.bottom: parent.bottom;
	anchors.bottomMargin: 10;
	
	Text
	{
		anchors.left: parent.left;
		anchors.verticalCenter: parent.verticalCenter;
		anchors.margins: 2;
		anchors.leftMargin: 6;
		width: 200;
		
		color: "white";
		font.pixelSize: 10;
		wrapMode: Text.WordWrap;
		text: toast.text;
	}
	
	ToolButton
	{
		anchors.right: parent.right;
		anchors.verticalCenter: parent.verticalCenter;
		anchors.margins: 2;
		
		width: 20;
		height: 20;
		
		iconSource: "img/close.svg";
		tooltip: "Zamknij";
		onClicked: { toast.visible = false; }
	}
}
