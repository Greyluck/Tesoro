#include <iostream>
#include "Coordinate.h"
using namespace std;

Coordinate Coordinate::askForCoordinate(int maxColums,int maxRows){
    Coordinate myCoordinate{};
    int acceptedCoordinate = false;
    while (!acceptedCoordinate) {
        int inputV, inputH;
        // Ingresa la coordenada vertical
        std::cout << "   - Ingrese la coordenada vertical: ";
        std::cin >> inputV;
        // Si la vertical esta bien, pide la horizontal
        if (inputV >= 0 && inputV < maxColums) {
            std::cout << "   - Ingrese la coordenada horizontal: ";
            std::cin >> inputH;
            // Si la horizontal tambeinte esta bien, las asigna y acepta la coordenada
            if (inputH >= 0 && inputH < maxRows) {
                myCoordinate.horizontal = inputH;
                myCoordinate.vertical = inputV;
                acceptedCoordinate = true;
                return myCoordinate;
            } else {
                std::cout << "  - Fila fuera del tablero, intente de nuevo \n";
            }
        } else {
            std::cout << "  - Columna fuera del tablero, intente de nuevo \n";
        }
    }
}
