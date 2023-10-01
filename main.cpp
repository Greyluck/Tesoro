#include <iostream>
#include "Board.h"

//------------------------------------------------------------------------------------------------------------
// PRINCIPAL | Esta seccion ejecuta el programa
//------------------------------------------------------------------------------------------------------------
int main() {
    //TODO: Checkear si esta bien setearlas aca o debe ir dentro de la clase tablero
    int COLUMNS = 20, ROWS = 20; // Tamaño del tablero por defecto.

    // Determinan el bando del jugador. (Positivo si es blanco, negativo si es negro)
    int curentplayer = 1; // Comienza el blanco

    // Creamos el tablero que vamos a usar defininedo las filas y columnas
    Board theBoard;
    int** myBoard = theBoard.createBoard(COLUMNS, ROWS);

    // Create the file
    theBoard.exportBoard(myBoard);

    // Muestra el tablero (No sera visible para jugadores)
    theBoard.printBoard(myBoard, COLUMNS, ROWS);

    // Solicita a ambos usuarios que ingresen sus tesoros.
    myBoard = theBoard.placeInitialChests(myBoard);

    // Turnos
    while (theBoard.victory){
        theBoard.playTheTurn(myBoard,curentplayer);
        theBoard.updateBoard(myBoard);
        curentplayer = -curentplayer;
    }
    // borrar el tablero
    theBoard.deleteBoard(myBoard);

}
