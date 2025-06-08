#pragma once
#include "Cancha.h"
#include "Jugador.h"
#include "ColeccionBomba.h"
#include "ColeccionHabilidad.h"
#include "ColeccionEnemigo.h"

class CControladora {
private:
	CCancha* cancha;
	CJugador* jugador;
	CColeccionBomba* bombas;
	CColeccionHabilidad* habilidades;
	CColeccionEnemigo* enemigos;
	int nivel;
	bool juegoTerminado;

public:
	CControladora();
	~CControladora() {};

	int getNivel();
	bool getJuegoTerminado() { return juegoTerminado; }
	void inicializar();
	void Dibujar();
	void reiniciarJuego();
	void verificarColisionExplosion();
	void verificarColisionEnemigo();
	void verificarColisionHabilidad();
	void dibujarCancha(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpIrrompible, Bitmap^ bmpRrompible, Bitmap^ bmpHabilidad);
	void dibujarJugador(Graphics^ g, Bitmap^ bmpJugador);
	void dibujarVidas(Graphics^ g, Bitmap^ bmpVida);
	void adicionarBomba();
	void moverJugador(Graphics^ g, Bitmap^ bmpJugador, Bitmap^ bmpBomba, Bitmap^ bmpExplosion, Bitmap^ bmpEnemigo, Bitmap^ bmpVida);

	CJugador* getJugador();
	void crearHabilidades();
	void crearEnemigos();
	CColeccionHabilidad* getColeccionHabilidades();
	CColeccionEnemigo* getColeccionEnemigos();
};