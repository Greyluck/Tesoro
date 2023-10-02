#include <iostream>
#include "Board.h"

//------------------------------------------------------------------------------------------------------------
// PRINCIPAL | Esta seccion ejecuta el programa
//------------------------------------------------------------------------------------------------------------
bool DEBUG = true;

int main() {
    // Creamos el tablero que vamos a usar defininedo las filas y columnas
    Board theBoard;
    theBoard.setInitialValues(20,20,10);
    theBoard.createMatrix();

    // Solicita a ambos usuarios que ingresen sus tesoros.
    theBoard.placeInitialChests();

    // Turnos
    while (!theBoard.returnVictoryStatus()){
        theBoard.playTheTurn();
    }
    // borrar el tablero
    //theBoard.deleteBoard(myBoard);

};