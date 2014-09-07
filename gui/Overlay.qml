import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
	default property alias contents: dialogContents.children;
	
	property int dialogWidth: 320;
	property int dialogHeight: 240;
	
	property string title;
	property list<ToolButton> dialogButtons;
	property ToolButton closeButton: ToolButton
	{
		iconSource: "img/close.svg";
		tooltip: "Zamknij";
		onClicked: { overlay.visible = false; }
	}
	
	id: overlay;
	visible: false;
	anchors.fill: parent;
	
	Rectangle
	{
		anchors.fill: parent;
		color: "black";
		opacity: 0.5;
	}
	
	MouseArea  // Zjada zdarzenia myszy, żeby overlay był modalny.
	{
		anchors.fill: parent;
	}
	
	Rectangle
	{
		width: dialogWidth;
		height: dialogHeight;
		
		anchors.horizontalCenter: parent.horizontalCenter;
		anchors.verticalCenter: parent.verticalCenter;
		
		color: "#aaaaaa";
		border.color: "#000000";
		
		RowLayout
		{
			width: parent.width;
			id: titlebar;
			spacing: 0;
			height: 34;
			
			Label
			{
				Layout.fillWidth: true;
				font.pixelSize: 18;
				text: " " + title;  // Margines byle jak zrobiony, ale lepszy nie chce działać.
			}
		}
		
		Item
		{
			id: dialogContents;
			anchors.top: titlebar.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.bottom: parent.bottom;
			anchors.margins: 5;
			anchors.topMargin: 0;
		}
	}
	
	Component.onCompleted:
	{
		for(var i = 0; i < dialogButtons.length; i++)
		{
			dialogButtons[i].parent = titlebar;
		}
		
		closeButton.parent = titlebar;
	}
}