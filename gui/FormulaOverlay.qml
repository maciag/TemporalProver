import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import CppBridge 1.0

Overlay
{
	signal save(string formula);
	
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
			var formula = formulaArea.getText(0, formulaArea.text.length);
			var validate = cppBridge.validate(formula);
			
			if(validate)
			{
				save(formula);
				toast.visible = false;
				formulaOverlay.visible = false;
			}
			
			else
			{
				toast.visible = true;
				toast.text = getErrorMessage(cppBridge.getErrorCode(), cppBridge.getErrorToken(), cppBridge.getErrorPosition());
			}
		}
	}
	]
	
	TextArea
	{
		id: formulaArea;
		anchors.fill: parent;
		textFormat: TextEdit.RichText;
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
	
	function setFormula(text)
	{
		formulaArea.text = text;
	}
	
	function focus()
	{
		formulaArea.focus = true;
	}
	
	function selectAll()
	{
		formulaArea.selectAll();
	}
	
	function getErrorMessage(code, token, position)
	{
		var pos = formulaArea.cursorPosition;
		
		if(position < formulaArea.text.length)
		{
			var formula = formulaArea.getText(0, formulaArea.text.length);
			var text = formula.substring(0, position);
			text += "<b><u><font color='red'>";
			text += formula.substring(position, position+1);
			text += "</font></u></b>";
			text += formula.substring(position+1, formula.length);
			
			formulaArea.text = text;
		}
		
		formulaArea.cursorPosition = pos;
		
		switch(code)
		{
			case ErrorCode.UnexpectedEnd:
			{
				return position + ": nieoczekiwany koniec formuły";
			}
			case ErrorCode.UnexpectedVar:
			{
				return position + ": nieoczekiwana zmienna \"" + token + "\"";
			}
			case ErrorCode.UnexpectedBinOper:
			{
				return position + ": nieoczekiwany operator binarny \"" + token + "\"";
			}
			case ErrorCode.UnexpectedUnOper:
			{
				return position + ": nieoczekiwana operator unarny \"" + token + "\"";
			}
			case ErrorCode.UnexpectedLeftBracket:
			{
				return position + ": nieoczekiwany nawias otwierający";
			}
			case ErrorCode.UnexpectedRightBracket:
			{
				return position + ": nieoczekiwany nawias zamykający";
			}
			case ErrorCode.InvalidToken:
			{
				return position + ": nieznany token \"" + token + "\"";
			}
			case ErrorCode.BracketUnclosed:
			{
				return position + ": brakuje nawiasu zamykającego";
			}
			case ErrorCode.SpareRightBracket:
			{
				return position + ": zbędny nawias zamykający";
			}
		}
	}
}