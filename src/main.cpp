#include <iostream>
#include <fstream>
#include "campo.h"
using namespace std;

int main(){
	
    // testeando
    //si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 3;
    posC.y = 2;

    Campo c(posG, posC);
    std::ofstream archivoCampo("campo.txt");
    c.guardar(archivoCampo);
    cout << c;

    return 0;
}
