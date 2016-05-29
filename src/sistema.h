#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED

#include "tipos.h"
#include "campo.h"
#include "drone.h"

class Sistema {

  public:
		Sistema();
		Sistema(const Campo &c, const Secuencia<Drone> &ds);

		const Campo& campo() const;
		EstadoCultivo estadoDelCultivo(const Posicion& p) const;
		const Secuencia<Drone>&	enjambreDrones() const;

		void crecer();
		void seVinoLaMaleza(const Secuencia<Posicion>& ps);
		void seExpandePlaga();
        void despegar(const Drone& d);
		bool listoParaCosechar() const;
		void aterrizarYCargarBaterias(Carga b);
		void fertilizarPorFilas();
		void volarYSensar(const Drone& d);

		void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar (std::istream& is);

		bool operator==(const Sistema& otroSistema) const;

	private:
        Campo _campo;
		Secuencia<Drone> _enjambre;
        Grilla<EstadoCultivo> _estado;


        void _mostrarEstadosCultivos(std::ostream &os) const;
        void _guardarEstadosCultivos(std::ostream &os) const;
        void _guardarDrones(std::ostream &os) const;

        void _leerSepararDatos(std::string &dSistema, std::string &dCampo,
                                std::string &dEnjambre, std::string &dEstadosCultivo);
        void _cargarCampo(std::string &dCampo);
        void _cargarDroneIndividual(Drone &d, std::string &dDrone);
        void _cargarEnjambre(std::string &dEnjambre);

        Posicion posGranero() const;

        void _cargarEstadosCultivo(std::string &dEstados);
        void _cargarEstadoIndividual(int x, int y, std::string dEstado);

        bool _igualEstados(const Sistema &s) const;
        bool _mismosDrones(const Sistema &s) const;
        bool _pertenece(Drone d, const Secuencia<Drone> enjambre) const;

        Posicion _parcelaCultivoLibre() const;
        bool _esLibreDeDrones(Posicion p) const;
        bool _esAdyacente(Posicion p1, Posicion p2) const;

        int _cuentaCosechables(int i) const;

        std::vector<Posicion> _posicionesConPlaga();
        void _plaguizarVecinos(Posicion p);
        std::vector<Posicion> _vecinosConCultivos(Posicion p);
        std::vector<Posicion> _adyacentes(Posicion p);
        bool _enRango(Posicion p);

        void _fertilizar(std::vector<Posicion> recorridoDrone);
        void _moverDrone(std::vector<Posicion> recorridoDrone, Drone d);
        std::vector<Posicion> _recorridoDrone(Drone d, int recorridoMaximo);
        int _recorridoMaximo(Drone d);
        int _minimo(int a, int b);
        int _fertAplicable(Drone d);
        int _cuentaProds(Producto p, std::vector<Producto> ps);
        int _cantFertilizables(int i, Drone d);
        int _parcelasLibres(Drone d);
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Sistema & s);
std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e);

#endif // SISTEMA_H_INCLUDED
