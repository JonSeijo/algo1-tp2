#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include "tipos.h"
#include <vector>

class Drone{
    public:
        Drone();
        Drone(ID i, const std::vector<Producto> &ps);

        ID id() const;
        Carga bateria() const;
        bool enVuelo() const;
        const Secuencia<Posicion>& vueloRealizado() const;
        Posicion posicionActual() const;
		const Secuencia<Producto>& productosDisponibles() const;

        bool vueloEscalerado() const;
		static Secuencia<InfoVueloCruzado>	vuelosCruzados(const Secuencia<Drone>& ds);

		void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar(std::istream& is);

        bool operator==(const Drone& otroDrone) const;

	private:
		bool dosOMenos(const Secuencia<int>) const;
		bool ordenada(const Secuencia<int>) const;
		Secuencia<int> damePrimeros(const Secuencia<Posicion> ps) const;
		Secuencia<int> dameSegundos(const Secuencia<Posicion> ps) const;

		ID _id;
		Carga _bateria;
		Secuencia<Posicion> _trayectoria;
		Secuencia<Producto> _productos;
		bool _enVuelo;
		Posicion _posicionActual;

        std::string _dameStringVueloRealizado() const;
        std::string _dameStringProductos() const;
        std::string _dameStringPosicion(Posicion p) const;
        std::string _dameStringProd(Producto p) const;
        void _leerSepararDatos(std::string &datos, std::string &dId, std::string &dBateria,
                                      std::string &dTrayectoria, std::string &dProductos);

        void _cargarBateria(std::string dBateria);
        void _cargarId(std::string dId);
        void _cargarTrayectoria(std::string dTrayectoria);
        void _cargarPosicionIndividual(std::string dPos);

        void _cargarProductos(std::string dProductos);
        void _cargarProductoIndividual(std::string dProd);


};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Drone & d);
std::ostream & operator<<(std::ostream & os, const Producto & p);

#endif // DRONE_H_INCLUDED
