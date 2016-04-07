# TicTacToe
A TicTacToe game I developed in my spare time. It allows a player to play against an AI, or allows two AI to play against each other. The AI uses MiniMax to decide on the best move each turn.

I did some things with this that I don't usually attempt, such as:
*Keeping all code in one file. I like to use headers, but thought I'd make it as simple as possible for someone to take the code and run it.
*Functions everywhere. I ensured that anything that was used more than once was placed into a function to be called, to reduce redundancy.
*Limited use of temporary variables where possible.

If I were to refactor, I'd definitely like to make it so you can save games to files, and to attempt some OO-style techniques, such as passing the players and boards around, instead of having so many global variables.
