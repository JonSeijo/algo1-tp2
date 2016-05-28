#include "sistema.h"
#include <algorithm>
#include <sstream>

Sistema::Sistema(){
    std::cout << "const vacio sistema" << std::endl;

}

Sistema::Sistema(const Campo &c, const Secuencia<Drone> &ds){
    _campo = c;
	_enjambre = ds;
	_estado.parcelas.resize(c.dimensiones().ancho, std::vector<EstadoCultivo>(c.dimensiones().largo, NoSensado));
    //initEstados();
}

const Campo & Sistema::campo() const{
	return _campo;
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const{
    return _estado.parcelas.at(p.x).at(p.y);
}

const Secuencia<Drone>& Sistema::enjambreDrones() const{
	return _enjambre;
}

void Sistema::crecer(){
    int i = 0;
    while (i < _campo.dimensiones().ancho){
        int j = 0;
        while (j < _campo.dimensiones().largo){

            if (_estado.parcelas.at(i).at(j) == RecienSembrado){
                _estado.parcelas.at(i).at(j) = EnCrecimiento;
            }else if (_estado.parcelas.at(i).at(j) == EnCrecimiento){
                _estado.parcelas.at(i).at(j) = ListoParaCosechar;
            }

            j++;
        }
        i++;
    }
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps){
    // Por el requiere, toda posicion de ps es Cultivo y esta en rango
    unsigned int i = 0;
    while (i < ps.size()){
        int x = ps.at(i).x;
        int y = ps.at(i).y;
        _estado.parcelas.at(x).at(y) = ConMaleza;
        i++;
    }
}

void Sistema::seExpandePlaga(){
    std::vector<Posicion> posicionesPlaga = _posicionesConPlaga();
    unsigned int i = 0;
    while (i < posicionesPlaga.size()) {
        _plaguizarVecinos(posicionesPlaga.at(i));
        i++;
    }

}

std::vector<Posicion> Sistema::_adyacentes(Posicion p){
    std::vector<Posicion> adyacentes;

    Posicion v1;
    v1.x = p.x -1;
    v1.y = p.y;

    Posicion v2;
    v2.x = p.x +1;
    v2.y = p.y;

    Posicion v3;
    v3.x = p.x;
    v3.y = p.y -1;

    Posicion v4;
    v4.x = p.x;
    v4.y = p.y +1;

    adyacentes.push_back(v1);
    adyacentes.push_back(v2);
    adyacentes.push_back(v3);
    adyacentes.push_back(v4);

    return adyacentes;
}

void Sistema::_plaguizarVecinos(Posicion p) {
    std::vector<Posicion> vecinosConCultivos = _vecinosConCultivos(p);
    unsigned int i = 0;
    while (i < vecinosConCultivos.size()){
        Posicion pPlaguizar;
        pPlaguizar.x = vecinosConCultivos.at(i).x;
        pPlaguizar.y = vecinosConCultivos.at(i).y;
        _estado.parcelas.at(pPlaguizar.x).at(pPlaguizar.y) = ConPlaga;
        i++;
    }
}

std::vector<Posicion> Sistema::_vecinosConCultivos(Posicion p){
    std::vector<Posicion> vecinosConCultivos;
    std::vector<Posicion> vecinosAChequear = _adyacentes(p);

    unsigned int i = 0;
    while (i < vecinosAChequear.size()){
        if(_enRango(vecinosAChequear.at(i)) &&
                _campo.contenido(vecinosAChequear.at(i)) == Cultivo){
            vecinosConCultivos.push_back(vecinosAChequear.at(i));
        }
        i++;
    }
    return vecinosConCultivos;
}

bool Sistema::_enRango(Posicion p){
    return 0 <= p.x && p.x < _campo.dimensiones().ancho &&
            0 <= p.y && p.y < _campo.dimensiones().largo;
}

std::vector<Posicion> Sistema::_posicionesConPlaga(){
    std::vector<Posicion> posicionesPlaga;
    int i = 0;

    while (i < _campo.dimensiones().ancho){
        int j = 0;
        while (j < _campo.dimensiones().largo){
            if (_estado.parcelas.at(i).at(j) == ConPlaga){
                Posicion p;
                p.x = i;
                p.y = j;
                posicionesPlaga.push_back(p);
            }
            j++;
        }
        i++;
    }

    return posicionesPlaga;
}

void Sistema::despegar(const Drone & d){
    unsigned int i = 0;
    while (i < _enjambre.size()){
        if (_enjambre.at(i) == d){
            _enjambre.at(i).cambiarPosicionActual(_parcelaCultivoLibre());
        }
        i++;
    }

    std::cout << "despego el drone" << std::endl;
}

Posicion Sistema::_parcelaCultivoLibre() const{
    Secuencia<Posicion> ps;

    int i = 0;
    while (i < _campo.dimensiones().ancho){
        int j = 0;
        while (j < _campo.dimensiones().largo){
            Posicion p;
            p.x = i;
            p.y = j;

            if (_esAdyacente(p, posGranero()) &&
                    _esLibreDeDrones(p)){
                ps.push_back(p);
            }
            j++;
        }
        i++;
    }

    // Solo necesito una, y el requiere garantiza que existe
    return ps.at(0);

}

bool Sistema::_esLibreDeDrones(Posicion p) const{

    unsigned int i = 0;
    bool libre = true;
    while (i < _enjambre.size()){
        if (_enjambre.at(i).posicionActual().x == p.x &&
                _enjambre.at(i).posicionActual().y == p.y){
            libre = false;
        }
        i++;
    }

    return libre;
}

bool Sistema::_esAdyacente(Posicion p1, Posicion p2) const{
    return (p1.x == p2.x-1 && p1.y == p2.y) ||
            (p1.x == p2.x+1 && p1.y == p2.y) ||
            (p1.x == p2.x && p1.y == p2.y -1) ||
            (p1.x == p2.x && p1.y == p2.y +1);
}

bool Sistema::listoParaCosechar() const{
	int totalCultivos = (_campo.dimensiones().ancho * _campo.dimensiones().largo) - 2;

	int cantCosechable = 0;
    unsigned int i = 0;

	while(i < _estado.parcelas.size()){
        unsigned int j = 0;
        while(j < _estado.parcelas.at(i).size()){
            if(_estado.parcelas.at(i).at(j) == ListoParaCosechar){
				cantCosechable++;
			}
			j++;
		}
		i++;
    }

    return ((cantCosechable / totalCultivos) >= 0.9);
}

void Sistema::aterrizarYCargarBaterias(Carga b){
    unsigned int i = 0;
    while(i < _enjambre.size()){
        if(_enjambre.at(i).bateria() < b){
            _enjambre.at(i).setBateria(100);
            _enjambre.at(i).moverA(posGranero());
            _enjambre.at(i).borrarVueloRealizado();

        }
    }
}

void Sistema::fertilizarPorFilas(){
    unsigned int i = 0;
    while (i < _enjambre.size()){
        if (_enjambre.at(i).enVuelo()){
            int recorridoMaximo = _recorridoMaximo(_enjambre.at(i));
            std::vector<Posicion> recorridoDrone = _recorridoDrone(_enjambre.at(i), recorridoMaximo);

            _moverDrone(recorridoDrone, _enjambre.at(i));
            _fertilizar(recorridoDrone);
        }

        i++;
    }

}

void Sistema::_fertilizar(std::vector<Posicion> recorridoDrone){
    unsigned int i = 0;
    while (i < recorridoDrone.size()){
        Posicion p = recorridoDrone.at(i);
        if(_estado.parcelas.at(p.x).at(p.y) == RecienSembrado ||
              _estado.parcelas.at(p.x).at(p.y) == EnCrecimiento){
            _estado.parcelas.at(p.x).at(p.y) = ListoParaCosechar;
        }
        i++;
    }
}

void Sistema::_moverDrone(std::vector<Posicion> recorridoDrone, Drone d){
    unsigned int i = 0;
    while (i < recorridoDrone.size()){
        // moverA actualiza la posicionActual y la trayectoria
        d.moverA(recorridoDrone.at(i));
        d.setBateria(d.bateria() - 1);
        d.sacarProducto(Fertilizante);
        i++;
    }
}

std::vector<Posicion> Sistema::_recorridoDrone(Drone d, int recorridoMaximo){
    std::vector<Posicion> posRecorridas;
    int i = 1;
    while (i < recorridoMaximo){
        Posicion p;
        p.x = d.posicionActual().x - i;
        p.y = d.posicionActual().y;
        posRecorridas.push_back(p);

        i++;
    }
    return posRecorridas;
}

int Sistema::_recorridoMaximo(Drone d){
    // Identico a la especificacion
    return _minimo(_minimo(_fertAplicable(d), d.bateria()), _parcelasLibres(d));
}

int Sistema::_parcelasLibres(Drone d){
    int posMasAlejada = 0;
    bool terminado = false;
    int i = 0;
    while (i <= d.posicionActual().x || !terminado){

        bool todoEsCultivo = true;
        int j = i;
        while (j <= d.posicionActual().x){
            Posicion p;
            p.x = j;
            p.y = d.posicionActual().y;
            if (_campo.contenido(p) != Cultivo){
                todoEsCultivo = false;
            }
            j++;
        }

        if (todoEsCultivo){
            posMasAlejada = i;
            terminado = true;
        }
        i++;
    }
    return d.posicionActual().x - posMasAlejada;
}

int Sistema::_fertAplicable(Drone d){
    // Crease o no, estoy cumpliendo la especificacion tal cual
    int primeraAlcanzable = 0;
    bool terminado = false;
    int i = 0;
    while (i <= d.posicionActual().x || !terminado){
        if (_cantFertilizables(i,d) <= _cuentaProds(Fertilizante, d.productosDisponibles())){
            primeraAlcanzable = i;
            terminado = true;
        }
        i++;
    }
    return (d.posicionActual().x - primeraAlcanzable);
}

int Sistema::_cuentaProds(Producto p, std::vector<Producto> ps){
    int contador = 0;
    unsigned int i = 0;
    while (i < ps.size()){
        if (ps.at(i) ==  p){
            contador++;
        }
        i++;
    }
    return contador;
}

int Sistema::_cantFertilizables(int i, Drone d){
    int cantFertilizbles = 0;
    int j = i;
    while (j < d.posicionActual().x){
        Posicion p;
        p.x = j;
        p.y = d.posicionActual().y;
        if (_campo.contenido(p) == Cultivo){
            if(_estado.parcelas.at(p.x).at(p.y) == RecienSembrado ||
                    _estado.parcelas.at(p.x).at(p.y) == EnCrecimiento){
                cantFertilizbles++;
            }
        }
        j++;
    }
    return cantFertilizbles;
}

int Sistema::_minimo(int a, int b){
    int minimo;
    if (a < b) minimo = a;
    else minimo = b;

    return minimo;
}

void Sistema::volarYSensar(const Drone & d){
}

void Sistema::mostrar(std::ostream & os) const{
    os << "---- SISTEMA ----\n-----------------" << std::endl;
    os << "\nCampo del sistema:\n       ----" << std::endl;
    os << _campo;

    os << "       ----\nEnjambre de drones.\n       ----" << std::endl;
    os << "Cant de drones: " << _enjambre.size() << "\n" << std::endl;
    unsigned int i = 0;
    while (i < _enjambre.size()){
        os << _enjambre.at(i);
        i++;
    }

    os << "\n        ----\nEstado de cultivos: \n        ----\n" << std::endl;

    _mostrarEstadosCultivos(os);

}

// NO SACAR ESPACIOS PLZ
void Sistema::guardar(std::ostream & os) const{
    os << "{ S ";
    _campo.guardar(os);
    os << " ";
    _guardarDrones(os);
    os << " ";
    _guardarEstadosCultivos(os);
    os << "}";

}

void Sistema::cargar(std::istream & is){
    std::string dSistema;
    std::string dCampo;
    std::string dEnjambre;
    std::string dEstadosCultivo;

    getline(is, dSistema,'\0');

    _leerSepararDatos(dSistema, dCampo, dEnjambre, dEstadosCultivo);
    _cargarCampo(dCampo);
    _cargarEnjambre(dEnjambre);
    _cargarEstadosCultivo(dEstadosCultivo);

}

void Sistema::_leerSepararDatos(std::string &dSistema, std::string &dCampo,
                                std::string &dEnjambre, std::string &dEstadosCultivo){
    int i = 1;
    int ultimoInicio = 0;
    int ultimoFin = 0;
    bool terminado = false;
    bool necesitaCierre = false;
    bool sinDrones = true;

    while (!terminado){
        if (dSistema[i] == '{'){
            ultimoInicio = i;
            necesitaCierre = true;
        }
        if (dSistema[i] == '}'){
                if (necesitaCierre){
                    if (dSistema[ultimoInicio+2] == 'C'){
                        dCampo = dSistema.substr(ultimoInicio, i-ultimoInicio+1);
                        ultimoFin = i;
                        necesitaCierre = false;
                    }
                    if (dSistema[ultimoInicio+2] == 'D'){
                        dEnjambre += dSistema.substr(ultimoInicio, i-ultimoInicio+1);
                        ultimoFin = i;
                        sinDrones = false;
                        necesitaCierre = false;
                    }

                }else{
                    if(sinDrones){
                        dEstadosCultivo = dSistema.substr(ultimoFin + 5, dSistema.npos - ultimoFin-6);
                    }else{
                        dEstadosCultivo = dSistema.substr(ultimoFin + 3, dSistema.npos - ultimoFin-4);
                    }

                    terminado = true;
                }
            }

        i++;

    }
}

void Sistema::_cargarCampo(std::string &dCampo){
    // Convierto la string con los datos a un iStringStream (que es un iStream)
    // para poder usar directamente l cargar de campo.
    // Si no hago esto podria copiar/pegar la funcion desde drone, en la parte que cargo desde string.
    // Como no puedo crear clase publica, ni tampoco quiero copypastear, hago este cambio de tipo

     std::istringstream campoIstream(dCampo);
    _campo.cargar(campoIstream);
}

void Sistema::_cargarDroneIndividual(Drone &d, std::string &dDrone){
    // Convierto la string con los datos a un iStringStream (que es un iStream)
    // para poder usar directamente 'cargar' de drone.
    // Si no hago esto podria copiar/pegar la funcion desde drone, en la parte que cargo desde string.
    // Como no puedo crear clase publica, ni tampoco quiero copypastear, hago este cambio de tipo
    std::istringstream droneIstream(dDrone);
    d.cargar(droneIstream);
}

void Sistema::_cargarEnjambre(std::string &dEnjambre){
    std::cout << "ENJAMBRE\n" << dEnjambre << std::endl;
    // Vacio el enjambre anterior.
    // La idea es crear drone nuevos con los datos, y appendearlos al enjambre
    _enjambre.resize(0);
    unsigned int i = 0;
    int ultimoInicio = 0;
    std::string dDrone = "";

    while (i < dEnjambre.size()){
        if (dEnjambre[i] == '{'){
            ultimoInicio = i;
        }
        if (dEnjambre[i] == '}'){
            Secuencia<Producto> ps;
            Drone d(0, ps);

            dDrone = dEnjambre.substr(ultimoInicio, i-ultimoInicio);
            _cargarDroneIndividual(d, dDrone);
            _enjambre.push_back(d);
        }
        i++;
    }
}

void Sistema::_cargarEstadosCultivo(std::string &dEstados){
    _estado.parcelas.resize(0, std::vector<EstadoCultivo>(0));
    _estado.parcelas.resize(_campo.dimensiones().ancho, std::vector<EstadoCultivo>(_campo.dimensiones().largo, NoSensado));

    int i = 1;
    int posX = 0;
    int posY = 0;
    bool terminado = false;
    bool necesitaCierre = false;
    std::string datoActual = "";

    std::cout << "\ncarga estados cultivos \n";

    while (!terminado){

        if (necesitaCierre){
            std::cout << dEstados[i];
            if (dEstados[i] == ','){
                _cargarEstadoIndividual(posX, posY, datoActual);
                datoActual = "";
                posX++;
            }

            else if (dEstados[i] == ']'){
                necesitaCierre = false;
                _cargarEstadoIndividual(posX, posY, datoActual);
                datoActual = "";
                posX = 0;
                posY++;
            }
            else{
                datoActual += dEstados[i];
            }
        }

        // necesitaCierre es util para ignorar la coma entre las listas
        if (dEstados[i] == '['){
            necesitaCierre = true;
        }

        if (dEstados[i] == ']' && dEstados[i+1] == '}'){
            terminado = true;
        }
        i++;
    }
    std::cout << "\nfin estados cultivos\n" << std::endl;
}

void Sistema::_cargarEstadoIndividual(int x, int y, std::string dEstado){
//RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado
    if(dEstado == "RecienSembrado") _estado.parcelas.at(x).at(y) = RecienSembrado;
    if(dEstado == "EnCrecimiento") _estado.parcelas.at(x).at(y) = EnCrecimiento;
    if(dEstado == "ListoParaCosechar") _estado.parcelas.at(x).at(y) = ListoParaCosechar;
    if(dEstado == "ConMaleza") _estado.parcelas.at(x).at(y) = ConMaleza;
    if(dEstado == "ConPlaga") _estado.parcelas.at(x).at(y) = ConPlaga;
    if(dEstado == "NoSensado") _estado.parcelas.at(x).at(y) = NoSensado;
}

void Sistema::_guardarDrones(std::ostream &os) const{
    os << "[";
    unsigned int i = 0;
    while (i < _enjambre.size()){
        _enjambre.at(i).guardar(os);
        i++;
        if (i < _enjambre.size()){
            os << ",";
        }
    }
    os << "]";
}

void Sistema::_guardarEstadosCultivos(std::ostream &os) const{
    int j = 0;
    os << "[";
    while (j < _campo.dimensiones().largo){
        os << "[";
        int i = 0;
        while (i < _campo.dimensiones().ancho){
            // Muestro contenido de parcela
            Posicion p;
            p.x = i;
            p.y = j;
            os << estadoDelCultivo(p);

            i++;
            if (i < _campo.dimensiones().ancho){
                os << ",";
            }
        }

        j++;
        if (j < _campo.dimensiones().largo){
            os << "], ";
        }else{
            os << "]";
        }
    }
    os << "]";
}

void Sistema::_mostrarEstadosCultivos(std::ostream &os) const{
    int j = 0;
    while (j < _campo.dimensiones().largo){
        // Muestro estado de cultivos
        os << "    ";
        int i = 0;
        while (i < _campo.dimensiones().ancho){
            Posicion p;
            p.x = i;
            p.y = j;
            os << estadoDelCultivo(p);

            i++;

            if (i < _campo.dimensiones().ancho){
                os << " ";
            }else{
                os << std::endl;
            }
        }
        j++;
    }
    os << std::endl;

}

bool Sistema::operator==(const Sistema & otroSistema) const{
    return _campo == otroSistema.campo() &&
            _igualEstados(otroSistema) &&
            _mismosDrones(otroSistema);
}

bool Sistema::_igualEstados(const Sistema &s) const{
    bool iguales = true;
    unsigned int i = 0;

    while (i < _estado.parcelas.size()){
        unsigned int j = 0;

        // Si _igualEstados se ejecuta es porque los campos eran iguales
        // Como los campos tienen dimension (igual a los estados de cultivo),
        //puedo acceder a las posiciones sin que se indefina
        while (j < _estado.parcelas.at(0).size()){
            Posicion p;
            p.x = i;
            p.y = j;
            if (estadoDelCultivo(p) != s.estadoDelCultivo(p)){
                iguales = false;
            }
            j++;
        }
        i++;
    }
    return iguales;
}

bool Sistema::_mismosDrones(const Sistema &s) const{

    bool mismos = true;
    if (_enjambre.size() == s.enjambreDrones().size()){

        unsigned int i = 0;
        // Recordar que como los ids son unicos,
        // no hay dos drones iguales en la misma lista

        while (i < _enjambre.size()){
            if (!_pertenece(_enjambre.at(i), s.enjambreDrones() )){
                mismos = false;
            }
            i++;
        }


    }else{
        mismos = false;
    }
    return mismos;
}

bool Sistema::_pertenece(Drone d, const Secuencia<Drone> enjambre) const{
    bool pertenece = false;
    unsigned int i = 0;
    while (i < enjambre.size()){
        if (d == enjambre.at(i)){
            pertenece = true;
        }
        i++;
    }
    return pertenece;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e){
    if (e == NoSensado) os << "NoSensado";
    if (e == RecienSembrado) os << "RecienSembrado";
    if (e == EnCrecimiento) os << "EnCrecimiento";
    if (e == ListoParaCosechar) os << "ListoParaCosechar";
    if (e == ConMaleza) os << "ConMaleza";
    if (e == ConPlaga) os << "ConPlaga";

    return os;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s){
    s.mostrar(os);
	return os;
}

Posicion Sistema::posGranero() const {
    Posicion posGranero;
    int i = 0;
    while(i < _campo.dimensiones().ancho){
        int j = 0;
        while(j < _campo.dimensiones().largo){
            Posicion p;
            p.x = i;
            p.y = j;
            if(_campo.contenido(p) == Granero){
                posGranero.x = i;
                posGranero.y = j;
            }
            j++;
        }
        i++;
    }
    return posGranero;
}
