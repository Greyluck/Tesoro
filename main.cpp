// Liberia usada para trabajar con funciones de entrada y salida
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>

//------------------------------------------------------------------------------------------------------------
// Esta seccion gestiona las constantes globales.
//------------------------------------------------------------------------------------------------------------
// Constantes que determinan los valores de las celdas
const int EMPTY = 0;        // No hay tesoro
const int CHEST= 6;        // Hay tesoro
const int FOUND = 5;        // Cavando
const int SPY = 7;          // Espia
const int CHESTS = 8;   // Hay 2 tesoros

const std::string WHITE = "blanco";
const std::string BLACK = "negro";

// Configuraciones de juego:
const int CHESTQUANTITY = 2;       // Cantidad de cofres por jugador //TODO: Actualizar
const int COLUMNS = 5, ROWS = 5; // Tamaño del tablero.

bool victory = false;               // Condicion de victoria.


//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga del manejo de coordenadas.
//------------------------------------------------------------------------------------------------------------
struct Coordinate{
    int vertical;
    int horizontal;
};

/** Solicita un int de 4 cifras y devuelve una coordenada (Con valor vertical y horizontal) */
Coordinate askForCoordinate(){
    int aceptedCoordinate = false;
    int inputCoordinate;
    Coordinate myCordinate{};

    while (!aceptedCoordinate){
        std::cout << "   - Ingrese la coordenada: ";
        std::cin >> inputCoordinate;

        // Extrae el valor horizontal
        myCordinate.horizontal = inputCoordinate%100;

        // Extrae el valor vertical
        myCordinate.vertical = (inputCoordinate - myCordinate.horizontal) /100;

        if (myCordinate.vertical >= 0 && myCordinate.horizontal >= 0){
            if (myCordinate.vertical < COLUMNS && myCordinate.horizontal < ROWS){
                std::cout << "  - Valor aceptado.\n";
                aceptedCoordinate = true;
            } else {
                std::cout << "  - Valor fuera del tablero, intente de nuevo: \n";
            }
        } else {
            std::cout << "  - El valor debe ser mayor a 0.\n";
        }
    }

    return myCordinate;
}
//------------------------------------------------------------------------------------------------------------


/** Crea un tablero (board) que sera usado para el juego. (Lo hace usando punteros)*/
int** createBoard(int columns, int rows){
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
    printf("                       TABLERO DE JUEGO: \n");

    // Lo que se agregue en esta seccion se imprimie una sola vez
    std::cout << "  |";

    // Carca los valores del titulo de las columnas
    for (int column = 0; column < columns; column++){
        if (column < 10){
            std::cout << "0" << column << "|";
        }
        else{
            std::cout << column  << "|";
        }
    }
    std::cout << "\n";

    // TODO: Por algun motivo la impresion del tablero se debe hacer opuesta a su creacion.
    for (int row = 0; row < rows; row++){
        // Lo que se agregue en esta seccion se imprimie en cada linea

        if (row < 0){
            std::cout << "0" << row << "|";
        }

        if (row < 10){
            std::cout << "0" << row << "|";
        }
        else{
            std::cout << row  << "|";
        }

        for (int column = 0; column < columns; column++){
            // Lo que se agregue en esta seccion se imprime en cada celda
            if (board[column][row]>=0){
                printf(" %i ", board[column][row]);
            } else { // Jugador Negro, numeros negativos (Hay que hacer lugar para el -)
                printf("%i ", board[column][row]);
            }


        }
        printf("|\n");
    }
    printf("\n");
}

/** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
int** placeChest(int** board,int player){
    std::string currentPlayerName;

    if (player == 1){
        currentPlayerName = WHITE;
    } else {
        currentPlayerName = BLACK;
    }

    // Consulta de cordenada ------------------------------
    std::cout << " - Donde colocaras el tesoro?:\n";
    Coordinate myCoordinate = askForCoordinate();
    int column = myCoordinate.vertical;
    int row    = myCoordinate.horizontal;
    std::cout << " - Tesoro enterrado en columna "<< column <<" fila " << row << "\n\n";
    // ----------------------------------------------------

    // Si la celda esta vacia graba el cofre con el signo de su jugador
    if (board[column][row] == EMPTY){
        board[column][row] = CHEST*player;
    }
    else{
        // Si la celda tiene un cofre del otro jugador
        if (board[column][row] == -CHEST){
            board[column][row] = CHESTS; // Asigna el valor para "Cofres dobles"
        }
        else {
            // Si se trata de colocar un tesoro en una celda con espia
            if (board[column][row] == -SPY){
                std::cout << "La cleda tiene un espia. Prueba en otro lado"<<"\n";
                placeChest(board,player);
            }
        }
    }

    return board;
}

/** Solicita al jugador ingresar todos los tesoros iniciales*/
int** placeInitialChests(int** board){
    int player = 1; // Comienza el jugador blanco
    // El jugador blanco coloca sus tesoros primero
    std::cout << "Turno del jugador blanco.\n";
        for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
            board = placeChest(board,player);
        }

    // Cambio de jugador (Juega el negro)
    player = -player;

    // El jugador negro coloca sus tesoros segundo.
    std::cout << "Turno del jugador negro.\n";
    // El jugador negro coloca sus tesoros depues
    for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
        board = placeChest(board,player);
    }
    return board;
}

/** Solo se utiliza cuando se mueve un cofre propio para sacarlo de casilla. .*/
int ** removeChest(int** board,int player,int column, int row){
    // Hay mas de un tesoro?
    if (board[column][row] == CHESTS){
        // Si, hay mas de un tesoro...
        board[column][row] = CHEST * -player; // ...dejo solo el del rival.
    } else {
        // No, esta el mio...
        if (board[column][row] == CHEST * player){
            board[column][row] = EMPTY; // ...la casilla queda vacia
        }
    }
    std::cout << " - Se saco el tesoro de la casilla "<< column <<"." << row << "\n";
    return board;
}

/** Mueve los tesoros*/
int** moveChest(int** board,int player,int column, int row){
    std::cout << "Moviendo el tesoro que estaba en "<< column <<"." << row << "...\n";

    // Saca el cofre de la posicion que estaba
    removeChest(board,player,column,row);

    // Solicita que ingreses las nuevas coordenas del cofre.
    placeChest(board,player);
    //TODO: El place debe verificar que no se lo coloque sobre un espia,si es asi, el espia cava.

    // Regresa el tablero
    return board;
}

/** Solicita al jugador ingresar una celda para colocar su espia*/
int** placeTheSpy(int** board,int player){
    // Determina si el usuario ya movio un tesoro.
    bool chestMoved = false;

    // Consulta de cordenada ------------------------------
    std::cout << " - Donde colocaras tu espia \n";
    Coordinate myCoordinate = askForCoordinate();
    int column = myCoordinate.vertical;
    int row    = myCoordinate.horizontal;
    std::cout << "   - El espia intentara ubicarse en columna "<< column <<" fila " << row << "\n";
    // ----------------------------------------------------

    // Habia tesoros?
    if ( board[column][row] == CHEST * player
      || board[column][row] == CHEST * -player
      || board[column][row] == CHESTS){
        std::cout << "       - El espia encontro un tesoro...";

        if(board[column][row] == CHEST * player){
            std::cout << " pero era nuestro. Necesitamos moverlo.\n";
            board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
            chestMoved = true;
        }

        if(board[column][row] == CHEST * -player){
            std::cout << " y es del rival! Empezando a cavar\n";
            board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
        }

        if(board[column][row] == CHESTS){
            std::cout << " de echo hay varios.";

            std::cout << "... Necesitamos mover el nuestro.";
            board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
            chestMoved = true;;

            std::cout << "... ya podemos empezar a cavar\n";
            board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
        }
    } else { // NO HABIA TESOROS

        // Hay otro espia?
        if (board[column][row] == SPY* -player){ // Si, nos matamos.
            std::cout << "       - Hay otro espia! Ambos mueren.\n";
            board[column][row]=EMPTY;
        } else {
            std::cout << "       - No hay nada aca, me quedo patruyando.\n";
                board[column][row] = SPY * player;
            }
    }

    //TODO: Esta hardcodeado para que se saltee esta parte por el momento.
    chestMoved=true;
    //--------------------------------------------------------------------------------------
    // Sino movi mi tesoro, puedo moverlo ahora
    if (!chestMoved){
        std::cout << " - Sobro tiempo. Movamos otro tesoro";
        Coordinate myCoordinate = askForCoordinate();
        int selectedColumn = myCoordinate.vertical;
        int selectedRow    = myCoordinate.horizontal;
        board = moveChest(board, player,selectedColumn,selectedRow);
    }//--------------------------------------------------------------------------------------


    return board;
}

/** Exporta el tablero para que solo el jugador correspondiente vea los valores*/
void exportBoard(){
}

/** Determina la sucesion de acciones que se realizan en un turno*/
void playTheTurn(int** board,int player){
    // Determina el nombre del jugador
    std::string currentPlayer;
    if (player == 1){
        currentPlayer = "blanco";
    } else {
        currentPlayer = "negro";
    }
    std::cout << "Turno del jugador "<< currentPlayer <<"\n";

    // El jugador coloca un espia.
    placeTheSpy(board,player);

}

int main() {
    // Determinan el bando del jugador. (Positivo si es blanco, negativo si es negro)
    int currentPlayer = 1; // Comienza el blanco

    // Crea el tablero que vamos a usar y lo muestra (No sera visible para jugadores)
    int **myBoard = createBoard(COLUMNS, ROWS);
    printBoard(myBoard, COLUMNS, ROWS);

    // Solicita a ambos usuarios que ingresen sus tesoros.
    myBoard = placeInitialChests(myBoard);
    printBoard(myBoard, COLUMNS, ROWS);

    // Turnos
    while (!victory){
        playTheTurn(myBoard,currentPlayer);
        printBoard(myBoard, COLUMNS, ROWS);
        currentPlayer = -currentPlayer;
    }
}

