#ifndef TESORO_BOARD_H
#define TESORO_BOARD_H
#include <iostream>

class Board {
    //------------------------------------------------------------------------------------------------------------
    // Constante y variables.
    //------------------------------------------------------------------------------------------------------------
    // Constantes que determinan los valores de las celdas del tablero
        const int EMPTY = 0;           // No hay tesoro
        const int CHEST= 6;            // Hay tesoro
        const int DIGGINGTIME = 5;     // Cavando
        const int SPY = 7;             // Espia
        const int CHESTS = 8;          // Hay 2 tesoros

    // Variables que determinan el tamaño del tablero y la cantidad de cofres
        int CHESTQUANTITY = 4;       // Cantidad de cofres por jugador
        int COLUMNS = 20, ROWS = 20; // Tamaño del tablero por defecto.

    // El modo debug se utiliza para mostrar u ocultar el tablero por consola.
    // (Se lo puede activar cambiando su estado a true.)
        const bool DEBUGMODE = true; // false/False

    // Variables para el funcionamiento del juego
        const std::string PLAYER1NAME = "blanco";           // Nombre del jugador 1
        const std::string PLAYER2NAME = "negro";            // Nombre del jugador 2

    // Cantidad de cofres en juego (Condicion de victoria)
        int playerOneRemainingChests = CHESTQUANTITY;       // Cofres restantes del jugador 1
        int playerTwoRemainingChests = CHESTQUANTITY;       // Cofres restantes del jugador 1




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
