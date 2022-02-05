#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Pathfinder.h"

class Maze {
public:
	Maze(sf::RenderWindow*, Grid&);
	bool isWall(int&, int&);
	void DrawMaze();
	void RecursiveDivision(int, int);

private:
	sf::RenderWindow* m_win;
	Grid& m_grid;
};

#endif