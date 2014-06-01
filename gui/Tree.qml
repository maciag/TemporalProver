import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
	property TreeNode root;
	
	width: scale * root.width;
	height: scale * root.height;
	transformOrigin: Item.TopLeft;
	
	Component.onCompleted:
	{
		root.parent = this;
	}
}