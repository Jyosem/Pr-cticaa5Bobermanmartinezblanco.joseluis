#include <iostream>
#include "Jugador.h"

CJugador::CJugador() {};
CJugador::~CJugador() {};

CJugador::CJugador(int x, int y) {
	this->x = x;
	this->y = y;
	dx = 0;
	dy = 0;
	vidas = VIDAS_INICIALES; // Inicializar con 3 vidas
	ultimaTecla = EDireccion::Abajo;
	direccion = EDireccion::Ninguna;
	indiceX = 0;
	indiceY = 2;
	for (int i = 0; i < HABILIDADES; i++) {
		habilidades[i] = false;
	}

	bombaAdicionalActiva = false;
	bombasUsadas = 0;
}

void CJugador::setDireccion(EDireccion direccion) {
	this->direccion = direccion;
}

// Añadir este método completo después de los métodos existentes:
void CJugador::activarBombaAdicional() {
	bombaAdicionalActiva = true;
	bombasUsadas = 0;
}

// Añadir este método para verificar si puede usar bomba adicional:
bool CJugador::puedeUsarBombaAdicional() {
	return bombaAdicionalActiva && bombasUsadas < MAX_BOMBAS_ADICIONALES;
}

// Añadir este método para registrar uso de bomba:
void CJugador::usarBombaAdicional() {
	if (bombaAdicionalActiva) {
		bombasUsadas++;
		if (bombasUsadas >= MAX_BOMBAS_ADICIONALES) {
			bombaAdicionalActiva = false;
		}
	}
}

void CJugador::reiniciarPosicion() {
	x = ANCHOIMAGEN;
	y = ALTOIMAGEN;
	dx = 0;
	dy = 0;
	direccion = EDireccion::Ninguna;
	ultimaTecla = EDireccion::Abajo;
	indiceX = 0;
	indiceY = 2;
}

void CJugador::dibujarVidas(Graphics^ g, Bitmap^ bmpVida) {
	// Dibujar corazones en la esquina superior izquierda
	for (int i = 0; i < vidas && i < VIDAS_INICIALES; i++) {
		Rectangle origen = Rectangle(0, 0, bmpVida->Width, bmpVida->Height);
		Rectangle destino = Rectangle(10 + (i * (ANCHO_CORAZON + 5)), 10, ANCHO_CORAZON, ALTO_CORAZON);
		g->DrawImage(bmpVida, destino, origen, GraphicsUnit::Pixel);
	}
}

void CJugador::controlarLimitesMovimiento(const CanchaArray& cancha) {
	int X = 0, Y = 0;
	for (int i = 0; i < FILAS; i++) {
		X = 0;
		for (int j = 0; j < COLUMNAS; j++) {
			std::cout << "cancha[i][j] = " << cancha[i][j] << std::endl;
			Rectangle intersecta = Rectangle(X, Y, ANCHOIMAGEN, ALTOIMAGEN);
			if ((cancha[i][j]->getTipo() == EElementos::paredes) || (cancha[i][j]->getTipo() == EElementos::rrompible)) {
				if (limiteArribaAbajo.IntersectsWith(intersecta)) { dy = 0; }
				if (limiteDerechaIzquierda.IntersectsWith(intersecta)) { dx = 0; }
			}
			X += ANCHOIMAGEN;
		}
		Y += ALTOIMAGEN;
	}
}

void CJugador::dibujar(Graphics^ g, Bitmap^ bmpJugador, const CanchaArray& cancha) {
	limiteDerechaIzquierda = Rectangle(x + 2 * FACTORZOOM + dx, y + 15 * FACTORZOOM, (ANCHOJUGADOR - 10) * FACTORZOOM, (ALTOJUGADOR - 20) * FACTORZOOM);
	limiteArribaAbajo = Rectangle(x + 2 * FACTORZOOM, y + 15 * FACTORZOOM + dy, (ANCHOJUGADOR - 10) * FACTORZOOM, (ALTOJUGADOR - 20) * FACTORZOOM);
	/*g->DrawRectangle(Pens::Red, limiteDerechaIzquierda);
	g->DrawRectangle(Pens::Blue, limiteArribaAbajo);*/
	controlarLimitesMovimiento(cancha);
	Rectangle rectangulo = Rectangle(indiceX * ANCHOJUGADOR, indiceY * ALTOJUGADOR, ANCHOJUGADOR, ALTOJUGADOR);
	Rectangle zoom = Rectangle(x, y, ANCHOJUGADOR * FACTORZOOM, ALTOJUGADOR * FACTORZOOM);
	g->DrawImage(bmpJugador, zoom, rectangulo, GraphicsUnit::Pixel);
	x += dx;
	y += dy;
}

// Y en Jugador.cpp:
void CJugador::disminuirVida() {
	if (vidas > 0 && !invulnerable) {
		vidas--;
		invulnerable = true;
		tiempoInvulnerabilidad = 30; // frames de invulnerabilidad
	}
}

void CJugador::actualizarInvulnerabilidad() {
	if (invulnerable) {
		tiempoInvulnerabilidad--;
		if (tiempoInvulnerabilidad <= 0) {
			invulnerable = false;
		}
	}
}

void CJugador::mover(Graphics^ g, Bitmap^ bmpJugador, const CanchaArray& cancha) {
	switch (direccion) {
	case EDireccion::Arriba:
		indiceY = 0;
		if ((indiceX >= 0) && (indiceX < 3)) {
			indiceX++;
		}
		else {
			indiceX = 0;
		}
		dx = 0;
		dy = -10;
		ultimaTecla = EDireccion::Arriba;
		break;
	case EDireccion::Abajo:
		indiceY = 2;
		if ((indiceX >= 0) && (indiceX < 3)) {
			indiceX++;
		}
		else {
			indiceX = 0;
		}
		dx = 0;
		dy = 10;
		ultimaTecla = EDireccion::Abajo;
		break;
	case EDireccion::Izquierda:
		indiceY = 3;
		if ((indiceX >= 0) && (indiceX < 3)) {
			indiceX++;
		}
		else {
			indiceX = 0;
		}
		dx = -10;
		dy = 0;
		ultimaTecla = EDireccion::Izquierda;
		break;
	case EDireccion::Derecha:
		indiceY = 1;
		if ((indiceX >= 0) && (indiceX < 3)) {
			indiceX++;
		}
		else {
			indiceX = 0;
		}
		dx = 10;
		dy = 0;
		ultimaTecla = EDireccion::Derecha;
		break;
	case EDireccion::Ninguna:
		dx = 0;
		dy = 0;
		switch (ultimaTecla) {
		case EDireccion::Abajo:
			indiceX = 0;
			indiceY = 2;
			break;
		case EDireccion::Arriba:
			indiceX = 0;
			indiceY = 0;
			break;
		case EDireccion::Izquierda:
			indiceX = 1;
			indiceY = 3;
			break;
		case EDireccion::Derecha:
			indiceX = 1;
			indiceY = 3;
			break;
		}
		break;
	}
	dibujar(g, bmpJugador, cancha);
}