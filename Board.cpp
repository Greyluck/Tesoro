#include <cstdio>
#include "Board.h"
#include "Coordinate.h"

using namespace std;
// Variables
int columns, rows, chestQuantity;
int curentPlayer;
int** matrix;
std::string curentplayerName, player1Name,player2Name;
int emptyFieldValue,chestFieldValue,multipleChestFieldValue,spyFieldValue;
bool debug;

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

    // Field values
    emptyFieldValue=1;
    chestFieldValue=6;
    spyFieldValue=7;
    multipleChestFieldValue=8;
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
        // Defino que jugador esta jugando.
        if (curentPlayer == 1) {
            curentplayerName = player1Name;
        } else {
            curentplayerName = player2Name;
        }

        // Indico el inicio de su turno
        std::cout << "Turno de " << curentplayerName << ".\n";

        // Coloco un cofre
        placeChest();

        // Cambio de jugador
        curentPlayer = -curentPlayer;
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









