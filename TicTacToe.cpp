#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <list>

static const int INFIN = 1000000;

// These are the possible game states
static enum { PLAYING, WINO, WINX, DRAW, QUIT } state;


// This is the player struct, defining the values needed for the players.
typedef struct {
	std::string name;
	char symbol;
	int move;
	bool selected;
	bool win;
} player;


void displayBoard();		// Display the current board position on the screen
void seedRandomGenerator(); // Seed the random generator (rand function) with the current time
void getMove();             // Get the move for the current player (Human or AI)
void displayResult();		// Display the final result of the game
void selectGameType();      // Get the user to choose a game type (Human VS AI, AI VS AI)
void getPlayerSymbol();     // Get symbol (X, O) for the first player selected
void getPlayerName();       // Gets the name of the current player
void resetPlayerName();     // Reset the name of the players
void findWinner();          // This function works out which player won the game
void resetWinner();         // Resets the winners before starting new game
void updateBoard();         // Places the correct symbol in the chosen square  
void resetMenu();			// Resets the menu if an invalid input is entered
void verifyMove();          // Verifies if the current move is legal
void resetGameState();      // Resets the state of the game
void resetBoard();          // Clears the board, removes all move that has been made
void initPlayerMove();      // Resets the "player move" variable for the start of the game 
void displayProgress();     // Displays the current board state
void updateGame();          // Generic update function, calls other update functions
bool verifySymbol();        // Check if the symbol that the player has entered is valid
bool verifySelection();     // Verifies the validity of the current selection made by the player (checks 0-9)
bool isGameOver();          // Checks if the game is over
bool isSquareFree();		// Verifies if the square chosen by the current player is free 

void generateMoves(char _board[9], std::list<int> &moveList);		// Generates the list of moves for the current board - Used for MiniMax

void checkGameState(char board[9]);									// Determine current game state

// This makes an evaluation of the current board position
// The function returns +INFIN if the computer is winning,
// -INFIN if the computer is losing, and 0 for a draw
int evaluatePosition(char _board[9], player _player);


// The three functions below implement MiniMax

int MiniMax(char _board[9], player _player);	
int MinMove(char _board[9], player _player);	
int MaxMove(char _board[9], player _player);	

static player player1, player2, currentPlayer; //The players
static std::string gameType; // 'Human VS AI', 'AI VS AI'
static std::string prevGameType; // Used for storing previous 'game type'

static char board[9] = { 0 }; // Stores the board and state
static char cSymbol; // The current player's symbol (X, O)
static int nMove; // The most recent move by the current player, used to check validity


int main() 
{
	seedRandomGenerator();
	selectGameType();

	if (state != QUIT) 
	{
		getPlayerName();
		getPlayerSymbol();

		while (state != QUIT) 
		{
			while (state == PLAYING) 
			{
				initPlayerMove();
				getMove();
				updateGame();
			}

			if (isGameOver()) 
			{
				findWinner();
				displayResult();
				resetGameState();
				resetWinner();
				resetBoard();
			}

			selectGameType();
		}
	}
	return 0;
}

void selectGameType() 
{
	std::cout << "   1 - Play a game against the computer" << std::endl;
	std::cout << "   2 - Start an automatic game" << std::endl;
	std::cout << "   3 - Quit the program" << std::endl;
	std::cout << "\nSelection: ";

	int choice;
	std::cin >> choice;

	if (!std::cin.good()) 
	{
		std::cout << "Please enter a valid choice" << std::endl;
		resetMenu();
	}

	switch (choice) 
	{
	case 1:
		gameType = "Human VS AI";
		break;
	case 2:
		gameType = "AI VS AI";
		break;
	case 3:
		state = QUIT;
		break;
	default:
		std::cout << "Invalid Selection." << std::endl;
		resetMenu();
	}

	if (choice > 0 && choice < 3) 
	{
		if (prevGameType != "" && gameType != prevGameType) 
		{
			resetPlayerName();
			getPlayerName();
			getPlayerSymbol();
		}

		if (gameType.length() > 0) 
		{
			prevGameType = gameType;
		}
	}
}

void getPlayerName()
{
	std::cin.sync();
	if (gameType == "Human VS AI") 
	{
		std::cout << "\nPlease enter your name: ";
		std::cin >> player1.name;
		if (player1.name.length() == 0) 
		{
			getPlayerName();
		}
		player2.name = "The Computer";
	}

	else if (gameType == "AI VS AI") 
	{
		player1.name = "AI P1";
		player2.name = "AI P2";
	}
}

void resetPlayerName() 
{
	player1.name.erase();
}

void getPlayerSymbol() 
{
	if (gameType == "Human VS AI") 
	{

		if (rand() % 2 == 0) 
		{
			rand() % 2 == 0 ? player2.symbol = 'X' : player2.symbol = 'O';
			cSymbol = player2.symbol;
			player2.selected = 1;
			std::cout << player2.name << " will play \'" << player2.symbol << "\'" << std::endl;
		}

		else 
		{
			std::cout << player1.name << ", please enter your symbol. (X or O): ";
			std::cin >> player1.symbol;
			player1.symbol = toupper(player1.symbol);
			cSymbol = player1.symbol;
			player1.selected = 1;
		}
	}

	else if (gameType == "AI VS AI") 
	{
		std::string playerName;
		int sel = rand() % 2;

		if (sel == 0) 
		{
			rand() % 2 == 0 ? player1.symbol = 'X' : player1.symbol = 'O';
			playerName = player1.name;
			player1.selected = 1;
			cSymbol = player1.symbol;
		} 

		else if (sel == 1) 
		{
			rand() % 2 == 0 ? player2.symbol = 'X' : player2.symbol = 'O';
			playerName = player2.name;
			player2.selected = 1;
			cSymbol = player2.symbol;
		}

		std::cout << playerName << " will play \'" << cSymbol << "\'" << std::endl;
	}

	if (!std::cin.good() || verifySymbol()) 
	{
		std::cout << "The only valid symbols are X and O." << std::endl;
		system("pause");
		getPlayerSymbol();
	}

	if (!player2.selected) 
	{
		player1.symbol == 'X' ? player2.symbol = 'O' : player2.symbol = 'X';
		player1.symbol == 'O' ? player2.symbol = 'X' : player2.symbol = 'O';
	}

	else if (!player1.selected) 
	{
		player2.symbol == 'X' ? player1.symbol = 'O' : player1.symbol = 'X';
		player2.symbol == 'O' ? player1.symbol = 'X' : player1.symbol = 'O';
	}
	state = PLAYING;
	displayBoard();
}

void getMove() 
{
	std::cin.sync();
	if (player1.selected) 
	{
		if (gameType == "Human VS AI")
		{
			std::cout << "\n" << player1.name << " ,please enter your move (1 - 9): ";
			std::cin >> player1.move;
			if (!std::cin.good())
			{
				std::cin.clear();
				std::cin.sync();
			}
			nMove = player1.move;
			cSymbol = player1.symbol;
			currentPlayer = player1;
			player1.selected = 0;
			player2.selected = 1;
		}
		else if (gameType == "AI VS AI")
		{
			if (player1.selected)
			{
				player1.move = MiniMax(board, player1);
				nMove = player1.move;
				cSymbol = player1.symbol;
				currentPlayer = player1;
				player1.selected = 0;
				player2.selected = 1;
				resetGameState();
				Sleep(500);
			}
		}
	}
	else if (player2.selected) 
	{
		player2.move = MiniMax(board, player2);
		nMove = player2.move;
		cSymbol = player2.symbol;
		currentPlayer = player2;
		player1.selected = 1;
		player2.selected = 0;
		resetGameState();
		Sleep(500);
	}
	
	verifyMove();
	if (isGameOver()) 
		return;
}

void findWinner() 
{
	if (state == WINX && player1.symbol == 'X') 
	{
		player1.win = 1;
	}
	else if (state == WINO && player1.symbol == 'O') 
	{
		player1.win = 1;
	}
	else if (state == WINX && player2.symbol == 'X') 
	{
		player2.win = 1;
	}
	else if (state == WINO && player2.symbol == 'O') 
	{
		player2.win = 1;
	}
}

void resetWinner() 
{
	player1.win = 0;
	player2.win = 0;
}

bool verifySymbol() 
{
	return (cSymbol != 'X' && cSymbol != 'O');
}

bool verifySelection() 
{
	return !(nMove > 0 && nMove < 10);
}

void initPlayerMove() 
{
	player1.move = -1;
	player2.move = -1;
}

bool isGameOver() 
{
	return (state == WINX || state == WINO || state == DRAW);
}

void resetGameState() 
{
	state = PLAYING;
}

void resetBoard() 
{
	for (int i = 0; i < 9; ++i) 
	{
		board[i] = 0;
	}
}

void updateBoard() 
{
	if (state == PLAYING) {
		if (player1.move != -1 && player2.move == -1) 
		{
			board[player1.move - 1] = player1.symbol; //subtract 1 because number entered is for human readability
		}
		else if (player2.move != -1) 
		{
			board[player2.move - 1] = player2.symbol;
		}
	}
}

void displayResult() 
{
	if (player1.win) 
	{
		std::cout << player1.name << " has won the game!" << std::endl;
	}
	else if (player2.win) 
	{
		std::cout << player2.name << " has won the game!" << std::endl;
	}
	else if (player1.win == 0 && player2.win == 0) {
		std::cout << "There is no winner. \nThis game is a draw." << std::endl;
	}
	system("pause");
	system("cls");
}

void updateGame() 
{
	updateBoard();
	displayProgress();
	checkGameState(board);
}

bool isSquareFree() 
{
	if (player1.move != -1 && player2.move == -1) {
		return board[player1.move - 1] == 0;
	}
	else if (player2.move != -1) {
		return board[player2.move - 1] == 0;
	}
	return 0;
}

void displayBoard() 
{
	char _board[9] = { 0 };

	for (int i = 0; i < 9; i++)
	{
		_board[i] = (_board[i] == board[i]) ? i + 49 : board[i]; 
	}

	std::cout << std::endl;
	std::cout << " " << _board[0] << " | " << _board[1] << " | " << _board[2] << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " << _board[3] << " | " << _board[4] << " | " << _board[5] << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " << _board[6] << " | " << _board[7] << " | " << _board[8] << std::endl;
	std::cout << std::endl;
	std::cin.sync();
}

void displayProgress() 
{
	std::cout << "\n\nBoard state after " << currentPlayer.name << "\'s move:" << std::endl;
	displayBoard();
}

void verifyMove() 
{
	if (verifySelection() || !isSquareFree()) 
	{
		std::cout << "Invalid Move.\n" << std::endl;
		if (player2.move == -1) 
		{
			player1.selected = 1;
			player2.selected = 0;
		}
		else {
			player1.selected = 0;
			player2.selected = 1;
		}
		if (gameType == "Human VS AI") 
		{
			player1.selected = 1;
			player2.selected = 0;
		}
		getMove();
	}
}

void seedRandomGenerator() 
{
	srand((unsigned)time(NULL));
}

void resetMenu() 
{
	system("pause");
	system("cls");
	std::cin.clear();
	std::cin.sync();
	selectGameType();
}

void checkGameState(char board[9]) 
{
	if ((board[0] == cSymbol && board[1] == cSymbol && board[2] == cSymbol) || //Row 1
		(board[3] == cSymbol && board[4] == cSymbol && board[5] == cSymbol) || //Row 2
		(board[6] == cSymbol && board[7] == cSymbol && board[8] == cSymbol) || //Row 3
		(board[0] == cSymbol && board[3] == cSymbol && board[6] == cSymbol) || //Col 1
		(board[1] == cSymbol && board[4] == cSymbol && board[7] == cSymbol) || //Col 2
		(board[2] == cSymbol && board[5] == cSymbol && board[8] == cSymbol) || //Col 3
		(board[0] == cSymbol && board[4] == cSymbol && board[8] == cSymbol) || //Diag left to right
		(board[2] == cSymbol && board[4] == cSymbol && board[6] == cSymbol))   //Dial right to left
	{
		if (cSymbol == 'X') 
		{
			state = WINX;
		}
		else if (cSymbol == 'O') 
		{
			state = WINO;
		}
	}
	else {
		state = DRAW;
		for (int i = 0; i < 9; ++i) 
		{
			if (board[i] == 0) 
			{
				state = PLAYING;
				break;
			}
		}
	}
}

void generateMoves(char _board[9], std::list<int> &moveList) 
{
	for (int i = 0; i < 9; ++i) 
	{
		if (_board[i] == 0) 
		{
			moveList.push_back(i);
		}
	}
}

int evaluatePosition(char _board[9], player _player) 
{
	checkGameState(_board);
	if (isGameOver()) 
	{
		if ((state == WINX && _player.symbol == 'X') ||	(state == WINO && _player.symbol == 'O')) 
		{
			return +INFIN;
		}
		else if ((state == WINX && _player.symbol == 'O') || (state == WINO && _player.symbol == 'X')) {
			return -INFIN;
		}
		else if (state == DRAW) 
		{
			return 0;
		}
	}
	return -1;
}

int MiniMax(char _board[9], player _player) 
{
	int best = -INFIN, index = 0;
	std::list<int> moveList;
	char bestMoves[9] = { 0 };
	generateMoves(_board, moveList);
	while (!moveList.empty()) 
	{
		_board[moveList.front()] = _player.symbol;
		cSymbol = _player.symbol;
		int val = MinMove(_board, _player);
		if (val > best) 
		{
			best = val;
			index = 0;
			bestMoves[index] = moveList.front() + 1;
		}
		else if (val == best) 
		{
			bestMoves[++index] = moveList.front() + 1;
		}
		_board[moveList.front()] = 0;
		moveList.pop_front();
	}
	if (index > 0) 
	{
		index = rand() % index;
	}
	return bestMoves[index];
}

int MinMove(char _board[9], player _player) 
{
	int posValue = evaluatePosition(_board, _player);
	if (posValue != -1) 
	{
		return posValue;
	}
	int best = +INFIN;
	std::list<int> moveList;
	generateMoves(_board, moveList);
	while (!moveList.empty()) 
	{
		_player.symbol == 'X' ? cSymbol = 'O' : cSymbol = 'X';
		_board[moveList.front()] = cSymbol;
		int val = MaxMove(_board, _player);
		if (val < best) 
		{
			best = val;
		}
		_board[moveList.front()] = 0;
		moveList.pop_front();
	}
	return best;
}

int MaxMove(char _board[9], player _player) 
{
	int posValue = evaluatePosition(_board, _player);
	if (posValue != -1) 
	{
		return posValue;
	}
	int best = -INFIN;
	std::list<int> moveList;
	generateMoves(_board, moveList);
	while (!moveList.empty()) 
	{
		_player.symbol == 'X' ? cSymbol = 'X' : cSymbol = 'O';
		_board[moveList.front()] = cSymbol;
		int val = MinMove(_board, _player);
		if (val > best) 
		{
			best = val;
		}
		_board[moveList.front()] = 0;
		moveList.pop_front();
	}
	return best;
}
