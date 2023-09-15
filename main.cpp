// Liberia usada para trabajar con funciones de entrada y salida
#include <iostream>


/** Crea un tablero (board) que sera usado para el juego.*/
int** createBoard(int columns, int rows){
    // TODO: ver porque no funciona con int board[columns][rows];

    // Crea el tablero que voy a usar (sus celdas no tienen info)
    int** board = 0;

    // Recorre cada columna...
    board = new int*[columns];
    for (int colum = 0; colum < columns; colum++){

        // ... en cada columna, recorre cada fila (Es decir, va celda por celda)
        board[colum] = new int[rows];
        for (int row = 0; row < rows; row++){

            // ... en cada celda asigna el valor de casillero vacio.
            board[colum][row] = 0;
        }
    }
    return board;
}

/** Muestra el tablero (board) que sera usado para el juego.*/
void printBoard(int** board,int columns,int rows){
    printf("TABLERO DE JUEGO: \n");

    // Lo que se agregue en esta seccion se imprimie una sola vez
    std::cout << "  |";
    for (int column = 0; column < columns; column++){
        if (column < 10){
            std::cout << "0" << column << "|";
        }
        else{
            std::cout << column  << "|";
        }
    }
    std::cout << "\n";

    // TODO: Por algun motivo la impresion del tablero se debe hacer opuesta a su creacion. Revisar luego.
    for (int row = 0; row < rows; row++){
        // Lo que se agregue en esta seccion se imprimie en cada linea
        if (row < 10){
            std::cout << "0" << row << "|";
        }
        else{
            std::cout << row  << "|";
        }

        for (int column = 0; column < columns; column++){
            // Lo que se agregue en esta seccion se imprime en cada celda
            printf(" %i ", board[column][row]);
        }
        printf("|\n");
    }
}

/** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
int** placeChest(int** board,int player){
    const int EMPTY = 0;
    const int CHEST1 = 1;
    const int CHEST2 = -1;
    const int BOTHCHESTS = 3;

    int column, row;
    std::cout << "Jugador "<< player <<", elige la columna donde colocaras el nuevo tesoro: \n";
    std::cin >> column;
    std::cout << "Elige la fila donde colocaras este tesoro: \n";
    std::cin >> row;
    std::cout << "Eligiste la columna "<< column <<" y la fila "<< row << " para esconder este tesoro.\n";
    if (board[column][row] == EMPTY){
        board[column][row] = player;
    }
    else{
        if (board[column][row] == CHEST1 || board[column][row] == CHEST2){
            board[column][row] = BOTHCHESTS;
        }
    }

    return board;
}

/** Solicita al jugador ingresar todos los tesoros iniciales*/
int** placeChests(int** board){
    const int CHESTQUANTITY = 4;

    // El jugador 1 coloca sus tesoros primero
    int player = 1;
    for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
        board = placeChest(board,player);
    }

    // El jugador 2 coloca sus tesoros depues
    player = 2;
    for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
        board = placeChest(board,player);
    }
    return board;
}

/** Solicita al jugador ingresar una celda para colocar su espia*/
void placeTheSpy(){
}

/** Pregunta al jugador si desea mover un tesoro*/
void moveTheChesty(){
}

/** Exporta el tablero para que solo el jugador correspondiente vea los valores*/
void exportBoard(){
}

/** Determina la sucesion de acciones que se realizan cada turno*/
void playTheTurn(){

}

int main() {
    // Determina que valor esta jugando.
    int player = 1;

    // El tamaño del tablero es constante. Por lo tanto tambien sus filas y columnas
    const int COLUMNS = 20, ROWS = 20;

    // Crea el tablero que vamos a usar
    int **myBoard = createBoard(COLUMNS, ROWS);

    // Muestra por consola el estado del tablero (Esto no sera visible para los jugadores)
    printBoard(myBoard, COLUMNS, ROWS);

    // Solicita al usuario que ingrese sus tesoros.
    myBoard = placeChests(myBoard);

    // Muestra por consola el estado del tablero (Esto no sera visible para los jugadores)
    printBoard(myBoard, COLUMNS, ROWS);

    // Turno
    playTheTurn();
    /** TURNO:
     *  - el jugador coloca un espia.
     *  - el jugador recibe una notificacion de tesoro.
     *  - el jugador mueve un tesoro.
     *  - se exporta su tablero para que lo vea al terminar.
     */

}

