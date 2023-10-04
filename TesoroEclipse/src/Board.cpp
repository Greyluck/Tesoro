#include <cstdio>
#include "Board.h"
#include "Coordinate.h"

using namespace std;
// Variables
int columns, rows, chestQuantity;
int curentPlayer;
int** matrix;
std::string curentPlayerName, player1Name,player2Name;
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
    curentPlayerName = player1Name;

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

    // Setea el nombre del jugador
    if (curentPlayer == 1){
        curentPlayerName = player1Name;
    } else {
        curentPlayerName = player2Name;
    }

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
    // Sino recibe argumentos, setea los defaults y llama al metodo completo.
    bool replacing = false;
    int previousColumn, previousRow;
    previousColumn = -1;    // Value out of board
	previousRow    = -1;    // Value out of board
	placeChest(replacing, previousColumn, previousRow);

}

void Board::placeChest(bool replacing, int previousColumn, int previousRow){
    // Debug para ver el tablero.

	if (debug){
        printBoard();
    }

    // Indico el inicio de su turno
	std::cout << "--------------------------------------------------- " << endl;
	std::cout << "Turno de " << curentPlayerName << "." << endl;

    // Crea las variables para las nuevas coordenadas del tesoro.
    int newColumn;
    int newRow;

    // Verifica que se haya colocado el tesoro.
    bool alocatedChest=false;
    while (!alocatedChest){
        // Verifica si el usuario esta moviendo el tesoro o colocandolo por primera vez.
        if (replacing){
            int direction;

            // Verifica que el movimiento este dentro del tabler.
            bool movedInsideBoard = false;
            while (!movedInsideBoard){
                // Consulta la nueva cordenada basada en la anterior
                std::cout << " - Hacia donde movera el tesoro el jugador" << curentPlayerName <<endl;
                std::cout << "   (1) Arriba (2) Abajo (3) Izquierda (4) Derecha:"<<endl;
                std::cin >> direction;
                switch (direction) {
                    case 1:     // Arriba
                        newColumn = previousColumn - 1;
                        newRow = previousRow;
                        break;
                    case 2:     // Abajo
                        newColumn = previousColumn + 1;
                        newRow = previousRow;
                        break;
                    case 3:     // Izq
                        newColumn = previousColumn;
                        newRow = previousRow - 1;
                        break;
                    case 4:     // Der
                        newColumn = previousColumn;
                        newRow = previousRow + 1;
                        break;
                    default:    // Default
                        break;
                }

                // Verifica que la nueva celda este dentro del tablero. Sino lo esta, pregunta de nuevo.
                if (newRow>=0 && newRow<rows && newColumn>=0 && newColumn<columns){
                    movedInsideBoard=true;
                }
            }


        } else {
            // Consulta de cordenada pidiendole a la usuario que la ingrese
            std::cout << " - Donde colocara el tesoro jugador "<< curentPlayerName <<"?:"<<endl;
            Coordinate myCoordinate;
            myCoordinate.askForCoordinate(columns,rows);
            newColumn = myCoordinate.vertical;
            newRow    = myCoordinate.horizontal;
        }
        std::cout << "    - Coordenada definada en " << newColumn << "|" <<newRow<<":"<<endl;

        // ----------------------------------------------------

        // Si la celda esta vacia graba el cofre con el signo de su jugador
        if (matrix[newColumn][newRow] == emptyFieldValue){
            matrix[newColumn][newRow] = chestFieldValue*curentPlayer;
            alocatedChest=true;
        }
        else{
            // Si la celda tiene un cofre del otro jugador
            if (matrix[newColumn][newRow] == -chestFieldValue){
                matrix[newColumn][newRow] = multipleChestFieldValue; // Asigna el valor para "Cofres dobles"
                alocatedChest=true;
            }
            else {
                // Si se trata de colocar un tesoro en una celda con espia enemigo
                if (matrix[newColumn][newRow] == -spyFieldValue){
                    std::cout << "   - La celda tiene un espia enemigo. Prueba en otro lado"<<endl;
                    placeChest();
                } else {
                    // Si se trata de colocar un tesoro en una celda con un tesoro o espia propio
                    std::cout << "   - La celda esta ocupada, intenta con otra."<<"\n"<<endl;
                }
            }
        }
    }
    // Si la celda era correcta:
    std::cout << " - Tesoro enterrado en columna "<< newColumn <<" fila " << newRow << "\n"<<endl;
}

void Board::placeInitialChests() {
    // Por cada cofre inicial ejecuto la colocacion del mismo por turnos.
    for (int chest = 1; chest <= chestQuantity * 2; chest++) {
        std::cout << "----------------------------------------------------------------"<<endl;
        std::cout << "                    Colocando tesoros iniciales."<<endl;
        std::cout << "----------------------------------------------------------------"<<endl;

        // Coloco un cofre
        placeChest();

        // Cambio de jugador
        swapCurentPlayer();
    }
}


void Board::printBoard(){
    if (debug){
    	std::cout << "                       TABLERO DE JUEGO:"<< endl;

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
            std::cout<<"|"<<endl;
        }
        std::cout <<endl;
    }
    // Exporta los tableros
    exportBoard();
}

void Board::exportBoard() {
    //--------------------------------------------------------
    FILE *masterFile;
    FILE *playerOneFile;
    FILE *playerTwoFile;

    masterFile    = fopen("boards/ResultadoMaestros.csv", "w");
    playerOneFile = fopen("boards/Resultado1.csv",        "w");
    playerTwoFile = fopen("boards/Resultado2.csv",        "w");
    //TODO: std::cout << "(Debug: Punto previo al error de exportacion)"<< endl;

    /*//TODO: Por algun motivo dejo de exportar los tableros cuando pase de CLion a Eclipse
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
    */
    //TODO: std::cout << "(Debug: La funcion de guardado de tablero fue removida, ver comentario de la funcion)" << endl;
}


void Board::playTheTurn(){
    std::cout << "----------------------------------------------------------------"<< endl;
    std::cout << "                    Comenzando el juego."<< endl;
    std::cout << "----------------------------------------------------------------"<< endl;
    std::cout << "Turno del jugador "<< curentPlayerName << endl;
    if (debug){
        printBoard();
    }
    // El jugador coloca un espia.
    placeTheSpy();
    // Actualiza el tablero
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
        std::cout << " - Donde colocara el espia el jugador "<< curentPlayerName<<"?"<< endl;
        Coordinate myCoordinate;
        myCoordinate.askForCoordinate(columns,rows);
        int column = myCoordinate.vertical;
        int row    = myCoordinate.horizontal;
        std::cout << "   - El espia intentara ubicarse en:"<< endl;
        std::cout << "      columna "<< column <<" fila " << row << endl;
        // ----------------------------------------------------
        
        // Logica de que pasa cuando elijo donde colocar mi espia ------------------------------
        // Vacio -> Patrullo.
        if (matrix[column][row] == emptyFieldValue){
			std::cout << "       - Aca no hay nada, me quedo patruyando."<< endl;
			// Asigno al espia a la celda
			matrix[column][row] = spyFieldValue * curentPlayer;
			spyPlaced=true;
        } else {
			// Hay otro espia -> Nos matamos
			if (matrix[column][row] == spyFieldValue* -curentPlayer){ // Si, nos matamos.
			   std::cout << "       - Hay otro espia! Ambos mueren."<< endl;
			   // Vacio la celda
			   matrix[column][row]=emptyFieldValue;
			   printBoard();
			   spyPlaced=true;
			} else {

				// Hay tesoro enemigo -> Desentierro
				if ( matrix[column][row] == chestFieldValue * -curentPlayer){
					std::cout << "       - Hay un tesoro rival!, empezando a cavar."<< endl;
					// Empiezo a cavar
					digForTreasure(column,row);
					spyPlaced=true;
				} else {

					// Hay tesoro propio -> Muevo mi tesoro y me coloco.
					if(matrix[column][row] == chestFieldValue* curentPlayer){
						std::cout << "       - Hay un tesoro propio, necesitamos moverlo."<< endl;
						// Debo mover mi tesoro.
						moveChest(column,row);
						chestMoved = true;
						// Asigno al espia a la celda
						matrix[column][row] = spyFieldValue * curentPlayer;;
						spyPlaced=true;
					}
				}
			}
        }
        // ----------------------------------------------------

		// Sino movi mi tesoro al colocar mi espia, lo hago ahora.
		if (!chestMoved){
	        std::cout << " - Hora de mover uno de tus tesoros"<< endl;
			myCoordinate.askForCoordinate(columns,rows);
			int chestColumn = myCoordinate.vertical;
			int chestRow    = myCoordinate.horizontal;
			moveChest(chestColumn,chestRow);
			chestMoved = true;
		}
		std::cout << "Fin de espia"<< endl;
    }

    // Muestra el tablero si estamos en modo debug
    if (debug){
        printBoard();
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

    std::cout << "Hacia donde lo memovemos? "<< column <<"|" << row << "...\n";










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
        std::cout << "Gano el jugador "<<curentPlayerName<<".\n";
        victory = true;
    } else {
        if (playerTwoRemainingChests <= 0) {
            std::cout << "Gano el jugador " << curentPlayerName << "." << endl;
            victory = true;
        }
    }
}

void Board::deleteMatrix(Board board) {
    //Se tiene que borrar cada fila primero, y luego borrar la matrix
    for (int column = 0; column < columns; column++) {
        // Borra las filas
        delete[] matrix[column];
    }
    // Borrar el tablero
    delete[] matrix;
}
//TODO: Revisar si se requiere crear mas metodos para liberar memoria








