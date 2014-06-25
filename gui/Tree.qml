import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
	id: tree;
	property TreeCanvas canvas;
	property int margin: 20;
	
	property int contentWidth: 0;
	property int contentHeight: 0;
	
	transformOrigin: Item.TopLeft;
	
	Component.onCompleted: setPrefix();
	
	QtObject
	{
		id: treeGen;
		property int index;
		property int depth;
		property int x;
		property int y;
		property variant tokens;
	}
	
	function setPrefix(prefix)
	{
		if(prefix == undefined)
			return;
		
		// Utworzenie canvas 
		var component = Qt.createComponent("TreeCanvas.qml");
		canvas = component.createObject(this);
		
		// Wygenerowanie drzewa
		treeGen.index = 0;
		treeGen.depth = 0;
		treeGen.x = 0;
		treeGen.y = 0;
		treeGen.tokens = prefix.split(" ");
		
		computeToken();
		
		width = /*scale * */contentWidth;
		height = /*scale * */contentHeight;
		
		canvas.width = contentWidth;
		canvas.height = contentHeight;
	}
	
	function computeToken()
	{
		var nodeToken = treeGen.tokens[treeGen.index];
		var argCount = cppBridge.getOperatorArgCount(nodeToken);
		
		var nodeX = treeGen.x;
		
		// Zapis infiksowy
		var prefix = nodeToken + " ";
		var argBalance = cppBridge.getOperatorArgCount(nodeToken);
		var i = treeGen.index+1;
		
		while(argBalance > 0)
		{
			prefix += treeGen.tokens[i] + " ";
			argBalance += cppBridge.getOperatorArgCount(treeGen.tokens[i]) - 1;
			i++;
		}
		
		var infix = cppBridge.toInfix(prefix);
		
		// Dodanie węzła i podwęzłów
		if(argCount == 0)
		{
			treeGen.index++;
			
			nodeX = treeGen.x;
			canvas.nodes.push({x: nodeX, y: treeGen.y, token: nodeToken, value: infix});
		}
		
		else if(argCount == 1)
		{
			treeGen.index++;
			
			treeGen.y++;
			nodeX = computeToken();
			treeGen.y--;
			canvas.nodes.push({x: nodeX, y: treeGen.y, token: nodeToken, value: infix});
			
			// Linia
			canvas.linePoints.push(Qt.point(nodeX, treeGen.y));
			canvas.linePoints.push(Qt.point(nodeX, treeGen.y+1));
		}
		
		else if(argCount == 2)
		{
			treeGen.index++;
			
			treeGen.y++;
			var lchildX = computeToken();
			treeGen.y--;
			treeGen.x++;
			
			nodeX = treeGen.x;
			canvas.nodes.push({x: nodeX, y: treeGen.y, token: nodeToken, value: infix});
			treeGen.x++;
			
			treeGen.y++;
			var rchildX = computeToken();
			treeGen.y--;
			
			// Linie
			canvas.linePoints.push(Qt.point(nodeX, treeGen.y));
			canvas.linePoints.push(Qt.point(lchildX, treeGen.y+1));
			canvas.linePoints.push(Qt.point(nodeX, treeGen.y));
			canvas.linePoints.push(Qt.point(rchildX, treeGen.y+1));
			
		}
		
		// Aktualizacja rozmiarów
		var requiredWidth = 2*margin + 20 * (nodeX+1);
		var requiredHeight = 2*margin + 20 + (40*treeGen.y);
		
		contentWidth = contentWidth < requiredWidth ? requiredWidth : contentWidth;
		contentHeight = contentHeight < requiredHeight ? requiredHeight : contentHeight;
		
		return nodeX;
	}
}