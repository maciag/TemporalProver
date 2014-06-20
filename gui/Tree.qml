import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
	id: tree;
	property TreeNode root;
	property TreeCanvas canvas;
	property int margin: 20;
	
	width: root == null ? 0 : scale * root.width;
	height: root == null ? 0 : scale * root.height;
	
	transformOrigin: Item.TopLeft;
	
	Component.onCompleted: updateTree();
	
	function updateTree()
	{
		if(root == null)
			return;
		
		// Utworzenie canvas 
		var component = Qt.createComponent("TreeCanvas.qml");
		canvas = component.createObject(this);
		
		// Aktualizacja wymiarów
		width = scale * root.width + 2*margin;
		height = scale * root.height + 2*margin;
		canvas.width = width / scale;
		canvas.height = height / scale;
		
		// Wyliczenie punktów
		canvas.points = [];
		var rootCircle = root.children[0];
		var coords = mapFromItem(rootCircle, rootCircle.x, rootCircle.y);
		
		addLines(root, (width-2*margin)/(2*scale), 10);
		
		root.x = margin;
		root.y = margin;
		
		// Ustawienie nadrzędnych
		root.parent = this;
	}
	
	// Dodaje linie łączące węzeł z jego dziećmi
	function addLines(node, parentX, parentY)
	{
		var childCont = node.children[1];
		
		for(var i = 0; i < childCont.children.length; i++)
		{
			var childCircle = childCont.children[i].children[0];
			var childCoords = mapFromItem(childCircle, childCircle.x, childCircle.y);
			
			// Z jakiegoś powodu w childCoords lądują liczby za małe o 7. Korekta.
			childCoords.x += 7;
			childCoords.y += 7;
			
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