#include "sistema.h"
#include "tipos.h"
#include <algorithm>

Sistema::Sistema(){
}

Sistema::Sistema(const Campo &c, const Secuencia<Drone> &ds){
	_campo = c;
	_enjambre = ds;
	initEstados();
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
}

void Sistema::seExpandePlaga(){
}

void Sistema::despegar(const Drone & d){
}

bool Sistema::listoParaCosechar() const{
	return false;
}

void Sistema::aterrizarYCargarBaterias(Carga b){
}

void Sistema::fertilizarPorFilas(){
}

void Sistema::volarYSensar(const Drone & d){
}

void Sistema::mostrar(std::ostream & os) const{
}

void Sistema::guardar(std::ostream & os) const{
}

void Sistema::cargar(std::istream & is){
}

bool Sistema::operator==(const Sistema & otroSistema) const{
	return this->_campo == otroSistema._campo 
			&& this->_estado == otroSistema._estado
			/*&& std::is_permutation(_enjambre.begin(), _enjambre.end(), otroSistema._enjambre.begin())*/;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s){
	// TODO: insert return statement here
	return os;
}

/*void Sistema::initEstados(){
	unsigned int i = 0;
	while(i < _estado.parcelas.size()){
		unsigned int j = 0;
		while(j < _estado.parcelas.at(i).size()){
			_estado.parcelas.at(i).at(j) = NoSensado;
			j++;
		}
		i++;
	}
}*/


void Sistema::initEstados(){
	unsigned int i = 0;
	while(i < _estado.parcelas.size()){
		std::fill(_estado.parcelas.at(i).begin(), _estado.parcelas.at(i).end(), NoSensado);		
		i++;
	}
}