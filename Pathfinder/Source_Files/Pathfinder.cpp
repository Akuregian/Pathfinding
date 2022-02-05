#include "../Header_Files/Pathfinder.h"

const sf::Color BGNODECOL = sf::Color(119, 136, 153);
const sf::Color SEARCHNODECOL = sf::Color(70, 130, 180);
const sf::Color OUTERSEARCHNODECOL = sf::Color(139, 0, 140);
const sf::Color PATHNODECOL = sf::Color(255, 215, 0);
const sf::Color STARTNODECOL = sf::Color(72, 61, 139);
const sf::Color MOUSESTARTNODECOL = sf::Color(72, 61, 139, 100);
const sf::Color ENDNODECOL = sf::Color(255, 140, 0);
const sf::Color MOUSEENDNODECOL = sf::Color(255, 140, 0, 100);
const sf::Color LINECOL = sf::Color(0, 139, 139);

// Helper Function to Sorta Deque based of LOWEST globalScore FIRST
bool CompareGlobalScores(Node* lhs, Node* rhs){
	return lhs->globalfGoal < rhs->globalfGoal;
}

// Helper Function to Sorta Deque based of LOWEST LocalScore FIRST
bool CompareLocalScores(Node* lhs, Node* rhs){
	return lhs->localfGoal < rhs->localfGoal;
}

Grid::Grid(sf::RenderWindow* window)
	: m_win(window) {

	// Grab our Alotted Zone
	int zoneX = m_win->getSize().x - 240;
	int zoneY = m_win->getSize().y - (OFFSET * 2);

	// Defualt Rows and Cols should be 25 x 25

	m_ROWS = 15;
	m_COLS = 15;

	// BlockSize = (ZoneSize / RowsOrCols) - WALLTHICKNESS
	m_BlockXSize = std::floor( zoneX / ( m_ROWS ) - WALLTHICKNESS);
	m_BlockYSize = std::floor( zoneY / ( m_COLS ) - WALLTHICKNESS);

	// Initialize the Size of the Grid
	for (int i = 0; i < m_ROWS; i++) {
		std::deque<Node*> rowOfNodes;
		for (int j = 0; j < m_COLS; j++) {
			Node* node = new Node;
			rowOfNodes.push_back(node);
		}
		nodes.push_back(rowOfNodes);
	}


	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes[i].size(); j++) {
			nodes[i][j]->rect.setSize(sf::Vector2f(m_BlockXSize, m_BlockYSize));
			nodes[i][j]->rect.setFillColor(BGNODECOL);
			nodes[i][j]->parent = nullptr;
			nodes[i][j]->x = i;
			nodes[i][j]->y = j;
			nodes[i][j]->rect.setPosition(sf::Vector2f(i * (m_BlockXSize + WALLTHICKNESS) + OFFSET, j * (m_BlockYSize + WALLTHICKNESS) + OFFSET));
		}
	}

	srand(time(NULL));
	mouseNode.setSize(sf::Vector2f(m_BlockXSize, m_BlockYSize));
	SetupButtons();
	GrabNeighboringNodes();
	setStart();
	setEnd();
}

Grid::Grid(sf::RenderWindow* window, int rows, int cols)
	: m_win(window) {

	// Alotted Space to Place the Nodes within the window
	int zoneX = m_win->getSize().x - 240;
	int zoneY = m_win->getSize().y - (OFFSET * 2);

	// BlockSize = (ZoneSize / RowsOrCols) - WALLTHICKNESS
	int BlockSizeX = std::floor( zoneX / ( rows ) - WALLTHICKNESS);
	int BlockSizeY = std::floor( zoneY / ( cols ) - WALLTHICKNESS);

	// Set the New Block Size Dimensions
	m_BlockXSize = BlockSizeX;
	m_BlockYSize = BlockSizeY;

	m_ROWS = std::floor( zoneX / ( m_BlockXSize + WALLTHICKNESS ));
	m_COLS = std::floor( zoneY / ( m_BlockYSize + WALLTHICKNESS ));

	// Initialize the Size of the Grid
	for (int i = 0; i < m_ROWS; i++) {
		std::deque<Node*> rowOfNodes;
		for (int j = 0; j < m_COLS; j++) {
			Node* node = new Node;
			rowOfNodes.push_back(node);
		}
		nodes.push_back(rowOfNodes);
	}


	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes[i].size(); j++) {
			nodes[i][j]->rect.setSize(sf::Vector2f(m_BlockXSize, m_BlockYSize));
			nodes[i][j]->rect.setFillColor(BGNODECOL);
			nodes[i][j]->parent = nullptr;
			nodes[i][j]->x = i;
			nodes[i][j]->y = j;
			nodes[i][j]->rect.setPosition(sf::Vector2f(i * (m_BlockXSize + WALLTHICKNESS) + OFFSET, j * (m_BlockYSize + WALLTHICKNESS) + OFFSET));
		}
	}

	srand(time(NULL));
	mouseNode.setSize(sf::Vector2f(BlockSizeX, BlockSizeY));
	SetupButtons();
	GrabNeighboringNodes();
	setStart();
	setEnd();
}

void Grid::DrawNodesAndButtons() {
	// Draws Nodes

	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {

			m_win->draw(nodes[i][j]->rect);
		}
	}

	//  Display Buttons
	if (!m_buttons.empty()) {
		for (auto i : m_buttons) {
			i->buttonDisplay();
		}
	}

	// draw Selected Button
}

void Grid::DrawAlgorithmResults() {
	AnimateTheSearch(0);
	constructPath();
	DrawPath(20.0f);
	m_startAlgorithm = false;
}

void Grid::Display() {

	// Draw The Grid and Buttons
	DrawNodesAndButtons();

	// For Each Button..
	CheckButtonPress();

	// Pick and Place Starting or End Node
	Pick_And_Place();

}

void Grid::SetupButtons() {

	for (int i = 0; i <= 9; i++) {
		m_buttons.push_back(new Button(sf::Color(31, 40, 51), 200, 60, m_win, 775, 10 + i * 75, 0));
	}

	m_buttons[0]->buttonSetup("Start", "start", 0);
	m_buttons[0]->tweakTextPosition(45, 5);

	m_buttons[1]->buttonSetup("A* Star", "astar", 1);
	m_buttons[1]->tweakTextPosition(25, 5);

	m_buttons[2]->buttonSetup("Dijkstra", "dijkstra", 1);
	m_buttons[2]->tweakTextPosition(20, 5);

	m_buttons[3]->buttonSetup("BFS", "greedy", 1);
	m_buttons[3]->tweakTextPosition(60, 5);

	m_buttons[4]->buttonSetup("Rando", "rando", 0);
	m_buttons[4]->tweakTextPosition(25, 5);

	m_buttons[5]->buttonSetup("Clear Path", "clear", 0);
	m_buttons[5]->tweakCharSize(35);
	m_buttons[5]->tweakTextPosition(10, 5);

	m_buttons[6]->buttonSetup("Reset", "reset", 0);
	m_buttons[6]->tweakTextPosition(40, 5);

	// Set Row Size
	m_buttons[7]->buttonSetup("Rows", "setRowSize", 0);
	m_buttons[7]->tweakCharSize(15);
	m_buttons[7]->ChangeButtonSize(sf::Vector2f(50, 50));
	m_buttons[7]->tweakTextPosition(5, 5);
	// Set Column Size
	m_buttons[8]->buttonSetup("Cols", "setColSize", 0);
	m_buttons[8]->tweakCharSize(15);
	m_buttons[8]->ChangeButtonSize(sf::Vector2f( 50, 50));
	m_buttons[8]->tweakTextPosition(68, -70);
	m_buttons[8]->tweakButtonPosition(sf::Vector2f(60, -75));
	// Confirm and Build new Grid Size
	m_buttons[9]->buttonSetup("Confirm", "confirmGridSize", 0);
	m_buttons[9]->tweakCharSize(15);
	m_buttons[9]->ChangeButtonSize(sf::Vector2f(80, 50));
	m_buttons[9]->tweakButtonPosition(sf::Vector2f(120, -150));
	m_buttons[9]->tweakTextPosition(130, -135);

}

void Grid::setButtonBooleans(bool astar, bool dijkstra, bool BFS) {
	m_aStar = astar;
	m_dijkstra = dijkstra;
	m_breadthFirstSearch = BFS;
}

void Grid::placeStart() {
	sf::Vector2i mPos = sf::Mouse::getPosition();
	if (m_startNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && m_startNode->grabbed) {
		mouseContains();
	}
}

void Grid::placeEnd() {
	sf::Vector2i mPos = sf::Mouse::getPosition();
	if (m_endNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && m_endNode->grabbed) {
		mouseContains();
	}
}

void Grid::Pick_And_Place() {
	// --- Creates the effect of picking and placing the Start and EndNode ----
	if (m_startNode != nullptr) {
		if (m_startNode->grabbed) {
			m_startNode->rect.setFillColor(sf::Color(BGNODECOL));
			sf::Vector2i mPos = sf::Mouse::getPosition(*m_win);
			mouseNode.setPosition(mPos.x - m_BlockXSize / 2, mPos.y - m_BlockYSize / 2);
			mouseNode.setFillColor(sf::Color(MOUSESTARTNODECOL));
			nodes[m_startNode->x][m_startNode->y]->rect.setFillColor(sf::Color(BGNODECOL));
			m_win->draw(mouseNode);
		}
		else if (!m_startNode->grabbed) {
			m_startNode->rect.setFillColor(sf::Color(STARTNODECOL));
			m_win->draw(m_startNode->rect);
		}
	}

	if (m_endNode != nullptr) {
		if (m_endNode->grabbed) {
			m_endNode->rect.setFillColor(sf::Color(BGNODECOL));
			sf::Vector2i mPos = sf::Mouse::getPosition(*m_win);
			mouseNode.setPosition(mPos.x - m_BlockXSize / 2, mPos.y - m_BlockYSize / 2);
			mouseNode.setFillColor(sf::Color(MOUSEENDNODECOL));
			m_win->draw(mouseNode);
		}
		else if (!m_endNode->grabbed) {
			m_endNode->rect.setFillColor(sf::Color(ENDNODECOL));
			m_win->draw(m_endNode->rect);
		}
	}
	// -----------------------------------------------------------------------
}

void Grid::mouseContains() {
	// Grab and Place the Start and End Node
	if (m_startNode != nullptr) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*m_win);
		if (m_startNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && !m_startNode->grabbed) {
			resetBoard();
			m_startNode->grabbed = true;
		}
		else if (m_startNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && m_startNode->grabbed) {
			placeStart();
			m_startNode->grabbed = false;
		}

		if (m_endNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && !m_endNode->grabbed) {
			resetBoard();
			m_endNode->grabbed = true;
		}
		else if (m_endNode->rect.getGlobalBounds().contains(mPos.x, mPos.y) && m_endNode->grabbed) {
			placeEnd();
			m_endNode->grabbed = false;
		}
	}

	if (!m_buttons.empty()) {
		for (auto i : m_buttons) {
			sf::Vector2i mPos = sf::Mouse::getPosition(*m_win);

			// If the Start Button is pressed
			if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "start") {
				resetBoard();
				m_startAlgorithm = true;
			}
			if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "astar") {
				setButtonBooleans(true, false, false);
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "dijkstra") {
				setButtonBooleans(false, true, false);
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "greedy") {
				setButtonBooleans(false, false, true);
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "rando") {
				m_mazeButtonPressed = true;
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "clear") {
				resetBoard();
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "reset") {
				m_clrScreen = true;
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "setRowSize") {
				setRow = true;
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "setColSize") {
				setCol = true;
			}
			else if (i->checkMouseClick(mPos.x, mPos.y) && i->returnId() == "confirmGridSize") {
				setGridSize = true;
			}
		}
	}

	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {
			// If the Start and End node are Placed on the Board
			if (m_startNode != nullptr && m_endNode != nullptr) {
				sf::Vector2i mPos = sf::Mouse::getPosition(*m_win);
				// Set the node to be a Wall
				if (nodes[i][j]->rect.getGlobalBounds().contains(mPos.x, mPos.y) && nodes[i][j]->isWalkable && !m_startNode->grabbed && !m_endNode->grabbed && !nodes[i][j]->isWall) {
					nodes[i][j]->isWalkable = false;
					nodes[i][j]->isWall = true;
					nodes[i][j]->rect.setFillColor(sf::Color::Black);
				}
				// Set the Node to be walkable
				else if (nodes[i][j]->rect.getGlobalBounds().contains(mPos.x, mPos.y) && !nodes[i][j]->isWalkable && nodes[i][j]->isWall) {
					nodes[i][j]->isWalkable = true;
					nodes[i][j]->isWall = false;
					nodes[i][j]->rect.setFillColor(sf::Color(BGNODECOL));
				}
				// Set the Grabbed Starting node too the current nodes position
				if (nodes[i][j]->rect.getGlobalBounds().contains(mPos.x, mPos.y) && nodes[i][j]->isWalkable && m_startNode->grabbed) {
					m_startNode = nodes[i][j];
					break;
				}
				// Set the Grabbed Ending node too the current nodes position
				if (nodes[i][j]->rect.getGlobalBounds().contains(mPos.x, mPos.y) && nodes[i][j]->isWalkable && m_endNode->grabbed) {
					m_endNode = nodes[i][j];
					break;
				}
			}
		}
	}
}

void Grid::CheckButtonPress() {

	for (auto i : m_buttons) {
		if (m_aStar) { // Set to true[elsewhere] if Mouse clicks button
			if (i->returnId() == "astar") {
				i->setIndicatorColor(sf::Color::Green);
			}
			else {
				i->setIndicatorColor(sf::Color::Black);
			}
		}
		else if (m_dijkstra) {
			if (i->returnId() == "dijkstra") {
				i->setIndicatorColor(sf::Color::Green);
			}
			else {
				i->setIndicatorColor(sf::Color::Black);
			}
		}
		else if (m_breadthFirstSearch) {
			if (i->returnId() == "greedy") {
				i->setIndicatorColor(sf::Color::Green);
			}
			else {
				i->setIndicatorColor(sf::Color::Black);
			}
		}
	}
}

bool Grid::GrabNeighboringNodes() {

	for (size_t x = 0; x <= m_ROWS - 1; x++) {
		for (size_t y = 0; y <= m_COLS - 1; y++) {
			// 
			if (y > 0) {
				nodes[x][y]->NeighborNodes.push_back(nodes[x][y - 1]);
			}
			if (y < m_COLS - 1) {
				nodes[x][y]->NeighborNodes.push_back(nodes[x][y + 1]);
			}
			if (x > 0) {
				nodes[x][y]->NeighborNodes.push_back(nodes[x - 1][y]);
			}
			if (x < m_ROWS - 1) {
				nodes[x][y]->NeighborNodes.push_back(nodes[x + 1][y]);
			}
		}
	}
	return true;
}

void Grid::ShowNeighborNodeConnections(Node* thisNodes) {

	thisNodes->rect.setFillColor(sf::Color(255, 95, 0));
	for (auto i : thisNodes->NeighborNodes) {
		i->rect.setFillColor(sf::Color::Red);
	}
}

float Grid::Heuristic(Node* a, Node* b) {
	return std::abs(std::sqrtf(std::powf((a->x - b->x), 2) + std::powf((a->y - b->y), 2)));
}

void Grid::AnimateTheSearch(float rate) {

	int index1 = 0;
	int index2 = 0;
	bool animationComplete = false;
	bool flag = false;
	while (!animationComplete) {
		// -------- For Animating the Search Algorithm using a clock Tick ---------
		if (m_clock.getElapsedTime().asMilliseconds() > rate) {
			m_clock.restart();

			//		if (index1 != m_OpenList.size() && flag) {
			//			m_OpenList[index1]->rect.setFillColor(sf::Color(OUTERSEARCHNODECOL));
			//			m_win->draw(m_OpenList[index1]->rect);
			//			index1++;
			//		}

			if (index2 != m_ClosedList.size()) {
				m_ClosedList[index2]->rect.setFillColor(sf::Color(SEARCHNODECOL)); // checkedNodes COLOR
				m_win->draw(m_ClosedList[index2]->rect);
				index2++;
				if (index2 == m_ClosedList.size()) {
					flag = true;
				}
			}
			DrawNodesAndButtons();
			m_startNode->rect.setFillColor(STARTNODECOL);
			m_endNode->rect.setFillColor(ENDNODECOL);
			m_win->draw(m_startNode->rect);
			m_win->draw(m_endNode->rect);
			m_win->display();


			//		if (index1 == m_OpenList.size() && index2 == m_ClosedList.size()) {
			//			animationComplete = true;
			//		}
			if (index2 == m_ClosedList.size()) {
				animationComplete = true;
			}
		}
	}
	// -------------------------------------------------------------------------
}

void Grid::DrawPath(float rate) {
	int index = m_path.size() - 1;
	while (index >= 0) {
		if (m_clock.getElapsedTime().asMilliseconds() > rate) {
			m_clock.restart();
			m_path[index]->rect.setFillColor(sf::Color(PATHNODECOL));
			m_win->draw(m_path[index]->rect);
			index--;
		}
		DrawNodesAndButtons();
		m_win->display();
	}
}

void Grid::tracePath() {
	for (int i = 0; i < m_path.size(); i++) {
		tracedLines.emplace_back(sf::Vector2f(m_path[i]->rect.getPosition().x + m_BlockXSize / 2, m_path[i]->rect.getPosition().y + m_BlockYSize / 2), LINECOL);
	}
	m_win->draw(&tracedLines[0], tracedLines.size(), sf::LinesStrip);
}

void Grid::generateRandoCommandoMaze() {
	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {
			int chance = rand() % 100;
			if (chance < 30 && nodes[i][j] != m_startNode && nodes[i][j] != m_endNode) {
				nodes[i][j]->isWalkable = false;
				nodes[i][j]->isWall = true;
				//nodes[i][j]->parent = nullptr;
				nodes[i][j]->rect.setFillColor(sf::Color::Black);
			}
		}
	}
}

void Grid::constructPath() {
	Node* par = m_endNode;
	if (par->parent == nullptr) {
		std::cout << "Blocked Path" << std::endl;
	}
	else if (m_endNode != nullptr) {
		par = par->parent;
		while (par->parent != nullptr) {
			m_path.push_back(par);
			par = par->parent;
		}
	}
	std::cout << "PathSize: " << m_path.size() << std::endl;
}

bool Grid::A_Star_Algorithm() {

	// Reset All The Nodes back to Default Before Algorithm Starts
	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {
			nodes[i][j]->visited = false;
			nodes[i][j]->globalfGoal = INFINITY;
			nodes[i][j]->localfGoal = INFINITY;
			nodes[i][j]->parent = nullptr;
		}
	}

	// Starting Setup
	Node* currNode = m_startNode; // Set the CurrentNthNotode to the Starting Node
	m_startNode->localfGoal = 0.0f;
	m_startNode->globalfGoal = Heuristic(m_startNode, m_endNode);
	// Add Starting Node to the Open List
	m_OpenList.push_back(m_startNode); // push_back(StartingNode)

	while (!m_OpenList.empty() && currNode != m_endNode) { // Loop until empty or we've found the end Node
		// Sort the Deque based of LOWEST globel score FIRST
		std::sort(m_OpenList.begin(), m_OpenList.end(), CompareGlobalScores);

		// Loop while not empty && if we visited a node already, delete
		while (!m_OpenList.empty() && m_OpenList.front()->visited) { 
			m_ClosedList.push_back(m_OpenList.front());
			m_OpenList.pop_front();
		}
		// if empty, break
		if (m_OpenList.empty()) { 
			break;
		}

		// Grab SORTED front Node and set to current
		currNode = m_OpenList.front(); 

		// set visited = true
		currNode->visited = true; 
		// Check All the Neighbors of the Current Node
		for (auto neighborNode : currNode->NeighborNodes) { // Loop Through Current Nodes Neighbors
			if (!neighborNode->visited && !neighborNode->isWall) { // If we havnt visited it, and its walkable -> push_back()
				m_OpenList.push_back(neighborNode);
			}

			// Calculate a new fCost from the CurrentNode to the Neighbor Node
			//float possibleLowerGoal = Heuristic(neighborNode, m_startNode) + Heuristic(neighborNode, m_endNode);
			float possibleLowerGoal = currNode->localfGoal + Heuristic(currNode, neighborNode);
			if (possibleLowerGoal < neighborNode->localfGoal) { // if that new fCost is Lower than the neighbors fCost, set to current
				std::cout << "LowerGoal: " << possibleLowerGoal << " < localfGoal: " << neighborNode->localfGoal << std::endl;
				neighborNode->parent = currNode; // Set that Neighbors parent equal too the currentNode, meaning step toward that direction
				neighborNode->localfGoal = possibleLowerGoal;
				neighborNode->globalfGoal = neighborNode->localfGoal + Heuristic(neighborNode, m_endNode);
			}
		}
	}
	return true;
}

bool Grid::DijkstrasAlgorithm() {

	// Reset All The Nodes back to Default Before Algorithm Starts
	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {
			nodes[i][j]->visited = false;
			nodes[i][j]->globalfGoal = INFINITY;
			nodes[i][j]->localfGoal = INFINITY;
			nodes[i][j]->parent = nullptr;
		}
	}

	// Starting Setup
	Node* currNode = m_startNode; // Set the CurrentNthNotode to the Starting Node
	currNode->localfGoal = 0.0f; // Set Its Local Goal to 0;
//	currNode->globalfGoal = Heuristic(m_startNode, currNode); // Grab the Distance from the StartingNode -> EndingNode
	m_OpenList.push_back(currNode); // push_back(StartingNode)

	while (!m_OpenList.empty() && currNode != m_endNode) { // Loop until empty or we've found the end Node
		std::sort(m_OpenList.begin(), m_OpenList.end(), CompareGlobalScores); // Sort the Deque based of LOWEST globel score FIRST
		while (!m_OpenList.empty() && m_OpenList.front()->visited) { // Loop while not empty && if we visited a node already
			m_ClosedList.push_back(m_OpenList.front());
			m_OpenList.pop_front();
		}

		if (m_OpenList.empty()) { // if empty, break
			break;
		}

		currNode = m_OpenList.front(); // Grab SORTED front Node and set to current
		currNode->visited = true; // set visited = true
		for (auto neighborNode : currNode->NeighborNodes) { // Loop Through Current Nodes Neighbors
			if (!neighborNode->visited && !neighborNode->isWall && neighborNode->isWalkable) { // If we havnt visited it, and its walkable -> push_back()
				m_OpenList.push_back(neighborNode);
			}

			// Calculate a new fCost from the CurrentNode to the Neighbor Node
			float possibleLowerGoal = currNode->localfGoal + Heuristic(currNode, neighborNode);
			if (possibleLowerGoal < neighborNode->localfGoal) { // if that new fCost is Lower than the neighbors fCost, set to current
				neighborNode->parent = currNode; // Set that Neighbors parent equal too the currentNode, meaning step toward that direction
				neighborNode->localfGoal = possibleLowerGoal; // Change the localFgoal to the floatValue
				neighborNode->globalfGoal = Heuristic(neighborNode, m_startNode); // set Global--> (NeighborNodes LocalFGoal + that Nodes dis to END)
			}
		}
	}
	return true;
}

bool Grid::BreadthFirstSearch() {
	// Reset All The Nodes back to Default Before Algorithm Starts
	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {
			nodes[i][j]->visited = false;
			nodes[i][j]->globalfGoal = INFINITY;
			nodes[i][j]->localfGoal = INFINITY;
			nodes[i][j]->parent = nullptr;
		}
	}

	// Starting Setup
	Node* currNode = m_startNode; // Set the CurrentNthNotode to the Starting Node
	currNode->localfGoal = 0.0f; // Set Its Local Goal to 0;
	currNode->globalfGoal = Heuristic(m_startNode, m_endNode); // Grab the Distance from the StartingNode -> EndingNode
	m_OpenList.push_back(m_startNode); // push_back(StartingNode)

	while (!m_OpenList.empty() && currNode != m_endNode) { // Loop until empty or we've found the end Node
		// Sort the Deque based of LOWEST globel score FIRST
		std::sort(m_OpenList.begin(), m_OpenList.end(), CompareGlobalScores);
		while (!m_OpenList.empty() && m_OpenList.front()->visited) { // Loop while not empty && if we visited a node already, delete
			m_ClosedList.push_back(m_OpenList.front());
			m_OpenList.pop_front();
		}

		if (m_OpenList.empty()) { // if empty, break
			break;
		}

		currNode = m_OpenList.front(); // Grab SORTED front Node and set to current
		currNode->visited = true; // set visited = true
		for (auto neighborNode : currNode->NeighborNodes) { // Loop Through Current Nodes Neighbors
			if (!neighborNode->visited && !neighborNode->isWall) { // If we havnt visited it, and its walkable -> push_back()
				m_OpenList.push_back(neighborNode);
			}

			// Calculate a new fCost from the neighborNode to the EndNode
			float possibleLowerGoal = Heuristic(neighborNode, m_endNode);
			if (possibleLowerGoal < neighborNode->localfGoal) { // if that new fCost is Lower than the neighbors fCost, set to current
				neighborNode->parent = currNode; // Set that Neighbors parent equal too the currentNode, meaning step toward that direction
				neighborNode->localfGoal = possibleLowerGoal; // Change the localFgoal to the floatValue
			}
		}
	}
	return true;
}

void Grid::resetBoard() {
	int wallsCounter = 0;

	// Draw a blank, Black canvas
	m_win->clear(sf::Color::Black);
	// Reset the nodes to original settings for next run
	for (int i = 0; i < m_ROWS; i++) {
		for (int j = 0; j < m_COLS; j++) {

			if (nodes[i][j]->isWall) {
				wallsCounter++;
			}

			if (nodes[i][j]->isWalkable && !nodes[i][j]->isWall) {
				nodes[i][j]->rect.setFillColor(sf::Color(BGNODECOL));
				nodes[i][j]->isWall = false;
				nodes[i][j]->isWalkable = true;
				m_win->draw(nodes[i][j]->rect);
			}
		}
	}

	std::cout << "Number of Walls: " << wallsCounter << std::endl;

	// Clear All Lists
	m_OpenList.clear();
	m_ClosedList.clear();
	m_path.clear();
	tracedLines.clear();
	m_win->display();

}
