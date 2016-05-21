#include <iostream>
#include "campo.h"
using namespace std;

int main(){
	
    // testeando
    //si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 5;
    posC.y = 0;

    Campo c(posG, posC);

    return 0;
}
