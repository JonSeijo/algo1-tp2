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

        void moverA(const Posicion pos);
        void setBateria(const Carga c);
        void borrarVueloRealizado();
        void cambiarPosicionActual(const Posicion p);
        void sacarProducto(const Producto p);

	private:
        bool _escalerado(const std::vector<Posicion> ps) const;
        bool _esEscalerado(const std::vector<Posicion> ps, int X, int Y) const;

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
        std::string _dameStringEnVuelo() const;
        std::string _dameStringPosActual() const;

        void _leerSepararDatos(std::string &datos, std::string &dId, std::string &dBateria,
                                      std::string &dTrayectoria, std::string &dProductos,
                                      std::string &dEnVuelo, std::string &dPosActual);

        void _cargarBateria(std::string dBateria);
        void _cargarId(std::string dId);
        void _cargarTrayectoria(std::string dTrayectoria);
        Posicion _cargarPosicionIndividual(std::string dPos);

        void _cargarProductos(std::string dProductos);
        void _cargarProductoIndividual(std::string dProd);
        void _cargarPosActual(std::string dPosActual);
        void _cargarEnVuelo(std::string dEnVuelo);

        template<class T>
        unsigned int cuenta(const T &x, const std::vector<T> &v) const;
        template<class T>
        bool mismos(const std::vector<T> &a, const std::vector<T> &b) const;

        static Secuencia<Secuencia< Posicion > > dameTrayectorias(const Secuencia<Drone>& ds);
        static Secuencia<Secuencia<Posicion> > agruparPosiciones(const Secuencia<Secuencia<Posicion> > pss);
        static Secuencia<Secuencia<InfoVueloCruzado> > agruparRepetidas(const Secuencia<Secuencia<Posicion> > tss);
        static Secuencia<InfoVueloCruzado> borrarSobrantesYOrdenar(const Secuencia<Secuencia<InfoVueloCruzado> > xss);
        static int _cuentaPos(Posicion p, Secuencia<Posicion> ps);
        static bool _perteneceInfoPos(Posicion p, Secuencia<InfoVueloCruzado> ps);

};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Drone & d);

#endif // DRONE_H_INCLUDED
