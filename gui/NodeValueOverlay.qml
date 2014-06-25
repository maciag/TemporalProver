import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import CppBridge 1.0

Overlay
{
	id: nodeValueOverlay;
	title: "Podgląd węzła";
	
	dialogButtons: []
	
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