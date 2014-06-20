import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

ColumnLayout
{
	default property alias contents: childContainer.children;
	property string value: "";
	property string character: "";
	property Label valueLabel: null;
	
	property RowLayout childContainer: RowLayout
	{
		id: childContainer;
		spacing: 20;
		anchors.horizontalCenter: parent.horizontalCenter;
	}
	
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
			
			onEntered:
			{
				parent.color = "#00A0E0"
				if(valueLabel != null)
					valueLabel.text = value;
				
			}
			
			onExited:
			{
				parent.color = "#0060A0";
				if(valueLabel != null)
					valueLabel.text = "";
			}
		}
		
		Text
		{
			anchors.horizontalCenter: parent.horizontalCenter;
			anchors.verticalCenter: parent.verticalCenter;
			
			color: "white";
			font.bold: true;
			text: character;
		}
	}
	
	Component.onCompleted:
	{
		childContainer.parent = this;
	}
}