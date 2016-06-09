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

Campo::Campo(const Posicion &posG, const Posicion &posC, Dimension dimension) {
    _dimension = dimension;
    _grilla = Grilla<Parcela>(_dimension);
    _grilla.parcelas[posG.x][posG.y] = Granero;
    _grilla.parcelas[posC.x][posC.y] = Casa;
}

Campo::Campo(const Posicion &posG, const Posicion &posC){
   // La dimension es +1 porque vendria a ser "el largo de la lista"
   // Si el mas lejano esta en el 0, no quiero dimension 0 porque no podria entrar
   // quiero dimension +1
    _dimension.ancho = std::max(posG.x, posC.x) + 1;
    _dimension.largo = std::max(posG.y, posC.y) + 1;

    // Si las dimensiones son 2x1, no hay espacio para cultivos.
    // Para solucionarlo, lo conviertimos en un 2x2
    if ((_dimension.ancho == 1 && _dimension.largo == 2)
            || (_dimension.ancho == 2 && _dimension.largo == 1)){
        _dimension.ancho = 2;
        _dimension.largo = 2;
    }

    // Todas las parcelas son de cultivo
    _grilla.parcelas.resize(_dimension.ancho, std::vector<Parcela>(_dimension.largo, Cultivo));

    // Manualmente seteo el granero y la casa donde corresponde
    _grilla.parcelas.at(posG.x).at(posG.y) = Granero;
    _grilla.parcelas.at(posC.x).at(posC.y) = Casa;
}

Dimension Campo::dimensiones() const{
	return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const{
	return _grilla.parcelas.at(p.x).at(p.y);
}

// Muestra el campo en consola
void Campo::mostrar(std::ostream & os) const{
    os << "Campo" << std::endl;
    os << "Dimensiones: " << std::endl;
    os << "    " << "Ancho: " << _dimension.ancho << std::endl;
    os << "    " << "Largo: " << _dimension.largo << std::endl;
    os << "Contenidos: " << std::endl;

    int j = 0;
    while (j < _dimension.largo){
        os << "    ";
        int i = 0;
        while (i < _dimension.ancho){
            // Muestro contenido de parcela
            os << contenido({i,j});
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

// Guarda la informacion del campo en un archivo de texto
// Mientras queden cosas por guardar, pongo una coma al final del dato
void Campo::guardar(std::ostream & os) const{
    os << "{ C ";
    os << "[" << _dimension.ancho << "," << _dimension.largo << "] ";

    int j = 0;
    os << "[";
    while (j < _dimension.largo){
        os << "[";
        int i = 0;
        while (i < _dimension.ancho){
            // Guardo contenido de parcela
            os << contenido({i,j});
            i++;

            if (i < _dimension.ancho){
                os << ",";
            }
        }

        j++;
        if (j < _dimension.largo){
            os << "],";
        }else{
            os << "]";
        }
    }

    os << "]}";
}

// Carga el campo de acuerdo a la forma de almacenar info de la funcion 'guardar()'
void Campo::cargar(std::istream & is){
    std::string datosCampo;
    std::string dDimension;
    std::string dGrilla;

    // Guarda todo el string de texto en 'datosCampo'
    getline(is, datosCampo,'}');

    // Divide el string en las partes correspondientes a cada dato
    _leerSepararDatos(datosCampo, dDimension, dGrilla);

    // Carga los datos como correspondam
    _cargarDimension(dDimension);
    _cargarGrilla(dGrilla);
}

// Los parametros son tomados por referencia
void Campo::_leerSepararDatos(std::string &datos,
                              std::string &dDimension,
                              std::string &dGrilla){

    bool terminado = false;
    int cantEspacios = 0;
    int i = 0;

    // Distingue la categoria de los datos correspondiente de acuerdo a la cantidad de espacios.
    // Solamente hay espacios cuando se cambia la categoria (dimension, contenidosParcelas)
    while (!terminado){
        if (datos[i] == ' ') cantEspacios++;

        if (cantEspacios == 2) dDimension += datos[i];

        // Si llega al ultimo espacio, toma todo lo que queda en el ultimo dato
        if (cantEspacios == 3){
            dGrilla = datos.substr(i, datos.npos);
            terminado = true;
        }
        i++;
    }
}

// Usa la posicion del separador para dividir,
// porque no sabe de antemano cuantas cifras tiene la dimension
void Campo::_cargarDimension(std::string &dDimension){
    int posSeparador = dDimension.find(',');
    _dimension.ancho = atoi(dDimension.substr(2, posSeparador - 2).c_str());
    _dimension.largo = atoi(dDimension.substr(posSeparador + 1, dDimension.length() - posSeparador - 1).c_str());
}

void Campo::_cargarGrilla(std::string &dGrilla){
    _grilla.parcelas.resize(0, std::vector<Parcela>(0));
    _grilla.parcelas.resize(dimensiones().ancho, std::vector<Parcela>(dimensiones().largo, Cultivo));

    int i = 2;
    int posX = 0;
    int posY = 0;
    bool terminado = false;
    bool necesitaCierre = false;
    std::string datoActual = "";

    while (!terminado){
        // necesitaCierre es util para ignorar la coma entre las listas
        if (necesitaCierre){
            // Si llega a una coma, es porque termino de leer la palabra actual, pero la fila continua
            if (dGrilla[i] == ','){
                _cargarParcelaIndividual(posX, posY, datoActual);
                datoActual = "";
                posX++;
            }

            // Si llega a un cierre, se termino de leer la fila actual y paso a la siguiente
            else if (dGrilla[i] == ']'){
                necesitaCierre = false;
                _cargarParcelaIndividual(posX, posY, datoActual);
                datoActual = "";
                posX = 0;
                posY++;
            }
            else{
                datoActual += dGrilla[i];
            }
        }

        // necesitaCierre es util para ignorar la coma entre las listas
        if (dGrilla[i] == '['){
            necesitaCierre = true;
        }

        // El doble ']]' solo ocurre al final
        if (dGrilla[i] == ']' && dGrilla[i+1] == ']'){
            terminado = true;
        }
        i++;
    }
}

// Dada una posicion y un dato (String) que contiene una parcela, la carga donde corresponde
void Campo::_cargarParcelaIndividual(int posX, int posY, std::string datoActual){
    _grilla.parcelas.at(posX).at(posY) = _dameParcela(datoActual);
}

// Dada una string con el nombre, devuelve la parcela correspondiente
Parcela Campo::_dameParcela(std::string sParcela){
    Parcela parc = Cultivo;
    if (sParcela == "Granero") parc = Granero;
    if (sParcela == "Casa") parc = Casa;

    return parc;
}


bool Campo::operator==(const Campo & otroCampo) const{
    return _dimension.ancho == otroCampo.dimensiones().ancho
            && _dimension.largo == otroCampo.dimensiones().largo
            && _grillaIgual(otroCampo);
}

// Asume que los campos tienen la misma dimension
// Devuelve verdadero si tienen parcelas iguales en las mismas posiciones
bool Campo::_grillaIgual(const Campo &c) const{

    // CHEQUEAR ANTES SI TIENEN EL MISMO TAMAÑO PORQUE ROMPE. O ACLARAR QUE LO ASUMO DE ANTEMANO

    bool iguales = true;
    int i = 0;
    while (i < _dimension.ancho){
        int j = 0;
        while (j < _dimension.largo){
            if (contenido({i,j}) != c.contenido({i,j})){
                iguales = false;
            }
            j++;
        }
        i++;
    }

    return iguales;
}

std::ostream & operator<<(std::ostream & os, const Campo & c){
    c.mostrar(os);
    return os;
}

