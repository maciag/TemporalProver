import QtQuick 2.2
import QtQuick.Controls 1.1

Canvas
{
	property var nodes: [];  // Węzły
	property var linePoints: [];  // Pozycje x i y węzłów - łączenia 1 i 2, 3 i 4 itd.
	property string type;
	
	signal nodeClicked(string token, string value);
	
	anchors.top: parent.top;
	anchors.left: parent.left;
	transformOrigin: Item.TopLeft;
	
	renderStrategy: Canvas.Threaded;
	renderTarget: Canvas.FramebufferObject;
	
	id: canvas;
	
	MouseArea
	{
		anchors.fill: parent;
		acceptedButtons: Qt.LeftButton;
		
		onClicked:
		{
			
			if(mouse.button == Qt.LeftButton)
			{
				var x = Math.floor((mouse.x - margin) / 20);
				var y = Math.floor(((mouse.y - margin) / 40) - ((((mouse.y - margin) % 40) > 20) ? 1000000 : 0));  // Między wierszami skieruj na brak węzła
				
				for(var i = 0; i < nodes.length; i++)
				{
					if(nodes[i].x == x && nodes[i].y == y)
					{
						var value = "";
						if(type == "expression")
						{
							value = cppBridge.toInfix(nodes[i].value);
						}
						else if(type == "result")
						{
							value = formulaGroupToInfix(nodes[i].value);
						}
						
						nodeClicked(nodes[i].token, value);
					}
				}
			}
		}
	}
	
	onPaint:
	{
		visible = false;
		
		var context = getContext("2d");
		
		context.strokeStyle = "black";
		context.fillStyle = "white";
		context.fillRect(0, 0, width, height);
		
		context.font = "bold 14px sans-serif";
		context.textAlign = "center";
		context.textBaseline = "middle";
		
		// Linie
		context.lineWidth = 2;
		context.strokeStyle = "black";
		
		for(var i = 0; i < linePoints.length; i += 2)
		{
			context.moveTo(margin+20*linePoints[i].x+10, margin+40*linePoints[i].y+10);
			context.lineTo(margin+20*linePoints[i+1].x+10, margin+40*linePoints[i+1].y+10);
			context.stroke();
		}
		
		// Węzły
		context.fillStyle = "black";
		for(var i = 0; i < nodes.length; i++)
		{
			context.ellipse(margin+20*nodes[i].x, margin+40*nodes[i].y, 20, 20);
			context.fill();
			context.stroke();
			
		}
		
		// Tokeny węzłów
		context.fillStyle = "white";
		for(var i = 0; i < nodes.length; i++)
		{
			context.fillText(nodes[i].token, margin+20*nodes[i].x+10, margin+40*nodes[i].y+12);
		}
	}
	
	onPainted:
	{
		progressOverlay.visible = false;
		visible = true;
	}
	
	function formulaGroupToInfix(formulaGroup)
	{
		var formulas = formulaGroup.split(";");
		var res = "";
		
		for(var i = 0; i < formulas.length; i++)
		{
			res += (i%2 ? "<font color='#008000'>" : "<font color='#000080'>");
			
			var formula = cppBridge.toInfix(formulas[i]);
			formula = formula.split("<").join("&lt;");
			formula = formula.split(">").join("&gt;");
			
			res += formula;
			res += "</font>";
			
			if(i != formulas.length-1)
				res += "<br/>";
		}
		
		return res;
	}
}