#include "drone.h"
#include <algorithm>
#include <sstream>

Drone::Drone(){
    _id = 0;
    _bateria = 100;
    _enVuelo = false;
    // Si lo pongo en (0,0) cumple la especificacion
    // Le defino una posicion valida porque sino se rompe en otro lado
    _posicionActual.x = 0;
    _posicionActual.x = 0;
}

Drone::Drone(ID i, const std::vector<Producto>& ps){
	_id = i;
	_productos = ps;
	_bateria = 100;
    _enVuelo = false;
    // Si lo pongo en (0,0) cumple la especificacion
    // Le defino una posicion valida porque sino se rompe en otro lado
    _posicionActual.x = 0;
    _posicionActual.y = 0;
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
    return _enVuelo && _escalerado();
}

bool Drone::_escalerado() const{
    return _esEscalerado(1,1) || _esEscalerado(1,-1) ||
            _esEscalerado(-1,1) || _esEscalerado(-1,-1);
}

bool Drone::_esEscalerado(int X, int Y) const{
    std::cout << "_esEscalerado    x " << X << " y " << Y << "\n";
    bool cumplenTodos = true;
    unsigned int i = 0;
    while (i < _trayectoria.size()-2){

        if ((_trayectoria.at(i).x - _trayectoria.at(i+2).x) != X ||
                (_trayectoria.at(i).y - _trayectoria.at(i+2).y) != Y ){
            cumplenTodos = false;
        }
        i++;
    }
    return cumplenTodos;
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds){
	Secuencia<Secuencia<Posicion> > pss = dameTrayectorias(ds);
    Secuencia<Secuencia<Posicion> > tss = agruparPosiciones(pss);
    Secuencia<Secuencia<InfoVueloCruzado> > xss = agruparRepetidas(tss);
    Secuencia<InfoVueloCruzado> yss = borrarSobrantesYOrdenar(xss);

    return yss;
}

//Toma una secuencia de Drones y devuelve una secuencia de secuencias de posiciones con las trayectorias de esos Drones
Secuencia<Secuencia<Posicion> > Drone::dameTrayectorias(const Secuencia<Drone>& ds){
    Secuencia<Secuencia<Posicion> > res;
    unsigned int i = 0;
    while(i < ds.size() ){
        res.push_back(ds.at(i)._trayectoria);
        i++;
    }
    return res;
}

//Rearma la matriz agrupando las posiciones de los Drones de acuerdo al índice en la trayectoria
Secuencia<Secuencia<Posicion> > Drone::agruparPosiciones(const Secuencia<Secuencia<Posicion> > pss){
    Secuencia<Secuencia<Posicion> > res;
    unsigned int j = 0;
    while (j < pss.at(0).size()){

        Secuencia<Posicion> posicionesEnMomento;
        posicionesEnMomento.resize(0);

        unsigned int i = 0;
        while (i < pss.size()){

            posicionesEnMomento.push_back(pss.at(i).at(j));

            i++;
        }
        res.push_back(posicionesEnMomento);
        j++;
    }
    return res;
}

bool Drone::_perteneceInfoPos(Posicion p, Secuencia<InfoVueloCruzado> ps){
    bool pertenece = false;
    unsigned int i = 0;
    while (i < ps.size()){
        if(p.x == ps.at(i).posicion.x && p.y == ps.at(i).posicion.y){
            pertenece = true;
        }
        i++;
    }
    return pertenece;
}

int Drone::_cuentaPos(Posicion p, Secuencia<Posicion> ps){
    int contador = 0;
    unsigned int i = 0;
    while (i < ps.size()){
        if (p.x == ps.at(i).x && p.y == ps.at(i).y){
            contador++;
        }
        i++;
    }
    return contador;
}

// En cada secuencia de la matriz agrupa las posiciones según la cantidad de repeticiones
// en la forma InfoVuelosRealizados "((x,y),z)",
// filtrando los repetidos y los que tienen solamente 1 cruce (consigo mismo)

Secuencia<Secuencia<InfoVueloCruzado> > Drone::agruparRepetidas(const Secuencia<Secuencia<Posicion> > tss){

    Secuencia<Secuencia<InfoVueloCruzado> > res;
    unsigned int i = 0;
    while (i < tss.size()){
        Secuencia<InfoVueloCruzado> crucesEnPos;
        crucesEnPos.resize(0);
        unsigned int j = 0;

        while (j < tss.at(i).size()){

            Posicion posMomento;
            posMomento.x = tss.at(i).at(j).x;
            posMomento.y = tss.at(i).at(j).y;

            int cantCruces = _cuentaPos(posMomento, tss.at(i));

            if (!_perteneceInfoPos(posMomento, crucesEnPos)){
                if (cantCruces > 1){
                    InfoVueloCruzado info;
                    info.posicion = posMomento;
                    info.cantidadCruces = cantCruces;
                    crucesEnPos.push_back(info);
                }
            }
            j++;
        }
        res.push_back(crucesEnPos);
        i++;
    }

    return res;
}

//Borra aquellos elementos en los que haya 1 solo cruce y ordena a los demás de menor a mayor
Secuencia<InfoVueloCruzado> Drone::borrarSobrantesYOrdenar(const Secuencia<Secuencia<InfoVueloCruzado> > xss){

    // 'Aplano' la secuencia de secuencias
    // Todos tienen cruces > 1 porque los filtre antes
    // by: jon
    Secuencia<InfoVueloCruzado> res;
    unsigned int i = 0;
    while (i < xss.size()){
        unsigned int j = 0;
        while (j < xss.at(i).size()){
            res.push_back(xss.at(i).at(j));
            j++;
        }
        i++;
    }

    // Ordena de forma creciente por cantidadCruces
    // by: lucasB
    unsigned int k = 0;
    while(k < res.size()){
        unsigned int l = k + 1;
        while(l < res.size()){
            if(res.at(k).cantidadCruces > res.at(l).cantidadCruces){
                InfoVueloCruzado c;
                InfoVueloCruzado d;
                c = res.at(k);
                d = res.at(l);
                res.at(k) = d;
                res.at(l) = c;
                l++;
            }
            else{
                l++;
            }
        }
        k++;
    }
    return res;
}


// La posicion actual es el final de trayectoria.
// Ver si lo mostramos aparte. En ese caso, que hacer cuando no esta en vuelo? no mostrarlo?
void Drone::mostrar(std::ostream & os) const{
    os << "Drone" << std::endl;
    os << "    Id: " << _id << std::endl;
    os << "    Bateria: " << _bateria << std::endl;
    os << "    En vuelo: ";
    if (_enVuelo) os << "Si" << std::endl;
    else os << "No" << std::endl;

    os << "    PosActual: " << _posicionActual.x << " " << _posicionActual.y << std::endl;

    os << "    Vuelos realizados: ";
    unsigned int i = 0;
    while (i < _trayectoria.size()){
        os << "[" << _trayectoria.at(i).x  << "," <<_trayectoria.at(i).y << "] ";
        i++;
    }
    os << std::endl;


    os << "    Productos disponibles: ";
    unsigned int j = 0;
    while (j < _productos.size()){
        os << _productos.at(j) << "  ";
        j++;
    }
    os << std::endl;



}

void Drone::guardar(std::ostream & os) const{
    os << "{ D ";
    os << _id << " ";
    os << _bateria << " ";
    os << _dameStringVueloRealizado() << " ";
    os << _dameStringProductos() << " ";
    os << _dameStringEnVuelo() << " ";
    os << _dameStringPosActual();
    os << "}";
}

void Drone::cargar(std::istream & is){
    std::string datosDrone;
    std::string dId;
    std::string dBateria;
    std::string dTrayectoria;
    std::string dProductos;
    std::string dEnVuelo;
    std::string dPosActual;

    // Uso getLine porque preserva los espacios
    getline(is, datosDrone,'}');
    // datosDrone:
    //{ D 12 100 [[0,0],[1,0]] [Herbicida, Plaguicida]

    _leerSepararDatos(datosDrone, dId, dBateria, dTrayectoria, dProductos, dEnVuelo, dPosActual);
    _cargarId(dId);
    _cargarBateria(dBateria);
    _cargarTrayectoria(dTrayectoria);
    _cargarProductos(dProductos);
    _cargarEnVuelo(dEnVuelo);
    _cargarPosActual(dPosActual);
}

void Drone::_cargarPosActual(std::string dPosActual){
    _posicionActual = _cargarPosicionIndividual(dPosActual);
}

void Drone::_cargarEnVuelo(std::string dEnVuelo){
    bool leyoEstado = false;
    bool terminado = false;
    unsigned int i = 0;
    while (!terminado){
        if (dEnVuelo[i] == 't'){
            leyoEstado = true;
            _enVuelo = true;
        }

        i++;
        if (i >= dEnVuelo.size()){
            terminado = true;
        }
    }

    if (!leyoEstado){
        _enVuelo = false;
    }

}

void Drone::_cargarProductos(std::string dProductos){
    // Vaciar el vector de productos y cargar los nuevos
    // dProductos:_[Herbicida,Plaguicida]
    _productos.resize(0);

    // Separo los productos uno a uno y los voy cargando
    std::string dActual;
    bool terminar = false;
    int i = 2;

    while (!terminar){
        if (dProductos[i] == ','){
            _cargarProductoIndividual(dActual);
            dActual = "";
            i++;
        }
        if (dProductos[i] == ']'){
            if(dActual != ""){
                _cargarProductoIndividual(dActual);
            }
            terminar = true;

        }else{
            dActual += dProductos[i];
            i++;
        }
    }

}

void Drone::_cargarProductoIndividual(std::string dProd){
    // Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance ;
    if (dProd == "Herbicida") _productos.push_back(Herbicida);
    if (dProd == "HerbicidaLargoAlcance") _productos.push_back(HerbicidaLargoAlcance);
    if (dProd == "Plaguicida") _productos.push_back(Plaguicida);
    if (dProd == "PlaguicidaBajoConsumo") _productos.push_back(PlaguicidaBajoConsumo);
    if (dProd == "Fertilizante") _productos.push_back(Fertilizante);
}

void Drone::_cargarTrayectoria(std::string dTrayectoria){

    // Vacio la trayectoria antigua del drone
    _trayectoria.resize(0);

    if (dTrayectoria[1] == '[' && dTrayectoria[2] == ']'){
        // La trayectoria es vacia
        _enVuelo = false;
    }else{
        // La trayectoria NO es vacia
        bool terminado = false;
        std::string dActual = "";
        int i = 2;

        while (!terminado){
            if ((dTrayectoria[i] == ',') && (dTrayectoria[i+1] == '[')){
                Posicion p = _cargarPosicionIndividual(dActual);
                _trayectoria.push_back(p);
                dActual = "";
                i++;
            }

            if((dTrayectoria[i] == ']') && (dTrayectoria[i-1] == ']')){
                Posicion p = _cargarPosicionIndividual(dActual);
                _trayectoria.push_back(p);
                terminado = true;
            }else{
                dActual += dTrayectoria[i];
                i++;
            }
        }

        _enVuelo = true;
        _posicionActual = _trayectoria.back();

    }
}

Posicion Drone::_cargarPosicionIndividual(std::string dPos){
    int posSeparador = dPos.find(',');
    int x = 0;
    if(dPos[0] == '['){
        x = atoi(dPos.substr(1, posSeparador - 1).c_str());
    }else{
        x = atoi(dPos.substr(2, posSeparador - 1).c_str());
    }
    int y = atoi(dPos.substr(posSeparador + 1, dPos.length() - posSeparador - 2).c_str());

    Posicion p;
    p.x = x;
    p.y = y;

    return p;
    //_trayectoria.push_back(p);
    //std::cout << "x: " << x << " y: " << y << std::endl;
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

/* Separa la string con todos los datos en las categorias correspondientes*/
void Drone::_leerSepararDatos(std::string &datos, std::string &dId, std::string &dBateria,
                              std::string &dTrayectoria, std::string &dProductos,
                              std::string &dEnVuelo, std::string &dPosActual){
    // Leo los datos y los separo

    bool terminado = false;
    int cantEspacios = 0;
    int i = 0;
    while (!terminado){
        if (datos[i] == ' ') cantEspacios++;

        if (cantEspacios == 2) dId += datos[i];

        if (cantEspacios == 3) dBateria += datos[i];

        if (cantEspacios == 4) dTrayectoria += datos[i];

        if (cantEspacios == 5) dProductos += datos[i];

        if (cantEspacios == 6) dEnVuelo += datos[i];

        if (cantEspacios == 7){
            dPosActual = datos.substr(i, datos.npos);
            terminado = true;
        }

        i++;
    }

}

std::string Drone::_dameStringEnVuelo() const{
    std::string cadena = "";
    if(_enVuelo) cadena = "true";
    else cadena = "false";

    return cadena;
}

std::string Drone::_dameStringPosActual() const{
    return _dameStringPosicion(posicionActual());
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
            cadenaProductos += ",";
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
    return _id == otroDrone.id() &&
           _bateria == otroDrone.bateria() &&
           _enVuelo == otroDrone.enVuelo() &&
           _posicionActual == otroDrone.posicionActual() &&
           _igualTrayectoria(_trayectoria, otroDrone.vueloRealizado()) &&
           mismos(_productos, otroDrone.productosDisponibles());
            // _mismosProductos(_productos, otroDrone.productosDisponibles());
}

template<class T>
unsigned int Drone::cuenta(const T &x, const std::vector<T> &v) const{
    unsigned int cant = 0;
    for (unsigned int i = 0; i < v.size(); ++i) {
        if (x == v[i]) ++cant;
    }
    return cant;
}

template<class T>
bool Drone::mismos(const std::vector<T> &a, const std::vector<T> &b) const {
    bool res = a.size() == b.size();
    for (unsigned int i = 0; res && i < a.size(); ++i) {
        res = cuenta(a[i], a) == cuenta(a[i], b);
    }
    return res;
}

bool Drone::_igualTrayectoria(Secuencia<Posicion> trA, Secuencia<Posicion> trB) const{
    Posicion posA;
    Posicion posB;

    bool iguales = true;
    if (trA.size() == trB.size()){
        unsigned int i = 0;
        while (i < trA.size()){
            posA.x = trA.at(i).x;
            posA.y = trA.at(i).y;

            posB.x = trB.at(i).x;
            posB.y = trB.at(i).y;

            if ((posA.x != posB.x) || (posA.y != posB.y)){
                iguales = false;
            }
            i++;
        }

    }else{
        iguales = false;
    }

    return iguales;
}


bool Drone::_mismosProductos(Secuencia<Producto> secuA, Secuencia<Producto> secuB) const{
    bool tienenMismos = true;
    if (secuA.size() == secuB.size()){
        unsigned int i = 0;
        int cuentaA;
        int cuentaB;
        while (i < secuA.size()){
            cuentaA = std::count(secuA.begin(), secuA.end() - 1, secuA.at(i));
            cuentaB = std::count(secuB.begin(), secuB.end() - 1, secuB.at(i));
            if (cuentaA != cuentaB){
                tienenMismos = false;
            }
            i++;
        }
    }else{
        tienenMismos = false;
    }

    return tienenMismos;
}

std::ostream & operator<<(std::ostream & os, const Drone & d){
    d.mostrar(os);
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



void Drone::moverA(const Posicion pos){
    _enVuelo = true;
    _trayectoria.push_back(pos);
    _posicionActual = pos;
}

void Drone::setBateria(const Carga c){
    _bateria = c;
}           

void Drone::borrarVueloRealizado(){
    //lean la especificacion de esta
    //no estoy seguro de que este bien especificada
    _enVuelo = false;
    _trayectoria.clear();
}
            
void Drone::cambiarPosicionActual(const Posicion p){
    _posicionActual = p;
}
            
void Drone::sacarProducto(const Producto p){
    unsigned int i = 0;
    while(i < _productos.size()){
        if(_productos.at(i) == p){
            _productos.erase(_productos.begin() + i);
            break;
        }

        i++;
    }
    
}
