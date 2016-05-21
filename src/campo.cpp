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
    os << "Campo" << std::endl;
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
    std::cout << infoDelCampo << std::endl;

    _leerYCargarDatos(infoDelCampo, _dimension, _grilla);

}

// Recibe el string de datos SIN LAS LLAVES
// NO TOCAR PORQUE ESTO LO USA EL CARGAR
void Campo::_leerYCargarDatos(std::string datos, Dimension &dim, Grilla<Parcela> &grilla){
   /*
    // Quiero que mi variable que apunta comience en la C
    // Puede haber un espacio inicial, tenerlo en cuenta
    int i = 0;
    if (datos[1] == 'C'){
        i = 1;
    }

    datos = datos.substr(i, datos.npos);
    std::cout << datos << std::endl;
*/
    bool terminado = false;
    bool leer = false;
    std::string loQueQuiero = "";
    int i = 0;
    while (!terminado){
        if (datos[i] == '['){
            leer = true;
        }
        if (leer){
            loQueQuiero += datos[i];
        }
        if (datos[i] == ']'){
            terminado = true;
        }
        i++;
    }

    std::cout << loQueQuiero << std::endl;

  //  dim.ancho = datos[3];
  //  dim.largo = datos[5];

  //  std::cout

//    datos = datos.substr(7, datos.npos);

    //_cargarDimension(i, datos, dim);
}

/*void Campo::_cargarDimension(int &i, std::string datos, Dimension &dim){
    datosDimension = datos.substr(i + 3, 3);
    dim.ancho = datos[3];
    dim.largo = datos[5];
}*/

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
