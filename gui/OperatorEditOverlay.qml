import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Overlay
{
	signal save();
	
	property string and: "&";
	property string or: "|";
	property string xor: "^";
	property string impl: ">";
	property string eq: "=";
	property string not: "!";
	property string tempU: "U";
	property string tempX: "X";
	property string tempG: "G";
	property string tempF: "F";
	
	id: operatorEditOverlay;
	title: "Operatory";
	dialogWidth: 300;
	dialogHeight: 350;
	
	dialogButtons: [
	ToolButton
	{
		iconSource: "img/load.svg";
		tooltip: "Wczytaj z pliku";
		onClicked:
		{
			// TODO Wczytanie operatorów z pliku
		}
	},
	
	ToolButton
	{
		iconSource: "img/confirm.svg";
		tooltip: "Zatwierdź";
		onClicked:
		{
			if(validateOperators())
			{
				toast.visible = false;
				operatorEditOverlay.visible = false;
				
				and = andField.text;
				or = orField.text;
				xor = xorField.text;
				impl = implField.text;
				eq = eqField.text;
				not = notField.text;
				tempU = tempUField.text;
				tempX = tempXField.text;
				tempG = tempGField.text;
				tempF = tempFField.text;
				
				save();
			}
		}
	}
	]
	
	GridLayout
	{
		columns: 2;
		anchors.fill: parent;
		
		Label { text: "Koniunkcja"; }
		OperatorField {id: andField; }
		
		Label { text: "Alternatywa"; }
		OperatorField {id: orField; }
		
		Label { text: "Alternatywa wykl."; }
		OperatorField {id: xorField; }
		
		Label { text: "Implikacja"; }
		OperatorField {id: implField; }
		
		Label { text: "Równoważnośc"; }
		OperatorField {id: eqField; }
		
		Label { text: "Negacja"; }
		OperatorField {id: notField; }
		
		Label { text: "Op. temporalny U"; }
		OperatorField {id: tempUField; }
		
		Label { text: "Op. temporalny X"; }
		OperatorField {id: tempXField; }
		
		Label { text: "Op. temporalny G"; }
		OperatorField {id: tempGField; }
		
		Label { text: "Op. temporalny F"; }
		OperatorField {id: tempFField; }
	}
	
	Toast
	{
		id: toast;
		width: 250;
	}
	
	function updateTextFields()
	{
		andField.text = and;
		orField.text = or;
		xorField.text = xor;
		implField.text = impl;
		eqField.text = eq;
		notField.text = not;
		tempUField.text = tempU;
		tempXField.text = tempX;
		tempFField.text = tempF;
		tempGField.text = tempG;
	}
	
	function validateOperators()
	{
		var operatorNames = ["koniunkcji", "alternatywy", "alternatywy wykl.", "implikacji", "równoważności", "negacji",
			"tempor. U", "tempor. X", "tempor. G", "tempor. F"];
		var operatorFields = [andField, orField, xorField, implField, eqField, notField, tempUField, tempXField, tempGField, tempFField];
		
		// Sprawdzanie, czy operator nie jest pusty
		for(var i = 0; i < 10; i++)
		{
			if(operatorFields[i].text == "")
			{
				toast.text = "Błąd: operator " + operatorNames[i] + " jest pusty.";
				toast.visible = true;
				return false;
			}
		}
		
		// Sprawdzanie wspólnych początków
		for(var i = 0; i < 10; i++)
		{
			for(var j = 0; j < 10; j++)
			{
				if(i == j)
					continue;
				
				if(operatorFields[i].text.indexOf(operatorFields[j].text) == 0)
				{
					toast.text = "Błąd: operator " + operatorNames[i] + " zaczyna się operatorem " + operatorNames[j] + ".";
					toast.visible = true;
					return false;
				}
			}
		}
		
		return true;
	}
}