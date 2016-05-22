#include <iostream>
#include <fstream>
#include "campo.h"
#include "drone.h"
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

    Drone d(12, productos);

    // Prueba guardado Drone
    std::ofstream archivoDrone("drone.txt");
    d.guardar(archivoDrone);
    archivoDrone.close();


    return 0;
}
