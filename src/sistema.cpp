#include "sistema.h"
#include "auxiliares.h"
#include <algorithm>
#include <sstream>

Sistema::Sistema(){
    Posicion posG;
    posG.x = 0;
    posG.y = 0;

    Posicion posC;
    posC.x = 1;
    posC.y = 0;
    Campo c(posG, posC);

    Secuencia<Drone> ds;
    ds.resize(0);

    _campo = c;
    _enjambre = ds;
    _estado.parcelas.resize(c.dimensiones().ancho, std::vector<EstadoCultivo>(c.dimensiones().largo, NoSensado));
}

Sistema::Sistema(const Campo &c, const Secuencia<Drone> &ds){
    _campo = c;
	_enjambre = ds;
	_estado.parcelas.resize(c.dimensiones().ancho, std::vector<EstadoCultivo>(c.dimensiones().largo, NoSensado));
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
    // posicionesPlaga es una lista con todas las posiciones que tienen plaga antes de modificar el sistema
    std::vector<Posicion> posicionesPlaga = _posicionesConPlaga();
    unsigned int i = 0;
    while (i < posicionesPlaga.size()) {
        // para cada posicion con plaga, expandir la plaga a todos los cultivos vecinos
        _plaguizarVecinos(posicionesPlaga.at(i));
        i++;
    }

}

// Dada una posicion, devuelve una lista pon las 4 posiciones adyacentes (No chequea rango)
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
    // vecinosConCultivos sonlas nuevas parcelas que deben tener plaga,
    // es decir, los cultivos adyacentes que esten en rango
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

// Devuelve una lista con los cultivos adyacentes que esten en rango
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
                posicionesPlaga.push_back({i,j});
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
            _enjambre.at(i).moverA(_parcelaCultivoLibre());
        }
        i++;
    }
}

// Cualquier parcela de cultivo libre es correcta
// En particular, la primera que encuentra es correcta
Posicion Sistema::_parcelaCultivoLibre() const{
    Posicion res;

    int i = 0;
    while (i < _campo.dimensiones().ancho){
        int j = 0;
        while (j < _campo.dimensiones().largo){
            Posicion p;
            p.x = i;
            p.y = j;

            // Es libre si la parcela adyacente es cultivo y no tiene drones
            if (_esAdyacente(p, posGranero()) &&
                    _campo.contenido(p) == Cultivo &&
                    _esLibreDeDrones(p)){
                res = p;
                break;
            }
            j++;
        }
        i++;
    }

    return res;
}

bool Sistema::_esLibreDeDrones(Posicion p) const{

    unsigned int i = 0;
    bool libre = true;
    while (i < _enjambre.size()){
        if (_enjambre.at(i).posicionActual().x == p.x &&
                _enjambre.at(i).posicionActual().y == p.y &&
                _enjambre.at(i).enVuelo()){
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
    float cantCosechable = _cuentaCosechables();
    float totalCultivos = _parcelasCultivo().size();
    return cantCosechable/totalCultivos >= 0.9f;
}

int Sistema::_cuentaCosechables() const{
    int total = 0;
    unsigned int i = 0;
    while(i < _parcelasCultivo().size()){
        if (estadoDelCultivo(_parcelasCultivo().at(i)) == ListoParaCosechar){
            total++;
        }
        i++;
    }
    return total;
}

std::vector<Posicion> Sistema::_parcelasCultivo() const{
    std::vector<Posicion> listaParcelasCultivo;

    int i = 0;
    while (i < campo().dimensiones().ancho){
        _parcelasCultivoLargo(listaParcelasCultivo, i);
        i++;
    }

    return listaParcelasCultivo;
}

void Sistema::_parcelasCultivoLargo(std::vector<Posicion> &parcelasCultivoLargo, int fila) const{
    int j = 0;
    while (j < campo().dimensiones().largo){
        Posicion p;
        p.x = fila;
        p.y = j;
        if (campo().contenido(p) == Cultivo){
            parcelasCultivoLargo.push_back(p);
        }
        j++;
    }
}

void Sistema::aterrizarYCargarBaterias(Carga b){
    unsigned int i = 0;
    while(i < _enjambre.size()){
        if(_enjambre.at(i).bateria() < b){
            _enjambre.at(i).setBateria(100);
            _enjambre.at(i).borrarVueloRealizado();
            // borrarVueloRealizado pone enVuelo en false, puedo usar cambiarPosicionActual
            _enjambre.at(i).cambiarPosicionActual(posGranero());
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

void Sistema::_moverDrone(std::vector<Posicion> recorridoDrone, Drone &d){
    unsigned int i = 0;
    while (i < recorridoDrone.size()){
        d.moverA(recorridoDrone.at(i));
        d.setBateria(d.bateria() - 1);
        d.sacarProducto(Fertilizante);
        i++;
    }
}

// Devuelve una lista con todas las posiciones por las que el drone pasa
std::vector<Posicion> Sistema::_recorridoDrone(Drone d, int recorridoMaximo){
    std::vector<Posicion> posRecorridas;
    int i = 1;
    std::cout << "RECORRIDO MAXIMO : " << recorridoMaximo << std::endl;
    while (i <= recorridoMaximo){
        Posicion p;
        p.x = d.posicionActual().x - i;
        p.y = d.posicionActual().y;
        posRecorridas.push_back(p);

        i++;
    }
    return posRecorridas;
}

// La cantidad maxima de posiciones que puede recorrer, esta limitado por
// su bateria, la cantidad de fertilizante y las parcelas libres en su direccion
int Sistema::_recorridoMaximo(Drone d){
    return _minimo(_minimo(_fertAplicable(d), d.bateria()), _parcelasLibres(d));
}

// Devuelve la cantidad de parcelas validad a su izquierda
int Sistema::_parcelasLibres(Drone d){
    int posMasAlejada = 0;
    bool terminado = false;
    int i = 0;
    while (!terminado){

        int j = i;
        bool todoEsCultivo = true;
        // Devuelve verdadero si desde i hasta la posActual, todo es cultivo
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

        // Si i es mayor a la coordenada x de la posicion,
        // cuando se haga la resta con la posicionMasAlejada
        // puede ocurrir que el drone se salga del borde, por eso para de contar aca
        if ( i >= d.posicionActual().x){
            terminado = true;
        }
    }
    return d.posicionActual().x - posMasAlejada;
}

// Cuenta cuanto fertilizante puede aplicar el drone. Ver auxiliar en especificacion.
int Sistema::_fertAplicable(Drone d){
    int primeraAlcanzable = 0;
    bool terminado = false;
    int i = 0;
    while (!terminado){
        if (_cantFertilizables(i,d) <= _cuentaProds(Fertilizante, d.productosDisponibles())){
            primeraAlcanzable = i;
            terminado = true;
        }
        i++;
        if (i <= d.posicionActual().x){
            terminado = true;
        }
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
    Secuencia<Posicion> adyacentesvalidas = _darPosicionesValidas(d.posicionActual());
    moverUnaPosicionAlDrone(adyacentesvalidas.at(0), d);
    if(_campo.contenido(adyacentesvalidas.at(0)) != Cultivo){
        restarBateria(d.bateria() - 1, d);
    }else{
        if(_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ListoParaCosechar){
            restarBateria(d.bateria() - 1, d);
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == NoSensado)){
            restarBateria(d.bateria() - 1, d);
            cambiaARecienSembrado(adyacentesvalidas.at(0));
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == RecienSembrado) || 
        (_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == EnCrecimiento)){
            restarBateria(d.bateria() - 1, d);
            if(tieneFertilizante(d)){
                quitarProducto(Fertilizante, d);
                cambiaAListoParaCosechar(adyacentesvalidas.at(0));
            }
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ConMaleza) && (d.bateria() >= 6)){
            if(tieneHerbicida(d)){
                restarBateria(d.bateria() - 6, d);
                quitarProducto(Herbicida, d);
                cambiaARecienSembrado(adyacentesvalidas.at(0));
            }
            if(!tieneHerbicida(d) && tieneHerbicidaLargoAlcance(d)){
                restarBateria(d.bateria() - 6, d);
                quitarProducto(HerbicidaLargoAlcance, d);
                cambiaAdyacentesConMalezaARecienSembrado(adyacentesvalidas.at(0));
            }
            if(!tieneHerbicida(d) && !tieneHerbicidaLargoAlcance(d)){
                restarBateria(d.bateria() - 1, d);
            }
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ConMaleza) && (d.bateria() < 6)){
            restarBateria(d.bateria() - 1, d);
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ConPlaga) && (d.bateria() >= 11)){
            if(tienePlaguicidaBajoConsumo(d)){
                restarBateria(d.bateria() - 6, d);
                quitarProducto(PlaguicidaBajoConsumo, d);
                cambiaARecienSembrado(adyacentesvalidas.at(0));
            }
            if(!tienePlaguicidaBajoConsumo(d) && (tienePlaguicida(d))){
                restarBateria(d.bateria() - 11, d);
                quitarProducto(Plaguicida, d);
                cambiaARecienSembrado(adyacentesvalidas.at(0));
            }
            if(!tienePlaguicidaBajoConsumo(d) && !tienePlaguicida(d)){
                restarBateria(d.bateria() - 1, d);
            }
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ConPlaga) && (d.bateria() < 11) && (d.bateria() >= 6)){
            if(tienePlaguicidaBajoConsumo(d)){
                restarBateria(d.bateria() - 6, d);
                quitarProducto(PlaguicidaBajoConsumo, d);
                cambiaARecienSembrado(adyacentesvalidas.at(0));
            }
            else{
                restarBateria(d.bateria() - 1, d);
            }
        }
        if((_estado.parcelas.at(adyacentesvalidas.at(0).x).at(adyacentesvalidas.at(0).y) == ConPlaga) && (d.bateria() < 6)){
            restarBateria(d.bateria() - 1, d);
        }
    }
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

    while (!terminado){

        if (necesitaCierre){
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
            os << "],";
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

template<class T>
unsigned int Sistema::cuenta(const T &x, const std::vector<T> &v) const{
    unsigned int cant = 0;
    for (unsigned int i = 0; i < v.size(); ++i) {
        if (x == v[i]) ++cant;
    }
    return cant;
}

template<class T>
bool Sistema::mismos(const std::vector<T> &a, const std::vector<T> &b) const {
    bool res = a.size() == b.size();
    for (unsigned int i = 0; res && i < a.size(); ++i) {
        res = cuenta(a[i], a) == cuenta(a[i], b);
    }
    return res;
}

bool Sistema::_mismosDrones(const Sistema &s) const{
    // DEFINIR EL MISMOS DENTRO DEL SISTEMA
    return mismos(_enjambre, s.enjambreDrones());
    /*
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
    return mismos;  */
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

Secuencia<Posicion> Sistema::_darPosicionesValidas(const Posicion posactual){
    Secuencia<Posicion> validas;
    int j = 0;
    while(j < _campo.dimensiones().ancho){
        int k = 0;
        while(k < _campo.dimensiones().largo){
            Posicion q;
            q.x = j;
            q.y = k;
            if((_esAdyacente(q, posactual))&&(_esLibreDeDrones(q))){
                validas.push_back(q);
            }
            k++;
        }
        j++;
    }
    return validas;
}

bool Sistema::tieneFertilizante(const Drone d){
    unsigned int i = 0;
    bool t = false;
    while(i < d.productosDisponibles().size()){
        if(d.productosDisponibles().at(i) == Fertilizante){
            t = true;
        }
        i++;
    }
    return t;
}

bool Sistema::tienePlaguicida(const Drone d){
    unsigned int i = 0;
    bool t = false;
    while(i < d.productosDisponibles().size()){
        if(d.productosDisponibles().at(i) == Plaguicida){
            t = true;
        }
        i++;
    }
    return t;
}

bool Sistema::tienePlaguicidaBajoConsumo(const Drone d){
    unsigned int i = 0;
    bool t = false;
    while(i < d.productosDisponibles().size()){
        if(d.productosDisponibles().at(i) == PlaguicidaBajoConsumo){
            t = true;
        }
        i++;
    }
    return t;
}

bool Sistema::tieneHerbicida(const Drone d){
    unsigned int i = 0;
    bool t = false;
    while(i < d.productosDisponibles().size()){
        if(d.productosDisponibles().at(i) == Herbicida){
            t = true;
        }
        i++;
    }
    return t;
}

bool Sistema::tieneHerbicidaLargoAlcance(const Drone d){
    unsigned int i = 0;
    bool t = false;
    while(i < d.productosDisponibles().size()){
        if(d.productosDisponibles().at(i) == HerbicidaLargoAlcance){
            t = true;
        }
        i++;
    }
    return t;
}

Secuencia<Posicion> Sistema::adyacentesACambiar(const Posicion posactual){
    Secuencia<Posicion> estadoacambiar;
    int j = 0;
    while(j < _campo.dimensiones().ancho){
        int k = 0;
        while(k < _campo.dimensiones().largo){
            Posicion q;
            q.x = j;
            q.y = k;
            if((_esAdyacente(q, posactual)) && (_campo.contenido(q) == Cultivo)){
                if(estadoDelCultivo(q) == ConMaleza){
                    estadoacambiar.push_back(q);
                }
            }
            k++;
        }
        j++;
    }
    return estadoacambiar;
}

void Sistema::cambiaAListoParaCosechar(const Posicion posactual){
    _estado.parcelas.at(posactual.x).at(posactual.y) = ListoParaCosechar;
}

void Sistema::cambiaARecienSembrado(const Posicion posactual){
    _estado.parcelas.at(posactual.x).at(posactual.y) = RecienSembrado;
}

void Sistema::cambiaAdyacentesConMalezaARecienSembrado(const Posicion posactual){
    Secuencia<Posicion> xs = adyacentesACambiar(posactual);
    xs.push_back(posactual);
    unsigned int i = 0;
    while(i < xs.size()){
        _estado.parcelas.at(xs.at(i).x).at(xs.at(i).y) = RecienSembrado;
        i++;
    }
}

void Sistema::moverUnaPosicionAlDrone(Posicion p, Drone d){
    unsigned int i = 0;
    while(i < _enjambre.size()){
        if(_enjambre.at(i).id() == d.id()){
            _enjambre.at(i).moverA(p);
        }
        i++;
    }
}

void Sistema::restarBateria(int x, Drone d){
    unsigned int i = 0;
    while(i < _enjambre.size()){
        if(_enjambre.at(i).id() == d.id()){
            _enjambre.at(i).setBateria(x);
        }
        i++;
    }
}

void Sistema::quitarProducto(Producto p, Drone d){
    unsigned int i = 0;
    while(i < _enjambre.size()){
        if(_enjambre.at(i).id() == d.id()){
            _enjambre.at(i).sacarProducto(p);
        }
        i++;
    }
}
