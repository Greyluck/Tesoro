// Liberia usada para trabajar con funciones de entrada y salida
#include <iostream>


//------------------------------------------------------------------------------------------------------------
// Esta seccion gestiona las constantes globales.
//------------------------------------------------------------------------------------------------------------
// Constantes que determinan los valores de las celdas
const int EMPTY = 0;           // No hay tesoro
const int CHEST= 6;            // Hay tesoro
const int DIGGINGTIME = 5;     // Cavando
const int SPY = 7;             // Espia
const int CHESTS = 8;          // Hay 2 tesoros

// Configuraciones de juego:
const int CHESTQUANTITY = 5;       // Cantidad de cofres por jugador //TODO: Actualizar
const int COLUMNS = 20, ROWS = 20; // Tamaño del tablero.

// Variables para el funcionamiento del juego
const std::string PLAYER1NAME = "blanco";           // Nombre del jugador 1
const std::string PLAYER2NAME = "negro";            // Nombre del jugador 2
int playerOneRemainingChests = CHESTQUANTITY;       // Cofres restantes del jugador 1
int playerTwoRemainingChests = CHESTQUANTITY;       // Cofres restantes del jugador 1
bool victory = false;                               // Condicion de victoria.


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


//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga exportar la info para archivos externos.
//------------------------------------------------------------------------------------------------------------
/** Exporta el tablero para que solo el jugador correspondiente vea los valores*/
void exportBoard(int** board) {
    //--------------------------------------------------------
    FILE *masterFile;
    FILE *playerOneFile;
    FILE *playerTwoFile;

    masterFile    = fopen("../Boards/ResultadoMaestros.csv", "w");
    playerOneFile = fopen("../Boards/Resultado1.csv",        "w");
    playerTwoFile = fopen("../Boards/Resultado2.csv",        "w");

    for (int row = 0; row < ROWS; row++){
        for (int column = 0; column < COLUMNS; column++){
            // Imprime toda la info
            fprintf(masterFile, "|%i", board[column][row]);

            // Imprime la info que el jugador 1 puede ver.
            if (board[column][row]==-SPY || board[column][row]==-CHEST){
                fprintf(playerOneFile, "|%i", 0);
            } else {
                fprintf(playerOneFile, "|%i", board[column][row]);
            }

            // Imprime la info que el jugador 2 puede ver.
            if (board[column][row]==SPY || board[column][row]==CHEST){
                fprintf(playerTwoFile, "|%i", 0);
            } else {
                // Si es espia o cofre, le invierte el signo para que el jugador 2 lo vea normal
                if (board[column][row]==-SPY || board[column][row]==-CHEST){
                    fprintf(playerTwoFile, "|%i", -board[column][row]);
                } else {
                    fprintf(playerTwoFile, "|%i", board[column][row]);
                }

            }

        }
        fprintf(masterFile, "|\n");
        fprintf(playerOneFile, "|\n");
        fprintf(playerTwoFile, "|\n");
    }
    //--------------------------------------------------------
    fclose(masterFile);
    fclose(playerOneFile);
    fclose(playerTwoFile);
}



//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga de crear o imprir el tablero
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

    // Exporta los tableros
    exportBoard(board);
}

void updateBoard(int** board){
    // Vacia el contador de cofres, para ir sumandolos a medida que aparescan
    playerOneRemainingChests = 0;
    playerTwoRemainingChests = 0;

    // Recorre todas las celdas
    for (int column = 0; column < COLUMNS ; column++) {
        for (int row = 0; row < ROWS ; row++){
            // Procede a cavar
            if (0<board[column][row] && board[column][row]<6)
                board[column][row]--;

            // Si hay un cofre lo cuento
            if (board[column][row]==CHEST){
                playerOneRemainingChests++;
            }
            if (board[column][row]==-CHEST){
                playerTwoRemainingChests++;
            }
            if (board[column][row]==CHESTS){
                playerOneRemainingChests++;
                playerTwoRemainingChests++;
            }
        }
    }

    if (playerOneRemainingChests <= 0){
        std::cout << "Gano el jugador "<<PLAYER2NAME<<".\n";
        victory = true;
    } else {
        if (playerTwoRemainingChests <= 0) {
            std::cout << "Gano el jugador " << PLAYER1NAME << ".\n";
            victory = true;
        }
    }
}


//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga de mover y colocar cofres y espias
//------------------------------------------------------------------------------------------------------------
/** Solicita al jugador ingresar una celda para colocar un unico tesoro*/
int** placeChest(int** board,int player){
    // TODO: Se requiere verificar que los jugadores no sean capaces de colocar tesoros sobre sus campos llenos.

    std::string curentplayerName;
    bool alocatedChest=false;

    if (player == 1){
        curentplayerName = PLAYER1NAME;
    } else {
        curentplayerName = PLAYER1NAME;
    }
    int column;
    int row;

    while (!alocatedChest){
        // Consulta de cordenada ------------------------------
        std::cout << " - Donde colocaras el tesoro?:\n";
        Coordinate myCoordinate = askForCoordinate();
        column = myCoordinate.vertical;
        row    = myCoordinate.horizontal;
        // ----------------------------------------------------

        // Si la celda esta vacia graba el cofre con el signo de su jugador
        if (board[column][row] == EMPTY){
            board[column][row] = CHEST*player;
            alocatedChest=true;
        }
        else{
            // Si la celda tiene un cofre del otro jugador
            if (board[column][row] == -CHEST){
                board[column][row] = CHESTS; // Asigna el valor para "Cofres dobles"
                alocatedChest=true;
            }
            else {
                // Si se trata de colocar un tesoro en una celda con espia enemigo
                if (board[column][row] == -SPY){
                    std::cout << "   - La celda tiene un espia enemigo. Prueba en otro lado"<<"\n";
                    placeChest(board,player);
                } else {
                    // Si se trata de colocar un tesoro en una celda con un tesoro o espia propio
                    std::cout << "   - La celda esta ocupada, intenta con otra."<<"\n\n";
                }
            }
        }
    }
    // Si la celda era correcta:
    std::cout << " - Tesoro enterrado en columna "<< column <<" fila " << row << "\n\n";

    printBoard(board, COLUMNS, ROWS);
    return board;
}

/** Solicita al jugador ingresar todos los tesoros iniciales*/
int** placeInitialChests(int** board){
    int player = 1; // Comienza el jugador blanco
    std::string curentplayerName;

    // Por cada cofre inicial ejecuto la colocacion del mismo por turnos.
    for (int chest = 1; chest <= CHESTQUANTITY*2; chest++) {
        // Defino que jugador esta jugando.
        if (player == 1){
            curentplayerName = PLAYER1NAME;
        } else {
            curentplayerName = PLAYER2NAME;
        }

        // Indico el inicio de su turno
        std::cout << "Turno del jugador " << curentplayerName <<".\n";

        // Coloco un cofre
        board = placeChest(board,player);

        // Cambio de jugador
        player = -player;
    }
    return board;
}

/** Solo se utiliza cuando se mueve un cofre propio para sacarlo de casilla. .*/
int ** removeChest(int** board,int player,int column, int row){
    // Hay mas de un tesoro?
    if (board[column][row] == CHESTS){
        // Si, hay mas de un tesoro...
        board[column][row] = CHEST * -player; // ...dejo el del rival
    } else {
        // No, esta el mio...
        if (board[column][row] == CHEST * player){
            board[column][row] = SPY; // ...me quedo en la casilla de donde saque el tesoro.
        }
    }
    std::cout << " - Se saco el tesoro de la casilla "<< column <<"." << row << "\n";
    return board;
}

/** Mueve los tesoros*/
int** moveChest(int** board,int player,int column, int row){
    // TODO: IMPORTANTE: Los cofres deberian poder moverse solamente a celdas adyacentes,
    // TODO: para ello se debera comparar los valores de la columna y la fila y ver que la diferencia con la
    // TODO: posicion sea de 1. Por el momento se lo dejara con posicionamiento libre.

    std::cout << "Moviendo el tesoro que estaba en "<< column <<"|" << row << "...\n";

    // Saca el cofre de la posicion que estaba y lo remplaza por el espia.
    removeChest(board,player,column,row);

    // Solicita que ingreses las nuevas coordenas del cofre.
    placeChest(board,player);
    //TODO: El place debe verificar que no se lo coloque sobre un espia,si es asi, el espia cava.

    // Regresa el tablero
    return board;
}

/** Setea una asilla para ser una zona de excavacion, durante la cual no se puede seleccionar.*/
int** digForTreasure (int** board,int player,int column, int row){
    std::cout << "Desenterrando el tesoro rival de la fila "<< column <<"|" << row << "...\n";
    board[column][row] = DIGGINGTIME;
    return board;
}


/** Solicita al jugador ingresar una celda para colocar su espia*/
int** placeTheSpy(int** board,int player){
    // Determina si el usuario ya movio un tesoro.
    bool chestMoved = false;
    bool spyPlaced = false;

    while (!spyPlaced){
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
            spyPlaced=true;

            if(board[column][row] == CHEST * player){
                std::cout << " pero era nuestro. Necesitamos moverlo.\n\n";
                board = moveChest(board,player,column,row);     // Debo mover mi tesoro.
                chestMoved = true;
                spyPlaced=true;
            }

            if(board[column][row] == CHEST * -player){
                std::cout << " y es del rival! Esta a extrayendo el tesoro!\n\n";
                board = digForTreasure(board,player,column,row);     // Empezamos a cavar
                spyPlaced=true;
            }

            if(board[column][row] == CHESTS){
                std::cout << " de echo hay varios.";
                std::cout << "... Necesitamos mover el nuestro.\n";
                board = moveChest(board,player,column,row);          // Debo mover mi tesoro...
                board = digForTreasure(board,player,column,row);     // ... y empezar a cavar
                chestMoved = true;
                spyPlaced = true;

                std::cout << "... ya podemos empezar a cavar\n";
                board = digForTreasure(board,player,column,row);     // Empezamos a cavar
            }
        } else { // NO HABIA TESOROS
            // Hay otro espia?
            if (board[column][row] == SPY* -player){ // Si, nos matamos.
                std::cout << "       - Hay otro espia! Ambos mueren.\n";
                board[column][row]=EMPTY;
                printBoard(board, COLUMNS, ROWS);
                spyPlaced=true;
            }

            if (board[column][row] == SPY* player){ // Si, pero es nuestro.
                std::cout << "       - Hay otro espia... pero es nuestro, me voy a otro lado.\n";
                //NOTA: ACA NO LLEVA "spyPlaced=true;"
            }

            else {
                std::cout << "       - No hay nada aca, me quedo patruyando.\n";
                board[column][row] = SPY * player;
                printBoard(board, COLUMNS, ROWS);
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

    }


    return board;
}



//------------------------------------------------------------------------------------------------------------
// Esta seccion se encarga de manejar los turnos
//------------------------------------------------------------------------------------------------------------

/** Determina la sucesion de acciones que se realizan en un turno*/
void playTheTurn(int** board,int player){
    // Determina el nombre del jugador
    std::string curentplayer;
    if (player == 1){
        curentplayer = PLAYER1NAME;
    } else {
        curentplayer = PLAYER2NAME;
    }
    std::cout << "Turno del jugador "<< curentplayer <<"\n";

    // El jugador coloca un espia.
    placeTheSpy(board,player);

}

//------------------------------------------------------------------------------------------------------------
// Esta seccion ejecuta el programa
//------------------------------------------------------------------------------------------------------------
int main() {
    // Determinan el bando del jugador. (Positivo si es blanco, negativo si es negro)
    int curentplayer = 1; // Comienza el blanco

    // Crea el tablero que vamos a usar
    int **myBoard = createBoard(COLUMNS, ROWS);

    // Create the file
    exportBoard(myBoard);

    // Muestra el tablero (No sera visible para jugadores)
    printBoard(myBoard, COLUMNS, ROWS);

    // Solicita a ambos usuarios que ingresen sus tesoros.
    myBoard = placeInitialChests(myBoard);

    // Turnos
    while (!victory){
        playTheTurn(myBoard,curentplayer);
        updateBoard(myBoard);
        curentplayer = -curentplayer;
    }
}

