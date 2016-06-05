#ifndef CAMPO_H_INCLUDED
#define CAMPO_H_INCLUDED

#include "tipos.h"

class Campo {
	public:
		Campo();
		Campo(const Posicion &posG, const Posicion &posC);
		Campo(const Posicion &posG, const Posicion &posC, Dimension dimension);


		Dimension dimensiones() const;
		Parcela contenido(const Posicion &pos) const;

		void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar (std::istream& is);

		bool operator==(const Campo& otroCampo) const;

	private:
		Dimension _dimension;
		Grilla<Parcela> _grilla;

        void _leerSepararDatos(std::string &datos,
                                      std::string &dDimension,
                                      std::string &dGrilla);
        void _cargarDimension(std::string &dDimension);
        void _cargarGrilla(std::string &dGrilla);

        void _leerYCargarDatos(std::string datos, Dimension &dim, Grilla<Parcela> &grilla);
        std::string _dameStringConDato(std::string &datos, bool objetoCompuesto);
        int _contarOcurrencias(std::string cadena, char aBuscar);
        void _setearParcelaEnCarga(std::string textoAnterior, Grilla<Parcela> &grilla,
                                   int elementosPorFila, Parcela parcelaCorrespondiente);
        bool _grillaIgual(const Campo &c) const;
};


// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os, const Campo & c);

#endif // CAMPO_H_INCLUDED
