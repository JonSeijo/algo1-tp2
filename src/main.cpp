#include <iostream>
#include <fstream>
#include "campo.h"
#include "drone.h"
#include "sistema.h"
#include "tipos.h"

using namespace std;

int main(){
	
    // testeando
    //si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 1;
    posC.y = 0;

    Campo c(posG, posC);
    /*
    Prueba de guardado
    std::ofstream archivoCampo("campo.txt");
    c.guardar(archivoCampo);
    archivoCampo.close();
    */
    //cout << c;

    Campo c2(posG, posC);
    // Prueba de cargado
    //std::ifstream archivoLecturaCampo("campo.txt");
    //c.cargar(archivoLecturaCampo);
    //archivoLecturaCampo.close();

    //cout << c;

    // Prueba igualdad
    //cout << (c == c2) << endl;

    std::vector<Producto> productos;
    productos.push_back(Herbicida);
    productos.push_back(Plaguicida);

    Drone d(5, productos);

    // Prueba de cargado Drone
    std::ifstream archivoLecturaDrone("drone.txt");
    d.cargar(archivoLecturaDrone);
    archivoLecturaDrone.close();

    Drone d1(5, productos);

    Secuencia<Drone> sec;
    sec.push_back(d);
    sec.push_back(d);

    Sistema sis(c, sec);

    ofstream archivoSistema("sistema.txt");
    sis.guardar(archivoSistema);
    // archivoSistema.close();

    return 0;
}
