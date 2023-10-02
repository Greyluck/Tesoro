#include <cstdio>
#include "Board.h"
#include "Coordinate.h"

using namespace std;
// Variables
int columns, rows, chestQuantity;
int curentPlayer;
int** matrix;
std::string curentplayerName, player1Name,player2Name;
int emptyFieldValue,chestFieldValue,multipleChestFieldValue,spyFieldValue,diggingFieldValue;
bool debug;
bool victory;

//------------------------------------------------------------------------------------------------------------
// TABLERO | Esta seccion se encarga de crear o imprir el tablero
//------------------------------------------------------------------------------------------------------------
void Board::setInitialValues(int mycolumns, int myrows, int mychestQuantity){
    // Game values
    columns=mycolumns;
    rows=myrows;
    chestQuantity=mychestQuantity;
    debug = true;

    // Player values
    curentPlayer=1;
    player1Name = "Blanco";
    player2Name = "Negro";
    curentplayerName = player1Name;

    // Field values
    emptyFieldValue=0;
    chestFieldValue=6;
    spyFieldValue=7;
    multipleChestFieldValue=8;
    diggingFieldValue=5;

    // Condicion de victora:
    victory = false;
}

bool Board::returnVictoryStatus(){
    return victory;
}

void Board::swapCurentPlayer(){
    curentPlayer = -curentPlayer;
}

void Board::createMatrix(){
    // Recorre cada columna...
    matrix = new int*[columns];
    for (int colum = 0; colum < columns; colum++){

        // ... en cada columna, recorre cada fila (Es decir, va celda por celda)
        matrix[colum] = new int[rows];
        for (int row = 0; row < rows; row++){

            // ... en cada celda asigna el valor de casillero vacio.
            matrix[colum][row] = 0;
        }
    }
}

void Board::placeChest(){
    if (debug){
        printBoard();
    }

    // TODO: Se requiere verificar que los jugadores no sean capaces de colocar tesoros sobre sus campos llenos.
    bool alocatedChest=false;

    if (curentPlayer == 1){
        curentplayerName = player1Name;
    } else {
        curentplayerName = player2Name;
    }
    int column;
    int row;

    while (!alocatedChest){
        // Consulta de cordenada ------------------------------
        std::cout << " - Donde colocaras el tesoro?:\n";
        Coordinate myCoordinate;
        myCoordinate.askForCoordinate(columns,rows);
        column = myCoordinate.vertical;
        row    = myCoordinate.horizontal;
        std::cout << "    - Coordenada definada en " << column << "|" <<row<<":\n";

        // ----------------------------------------------------

        // Si la celda esta vacia graba el cofre con el signo de su jugador
        if (matrix[column][row] == emptyFieldValue){
            matrix[column][row] = chestFieldValue*curentPlayer;
            alocatedChest=true;
        }
        else{
            // Si la celda tiene un cofre del otro jugador
            if (matrix[column][row] == -chestFieldValue){
                matrix[column][row] = multipleChestFieldValue; // Asigna el valor para "Cofres dobles"
                alocatedChest=true;
            }
            else {
                // Si se trata de colocar un tesoro en una celda con espia enemigo
                if (matrix[column][row] == -spyFieldValue){
                    std::cout << "   - La celda tiene un espia enemigo. Prueba en otro lado"<<"\n";
                    placeChest();
                } else {
                    // Si se trata de colocar un tesoro en una celda con un tesoro o espia propio
                    std::cout << "   - La celda esta ocupada, intenta con otra."<<"\n\n";
                }
            }
        }
    }
    // Si la celda era correcta:
    std::cout << " - Tesoro enterrado en columna "<< column <<" fila " << row << "\n\n";
}

void Board::placeInitialChests() {
    // Por cada cofre inicial ejecuto la colocacion del mismo por turnos.
    for (int chest = 1; chest <= chestQuantity * 2; chest++) {
        // Indico el inicio de su turno
        std::cout << "Turno de " << curentplayerName << ".\n";

        // Coloco un cofre
        placeChest();

        // Cambio de jugador
        swapCurentPlayer();
    }
}


void Board::printBoard(){
    if (debug){
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

        // Por algun motivo la impresion del tablero se debe hacer opuesta a su creacion.
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
                if (matrix[column][row]>=0){
                    printf(" %i ", matrix[column][row]);
                } else { // Jugador Negro, numeros negativos (Hay que hacer lugar para el -)
                    printf("%i ", matrix[column][row]);
                }
            }
            printf("|\n");
        }
        printf("\n");
    }

    // Exporta los tableros
    exportBoard();
}

void Board::exportBoard() {
    //--------------------------------------------------------
    FILE *masterFile;
    FILE *playerOneFile;
    FILE *playerTwoFile;

    masterFile    = fopen("../Boards/ResultadoMaestros.csv", "w");
    playerOneFile = fopen("../Boards/Resultado1.csv",        "w");
    playerTwoFile = fopen("../Boards/Resultado2.csv",        "w");

    for (int row = 0; row < rows; row++){
        // TODO: Revisar que pasa cuando exporta casilla doble
        for (int column = 0; column < columns; column++){
            // Imprime toda la info
            fprintf(masterFile, "|%i", matrix[column][row]);

            // Imprime la info que el jugador 1 puede ver.
            if (matrix[column][row]==-spyFieldValue || matrix[column][row]==-chestFieldValue){
                fprintf(playerOneFile, "|%i", 0);
            } else {
                fprintf(playerOneFile, "|%i", matrix[column][row]);
            }

            // Imprime la info que el jugador 2 puede ver.
            if (matrix[column][row]==spyFieldValue || matrix[column][row]==chestFieldValue){
                fprintf(playerTwoFile, "|%i", 0);
            } else {
                // Si es espia o cofre, le invierte el signo para que el jugador 2 lo vea normal
                if (matrix[column][row]==-spyFieldValue || matrix[column][row]==-chestFieldValue){
                    fprintf(playerTwoFile, "|%i", -matrix[column][row]);
                } else {
                    fprintf(playerTwoFile, "|%i", matrix[column][row]);
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


void Board::playTheTurn(){
    std::cout << "Turno del jugador "<< curentplayerName <<"\n";
    // El jugador coloca un espia.
    placeTheSpy();
    // Actualiza el tabler
    updateBoard();
    // Da paso al siguiente jugador
    swapCurentPlayer();
}

void Board::placeTheSpy(){
    // Determina si el usuario ya movio un tesoro.
    bool chestMoved = false;
    bool spyPlaced = false;

    while (!spyPlaced){
        // Consulta de cordenada ------------------------------
        std::cout << " - Donde colocaras tu espia \n";
        Coordinate myCoordinate;
        myCoordinate.askForCoordinate(columns,rows);
        int column = myCoordinate.vertical;
        int row    = myCoordinate.horizontal;
        std::cout << "   - El espia intentara ubicarse en columna "<< column <<" fila " << row << "\n";
        // ----------------------------------------------------

        // Habia tesoros?
        if ( matrix[column][row] == chestFieldValue* curentPlayer
             || matrix[column][row] == chestFieldValue* -curentPlayer
             || matrix[column][row] == multipleChestFieldValue){
            std::cout << "       - El espia encontro un tesoro...";
            spyPlaced=true;

            if(matrix[column][row] == chestFieldValue* curentPlayer){
                std::cout << " pero era nuestro. Necesitamos moverlo.\n\n";
                moveChest(column,row);     // Debo mover mi tesoro.
                chestMoved = true;
                spyPlaced=true;
            }

            if(matrix[column][row] == chestFieldValue* -curentPlayer){
                std::cout << " y es del rival! Esta a extrayendo el tesoro!\n\n";
                digForTreasure(column,row);     // Empezamos a cavar
                spyPlaced=true;
            }

            if(matrix[column][row] == multipleChestFieldValue){
                std::cout << " de echo hay varios.";
                std::cout << "... Necesitamos mover el nuestro.\n";
                moveChest(column,row);          // Debo mover mi tesoro...
                digForTreasure(column,row);     // ... y empezar a cavar
                chestMoved = true;
                spyPlaced = true;

                std::cout << "... ya podemos empezar a cavar\n";
                digForTreasure(column,row);     // Empezamos a cavar
            }
        } else { // NO HABIA TESOROS
            // Hay otro espia?
            if (matrix[column][row] == spyFieldValue* -curentPlayer){ // Si, nos matamos.
                std::cout << "       - Hay otro espia! Ambos mueren.\n";
                matrix[column][row]=emptyFieldValue;
                printBoard();
                spyPlaced=true;
            }

            if (matrix[column][row] == spyFieldValue* curentPlayer){ // Si, pero es nuestro.
                std::cout << "       - Hay otro espia... pero es nuestro, me voy a otro lado.\n";
                //NOTA: ACA NO LLEVA "spyPlaced=true;"
            }

            else {
                std::cout << "       - No hay nada aca, me quedo patruyando.\n";
                matrix[column][row] = spyFieldValue * curentPlayer;
                printBoard();
            }
        }

        //TODO: Agregar que pasa sino movi el tesoro este turno (Ver version previa).
    }
}

void Board::removeChest(int column, int row){
    // Hay mas de un tesoro?
    if (matrix[column][row] == multipleChestFieldValue){
        // Si, hay mas de un tesoro...
        matrix[column][row] = chestFieldValue * -curentPlayer; // ...dejo el del rival
    } else {
        // No, esta el mio...
        if (matrix[column][row] == chestFieldValue * curentPlayer){
            matrix[column][row] = spyFieldValue; // ...me quedo en la casilla de donde saque el tesoro.
        }
    }
    std::cout << " - Se saco el tesoro de la casilla "<< column <<"." << row << "\n";
}

void Board::moveChest(int column, int row){
    // TODO: IMPORTANTE: Los cofres deberian poder moverse solamente a celdas adyacentes,
    // para ello se debera comparar los valores de la columna y la fila y ver que la diferencia con la
    // posicion sea de 1. Por el momento se lo dejara con posicionamiento libre.

    std::cout << "Moviendo el tesoro que estaba en "<< column <<"|" << row << "...\n";

    // Saca el cofre de la posicion que estaba y lo remplaza por el espia.
    removeChest(column,row);

    // Solicita que ingreses las nuevas coordenas del cofre.
    placeChest();
    //TODO: El place debe verificar que no se lo coloque sobre un espia,si es asi, el espia cava.

    // Regresa el tablero
}

void Board::digForTreasure (int column, int row){
    std::cout << "Desenterrando el tesoro rival de la fila "<< column <<"|" << row << "...\n";
    matrix[column][row] = diggingFieldValue;
}

void Board::updateBoard(){
    // Vacia el contador de cofres, para ir sumandolos a medida que aparescan
    int playerOneRemainingChests, playerTwoRemainingChests = 0;

    // Recorre todas las celdas
    for (int column = 0; column < columns ; column++) {
        for (int row = 0; row < rows ; row++){
            // Procede a cavar (reduce el valor de la celda en 1)
            if (0<matrix[column][row] && matrix[column][row]<6)
                matrix[column][row]--;

            // Si hay un cofre lo cuento
            if (matrix[column][row]==chestFieldValue){
                playerOneRemainingChests++;
            }
            if (matrix[column][row]==-chestFieldValue){
                playerTwoRemainingChests++;
            }
            if (matrix[column][row]==multipleChestFieldValue){
                playerOneRemainingChests++;
                playerTwoRemainingChests++;
            }
        }
    }

    if (playerOneRemainingChests <= 0){
        std::cout << "Gano el jugador "<<curentplayerName<<".\n";
        victory = true;
    } else {
        if (playerTwoRemainingChests <= 0) {
            std::cout << "Gano el jugador " << curentplayerName << ".\n";
            victory = true;
        }
    }
}

//TODO: Revisar si se requiere crear mas metodos para liberar memoria








