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

            //Para correr los tests (NO TOCAR)
        friend class test_sistema_lo_basico_crecer_funciona_ok_Test;
        friend class test_sistema_se_vino_la_maleza_lista_vacia_no_modifica_nada_Test;
        friend class test_sistema_se_vino_la_maleza_se_pone_con_maleza_las_posiciones_de_la_lista_Test;
        friend class test_sistema_se_expande_plaga_si_no_hay_plaga_no_cambia_Test;
        friend class test_sistema_se_expande_plaga_si_hay_plaga_se_expande_Test;
        friend class test_sistema_listo_para_cosechar_si_es_menos_del_90_por_ciento_retorna_false_Test;
        friend class test_sistema_listo_para_cosechar_si_es_exactamente_el_90_por_ciento_retorna_true_Test;
        friend class test_sistema_listo_para_cosechar_si_es_mas_del_90_por_ciento_retorna_true_Test;
        friend class test_sistema_volar_y_sensar_todo_libre_Test;



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

        template<class T>
        unsigned int cuenta(const T &x, const std::vector<T> &v) const;

        template<class T>
        bool mismos(const std::vector<T> &a, const std::vector<T> &b) const;

        Posicion _parcelaCultivoLibre() const;
        bool _esLibreDeDrones(Posicion p) const;
        bool _esAdyacente(Posicion p1, Posicion p2) const;

        int _cuentaCosechables(std::vector<EstadoCultivo> fila, int i) const;

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

        Secuencia<Posicion> _darPosicionesValidas(const Posicion posactual);
        bool tieneFertilizante(const Drone d);
        bool tienePlaguicida(const Drone d);
        bool tienePlaguicidaBajoConsumo(const Drone d);
        bool tieneHerbicida(const Drone d);
        bool tieneHerbicidaLargoAlcance(const Drone d);
        Secuencia<Posicion> adyacentesACambiar(const Posicion posactual);
        void cambiaAListoParaCosechar(const Posicion posactual);
        void cambiaARecienSembrado(const Posicion posactual);
        void cambiaAdyacentesConMalezaARecienSembrado(const Posicion posactual);
        void moverUnaPosicionAlDrone(Posicion p, Drone d);
        void restarBateria(int x, Drone d);
        void quitarProducto(Producto p, Drone d);
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Sistema & s);


#endif // SISTEMA_H_INCLUDED
