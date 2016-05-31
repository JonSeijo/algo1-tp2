#include "tipos.h"

std::ostream &operator<<(std::ostream &os, const Parcela &p) {
	if (p == Cultivo) os << "Cultivo";
    if (p == Granero) os << "Granero";
    if (p == Casa) os << "Casa";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Producto &p) {
	if (p == Fertilizante) os << "Fertilizante";
    if (p == Plaguicida) os << "Plaguicida";
    if (p == PlaguicidaBajoConsumo) os << "PlaguicidaBajoConsumo";
    if (p == Herbicida) os << "Herbicida";
    if (p == HerbicidaLargoAlcance) os << "HerbicidaLargoAlcance";

    return os;
}

std::ostream &operator<<(std::ostream &os, const EstadoCultivo &e) {
	if (e == NoSensado) os << "NoSensado";
    if (e == RecienSembrado) os << "RecienSembrado";
    if (e == EnCrecimiento) os << "EnCrecimiento";
    if (e == ListoParaCosechar) os << "ListoParaCosechar";
    if (e == ConMaleza) os << "ConMaleza";
    if (e == ConPlaga) os << "ConPlaga";

    
    return os;
}

const bool operator==(const Posicion &p1, const Posicion &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

const bool operator==(const InfoVueloCruzado &a, const InfoVueloCruzado &b) {
    return a.posicion == b.posicion && a.cantidadCruces == b.cantidadCruces;
}