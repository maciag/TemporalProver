import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import CppBridge 1.0

Overlay
{
	id: nodeValueOverlay;
	title: "Podgląd węzła";
	
	dialogButtons: [
	ToolButton
	{
		iconSource: "img/save.svg";
		tooltip: "Zapisz do pliku";
		onClicked:
		{
			main.ioMode = "formula";
			main.ioTargetComponent = formulaArea;
			saveDialog.visible = true;
		}
	}
	]
	
	TextArea
	{
		id: formulaArea;
		readOnly: true;
		anchors.fill: parent;
	}
	
	function getFormula()
	{
		return formulaArea.text();
	}
	
	function setFormula(text)
	{
		formulaArea.text = text;
	}
}