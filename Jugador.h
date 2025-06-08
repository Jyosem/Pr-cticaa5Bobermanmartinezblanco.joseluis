#pragma once
#include <iostream>
#include "Cancha.h"
#define HABILIDADES 7
#define ANCHOJUGADOR 70
#define ALTOJUGADOR 128
#define FACTORZOOM 0.4
#define VIDAS_INICIALES 3
#define ANCHO_CORAZON 32
#define ALTO_CORAZON 32
using namespace System::Drawing;

enum EDireccion {
	Ninguna = 0,
	Abajo,
	Arriba,
	Izquierda,
	Derecha
};

class CJugador {
	int x;		// posicion en el eje X
	int y;		// posicion en el eje y
	int dx;		// desplazamiento en x
	int dy;		// desplazamiento en y
	int indiceX;
	int indiceY;
	int vidas;	// Nueva propiedad para las vidas
	bool invulnerable;
	// Añadir estas nuevas variables después de las existentes:
	bool bombaAdicionalActiva;
	int bombasUsadas;
	static const int MAX_BOMBAS_ADICIONALES = 3;
	int tiempoInvulnerabilidad;
	EDireccion ultimaTecla;
	EDireccion direccion;
	Rectangle limiteArribaAbajo;
	Rectangle limiteDerechaIzquierda;
	bool habilidades[HABILIDADES];
public:
	CJugador();
	~CJugador();




	CJugador(int x, int y);
	void setDx(int dx) {
		this->dx = dx;
	}
	void setDy(int dy) {
		this->dy = dy;
	}
	int getX() {
		return x + 1 * FACTORZOOM;
	}
	int getY() {
		return y + 20 * FACTORZOOM + dy;
	}

	// Métodos para manejo de vidas
	int getVidas() {
		return vidas;
	}
	void reiniciarVidas() {
		vidas = VIDAS_INICIALES;
	}
	bool estaVivo() {
		return vidas > 0;
	}

	void dibujar(Graphics^ g, Bitmap^ bmpJugador, const CanchaArray& cancha);
	void dibujarVidas(Graphics^ g, Bitmap^ bmpVida);
	void actualizarInvulnerabilidad();
	void disminuirVida();
	void mover(Graphics^ g, Bitmap^ bmpJugador, const CanchaArray& cancha);
	void setDireccion(EDireccion direccion);
	void controlarLimitesMovimiento(const CanchaArray& cancha);
	void reiniciarPosicion();
	// Añadir estas declaraciones:
	void activarBombaAdicional();
	bool puedeUsarBombaAdicional();
	void usarBombaAdicional();
};