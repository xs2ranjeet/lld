#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#ifndef ENUMS_H
#define ENUMS_H

enum GameStatus {
    ACTIVE,
    BLACK_WIN,
    WHITE_WIN,
    FORFEIT,
    STALEMATE,
    RESIGNATION
};

enum AccountStatus {
    ACTIVE,
    CLOSED,
    CANCELED,
    BLACKLISTED,
    NONE
};

enum PieceType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    NONE
};

class Address {
public:
    string streetAddress;
    string city;
    string state;
    string zipCode;
    string country;
};

class Person {
public:
    string name;
    Address address;
    string email;
    string phone;
};

class Account : public Person {
public:
    int id;
    string userName;
    string password;
    AccountStatus status;

    bool resetPassword() {
        // Implement password reset logic
        return true;
    }
};

class Admin : public Account {
public:
    bool blockUser(Account account) {
        // Implement block user logic
        return true;
    }

    bool unblockUser(Account account) {
        // Implement unblock user logic
        return true;
    }
};

class Player {
public:
    Person person;
    int totalGamesPlayed;
    bool whiteSide;

    Player(Person person, bool whiteSide) : person(person), whiteSide(whiteSide) {
        totalGamesPlayed = 0;
    }

    bool isWhiteSide() {
        return whiteSide;
    }

    bool isChecked() {
        // Implement check logic
        return false;
    }
};

class Date {
public:
    int day;
    int month;
    int year;
};

class Board;
class Box;

class Piece {
protected:
    bool killed;
    bool white;

public:
    Piece(bool white) : white(white), killed(false) {}

    virtual ~Piece() = default;

    bool isWhite() {
        return white;
    }

    bool isKilled() {
        return killed;
    }

    void setKilled(bool killed) {
        this->killed = killed;
    }

    virtual bool canMove(Board board, Box start, Box end) = 0;
};

class Box {
public:
    int x;
    int y;
    Piece *piece;

    Box(int x, int y, Piece *piece) : x(x), y(y), piece(piece) {}

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    Piece *getPiece() {
        return piece;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }

    void setPiece(Piece *piece) {
        this->piece = piece;
    }
};

class Board {
public:
    Date creationDate;
    vector<vector<Box *>> boxes;

    Board() {
        boxes = vector<vector<Box *>>(8, vector<Box *>(8, nullptr));
        resetBoard();
    }

    Box *getBox(int x, int y) {
        return boxes[x][y];
    }

    void resetBoard() {
        // Set up white pieces
        boxes[0][0] = new Box(0, 0, new Rook(true));
        boxes[0][1] = new Box(0, 1, new Knight(true));
        boxes[0][2] = new Box(0, 2, new Bishop(true));
        boxes[0][3] = new Box(0, 3, new Queen(true));
        boxes[0][4] = new Box(0, 4, new King(true));
        boxes[0][5] = new Box(0, 5, new Bishop(true));
        boxes[0][6] = new Box(0, 6, new Knight(true));
        boxes[0][7] = new Box(0, 7, new Rook(true));
        for (int i = 0; i < 8; i++) {
            boxes[1][i] = new Box(1, i, new Pawn(true));
        }

        // Set up black pieces
        boxes[7][0] = new Box(7, 0, new Rook(false));
        boxes[7][1] = new Box(7, 1, new Knight(false));
        boxes[7][2] = new Box(7, 2, new Bishop(false));
        boxes[7][3] = new Box(7, 3, new Queen(false));
        boxes[7][4] = new Box(7, 4, new King(false));
        boxes[7][5] = new Box(7, 5, new Bishop(false));
        boxes[7][6] = new Box(7, 6, new Knight(false));
        boxes[7][7] = new Box(7, 7, new Rook(false));
        for (int i = 0; i < 8; i++) {
            boxes[6][i] = new Box(6, i, new Pawn(false));
        }

        // Initialize remaining boxes without any piece
        for (int i = 2; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                boxes[i][j] = new Box(i, j, nullptr);
            }
        }
    }
};

class King : public Piece {
private:
    bool castlingDone;

public:
    King(bool white) : Piece(white), castlingDone(false) {}

    bool isCastlingDone() {
        return castlingDone;
    }

    void setCastlingDone(bool castlingDone) {
        this->castlingDone = castlingDone;
    }

    bool canMove(Board board, Box start, Box end) override {
        if (end.getPiece() != nullptr && end.getPiece()->isWhite() == this->isWhite()) {
            return false;
        }
        int x = abs(start.getX() - end.getX());
        int y = abs(start.getY() - end.getY());
        if (x + y == 1) {
            // Check if this move will not result in king being attacked, if so return true
            return true;
        }
        return canCastling(board, start, end);
    }

    bool canCastling(Board board, Box start, Box end) {
        if (this->isCastlingDone()) {
            return false;
        }
        // Logic to check castling
        return true;
    }
};

class Queen : public Piece {
public:
    Queen(bool white) : Piece(white) {}

    bool canMove(Board board, Box start, Box end) override {
        // Implement Queen's move logic
        return true;
    }
};

class Rook : public Piece {
public:
    Rook(bool white) : Piece(white) {}

    bool canMove(Board board, Box start, Box end) override {
        // Implement Rook's move logic
        return true;
    }
};

class Bishop : public Piece {
public:
    Bishop(bool white) : Piece(white) {}

    bool canMove(Board board, Box start, Box end) override {
        // Implement Bishop's move logic
        return true;
    }
};

class Knight : public Piece {
public:
    Knight(bool white) : Piece(white) {}

    bool canMove(Board board, Box start, Box end) override {
        if (end.getPiece() != nullptr && end.getPiece()->isWhite() == this->isWhite()) {
            return false;
        }
        int x = abs(start.getX() - end.getX());
        int y = abs(start.getY() - end.getY());
        return x * y == 2;
    }
};

class Pawn : public Piece {
public:
    Pawn(bool white) : Piece(white) {}

    bool canMove(Board board, Box start, Box end) override {
        if (end.getPiece() != nullptr && end.getPiece()->isWhite() == this->isWhite()) {
            return false;
        }
        int x = abs(start.getX() - end.getX());
        int y = abs(start.getY() - end.getY());
        if (x == 0) {
            if (y == 1) {
                return true;
            } else if (y == 2 && !this->isMoved()) {
                return true;
            }
        }
        return x * y == 2;
    }

    bool isMoved() {
        // Implement logic to check if the pawn has moved
        return false;
    }
};

class Move {
public:
    Box start;
    Box end;
    Piece *pieceKilled;
    Piece *pieceMoved;
    Player player;
    bool isCastlingMove;

    Move(Player player, Box startBox, Box endBox) : player(player), start(startBox), end(endBox), isCastlingMove(false) {
        pieceMoved = start.getPiece();
    }

    void setCastlingMove(bool isCastlingMove) {
        this->isCastlingMove = isCastlingMove;
    }

    bool getIsCastlingMove() {
        return isCastlingMove;
    }
};

class Game {
private:
    Player currentPlayer[2];
    Board board;
    Player currentTurn;
    GameStatus status;
    vector<Move *> movesPlayed;

    void initialize(Player player1, Player player2) {
        currentPlayer[0] = player1;
        currentPlayer[1] = player2;
        board.resetBoard();
        currentTurn = player1.isWhiteSide() ? player1 : player2;
        movesPlayed.clear();
    }

public:
    bool playerMove(Player player, int startX, int startY, int endX, int endY) {
        Box *start = board.getBox(startX, startY);
        Box *end = board.getBox(endX, endY);
        Move *move = new Move(player, *start, *end);
        return makeMove(move, player);
    }

    bool isEnd() {
        return status != ACTIVE;
    }

    GameStatus getStatus() {
        return status;
    }

    void setStatus(GameStatus status) {
        this->status = status;
    }

    bool makeMove(Move *move, Player player) {
        Piece *sourcePiece = move->start.getPiece();
        if (sourcePiece == nullptr) {
            return false;
        }
        if (player != currentTurn) {
            return false;
        }
        if (sourcePiece->isWhite() != player.isWhiteSide()) {
            return false;
        }

        if (!sourcePiece->canMove(board, move->start, move->end)) {
            return false;
        }
        Piece *destinationPiece = move->end.getPiece();
        if (destinationPiece != nullptr) {
            destinationPiece->setKilled(true);
            move->pieceKilled = destinationPiece;
        }

        if (dynamic_cast<King *>(sourcePiece) && !move->getIsCastlingMove()) {
            move->setCastlingMove(true);
        }

        movesPlayed.push_back(move);

        move->end.setPiece(move->start.getPiece());
        move->start.setPiece(nullptr);

        if (destinationPiece != nullptr && dynamic_cast<King *>(destinationPiece)) {
            status = player.isWhiteSide() ? WHITE_WIN : BLACK_WIN;
        }

        currentTurn = (currentTurn == currentPlayer[0]) ? currentPlayer[1] : currentPlayer[0];
        return true;
    }
};

class GameController {
public:
    bool validateMove() {
        // Implement move validation logic
        return true;
    }
};

class GameView : public Game {
public:
    void updateBoard(Board board) {
        // Implement board update logic
    }

    void updateMove(Player player, Box start, Box end) {
        // Implement move update logic
    }

    void printGameStatus(GameStatus status) {
        // Implement game status printing logic
    }

    bool makeMove(Move move) {
        // Implement move making logic
        return true;
    }
};

#endif