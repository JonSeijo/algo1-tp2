#include "campo.h"
#include <cmath>

Campo::Campo(){

}

// Propiedad de jonathan
Campo::Campo(const Posicion &posG, const Posicion &posC){
   // La dimension es +1 porque vendria a ser "el largo de la lista"
   // Si el mas lejano esta en el 0, no quiero dimension 0 porque no podria entrar
   // quiero dimension +1
    _dimension.ancho = _vectorMasLejano(posG, posC).x + 1;
    _dimension.largo = _vectorMasLejano(posG, posC).y + 1;

    // Esto hace que la grilla tenga dimensiones _dimensiones
    // Y que TODAS las posiciones se seteen con Cultivo
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));

    // Manualmente seteo las otras dos posiciones
    _grilla.parcelas.at(posG.x).at(posG.y) = Granero;
    _grilla.parcelas.at(posC.x).at(posC.y) = Casa;

    // TEST PARA VER CONTENIDOS DE PARCELAS. BORRAR LUEGO.

    int i = 0;
    while (i < _dimension.ancho){
        int j = 0;
        while (j < _dimension.largo){
            std::cout << "i " << i << " j " << j << std::endl;
            if (_grilla.parcelas.at(i).at(j) == Cultivo){
                std::cout << "Cultivo" << std::endl;
            }
            if (_grilla.parcelas.at(i).at(j) == Granero){
                std::cout << "Granero" << std::endl;
            }
            if (_grilla.parcelas.at(i).at(j) == Casa){
                std::cout << "Casa" << std::endl;
            }

            j++;
        }
        i++;
    }

}

/* Por el invariante tienen posiciones positivas,
 * entonces el mas alejado es el que tiene mayor norma*/
Posicion Campo::_vectorMasLejano(const Posicion &v1, const Posicion &v2){
    Posicion elMasLejano;
    if (_norma(v1) > _norma(v2)){
        elMasLejano = v1;
    }else{
        elMasLejano = v2;
    }

    return elMasLejano;
}

double Campo::_norma(const Posicion &v){
    return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2));
}

bool Campo::_posicionIgual(int i, int j, const Posicion &v){
    return i == v.x && j == v.y;
}

Dimension Campo::dimensiones() const{
	return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const{
	return _grilla.parcelas.at(p.x).at(p.y);
}

void Campo::mostrar(std::ostream & os) const{}

void Campo::guardar(std::ostream & os) const{}

void Campo::cargar(std::istream & is){}

bool Campo::operator==(const Campo & otroCampo) const{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Campo & c){
	return os;
}

std::ostream & operator<<(std::ostream & os, const Parcela & p){
	return os;
}

std::ostream & operator<<(std::ostream & os, const Producto & p){
	return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e){
	return os;
}
