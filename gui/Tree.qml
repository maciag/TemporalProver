import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
	id: tree;
	property TreeNode root;
	property int margin: 20;
	
	width: root == null ? 0 : scale * root.width;
	height: root == null ? 0 : scale * root.height;
	
	transformOrigin: Item.TopLeft;
	
	Canvas
	{
		property var points;  // Przechowuje punkty. Łączy liniami 1 i 2,  3 i 4 itd.
		
		anchors.top: parent.top;
		anchors.left: parent.left;
		
		width: parent.width / parent.scale;
		height: parent.height / parent.scale;
		renderStrategy: Canvas.Threaded;
		
		id: canvas;
		
		onPaint:
		{
			var context = getContext("2d");
			var lineWidth = 2/parent.scale < 1;
			context.lineWidth = (lineWidth) ? 1 : Math.round(lineWidth);
			context.strokeStyle = "black";
			
			for(var i = 0; i < points.length; i += 2)
			{
				context.moveTo(margin+points[i].x, margin+points[i].y);
				context.lineTo(margin+points[i+1].x, margin+points[i+1].y);
				context.stroke();
			}
		}
	}
	
	Component.onCompleted:
	{
		if(root == null)
			return;
		
		root.parent = this;
		canvas.points = [];
		
		// Wyliczenie punktów
		var rootCircle = root.children[0];
		var coords = mapFromItem(rootCircle, rootCircle.x, rootCircle.y);
		
		addLines(root, width/(2*scale), 10);
		
		width = width+2*margin;
		height = height+2*margin;
		
		root.x = margin;
		root.y = margin;
	}
	
	// Dodaje linie łączące węzeł z jego dziećmi
	function addLines(node, parentX, parentY)
	{
		var childCont = node.children[1];
		
		for(var i = 0; i < childCont.children.length; i++)
		{
			var childCircle = childCont.children[i].children[0];
			var childCoords = mapFromItem(childCircle, childCircle.x, childCircle.y);
			
			// Przeliczenie wpółrzędnych dziecka
			var x = Math.round(childCoords.x - (childCont.children[i].width-20)/2 + 10);
			var y = Math.round(childCoords.y + 10);
			
			// Dodanie punktów dla kreski
			canvas.points.push(Qt.point(x, y));
			canvas.points.push(Qt.point(parentX, parentY));
			
			addLines(childCont.children[i], x, y);
		}
	}
}