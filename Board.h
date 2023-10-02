#ifndef TESORO_BOARD_H
#define TESORO_BOARD_H
#include <iostream>

class Board {
    // Metodos
public:

    /** Setea los valores iniciales del juego*/
    static void setInitialValues(int columns, int rows, int chestQuantity);

    /** Crea un tablero (board) que sera usado para el juego.*/
    static void createMatrix();

    /** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
    static void placeChest();

    /** Solicita al jugador ingresar todos los tesoros iniciales*/
    static void placeInitialChests();

    static void printBoard();
    static void exportBoard();
};


#endif //TESORO_BOARD_H
