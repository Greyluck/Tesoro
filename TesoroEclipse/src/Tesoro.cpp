#include <iostream>
#include "Board.h"
using namespace std;

int main() {
	cout << "Starting program" << endl;
	Board theBoard;
    theBoard.setInitialValues(20,20,5);//TODO: Setear la cantidad de cofres
    theBoard.createMatrix();

    // Solicita a ambos usuarios que ingresen sus tesoros.
    theBoard.placeInitialChests();


    return 0;
}
