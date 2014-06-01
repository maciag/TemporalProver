import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

ColumnLayout
{
	default property alias contents: childContainer.children;
	property string value;
	
	id: node;
	spacing: 20;
	
	width: implicitWidth;
	height: implicitHeight;
	
	Rectangle
	{
		width: 20;
		height: 20;
		anchors.horizontalCenter: parent.horizontalCenter;
		
		color: "#0060A0";
		radius: 10;
		border.color: "black";
		border.width: 1;
		
		MouseArea
		{
			anchors.fill: parent;
			hoverEnabled: true;
			
			onEntered: { parent.color = "#00A0E0"; }
			onExited: { parent.color = "#0060A0"; }
		}
	}
	
	RowLayout
	{
		id: childContainer;
		spacing: 20;
		anchors.horizontalCenter: parent.horizontalCenter;
	}
}