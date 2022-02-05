#include "../Header_Files/MazeGenerator.h"

Maze::Maze(sf::RenderWindow* window, Grid& grid)
	: m_win(window), m_grid(grid) {
	grid.m_mazeButtonPressed = false;
}

void Maze::DrawMaze() {
	for (int i = 0; i < m_grid.m_ROWS; i++) {
		for (int j = 0; j < m_grid.m_COLS; j++) {
			if (!m_grid.nodes[i][j]->isMazeWall) {
				m_grid.nodes[i][j]->rect.setFillColor(sf::Color::Black);
				m_grid.nodes[i][j]->isWall = true;
				m_win->draw(m_grid.nodes[i][j]->rect);
			}
		}
	}
}

bool Maze::isWall(int& x, int& y) {
	if (0 <= x && x < m_grid.m_ROWS && 0 <= y && y < m_grid.m_COLS) {
		if (m_grid.nodes[x][y]->isMazeWall) {
			return true;
		}
	}
	return false;
}

void Maze::RecursiveDivision(int x, int y) {
	//	std::cout << "Recursive Divions Maze" << std::endl;

	// Ensure the Maze width and Height are Odd Numbers 2x + 1 (Skipped for now)

	// Initalize the starting Point of the Maze Path 'Digger'
	m_grid.nodes[x][y]->isMazeWall = false;

	// 0: Left, 1: Right, 2: Up, 3:Down
	std::deque<std::pair<int, int>> directions = { {1,0},{-1,0},{0,1},{0,-1} };
	std::random_shuffle(directions.begin(), directions.end());

	while (directions.size() > 0) {
		// Grab a Random Direction and Pop it off
		std::pair<int, int> directionToTry = directions[0];
		directions.pop_front();

		// Next node Position
		int nextPosX = x + (directionToTry.first * 2);
		int nextPosY = y + (directionToTry.second * 2);

		if (isWall(nextPosX, nextPosY)) {
			int linkX = x + directionToTry.first;
			int linkY = y + directionToTry.second;
			m_grid.nodes[linkX][linkY]->isMazeWall = false;
			RecursiveDivision(nextPosX, nextPosY);
		}

	}

	// Sets the 'Create Maze' Button backed to false
	m_grid.m_mazeButtonPressed = false;
}

