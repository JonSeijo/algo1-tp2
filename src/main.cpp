#include <iostream>
#include <fstream>
#include "campo.h"
#include "drone.h"
#include "sistema.h"
#include "tipos.h"
#include "auxiliares.h"

using namespace std;

int main(){
	
    // testeando
    // si me olvido de borrar sean libres de hacer mierda todo esto

    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 2;
    posC.y = 2;

    Campo c(posG, posC);
    Campo c2(posG, posC);

/*
        ofstream archivoCampo("campo.txt");
        c.guardar(archivoCampo);
        archivoCampo.close();
*/


       // Prueba cargar campo
     //   ifstream archivoLecturaCampo("campo.txt");
     //   c.cargar(archivoLecturaCampo);
     //   archivoLecturaCampo.close();

   //     cout << c;

    std::vector<Producto> productos1;
    productos1.push_back(Herbicida);
    productos1.push_back(Herbicida);
    productos1.push_back(Plaguicida);

    Drone d(5, productos1);

    std::vector<Producto> productos2;
    productos2.push_back(Herbicida);
    productos2.push_back(Plaguicida);
    productos2.push_back(Herbicida);

    Drone d1(5, productos2);
    Drone d2(5, productos1);
    d2.moverA({1,1});
    d1.moverA({1,1});

    cout << "igualDrones: " << (d1 == d2) << endl;
//    Drone d3(43, productos);
//    Drone d4(10, productos);

    Secuencia<Drone> sec1;
    sec1.push_back(d);
    sec1.push_back(d1);

    Secuencia<Drone> sec2;
    sec2.push_back(d1);
    sec2.push_back(d);

    //Sistema sis2(c, sec);
      // Prueba guardar sistema
    Sistema sis(c, sec1);
    Sistema sis1(c, sec1);
    Sistema sis2(c, sec2);

//    cout << "sistemas iguales: " << (sis1 == sis2) << endl;

    ofstream archivoSistema("sistema.txt");
    sis.guardar(archivoSistema);
    archivoSistema.close();


   // Prueba cargar sistema
    ifstream archivoLecturaSistema("sistema.txt");
    sis.cargar(archivoLecturaSistema);
    archivoLecturaSistema.close();

    ifstream fileDrone1("drone1.txt");
    d1.cargar(fileDrone1);
    fileDrone1.close();
   // cout << d1;
    ofstream fileDrone1Bis("drone1-bis.txt");
    d1.guardar(fileDrone1Bis);
    fileDrone1Bis.close();
    //cout << d1;
/*
    ifstream fileDrone2("drone2.txt");
    d2.cargar(fileDrone2);
    fileDrone2.close();

    ifstream fileDrone3("drone3.txt");
    d3.cargar(fileDrone3);
    fileDrone3.close();

    ifstream fileDrone4("drone4.txt");
    d4.cargar(fileDrone4);
    fileDrone4.close();

    Secuencia<Drone> listaDrones;
    listaDrones.push_back(d1);
    listaDrones.push_back(d2);
    listaDrones.push_back(d3);
    listaDrones.push_back(d4);

    Secuencia<InfoVueloCruzado> cruces = Drone::vuelosCruzados(listaDrones);

    unsigned int i = 0;
    cout << cruces.size() << endl;
    while (i < cruces.size()){
       // cout << "cruce " << i << endl;
        cout << "pos: (" << cruces.at(i).posicion.x << " " << cruces.at(i).posicion.y << ") " ;
        cout << "cantCruces: " << cruces.at(i).cantidadCruces << "\n" << endl;
        i++;
    }

    cout << "Vuelo Escalerado: " << d1.vueloEscalerado() << endl;

*/
    //cout << sis;
   // sis.crecer();
   // cout << sis;
    // cout << "sis == sis2: " << (sis == sis2) << endl;
   // sis.seExpandePlaga();
   // sis.fertilizarPorFilas();
    cout << sis;

    sis.fertilizarPorFilas();

    cout << sis;

    return 0;
}
