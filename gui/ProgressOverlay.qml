import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Overlay
{
	property string status;
	
	id: progressOverlay;
	title: "Rozkładanie...";
	dialogHeight: 43;
	
	closeButton: ToolButton
	{
		iconSource: "img/close.svg";
		tooltip: "Anuluj";
		onClicked:
		{
			cancel();
			progressOverlay.visible = false;
		}
	}
	
	ProgressBar
	{
		id: progressBar;
		
		anchors.left: parent.left;
		anchors.right: parent.right;
		anchors.top: parent.top;
		indeterminate: true;
		
		height: 5;
	}
	
	signal cancel();
}