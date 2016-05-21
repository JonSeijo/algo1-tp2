#include "drone.h"
#include <algorithm>

Drone::Drone(){
}

Drone::Drone(ID i, const std::vector<Producto>& ps){
	_id = i;
	_productos = ps;
	_bateria = 100;
	_enVuelo = false;
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
}

void Drone::cargar(std::istream & is){
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
bool Drone::ordenada(Secuencia<int> xs) const{
	Secuencia<int> r = xs;
	std::sort(xs.begin(), xs.end());
	return r == xs;
}
