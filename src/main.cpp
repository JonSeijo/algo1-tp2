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
    //productos.push_back(Plaguicida);

    Drone d(5, productos);

    /*
    // Prueba guardado Drone
    std::ofstream archivoDrone("drone.txt");
    d.guardar(archivoDrone);
    archivoDrone.close();
    */

    // Prueba de cargado Drone
    std::ifstream archivoLecturaDrone("drone.txt");
    d.cargar(archivoLecturaDrone);
    archivoLecturaDrone.close();

    cout << "id: " << d.id() << endl;
    cout << "bat: " << d.bateria() << endl;
    cout << "enVuelo: " << d.enVuelo() << endl;
    cout << "posActual: " << d.posicionActual().x << " " << d.posicionActual().y << endl;
    cout << "cant productos: " << d.productosDisponibles().size() << endl;


    Drone d1(5, productos);
    Drone d2(5, productos);

    cout << "Iguales: " << (d1 == d1) << endl;

    cout << "\n\n\n";
    cout << d;

    return 0;
}
