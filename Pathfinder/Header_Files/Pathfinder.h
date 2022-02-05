// Pathfinder.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Button.h"
#include<iostream>
#include <chrono>         // std::chrono::seconds
#include<SFML/Graphics.hpp>
#include <deque>
#include <list>
#include <algorithm> // std::sort Method

const int WALLTHICKNESS = 5;
const int OFFSET = 5;

struct Node
{
	Node() {}
	//	                Top  Left  Right Bottom
	bool isWalkable = true;
	bool isWall = false;
	bool visited = false;
	bool grabbed = false;
	// Maze Generator
	bool isMazeWall = true;

	double globalfGoal = 0; // Cost of the route to goal
	double localfGoal = 0; // distance too the goal if we took an alternative route
	int x = NULL;
	int y = NULL;
	sf::RectangleShape rect;
	std::deque<Node*> NeighborNodes;
	Node* parent = nullptr;

};


class Grid
{
public:
	Grid(sf::RenderWindow*);
	Grid(sf::RenderWindow*, int, int);
	void Display();
	void DrawNodesAndButtons();
	void DrawAlgorithmResults();
	void SetupButtons();
	void mouseContains();
	void CheckButtonPress();
	void setButtonBooleans(bool, bool, bool);
	void setStart() { m_startNode = nodes[1][1]; }
	void setEnd() { m_endNode = nodes[m_ROWS-2][m_COLS-2]; }
	void placeStart();
	void placeEnd();
	void Pick_And_Place();
	float Heuristic(Node*, Node*);
	bool GrabNeighboringNodes();
	void ShowNeighborNodeConnections(Node*);
	void AnimateTheSearch(float);
	void constructPath();
	void DrawPath(float);
	void tracePath();
	void generateRandoCommandoMaze();
	void resetBoard();

	bool A_Star_Algorithm();
	bool DijkstrasAlgorithm();
	bool BreadthFirstSearch();

	void setAStarValue(bool value) { m_aStar = value; }
	void setRowButtonValue(bool value) { setRow = value; }
	void setColButtonValue(bool value) { setCol = value; }
	void setGridSizeButtonValue(bool value) { setGridSize = value; }

	bool returnClrScreenButtonValue() { return m_clrScreen; }
	bool returnStartValue() { return m_startAlgorithm; }
	bool returnAStarValue() { return m_aStar; }
	bool returnDijkstraValue() { return m_dijkstra; }
	bool returnBreadthFirstSearch() { return m_breadthFirstSearch; }

	bool returnMazeButtonValue() { return m_mazeButtonPressed; }
	bool returnButtonRowValue() { return setRow; }
	bool returnButtonColValue() { return setCol; }
	bool returnButtonGridValue() { return setGridSize; }
	size_t returnRows() { return m_ROWS; }
	size_t returnCols() { return m_COLS; }

	void setRowSize(size_t newRowSize) { m_ROWS = newRowSize; }
	void setColSize(size_t newColSize) { m_COLS = newColSize; }


private:
	friend class Maze;
	bool aStartComplete = false;
	bool m_startAlgorithm = false;
	bool m_clrScreen = false;
	bool m_aStar = false;
	bool m_dijkstra = false;
	bool m_breadthFirstSearch = false;
	bool m_complete = false;
	bool m_trace = false;
	bool m_mazeButtonPressed = false;

	size_t m_ROWS;
	size_t m_COLS;
	size_t m_BlockXSize;
	size_t m_BlockYSize;

	std::deque<std::deque<Node*>> nodes;
	Node* m_startNode = nullptr;
	Node* m_endNode = nullptr;
	std::deque<Node*> m_OpenList;
	std::deque<Node*> m_ClosedList;
	std::deque<Node*> m_path;
	std::deque<Button*> m_buttons;
	std::vector<sf::Vertex> tracedLines;
	sf::RectangleShape mouseNode;
	sf::RectangleShape nodesZone;
	sf::RenderWindow* m_win;
	sf::Clock m_clock;

	//TextBox Booleans
	bool setRow = false;
	bool setCol = false;
	bool setGridSize = false;
};

#endif
