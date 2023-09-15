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

    // Crea un array con 20 letras usadas para identificar las casillas.
    std::string letters[20] = {"A","B","C","D","E","F","G","H","I","J",
                       "K","L","M","N","O","P","Q","R","S","T"};


    // Lo que se agregue en esta seccion se imprimie una sola vez
    std::cout << " |";
    for (int column = 0; column < columns; column++){
        std::string myLetter = letters[column];
        std::cout << myLetter;
    }
    std::cout << "\n";

    // TODO: Por algun motivo la impresion del tablero se debe hacer opuesta a su creacion. Revisar luego.
    for (int row = 0; row < rows; row++){
        // Lo que se agregue en esta seccion se imprimie en cada linea
        std::string myLetter = letters[row];
        std::cout << myLetter <<"|";

        for (int column = 0; column < columns; column++){
            // Lo que se agregue en esta seccion se imprime en cada celda
            printf("%i", board[column][row]);
        }
        printf("\n");
    }
    printf("\n");
}

/** Solicita al jugador ingresar una celda para colocar su tesoro*/
void placeChest(){
    char column, row;
    std::cout << "Elige la columna donde colocaras el tesoro: ";
    std::cin >> column;
    std::cout << "Elige la fila donde colocaras el tesoro: ";
    std::cin >> row;
    std::cout << "Eligiste la columna "<< column <<" y la fila "<< row;
}

/** Solicita al jugador ingresar una celda para colocar su epia*/


int main() {
    // El tamaño del tablero es constante. Por lo tanto tambien sus filas y columnas
    const int columns =20, rows = 20;
    
    // Crea el tablero que vamos a usar
    int** myBoard = createBoard (columns,rows);

    // Muesta el tablero que vamos a usar
    printBoard(myBoard,columns,rows);

    // Solicita al usuario que ingrese su tesoro.
    placeChest();
}

