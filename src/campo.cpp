#include "campo.h"
#include <cmath>
#include <fstream>
#include <cstdlib>

Campo::Campo(){
    _dimension.ancho = 3;
    _dimension.largo = 1;
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));
    _grilla.parcelas.at(0).at(0) = Granero;
    _grilla.parcelas.at(1).at(0) = Casa;
}

// Propiedad de jonathan
Campo::Campo(const Posicion &posG, const Posicion &posC){
   // La dimension es +1 porque vendria a ser "el largo de la lista"
   // Si el mas lejano esta en el 0, no quiero dimension 0 porque no podria entrar
   // quiero dimension +1
    _dimension.ancho = std::max(posG.x, posC.x) + 1;
    _dimension.largo = std::max(posG.y, posC.y) + 1;

    // Si las dimensiones son 2x1, no va a haber espacio para cultivos.
    // Para solucionarlo, lo convierto en un 2x2
    if ((_dimension.ancho == 1 && _dimension.largo == 2)
            || (_dimension.ancho == 2 && _dimension.largo == 1)){
        _dimension.ancho = 2;
        _dimension.largo = 2;
    }

    // Esto hace que la grilla tenga dimensiones _dimension
    // Y que TODAS las posiciones se seteen con Cultivo
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));

    // Manualmente seteo las otras dos posiciones
    _grilla.parcelas.at(posG.x).at(posG.y) = Granero;
    _grilla.parcelas.at(posC.x).at(posC.y) = Casa;
}

Dimension Campo::dimensiones() const{
	return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const{
	return _grilla.parcelas.at(p.x).at(p.y);
}

void Campo::mostrar(std::ostream & os) const{
   // os << "Campo" << std::endl;
    os << "Dimensiones: " << std::endl;
    os << "    " << "Ancho: " << _dimension.ancho << std::endl;
    os << "    " << "Largo: " << _dimension.largo << std::endl;
    os << "Contenidos: " << std::endl;

    int j = 0;
    while (j < _dimension.largo){
        // Muestro contenido de parcela
        os << "    ";
        int i = 0;
        while (i < _dimension.ancho){
            Posicion p;
            p.x = i;
            p.y = j;
            os << contenido(p);

            i++;

            if (i < _dimension.ancho){
                os << " ";
            }else{
                os << std::endl;
            }
        }

        j++;
    }
    os << std::endl;
}

// NO MODIFICAR ESPACIOS NI NADA PORQUE ROMPE EL 'CARGAR'
// SI SE TOCA, HAY TABLA
void Campo::guardar(std::ostream & os) const{
    os << "{ C ";
    os << "[" << _dimension.ancho << "," << _dimension.largo << "] ";

    int j = 0;
    os << "[";
    while (j < _dimension.largo){
        os << "[";
        int i = 0;
        while (i < _dimension.ancho){
            // Muestro contenido de parcela
            Posicion p;
            p.x = i;
            p.y = j;
            os << contenido(p);

            i++;

            if (i < _dimension.ancho){
                os << ",";
            }

        }

        j++;
        if (j < _dimension.largo){
            os << "], ";
        }else{
            os << "]";
        }
    }

    os << "]}";
}

// NO MODIFICAR NI TOCAR NADA PORQUE ESTA HECHO EN BASE AL 'GUARDAR'
// SI SE TOCA, HAY TABLA
void Campo::cargar(std::istream & is){
    // Obtiene todo lo del tipo entre '{' y '}'.
    // Aca no es tan util, puede servir mucho hacer algo asi en el de sistema
    std::string infoDelCampo;
    char var;
    while (is >> var){
        if (var == '{'){
            getline(is, infoDelCampo,'}');
            break;
        }
    }
    // std::cout << infoDelCampo << std::endl;
    _leerYCargarDatos(infoDelCampo, _dimension, _grilla);

}

// Recibe el string de datos SIN LAS LLAVES
// NO TOCAR PORQUE ESTO LO USA EL CARGAR
void Campo::_leerYCargarDatos(std::string datos, Dimension &dim, Grilla<Parcela> &grilla){
    std::string datoDimension = _dameStringConDato(datos, false);
    std::string datoContenido = _dameStringConDato(datos, true);

    // Como ahora tengo los datos separados, y se que forma tienen,
    // A mano rescato los valores y los asigno

    // Para dimension hago substrings quitando corchetes y separando con la coma
    int posSeparador = datoDimension.find(',');
    dim.ancho = atoi(datoDimension.substr(1, posSeparador - 1).c_str());
    dim.largo = atoi(datoDimension.substr(posSeparador + 1, datoDimension.length() - posSeparador - 2).c_str());

    // Resize todo con cultivo. Lo pongo en 0 primero para borrar granero y casa viejas
    grilla.parcelas.resize(0, std::vector<Parcela>(0, Cultivo));
    grilla.parcelas.resize(dim.ancho, std::vector<Parcela>(dim.largo, Cultivo));

    int indexGranero = datoContenido.find("Granero");
    int indexCasa = datoContenido.find("Casa");

    // String con todo el texto anterior a lo que quiero ubicar
    std::string textoAnteriorGranero = datoContenido.substr(1, indexGranero - 1);
    std::string textoAnteriorCasa = datoContenido.substr(1, indexCasa - 1);

    // Busca y ubica la parcela en la posicion correspondiente
    _setearParcelaEnCarga(textoAnteriorGranero, grilla, dim.ancho, Granero);
    _setearParcelaEnCarga(textoAnteriorCasa, grilla, dim.ancho, Casa);
}

void Campo::_setearParcelaEnCarga(std::string textoAnterior, Grilla<Parcela> &grilla, int elementosPorFila, Parcela parcelaCorrespondiente){
    int y = _contarOcurrencias(textoAnterior, ']');
    int x = _contarOcurrencias(textoAnterior, ',') - elementosPorFila*y;
    grilla.parcelas.at(x).at(y) = parcelaCorrespondiente;
}

int Campo::_contarOcurrencias(std::string cadena, char aBuscar){
    int contador = 0;
    unsigned int i = 0;
    while (i < cadena.length()){
        if (cadena[i] == aBuscar){
            contador++;
        }
        i++;
    }

    return contador;
}

// _dameStringConDato devuelve el primer objeto como string, Y LO ELIMINA del dato original
// Objeto simple: [x,x,x,x]
// Objeto compuesto: [ [x,x,x], [x,x,xx] ]
std::string Campo::_dameStringConDato(std::string &datos, bool objetoCompuesto){
    bool terminado = false;
    bool leer = false;
    char charAnterior;
    std::string stringConDato = "";
    int i = 0;
    while (!terminado){
        if (datos[i] == '['){
            leer = true;
        }
        if (leer){
            if (i > 0)
                charAnterior = datos[i-1];
            stringConDato += datos[i];
        }
        if (objetoCompuesto){
            // Objeto termina con "]]" si es compuesto
            terminado = (datos[i] == ']') && (charAnterior == ']');
        }else{
            // Objeto termina con un solo ']' si es simple
            terminado = (datos[i] == ']');
        }
        i++;
    }

    //std::cout << stringConDato << std::endl;
    datos = datos.substr(i, datos.npos);
    return stringConDato;
}

bool Campo::_grillaIgual(const Campo &c) const{
    bool iguales = true;
    int i = 0;
    int j = 0;
    while (i < _dimension.ancho){
        while (j < _dimension.largo){
            Posicion p;
            p.x = i;
            p.y = j;
            if (contenido(p) != c.contenido(p)){
                iguales = false;
            }
            j++;
        }
        i++;
    }

    return iguales;
}


bool Campo::operator==(const Campo & otroCampo) const{
    return _dimension.ancho == otroCampo.dimensiones().ancho
            && _dimension.largo == otroCampo.dimensiones().largo
            && _grillaIgual(otroCampo);
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

/*
 * DISCLAIMER (jonathan), estoy casi seguro que estan mal aca,
 * Porque son necesarias en drone y en sistema.
 * Producto no tiene nada que ver con campo, las pasteo donde me parece
 *

std::ostream & operator<<(std::ostream & os, const Producto & p){
	return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e){
	return os;
}
*/
