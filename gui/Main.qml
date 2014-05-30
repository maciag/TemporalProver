import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

ApplicationWindow
{
	title: "Prover";
	width: 800;
	height: 600;
	visible: true;
	
	// Układ i podkomponeny
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
			tabPosition: Qt.BottomEdge;
			
			Tab
			{
				title: "Drzewo prawdy";
			}
			
			Tab
			{
				title: "Drzewo wyrażenia";
			}
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
					iconSource: "img/add.svg";
					tooltip: "Dodaj";
				}
				
				ToolButton
				{
					iconSource: "img/edit.svg";
					tooltip: "Edytuj";
				}
				
				ToolButton
				{
					iconSource: "img/delete.svg";
					tooltip: "Usuń";
				}
				
				ToolButton
				{
					iconSource: "img/load.svg";
					tooltip: "Wczytaj z pliku";
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
					iconSource: "img/edit.svg";
					tooltip: "Edytuj";
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
	
	// Overlay'e dialogowe
	FormulaOverlay {}
	ProgressOverlay {}
}