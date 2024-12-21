#include<iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <sstream>
#include <unordered_map>

enum PieceType {
    X=1,
    O
};

std::unordered_map<PieceType,std::string> symbolMapping = {
    {X,"X"},
    {O,"O"}
};

class PlayingPiece {
    public:
    PieceType pieceType;
    PlayingPiece(){

    }

    PlayingPiece(PieceType pieceType){
        this->pieceType = pieceType;
    }
};

class PlayingPieceO : public PlayingPiece{
    public:
    PlayingPieceO():PlayingPiece(PieceType::O){}
};

class PlayingPieceX : public PlayingPiece{
    public:
    PlayingPieceX():PlayingPiece(PieceType::X){}
};

class Board{
    public:
    int size;
    std::vector<std::vector<PlayingPiece>> board;

    Board(int size) : size(size),board(size,std::vector<PlayingPiece>(size)){}

    bool addPiece(int row,int col, PlayingPiece piece){
        if(row>=size || col>=size) return false;
        if(symbolMapping[board[row][col].pieceType]!="") return false;

        board[row][col] = piece;
        return true;
    }

    std::list<std::pair<int,int>> getFreeCells(){
        std::list<std::pair<int,int>> freeCells;

        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(symbolMapping[board[i][j].pieceType]=="") freeCells.push_back({i,j});
            }
        }

        return freeCells;
        
    }

    void printBoard(){
        for(int i=0;i<size;i++){
            std::cout<<" | ";
            for(int j=0;j<size;j++){
                if(symbolMapping[board[i][j].pieceType]!="") std::cout<<" "<<symbolMapping[board[i][j].pieceType]<<" ";
                else std::cout<<"   ";

                std::cout<<" | ";
            }
            std::cout<<std::endl;
        }
    }
};

class Player{
    public:
        std::string name;
        PlayingPiece playingPiece;

        Player(std::string name, PlayingPiece playingPiece){
            this->name = name;
            this->playingPiece = playingPiece;
        }

        void setName(std::string name){
            this->name = name;
        }

        std::string getName(){
            return this->name;
        }

        void setPlayingPiece(PlayingPiece playingPiece){
            this->playingPiece = playingPiece;
        }

        PlayingPiece getPlayingPiece(){
            return this->playingPiece;
        }
};

class TicTacToe{
    public:
        std::deque<Player> players;
        Board *gameBoard;

        ~TicTacToe(){
            delete gameBoard;
        }

        void initializeGame(){
            PlayingPieceO noughtsPiece;
            PlayingPieceX crossPiece;

            Player player1("Atul",crossPiece);
            Player player2("Tushar",noughtsPiece);

            players.push_back(player1);
            players.push_back(player2);

            //initialize board
            gameBoard = new Board(3);
        }

        std::string startGame(){
            bool noWinner = true;

            while(noWinner){
                Player playerTurn = players.front();
                players.pop_front();

                gameBoard->printBoard();

                std::list<std::pair<int,int>> emptyCells = gameBoard->getFreeCells();

                if(emptyCells.empty()){
                    noWinner = false;
                    continue;
                }

                //Take input
                std::cout<<"Player "<<playerTurn.name<<" Enter row,column : ";
                std::string position;
                std::cin>>position;
                std::vector<std::string> values;
                std::stringstream ss(position);
                while (ss.good()) {
                    std::string substr;
                    getline(ss, substr, ',');
                    values.push_back(substr);
                }
                int inputRow = std::stoi(values[0])-1;
                int inputColumn = std::stoi(values[1])-1;

                //place the piece 
                bool isPiecePlacedSuccessfully = gameBoard->addPiece(inputRow,inputColumn,playerTurn.playingPiece);
                if(!isPiecePlacedSuccessfully){
                    std::cout<<"Incorrect Position, Please enter correct position!"<<std::endl;
                    players.push_front(playerTurn);
                    continue;
                }

                players.push_back(playerTurn);

                bool winner = isThereWinner(inputRow, inputColumn, playerTurn.playingPiece.pieceType);
                if(winner) {
                    gameBoard->printBoard();
                    return playerTurn.name;
                }
            }

            //tie
            return "tie";
        }

        bool isThereWinner(int row,int col,PieceType pieceType){
            bool rowMatch = true;
            bool columnMatch = true;
            bool diagonalMatch = true;
            bool antiDiagonalMatch = true;

            //need to check in row
            for(int i=0;i<gameBoard->size;i++) {
                if(symbolMapping[gameBoard->board[row][i].pieceType] == "" || gameBoard->board[row][i].pieceType != pieceType) {
                    rowMatch = false;
                }
            }

            //need to check in column
            for(int i=0;i<gameBoard->size;i++) {
                if(symbolMapping[gameBoard->board[i][col].pieceType] == "" || gameBoard->board[i][col].pieceType != pieceType) {
                    columnMatch = false;
                }
            }

            //need to check diagonals
            for(int i=0, j=0; i<gameBoard->size;i++,j++) {
                if(symbolMapping[gameBoard->board[i][j].pieceType] == "" || gameBoard->board[i][j].pieceType != pieceType) {
                    diagonalMatch = false;
                }
            }

            //need to check anti diagonals
            for(int i=0, j=gameBoard->size-1; i<gameBoard->size;i++,j--) {
                if(symbolMapping[gameBoard->board[i][j].pieceType] == "" || gameBoard->board[i][j].pieceType != pieceType) {
                    antiDiagonalMatch = false;
                }
            }

            return rowMatch || columnMatch || diagonalMatch || antiDiagonalMatch;
        }
};

int main(){
    TicTacToe game;
    game.initializeGame();
    std::string winner = game.startGame();
    if(winner=="tie"){
        std::cout<<"Game is "<<winner<<std::endl;
    }
    else{
        std::cout<<"Game winner is "<<winner<<std::endl;
    }
    return 0;
}