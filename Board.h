#ifndef TESORO_BOARD_H
#define TESORO_BOARD_H
#include <iostream>

class Board {

    //------------------------------------------------------------------------------------------------------------
    // COFRES | Esta seccion se encarga de mover y colocar cofres y espias en el tablero
    //------------------------------------------------------------------------------------------------------------
    /** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
    int** placeChest(int** board,int player);

    /** Se utiliza cuando se mueve un cofre propio para sacarlo de casilla. .*/
    int** removeChest(int** board,int player,int column, int row);

    /** Mueve los tesoros*/
    int** moveChest(int** board,int player,int column, int row);

    /** Setea una casilla para ser una zona de excavacion, durante la cual no se puede seleccionar.*/
    int** digForTreasure (int** board,int player,int column, int row);

    /** Solicita al jugador ingresar una celda para colocar su espia*/
    int** placeTheSpy(int** board,int player);


    public:
    //------------------------------------------------------------------------------------------------------------
    // TABLERO | Esta seccion se encarga de del manejo del tablero
    //------------------------------------------------------------------------------------------------------------
    /** Crea un tablero (board) que sera usado para el juego.*/
    static int** createBoard(int columns, int rows);

    /** Exporta el tablero para que solo el jugador correspondiente vea los valores*/
    void exportBoard(int** board);

    /** Muestra el tablero (board) que sera usado para el juego.*/
    void printBoard(int** board,int columns,int rows);

    /** Solicita al jugador ingresar todos los tesoros iniciales*/
    int** placeInitialChests(int** board);

    /** Limpia la memoria usada para el tablero.*/
    void deleteBoard(int** board);

    /** Determina la sucesion de acciones que se realizan en un turno*/
    void playTheTurn(int** board,int player);

    /** Actualiza el tablero con la informacion nueva. Debe usarse al final de los turnos.*/
    void updateBoard(int** board);

    bool victory = false;
};


#endif //TESORO_BOARD_H
