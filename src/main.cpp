#include <iostream>
#include "campo.h"
using namespace std;

int main(){
	
    // testeando
    //si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 67;
    posG.y = 34;

    Posicion posC;
    posC.x = 20;
    posC.y = 99;

    Campo c(posG, posC);

    return 0;
}
