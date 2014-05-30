import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Overlay
{
	property real progress;
	property string status;
	
	id: progressOverlay;
	title: "Liczę...";
	dialogHeight: 70;
	
	closeButton: ToolButton
	{
		iconSource: "img/close.svg";
		tooltip: "Anuluj";
		onClicked:
		{
			progressOverlay.visible = false;
		}
	}
	
	ProgressBar
	{
		id: progressBar;
		
		anchors.left: parent.left;
		anchors.right: parent.right;
		anchors.top: parent.top;
		value: progress;
		
		height: 5;
	}
	
	Rectangle
	{
		anchors.top: progressBar.bottom;
		anchors.bottom: parent.bottom;
		
		Label
		{
			anchors.verticalCenter: parent.verticalCenter;
			
			anchors.left: parent.left;
			anchors.right: parent.right;
			
			text: status;
		}
	}
}