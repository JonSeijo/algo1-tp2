#include "campo.h"
#include <cmath>
#include <fstream>

Campo::Campo(){
    _dimension.ancho = 3;
    _dimension.largo = 1;
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));
    _grilla.parcelas.at(0).at(0) = Granero;
    _grilla.parcelas.at(0).at(1) = Casa;
}

// Propiedad de jonathan
Campo::Campo(const Posicion &posG, const Posicion &posC){
   // La dimension es +1 porque vendria a ser "el largo de la lista"
   // Si el mas lejano esta en el 0, no quiero dimension 0 porque no podria entrar
   // quiero dimension +1
    _dimension.ancho = std::max(posG.x, posC.x) + 1;
    _dimension.largo = std::max(posG.y, posC.y) + 1;

    // Esto hace que la grilla tenga dimensiones _dimension
    // Y que TODAS las posiciones se seteen con Cultivo
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));

    // Manualmente seteo las otras dos posiciones
    _grilla.parcelas.at(posG.x).at(posG.y) = Granero;
    _grilla.parcelas.at(posC.x).at(posC.y) = Casa;

    // TEST PARA VER CONTENIDOS DE PARCELAS. BORRAR LUEGO.
/*
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
   */

}

Dimension Campo::dimensiones() const{
	return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const{
	return _grilla.parcelas.at(p.x).at(p.y);
}

void Campo::mostrar(std::ostream & os) const{
    os << "esto es un campo, papá";
}

void Campo::guardar(std::ostream & os) const{
    os << "{ C ";
    os << "[" << _dimension.ancho << "," << _dimension.largo << "] ";

    os << "[";
    int i = 0;
    while (i < _dimension.ancho){
        os << "[";
        int j = 0;
        while (j < _dimension.largo){
            // Muestro contenido de parcela
            Posicion p;
            p.x = i;
            p.y = j;
            os << contenido(p);

            j++;
            if (j < _dimension.largo){
                os << ",";
            }

        }

        i++;

        if (i < _dimension.ancho){
            os << "], ";
        }else{
            os << "]";
        }
    }

    os << "]}";
}

void Campo::cargar(std::istream & is){}

bool Campo::operator==(const Campo & otroCampo) const{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Campo & c){
    c.mostrar(os);
    return os;
}

std::ostream & operator<<(std::ostream & os, const Parcela & p){
    if (p == Cultivo) os << "Cultivo";
    if (p == Granero) os << "Granero";
    if (p == Casa) os << "Casa";

	return os;
}

std::ostream & operator<<(std::ostream & os, const Producto & p){
	return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e){
	return os;
}
