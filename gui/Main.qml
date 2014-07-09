import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import QtQuick.Dialogs 1.0

import CppBridge 1.0

ApplicationWindow
{
	id: main;
	
	title: "Prover";
	width: 800;
	height: 600;
	visible: true;
	
	property string ioMode;
	property TextArea ioTargetComponent;
	
	property FileDialog openDialog: FileDialog
	{
		id: loadDialog;
		title: "Wczytaj formułę";
		selectExisting: true;
		selectMultiple: false;
		nameFilters: [ "Pliki tekstowe (*.txt)" ];
		
		onAccepted:
		{
			if(ioMode == "formula")
			{
				ioTargetComponent.text = cppBridge.loadFile(fileUrl);
			}
			
/*			else if(ioMode == "data")
			{
				var lines = cppBridge.loadFile(fileUrl).split("\n");
				var mode = "";
				
				conclusionLabel.text = "0";
				predList.model.clear();
				
				for(var i = 0; i < lines.length; i++)
				{
					if(lines[i] == "")
						continue;
					
					if(lines[i] == "[PRED]")
						mode = "pred";
					
					else if(lines[i] == "[CONC]")
						mode = "conc";
					
					else if(mode == "pred")
					{
						predList.model.append({value: lines[i]});
					}
					
					else if(mode == "conc")
					{
						conclusionLabel.text = lines[i];
						return;  // Wczytuj tylko pierwszą konkluzję
					}
				}
				
				generateExpressionTree("");
			}
*/		}
	}
	
	property FileDialog saveDialog: FileDialog
	{
		id: saveDialog;
		title: "Zapisz formułę";
		selectExisting: false;
		selectMultiple: false;
		nameFilters: [ "Pliki tekstowe (*.txt)" ];
		
		onAccepted:
		{
			cppBridge.saveFile(fileUrl, ioTargetComponent.text);
		}
	}
	
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
			
/*			HeaderLabel
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
					
					if(currentRow >= 0)
						generateExpressionTree(model.get(currentRow).value);
					
					else
						generateExpressionTree("");
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
					
					onClicked:
					{
						ioMode = "data";
						loadDialog.visible = true;
					}
				}
			}
*/			
			HeaderLabel
			{
				text: "Formuła";
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
//							predList.selection.clear();
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
				id: resultLabel;
				text: "Nie obliczono";
			}
			
			Button
			{
				implicitWidth: 200;
				text: "Rozłóż";
				
				onClicked:
				{
/*					var preds = [];
					
					for(var i = 0; i < predList.rowCount; i++)
					{
						preds.push(predList.model.get(i).value);
					}
					
*/					progressOverlay.visible = true;
					cppBridge.startComputation(conclusionLabel.text);
				}
			}
			
			RowLayout
			{
				anchors.horizontalCenter: parent.horizontalCenter;
				
				ToolButton
				{
					iconSource: "img/zoomIn.svg";
					tooltip: "Powiększ";
					
					onClicked:
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
					
					onClicked:
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
	
	ProgressOverlay
	{
		id: progressOverlay;
		onCancel: cppBridge.abortComputation();
	}
	
	NodeValueOverlay { id: nodeValueOverlay; }
	
	OperatorEditOverlay
	{
		id: operatorEditOverlay;
		onSave: updateOperators();
	}
	
	function handleResults(resTree, result)
	{
		// Ustawiamy etykietę z wynikiem
		resultLabel.text = (result ? "spełnialne" : "niespełnialne");
		
		// Generujemy drzewo prawdy
		var tree = truthTreeTab.children[0].contentItem;
		
		if(tree.canvas != null)
		{
			tree.canvas.update();
			tree.canvas.destroy();
			tree.canvas = null;
		}
		
		if(resTree != "")
			tree.setResult(resTree);
		else
			tree.setResult("");
		
		if(tree.canvas != null)
			tree.canvas.nodeClicked.connect(nodePreview);
		
		// Ukrywamy nakładkę postępu i zmieniamy zakładkę
		progressOverlay.visible = false;
		tabs.currentIndex = 0;
	}
	
	/*	// Funkcje dotyczące predykatów
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
*/
	function editConc(formula)
	{
		conclusionLabel.text = formula;
		
//		predList.selection.clear();
		resultLabel.text = "Nie obliczono";
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
		cppBridge.setParserOperator(operatorEditOverlay.not, 5, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempU, 4, false);
		cppBridge.setParserOperator(operatorEditOverlay.tempX, 5, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempG, 5, true);
		cppBridge.setParserOperator(operatorEditOverlay.tempF, 5, true);
		
		cppBridge.setTruthTreeOperator("and", operatorEditOverlay.and);
		cppBridge.setTruthTreeOperator("or", operatorEditOverlay.or);
		cppBridge.setTruthTreeOperator("xor", operatorEditOverlay.xor);
		cppBridge.setTruthTreeOperator("impl", operatorEditOverlay.impl);
		cppBridge.setTruthTreeOperator("eq", operatorEditOverlay.eq);
		cppBridge.setTruthTreeOperator("not", operatorEditOverlay.not);
		cppBridge.setTruthTreeOperator("tempU", operatorEditOverlay.tempU);
		cppBridge.setTruthTreeOperator("tempX", operatorEditOverlay.tempX);
		cppBridge.setTruthTreeOperator("tempG", operatorEditOverlay.tempG);
		cppBridge.setTruthTreeOperator("tempF", operatorEditOverlay.tempF);
	}
}