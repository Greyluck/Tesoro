// Liberia usada para trabajar con funciones de entrada y salida
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>


//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga del manejo de coordenadas.
//------------------------------------------------------------------------------------------------------------
struct Coordinate{
    int vertical;
    int horizontal;
};

/** Solicita un float de 4 cifras y devuelve una coordenada (Con valor vertical y horizontal) */
Coordinate askForCoordinate(){
    float inputCoordinate;
    Coordinate myCordinate{};
    std::cout << "Ingrese la coordenada: \n";
    std::cin >> inputCoordinate;
    // Extrae el valor entero  (vertical)
    myCordinate.vertical = (int)inputCoordinate;
    // Extrae el valor decimal (horizontal)
    /** Al valor ingresado, le quita el entero, dejando solo lo decimal y lo multiplica por 100*/
    myCordinate.horizontal =  (int)((inputCoordinate-myCordinate.vertical)*100);
    return myCordinate;
}
//------------------------------------------------------------------------------------------------------------


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
    printf("                       TABLERO DE JUEGO: \n");

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

    // TODO: Por algun motivo la impresion del tablero se debe hacer opuesta a su creacion. (Revisar luego).
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
    printf("\n");
}

/** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
int** placeChest(int** board,int player){
   // Determinan el tipo de celda
    const int EMPTY = 0;
    const int CHEST = 6;
    const int CHESTS = 8;
    std::string currentPlayer;

    if (player == 1){
        currentPlayer = "blanco";
    } else {
        currentPlayer = "negro";
    }

    // Le solicita al usuario ingresar la coordenada del cofre.
    int column, row;
    std::cout << "Jugador "<< currentPlayer <<", elige la columna donde colocaras el nuevo tesoro: \n";
    std::cin >> column;
    std::cout << "Elige la fila donde colocaras este tesoro: \n";
    std::cin >> row;
    std::cout << "Eligiste la columna "<< column <<" y la fila "<< row << " para esconder este tesoro.\n";

    // Si la celda esta vacia graba el cofre con el signo de su jugador
    if (board[column][row] == EMPTY){
        board[column][row] = CHEST*player;
    }
    else{
        // Si la celda tiene un cofre del otro jugador
        if (board[column][row] == -CHEST){
            board[column][row] = CHESTS; // Asigna el valor para "Cofres dobles"
        }
    }
    return board;
}

/** Solicita al jugador ingresar todos los tesoros iniciales*/
int** placeChests(int** board){
    const int CHESTQUANTITY = 4;

    int player = 1; // Comienza el jugador blanco
    // El jugador blanco coloca sus tesoros primero
        for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
        board = placeChest(board,player);
    }

    player = -player; // Cambio de jugador (Juega el negro)
    // El jugador negro coloca sus tesoros depues
    for (int chest = 1; chest <= CHESTQUANTITY; chest++) {
        board = placeChest(board,player);
    }
    return board;
}

/** Saca un tesoro de su casilla*/
int ** removeChest(int** board,int player,int column, int row){
    //TODO (TENER EN CUENTA QUE SI SE SACA UN TESORO DE UNA CASILLA CON DOBLE TESORO DEBE QUEDAR EL OTRO)
    return board;
}

/** Mueve los tesoros*/
int** moveChest(int** board,int player,int column, int row){
    // Saca el cofre de la posicion que estaba.
    removeChest(board,player,column,row);

    // Solicita que ingreses donde moveras el tesoro
    placeChest(board,player);
    //TODO: El place debe verificar que no se lo coloque sobre un espia,si es asi, el espia cava.

    // Regresa el tablero
    return board;
}

/** Solicita al jugador ingresar una celda para colocar su espia*/
int** placeTheSpy(int** board,int player){

    // Constantes que determinan los valores de las celdas
    const int EMPTY = 0;        // No hay tesoro
    const int CHEST = 6;        // Hay tesoro
    const int FOUND = 5;        // Cavando
    const int SPY = 7;          // Espia
    const int BOTHCHESTS = 8;   // Hay 2 tesoros

    // Determina si el usuario ya movio un tesoro.
    bool chestMoved = false;

    int column, row;
    std::cout << "Jugador "<< player <<", elige la columna donde colocaras tu espia: \n";
    std::cin >> column;
    std::cout << "Elige la fila donde colocaras tu espia: \n";
    std::cin >> row;
    std::cout << "Eligiste la columna "<< column <<" y la fila "<< row << " para colocar tu espia.\n";

    // Hay dos tesoros?
    if (board[column][row] == BOTHCHESTS){   // Si, hay 2 tesoros
        board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
        chestMoved = true;
        board[column][row]= CHEST * -player; // Queda el de mi rival
    }
    // No hay 2 tesoros.
    else {
        // Estoy sobre mi tesoro?
        if (board[column][row] == CHEST*player){ //Estoy sobre mi tesoro
            board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
            board[column][row]=EMPTY;
        }
    }

    // Hay otro espia?
    if (board[column][row] == SPY* -player){ // Si, nos matamos.
        board[column][row]=EMPTY;
    } else {
        // Hay tesoro enemigo?
        if (board[column][row] == CHEST * -player) { // Si, me pongo a cavar.
            board[column][row] = FOUND * player;     // Cavando
            // TODO: Agregar notificacion al jugador de que se encontro un tesoro.
        } else {
            // Sino hay ni tesoro mi, ni del enemigo, ni otro espia, me quedo quieto.
            board[column][row] = SPY * player;
        }
    }

    // Sino movi mi tesoro, puedo moverlo ahora
    if (!chestMoved){
        //TODO: Ask the user to move the chest
        board = moveChest(board,player,column,row);
    }


    return board;
}
/** Pregunta al jugador si desea mover un tesoro*/
void moveTheChesty(){
}

/** Exporta el tablero para que solo el jugador correspondiente vea los valores*/
void exportBoard(){
}

/** Determina la sucesion de acciones que se realizan cada turno*/
void playTheTurn(int** board,int player){
    // El jugador coloca un espia.
    placeTheSpy(board,player);
      /** TURNO:
      *  - el jugador mueve un tesoro.
      *  - se exporta su tablero para que lo vea al terminar.
      */
    }

int main() {
    // Solicita coordinadas
    Coordinate myCoordinate = askForCoordinate();
    std::cout << myCoordinate.vertical << "|" << myCoordinate.horizontal<< "\n";

    // Determinan el bando del jugador. (Positivo si es blanco, negativo si es negro)
    int currentPlayer = 1; // Comienza el blanco

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
    playTheTurn(myBoard,currentPlayer);
    /** TURNO:
     *  - el jugador coloca un espia.
     *  - el jugador recibe una notificacion de tesoro.
     *  - el jugador mueve un tesoro.
     *  - se exporta su tablero para que lo vea al terminar.
     */


}

