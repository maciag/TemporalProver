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
		onClicked:
		{
			var validate = false;  // TODO
			
			if(validate)
			{
				save(formulaArea.text);
				formulaOverlay.visible = false;
			}
			
			else
			{
				toast.visible = true;
				toast.text = "Walidacja do zrobienia!";
			}
		}
	}
	]
	
	TextArea
	{
		id: formulaArea;
		anchors.fill: parent;
	}
	
	Toast
	{
		id: toast;
		width: 250;
	}
	
	function getFormula()
	{
		return formulaArea.text();
	}
	
	function setText(text)
	{
		formulaArea.text = text;
	}
	
	signal save(string formula);
}