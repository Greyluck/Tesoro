// Liberia usada para trabajar con funciones de entrada y salida
#include <iostream>

const char * crearTablero();

int main() {
    std::cout << "TABLERO DE JUEGO" << std::endl;
    const char* myTablero = crearTablero();
}

const char* crearTablero() {
    // tablero [columna][fila]
    const char* tablero[20][20];

    // Recorre cada columna y en cada columna, recorre cada fila. Luego asigna "0'
    for (int column = 0; column < 20; column++) {
        for (int row = 0; row < 20; row++) {
            tablero[column][row] = "1";
        }
    }

    // Recorre cada columna y en cada columna, recorre cada fila. Imprimiendo el tablero.
    for (int column = 0; column < 20; column++) {
        for (int row = 0; row < 20; row++) {
            std::cout << tablero[column][row];
        }
        std::cout << "\n";
    }
}
