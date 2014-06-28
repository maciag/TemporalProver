import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1	

ApplicationWindow
{
	title: "Prover";
	width: 800;
	height: 600;
	visible: true;
	
	// Układ i podkomponeny
	RowLayout
	{
		anchors.fill: parent;
		anchors.margins: spacing;
		
		TabView
		{
			id: tabs;
			
			Layout.fillWidth: true;
			Layout.fillHeight: true;
			Layout.minimumWidth: 200;
			Layout.minimumHeight: 200;
			tabPosition: Qt.BottomEdge;
			
			Tab
			{
				title: "Drzewo prawdy";
				id: truthTreeTab;
				
				ScrollView
				{
					Tree {}
				}
			}
			
			Tab
			{
				title: "Drzewo wyrażenia";
				id: expressionTreeTab;
				
				ScrollView
				{
					Tree {}
				}
			}
		}
		
		ColumnLayout
		{
			Layout.alignment: Qt.AlignTop | Qt.AlignLeft;
			
			HeaderLabel
			{
				text: "Predykaty";
			}
			
			TableView
			{
				id: predList;
				headerVisible: false;
				alternatingRowColors: false;
				selectionMode: SelectionMode.SingleSelection;
				Layout.minimumHeight: 100;
				model: ListModel {}
				
				TableViewColumn
				{
					role: "value";
					title: "";
				}
				
				onClicked:
				{
					tabs.currentIndex = 1;
					generateExpressionTree(model.get(currentRow).value);
				}
			}
			
			RowLayout
			{
				anchors.horizontalCenter: parent.horizontalCenter;
				ToolButton
				{
					iconSource: "img/add.svg";
					tooltip: "Dodaj";
					
					onClicked:
					{
						formulaOverlay.setFormula("");
						formulaOverlay.visible = true;
						formulaOverlay.save.connect(appendPred);
						
						formulaOverlay.focus();
						formulaOverlay.selectAll();
					}
				}
				
				ToolButton
				{
					iconSource: "img/edit.svg";
					tooltip: "Edytuj";
					
					onClicked:
					{
						if(predList.currentRow >= 0)
						{
							formulaOverlay.setFormula(predList.model.get(predList.currentRow).value);
							formulaOverlay.visible = true;
							formulaOverlay.save.connect(updatePred);
							
							formulaOverlay.focus();
							formulaOverlay.selectAll();
						}
					}
				}
				
				ToolButton
				{
					iconSource: "img/delete.svg";
					tooltip: "Usuń";
					
					onClicked:
					{
						var nextIndex = -1;
						if(predList.rowCount > predList.currentRow+1)
							nextIndex = predList.currentRow;
						else if(predList.currentRow > 0)
							nextIndex = predList.currentRow-1;
						
						if(predList.currentRow >= 0)
							predList.model.remove(predList.currentRow);
						
						predList.selection.clear();
						if(nextIndex >= 0)
						{
							predList.selection.select(nextIndex);
							generateExpressionTree(predList.model.get(nextIndex).value);
						}
						else
							generateExpressionTree("");
					}
				}
				
				ToolButton
				{
					iconSource: "img/load.svg";
					tooltip: "Wczytaj z pliku";
				}
			}
			
			HeaderLabel
			{
				text: "Konkluzja";
			}
			
			RowLayout
			{
				Layout.maximumWidth: 200;
				
				Label
				{
					id: conclusionLabel;
					text: "0";  // Niech konkluzja nigdy nie będzie pusta
					Layout.fillWidth: true;
					elide: Text.ElideRight;
					
					MouseArea
					{
						anchors.fill: parent;
						onClicked:
						{
							predList.selection.clear();
							generateExpressionTree(conclusionLabel.text);
							tabs.currentIndex = 1;
						}
					}
				}
				
				ToolButton
				{
					iconSource: "img/edit.svg";
					tooltip: "Edytuj";
					
					onClicked:
					{
						formulaOverlay.setFormula(conclusionLabel.text);
						formulaOverlay.visible = true;
						formulaOverlay.save.connect(editConc);
						
						formulaOverlay.focus();
						formulaOverlay.selectAll();
					}
				}
			}
			
			Button
			{
				implicitWidth: 200;
				text: "Operatory...";
				
				onClicked:
				{
					operatorEditOverlay.updateTextFields();
					operatorEditOverlay.visible = true;
				}
			}
			
			HeaderLabel
			{
				text: "Wynik";
			}
			
			Label
			{
				text: "Nie obliczono";
			}
			
			Button
			{
				implicitWidth: 200;
				text: "Rozłóż";
			}
			
			RowLayout
			{
				anchors.horizontalCenter: parent.horizontalCenter;
				
				ToolButton
				{
					iconSource: "img/zoomIn.svg";
					tooltip: "Powiększ";
					
					onClicked:  // TODO: do poprawy!
					{
						var currentTree = tabs.getTab(tabs.currentIndex).children[0].contentItem;
						
						if(currentTree.canvas != null)
						{
							var newScale = currentTree.canvas.scale + 0.1;
							currentTree.canvas.scale = newScale > 1 ? 1 : newScale;
							
							currentTree.width = currentTree.canvas.width * currentTree.canvas.scale;
							currentTree.height = currentTree.canvas.height * currentTree.canvas.scale;
						}
					}
				}
				
				ToolButton
				{
					iconSource: "img/zoomOut.svg";
					tooltip: "Pomniejsz";
					
					onClicked: // TODO: do poprawy!
					{
						var currentTree = tabs.getTab(tabs.currentIndex).children[0].contentItem;
						
						if(currentTree.canvas != null)
						{
							var newScale = currentTree.canvas.scale - 0.1;
							currentTree.canvas.scale = newScale < 0.1 ? 0.1 : newScale;
							
							currentTree.width = currentTree.canvas.width * currentTree.canvas.scale;
							currentTree.height = currentTree.canvas.height * currentTree.canvas.scale;
						}
					}
				}
			}
		}
	}
	
	// Przy onCompleted przełączamy zakładki, żeby oba scrolle były nie-undefined
	Component.onCompleted:
	{
		tabs.currentIndex = 1;
		tabs.currentIndex = 0;
	}
	
	// Overlay'e dialogowe
	FormulaOverlay { id: formulaOverlay; }
	ProgressOverlay { id: progressOverlay; }
	NodeValueOverlay { id: nodeValueOverlay; }
	OperatorEditOverlay
	{
		id: operatorEditOverlay;
		onSave: updateOperators();
	}
	
	// Funkcje dotyczące predykatów
	function appendPred(formula)
	{
		predList.model.append({value: formula});
		
		predList.selection.clear();
		predList.selection.select(predList.rowCount-1);
		generateExpressionTree(formula);
		tabs.currentIndex = 1;
		
		formulaOverlay.save.disconnect(appendPred);  // Odłączamy od razu sygnał
	}
	
	function updatePred(formula)
	{
		predList.model.set(predList.currentRow, {value: formula});
		
		generateExpressionTree(formula);
		tabs.currentIndex = 1;
		
		formulaOverlay.save.disconnect(updatePred);  // Odłączamy od razu sygnał
	}
	
	function editConc(formula)
	{
		conclusionLabel.text = formula;
		
		predList.selection.clear();
		generateExpressionTree(formula);
		tabs.currentIndex = 1;
		
		formulaOverlay.save.disconnect(editConc);
	}
	
	function generateExpressionTree(formula)
	{
		var tree = expressionTreeTab.children[0].contentItem;
		
		if(tree.canvas != null)
		{
			tree.canvas.update();
			tree.canvas.destroy();
			tree.canvas = null;
		}
		
		if(formula != "")
			tree.setPrefix(cppBridge.toPrefix(formula));
		else
			tree.setPrefix("");
		
		if(tree.canvas != null)
			tree.canvas.nodeClicked.connect(nodePreview);
	}
	
	// Podgląd węzła drzewa
	function nodePreview(token, value)
	{
		nodeValueOverlay.setFormula(value);
		nodeValueOverlay.visible = true;
	}
	
	// Aktualizacja operatorów
	function updateOperators()
	{
		cppBridge.clearParserOperators();
		
		cppBridge.setParserOperator(operatorEditOverlay.and, 3, false);
		cppBridge.setParserOperator(operatorEditOverlay.or, 3, false);
		cppBridge.setParserOperator(operatorEditOverlay.xor, 3, false);
		cppBridge.setParserOperator(operatorEditOverlay.impl, 2, false);
		cppBridge.setParserOperator(operatorEditOverlay.eq, 1, false);
		cppBridge.setParserOperator(operatorEditOverlay.not, 6, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempU, 4, false);
		cppBridge.setParserOperator(operatorEditOverlay.tempX, 5, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempG, 5, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempF, 5, true);
	}
}