import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

ApplicationWindow
{
	title: "Prover";
	width: 800;
	height: 600;
	visible: true;
	
	RowLayout
	{
		anchors.fill: parent;
		anchors.margins: spacing;
		
		TabView
		{
			Layout.fillWidth: true;
			Layout.fillHeight: true;
			Layout.minimumWidth: 200;
			Layout.minimumHeight: 200;
		}
		
		ColumnLayout
		{
			Layout.alignment: Qt.AlignTop | Qt.AlignLeft;
			
			HeaderLabel
			{
				text: "Predykaty";
			}
			
			TableView
			{
				Layout.minimumHeight: 100;
			}
			
			RowLayout
			{
				anchors.horizontalCenter: parent.horizontalCenter;
				ToolButton
				{
					text: "+";
				}
				
				ToolButton
				{
					text: "...";
				}
				
				ToolButton
				{
					text: "-";
				}
				
				ToolButton
				{
					text: "L";
				}
			}
			
			HeaderLabel
			{
				text: "Konkluzja";
			}
			
			RowLayout
			{
				Layout.maximumWidth: 200;
				
				Label
				{
					Layout.fillWidth: true;
					elide: Text.ElideRight;
				}
				
				ToolButton
				{
					text: "E";
				}
			}
			
			HeaderLabel
			{
				text: "Wynik";
			}
			
			Label
			{
				text: "Nie obliczono";
			}
			
			Button
			{
				implicitWidth: 200;
				text: "Rozłóż";
			}
		}
	}
}