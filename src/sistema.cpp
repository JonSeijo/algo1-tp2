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
}

void Sistema::despegar(const Drone & d){
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
}

void Sistema::fertilizarPorFilas(){
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

    std::cout << dSistema << "\n" << std::endl;

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

        if (dEstados[i] == '['){
            necesitaCierre = true;
        }

        if (!necesitaCierre){
            if (dEstados[i] == ']' && dEstados[i+1] == '}'){
                terminado = true;
            }
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
    return false;
    //this->_campo == otroSistema._campo
    //		&& this->_estado == otroSistema._estado
//			/*&& std::is_permutation(_enjambre.begin(), _enjambre.end(), otroSistema._enjambre.begin())*/;
//*/
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
