import QtQuick 2.2

Canvas
{
	property var points: [];  // Przechowuje punkty. Łączy liniami 1 i 2,  3 i 4 itd.
	
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