// Pathfinder.cpp : Defines the entry point for the application.
//

#include "../Header_Files/Pathfinder.h"
#include "../Header_Files/MazeGenerator.h"
#include "../Header_Files/TextBox.h"
#include <SFML/Graphics.hpp>

size_t checkIfNewRowSize_isValid(size_t rows) {

	// Make Sure The the Row Size > 0
	if (rows == 0) {
		rows += 2;
	}
	// Make Sure The the Row Size > 1
	if (rows == 1) {
		rows += 1;
	}
	return rows;
}

size_t checkIfNewColSize_isValid(size_t cols) {

	// Make Sure The the Column Size > 0
	if (cols == 0) {
		cols += 2;
	}
	// Make Sure The the Column Size > 1
	if (cols == 1) {
		cols += 1;
	}
	return cols;
}


int main() {
	// Window is Created
	sf::RenderWindow window(sf::VideoMode(1000, 755), "Pathfinding Application");

	// Grid is Created with Default Size
	std::unique_ptr<Grid> grid = std::make_unique<Grid>(&window);

	// Maze Class is Instantiated
	std::unique_ptr<Maze> maze = std::make_unique<Maze>(&window, *grid);

	// Stores the Default Size of Rows and Cols
	size_t newRowSize = grid->returnRows();
	size_t newColsSize = grid->returnCols();

	// Instantiate the TextBox's with Row Size && Column Size
	std::unique_ptr<TextBox> RowText = std::make_unique<TextBox>(&window, 25, newRowSize, sf::Color::White, false);
	std::unique_ptr<TextBox> ColumnText = std::make_unique<TextBox>(&window, 25, newColsSize, sf::Color::White, false);

	// ----- Set Rows and Columns within the Window and the Length of Text to be inputted  -----
	RowText->SetPosition( { 785, 555 } );
	RowText->SetLimit(true, 2);
	ColumnText->SetPosition( { 845, 555 } );
	ColumnText->SetLimit(true, 2);
	// ------------------------------------------------------------------------------------------


	while (window.isOpen()) {

		// ---------- Check If windows was Closed and for Button Clicks -----------
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		
			if (event.type == sf::Event::MouseButtonPressed) {
				grid->mouseContains();
			}

			// Text Entered Event
			if (event.type == sf::Event::TextEntered) {
				RowText->TypedOn(event);
				ColumnText->TypedOn(event);
			}
		}
		//-------------------------------------------------------------------------

		// ------------ Clear the Entire Board i.e. start witha a fresh board of 25x25 Nodes -----------
		if (grid->returnClrScreenButtonValue()) {

			// Delete All Pointers
			grid.reset();
			maze.reset();
			RowText.reset();
			ColumnText.reset();

			// Re-Assign All nullptrs to a new Pointer
			grid = std::make_unique<Grid>(&window, newRowSize, newColsSize);
			maze = std::make_unique<Maze>(&window, *grid);
			RowText = std::make_unique<TextBox>(&window, 25, grid->returnRows(), sf::Color::White, false);
			ColumnText = std::make_unique<TextBox>(&window, 25, grid->returnCols(), sf::Color::White, false);

			// Do some Setting up Again
			RowText->SetPosition( { 785, 555 } );
			RowText->SetLimit(true, 2);
			ColumnText->SetPosition( { 845, 555 } );
			ColumnText->SetLimit(true, 2);
		}
		//---------------------------------------------------------------------------------------------

		// ----------- If Start Button is Pressed ---------------
		if (grid->returnStartValue()) {
			// If A* Algorithm was selected
			if (grid->returnAStarValue()) {
				if (grid->A_Star_Algorithm()) {
					grid->DrawAlgorithmResults();
				}
			}
			// If Dijkstras Algorithm was selected
			else if (grid->returnDijkstraValue()) {
				if (grid->DijkstrasAlgorithm()) {
					grid->DrawAlgorithmResults();
				}
			}
			// If Greedy Algorithm was selected
			else if (grid->returnBreadthFirstSearch()) {
				if (grid->BreadthFirstSearch()) {
					grid->DrawAlgorithmResults();
				}
			}
			// If No Algorithm was selected, choose A* as Defualt
			else {
				grid->setAStarValue(true);
			}
			// --------------------------------------------------------
		}
		// Generate Maze, If the button was pressed
		else if (grid->returnMazeButtonValue()) {
			grid->resetBoard();
			maze->RecursiveDivision(1, 1); // (0, 16)
			maze->DrawMaze();
		}
		else if (grid->returnButtonRowValue()) {

			// Delete All Characters to Type in new Char
			RowText->DeleteAllChars();
			// Set RowText To be Recieve Keyboard Input
			RowText->SetSelected(true);
			// Set ColText To not Recieve Keyboard Input
			ColumnText->SetSelected(false);
			// Set the Button Value to False to stop entering if Statement
			grid->setRowButtonValue(false);
		}
		else if (grid->returnButtonColValue()) {

			// Delete All Characters to Type in new Char
			ColumnText->DeleteAllChars();
			// Set ColText To be Recieve Keyboard Input
			ColumnText->SetSelected(true);
			// Set RowText To not Recieve Keyboard Input
			RowText->SetSelected(false);
			// set the button Value to stop Entering if statement
			grid->setColButtonValue(false);
		}
		else if (grid->returnButtonGridValue()) {
			// before resetting, grab the new Row and Column Values
			newRowSize = std::stoi(RowText->returnText());
			newColsSize = std::stoi(ColumnText->returnText());
			
			// Check if the new Row and Col Size > 1
			newRowSize = checkIfNewRowSize_isValid(newRowSize);
			newColsSize = checkIfNewColSize_isValid(newColsSize);

			grid.reset();
			maze.reset();
			RowText.reset();
			ColumnText.reset();

			// Re-Assign All nullptrs to a new Pointer
			grid = std::make_unique<Grid>(&window, newRowSize, newColsSize);
			maze = std::make_unique<Maze>(&window, *grid);
			RowText = std::make_unique<TextBox>(&window, 25, newRowSize, sf::Color::White, false);
			ColumnText = std::make_unique<TextBox>(&window, 25, newColsSize, sf::Color::White, false);

			// Do some Setting up Again
			RowText->SetPosition( { 785, 555 } );
			RowText->SetLimit(true, 2);
			ColumnText->SetPosition( { 845, 555 } );
			ColumnText->SetLimit(true, 2);
			
			// Set The Value to Stop Entering this if Statement
			grid->setGridSizeButtonValue(false);
		}

		window.clear();
		grid->Display();
		RowText->DrawText();
		ColumnText->DrawText();
		window.display();
	}

	return 0;
}