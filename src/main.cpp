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
    posC.x = 6;
    posC.y = 2;

    Campo c(posG, posC);
    Campo c2(posG, posC);

    std::vector<Producto> productos;
    productos.push_back(Herbicida);
    productos.push_back(Plaguicida);

    Drone d(5, productos);
    Drone d1(5, productos);
    Drone d2(78, productos);
    Drone d3(43, productos);

    Secuencia<Drone> sec;
    sec.push_back(d);
    Sistema sis(c, sec);
    //Sistema sis2(c, sec);
 /*     // Prueba guardar sistema
    ofstream archivoSistema("sistema.txt");
    sis.guardar(archivoSistema);
    archivoSistema.close();
*/

   // Prueba cargar sistema
    ifstream archivoLecturaSistema("sistema.txt");
    sis.cargar(archivoLecturaSistema);
    archivoLecturaSistema.close();

    ifstream fileDrone1("drone1.txt");
    d1.cargar(fileDrone1);
    fileDrone1.close();

    ifstream fileDrone2("drone2.txt");
    d2.cargar(fileDrone2);
    fileDrone2.close();

    ifstream fileDrone3("drone3.txt");
    d3.cargar(fileDrone3);
    fileDrone3.close();

    Secuencia<Drone> listaDrones;
    listaDrones.push_back(d1);
    listaDrones.push_back(d2);
    listaDrones.push_back(d3);

    //Prueba, revisar la trayectoria que tienen en drone.cpp, en lo posible hacer Drones
    //con distintas trayectorias para probarlo, funciona cuando hay un solo drone en la lista
    //y cuando todos tienen las mismas posiciones, falta el caso más común pero no se como 
    //cambiarle trayectorias a drones sin modificar el cpp
    cout << "ENTRE" << endl;

    unsigned int j = 0;
    while(j < listaDrones.size()){
        cout << listaDrones.at(j);
        j++;
    }

    Secuencia<InfoVueloCruzado> cruces = Drone::vuelosCruzados(listaDrones);
    unsigned int i = 0;
    while (i < cruces.size()){
        cout << "cruce " << i << endl;
        cout << "pos:   (" << cruces.at(i).posicion.x << " " << cruces.at(i).posicion.y << ") " ;
        cout << "cant: " << cruces.at(i).cantidadCruces << "\n" << endl;
        i++;
    }
    cout << "SALI" << endl;
/*
    Secuencia<InfoVueloCruzado> x = d2.vuelosCruzados(sec);
    cout << "Hubo cruces " << x.size() << " veces, que pais generoso." << endl;
    cout<< x.at(0).posicion.x << endl;
    cout<< x.at(1).cantidadCruces << endl;
*/
    //cout << sis;
   // sis.crecer();
   // cout << sis;
    // cout << "sis == sis2: " << (sis == sis2) << endl;
   // sis.seExpandePlaga();
    //sis.fertilizarPorFilas();
    //cout << sis;


    return 0;
}
