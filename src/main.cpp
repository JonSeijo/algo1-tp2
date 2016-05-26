#include <iostream>
#include <fstream>
#include "campo.h"
#include "drone.h"
#include "sistema.h"
#include "tipos.h"

using namespace std;

int main(){
	
    // testeando
    // si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 1;
    posC.y = 0;

    Campo c(posG, posC);
    Campo c2(posG, posC);

    std::vector<Producto> productos;
    productos.push_back(Herbicida);
    productos.push_back(Plaguicida);

    Drone d(5, productos);
    Drone d1(5, productos);

    Secuencia<Drone> sec;
    //sec.push_back(d);
    //sec.push_back(d);

    Sistema sis(c, sec);
    Sistema sis2(c, sec);
    /*  // Prueba guardar sistema
    ofstream archivoSistema("sistema.txt");
    sis.guardar(archivoSistema);
    archivoSistema.close();
*/
   /*
    Prueba cargar sistema
    ifstream archivoLecturaSistema("sistema.txt");
    sis.cargar(archivoLecturaSistema);
    archivoLecturaSistema.close();
*/
    cout << "sis == sis2: " << (sis == sis2) << endl;

    return 0;
}
