#include "drone.h"
#include <algorithm>
#include <sstream>

Drone::Drone(){
}

Drone::Drone(ID i, const std::vector<Producto>& ps){
	_id = i;
	_productos = ps;
	_bateria = 100;
	_enVuelo = false;
    /*
    Trayectoria de prueba

    Posicion p1;
    p1.x = 0;
    p1.y = 0;

    Posicion p2;
    p2.x = 1;
    p2.y = 0;

    _trayectoria.push_back(p1);
    _trayectoria.push_back(p2);
    */
}

ID Drone::id() const{
	return _id;
}

Carga Drone::bateria() const{
	return _bateria;
}

bool Drone::enVuelo() const{
	return _enVuelo;
}

const Secuencia<Posicion>& Drone::vueloRealizado() const{
	return _trayectoria;
}

Posicion Drone::posicionActual() const{
	return _posicionActual;
}

const Secuencia<Producto>& Drone::productosDisponibles() const{
	return _productos;
}

bool Drone::vueloEscalerado() const{
	Secuencia<int> xs = damePrimeros(_trayectoria);
	Secuencia<int> ys = dameSegundos(_trayectoria);

	return _enVuelo 
			&& ordenada(xs)
			&& ordenada(ys)
			&& dosOMenos(xs)
			&& dosOMenos(ys);
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds){
	return Secuencia<InfoVueloCruzado>();
}

void Drone::mostrar(std::ostream & os) const{

}

void Drone::guardar(std::ostream & os) const{
    os << "{ D ";
    os << _id << " ";
    os << _bateria << " ";
    os << _dameStringVueloRealizado() << " ";
    os << _dameStringProductos();
    os << "}";
}

void Drone::cargar(std::istream & is){
    std::string datosDrone;
    std::string dId;
    std::string dBateria;
    std::string dTrayectoria;
    std::string dProductos;

    // Uso getLine porque preserva los espacios
    getline(is, datosDrone,'}');
    // datosDrone:
    //{ D 12 100 [[0,0],[1,0]] [Herbicida, Plaguicida]

    _leerSepararDatos(datosDrone, dId, dBateria, dTrayectoria, dProductos);
    _cargarId(dId);
    _cargarBateria(dBateria);
    _cargarTrayectoria(dTrayectoria);
    _cargarProductos(dProductos);


}

void Drone::_cargarProductos(std::string dProductos){
    // Vaciar el vector de productos y cargar los nuevos
}

void Drone::_cargarTrayectoria(std::string dTrayectoria){
    // Si es vacia, vaciar trayectoria
    //              enVuelo true

    // Si no es vacia, vaciar y poner las posiciones en ese orden
    //                 posicionActual sea el ultimo de trayectoria
    //                 enVuelo true
}

void Drone::_cargarBateria(std::string dBateria){
    // bateria:_BATERIA
    // Quito el espacio del principio
    dBateria = dBateria.substr(1, dBateria.npos);
    int newBateria = atoi(dBateria.c_str());
    _bateria = newBateria;
}

void Drone::_cargarId(std::string dId){
    // id:_ID
    // Quito el espacio del principio
    dId = dId.substr(1, dId.npos);
    int newId = atoi(dId.c_str());
    _id = newId;
}

void Drone::_leerSepararDatos(std::string &datos, std::string &dId, std::string &dBateria,
                              std::string &dTrayectoria, std::string &dProductos){
    // Leo los datos y los separo

    bool terminado = false;
    int cantEspacios = 0;
    int i = 0;
    while (!terminado){
        if (datos[i] == ' ') cantEspacios++;

        if (cantEspacios == 2) dId += datos[i];

        if (cantEspacios == 3) dBateria += datos[i];

        if (cantEspacios == 4) dTrayectoria += datos[i];

        if (cantEspacios == 5){
            dProductos = datos.substr(i, datos.npos);
            terminado = true;
        }
        i++;
    }


     std::cout << "id" << dId << "   bateria" << dBateria << std::endl;
     std::cout << "trayectoria:" << dTrayectoria << "  productos:" << dProductos << std::endl;

}



/* Devuelve un string con la trayectora del drone, de la forma [[x1,y1], [x2,y2]]*/
std::string Drone::_dameStringVueloRealizado() const{
    std::string cadenaVuelo = "";
    cadenaVuelo += '[';
    unsigned int i = 0;
    while (i < _trayectoria.size()){
        cadenaVuelo += _dameStringPosicion(_trayectoria.at(i));
        i++;
        if (i < _trayectoria.size()){
            cadenaVuelo += ',';
        }
    }
    cadenaVuelo += ']';
    return cadenaVuelo;
}

/* Dado una posicion, devuelve una string en la forma [x, y] */
std::string Drone::_dameStringPosicion(Posicion p) const{
    std::string cadena = "";
    std::stringstream sx;
    sx << p.x;
    std::stringstream sy;
    sy << p.y;

    cadena += '[' + sx.str() + ',' + sy.str() + ']';
    return cadena;
}

/* Devuelve un string de los productos del drone de la forma [prod1, prod2, prod3]*/
std::string Drone::_dameStringProductos() const{
    std::string cadenaProductos = "";
    cadenaProductos += '[';

    unsigned int i = 0;
    while (i < _productos.size()){
        cadenaProductos += _dameStringProd(_productos.at(i));
        i++;
        if (i < _productos.size()){
            cadenaProductos += ", ";
        }
    }

    cadenaProductos += ']';
    return cadenaProductos;
}

/* Dado un producto, devuelve su nombre como string*/
std::string Drone::_dameStringProd(Producto p) const{
    std::string res = "";
    if (p == Fertilizante) res = "Fertilizante";
    if (p == Plaguicida) res = "Plaguicida";
    if (p == PlaguicidaBajoConsumo) res = "PlaguicidaBajoConsumo";
    if (p == Herbicida) res = "Herbicida";
    if (p == HerbicidaLargoAlcance) res = "HerbicidaLargoAlcance";

    return res;
}

bool Drone::operator==(const Drone & otroDrone) const{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Drone & d){
	return os;
}

/*
* Toma un Secuencia<Posicion> y devuelve la secuencia de los elementos x
* de cada posicion
 * n = xs.size()
 * O(n)
*/
Secuencia<int> Drone::damePrimeros(const Secuencia<Posicion> ps) const{
	Secuencia<int> res;
	unsigned int i = 0;
	while(i < ps.size()){
		res.at(i) = ps.at(i).x;
		i++;
	}
	return res;
}

/*
* Toma un Secuencia<Posicion> y devuelve la secuencia de los elementos y
* de cada posicion
 * n = xs.size()
 * O(n²)
*/
Secuencia<int> Drone::dameSegundos(const Secuencia<Posicion> ps) const{
	Secuencia<int> res;
	unsigned int i = 0;
	while(i < ps.size()){
		res.at(i) = ps.at(i).y;
		i++;
	}
	return res;

}

/*
 * Toma un Secuencia<int> y devuelve true si ningun 
 * elemento aparece mas de dos veces, false en caso contrario
 * n = xs.size()
 * O(n²)
*/

bool Drone::dosOMenos(const Secuencia<int> xs) const{
	bool res = true;
	unsigned int i = 0;
	while(i < xs.size()){
		int cuenta = std::count(xs.begin(), xs.end() - 1, xs.at(i));
		if(cuenta > 2){
			res = false;
		}
		i++;
	}

	return res;
}
/*
* Toma un Secuencia<int> y devuelve un bool de si esta ordenada o no
* n = xs.size()
* O( n * log n )
*/
bool Drone::ordenada(const Secuencia<int> xs) const{
	Secuencia<int> r = xs;
	std::sort(r.begin(), r.end());
	return r == xs;
}

std::ostream & operator<<(std::ostream & os, const Producto & p){
    if (p == Fertilizante) os << "Fertilizante";
    if (p == Plaguicida) os << "Plaguicida";
    if (p == PlaguicidaBajoConsumo) os << "PlaguicidaBajoConsumo";
    if (p == Herbicida) os << "Herbicida";
    if (p == HerbicidaLargoAlcance) os << "HerbicidaLargoAlcance";

    return os;
}
