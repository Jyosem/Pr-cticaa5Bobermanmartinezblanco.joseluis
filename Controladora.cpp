#include "Controladora.h"
#include "Jugador.h"

CControladora::CControladora() {
    cancha = new CCancha();
    jugador = new CJugador(ANCHOIMAGEN, ALTOIMAGEN);
    bombas = new CColeccionBomba();
    habilidades = new CColeccionHabilidad();
    enemigos = new CColeccionEnemigo();
    nivel = 3;
    juegoTerminado = false;
}

int CControladora::getNivel() {
    return nivel;
}

void CControladora::inicializar() {
    cancha->Inicializar();
}

void CControladora::Dibujar() {
    cancha->Inicializar();
}

void CControladora::reiniciarJuego() {
    // Reiniciar el jugador
    delete jugador;
    jugador = new CJugador(ANCHOIMAGEN, ALTOIMAGEN);

    // Reiniciar la cancha
    delete cancha;
    cancha = new CCancha();
    cancha->Inicializar();

    // Limpiar bombas
    delete bombas;
    bombas = new CColeccionBomba();

    // Reiniciar habilidades
    delete habilidades;
    habilidades = new CColeccionHabilidad();
    crearHabilidades();

    // Reiniciar enemigos
    delete enemigos;
    enemigos = new CColeccionEnemigo();
    crearEnemigos();

    juegoTerminado = false;
}

void CControladora::verificarColisionExplosion() {
    // Obtener la posici�n del jugador
    int jugadorX = jugador->getX();
    int jugadorY = jugador->getY();

    // Crear el rect�ngulo de colisi�n del jugador
    Rectangle jugadorRect = Rectangle(jugadorX, jugadorY,
        ANCHOJUGADOR * FACTORZOOM,
        ALTOJUGADOR * FACTORZOOM);

    // Iterar sobre todas las bombas para verificar explosiones
    for (CBomba* bomba : bombas->getBombas()) {
        if (bomba->getEstado() == EBomba::explosion) {
            // Obtener las coordenadas de la bomba
            int bombaX = bomba->getX();
            int bombaY = bomba->getY();
            int cellX = bomba->getCellX();
            int cellY = bomba->getCellY();

            const int newAnchoImagen = ANCHOIMAGEN - 10;
            const int newAltoImagen = ALTOIMAGEN - 10;

            // Verificar colisi�n con el centro de la explosi�n
            Rectangle centroExplosion = Rectangle(bombaX, bombaY, newAnchoImagen, newAltoImagen);
            if (jugadorRect.IntersectsWith(centroExplosion)) {
                jugador->disminuirVida();
                return; // Solo quitar una vida por explosi�n
            }

            // Verificar colisi�n con explosi�n hacia la izquierda
            if (cellX > 0 && cancha->getCancha()[cellY][cellX - 1]->getTipo() != EElementos::paredes) {
                Rectangle izquierda = Rectangle(bombaX - newAnchoImagen, bombaY, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(izquierda)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisi�n con explosi�n hacia la derecha
            if (cellX < COLUMNAS - 1 && cancha->getCancha()[cellY][cellX + 1]->getTipo() != EElementos::paredes) {
                Rectangle derecha = Rectangle(bombaX + newAnchoImagen, bombaY, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(derecha)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisi�n con explosi�n hacia arriba
            if (cellY > 0 && cancha->getCancha()[cellY - 1][cellX]->getTipo() != EElementos::paredes) {
                Rectangle arriba = Rectangle(bombaX, bombaY - newAltoImagen, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(arriba)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisi�n con explosi�n hacia abajo
            if (cellY < FILAS - 1 && cancha->getCancha()[cellY + 1][cellX]->getTipo() != EElementos::paredes) {
                Rectangle abajo = Rectangle(bombaX, bombaY + newAltoImagen, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(abajo)) {
                    jugador->disminuirVida();
                    return;
                }
            }
        }
    }
}

void CControladora::verificarColisionEnemigo() {
    // Verificar si el jugador colisiona con alg�n enemigo
    // Esta funci�n debe implementar la detecci�n de colisi�n
    // entre el jugador y los enemigos

    // Por ahora, es una funci�n placeholder que debes completar
    // seg�n tu l�gica de detecci�n de colisiones con enemigos

    // Ejemplo de c�mo podr�a verse:
    /*
    Rectangle jugadorRect = Rectangle(jugador->getX(), jugador->getY(),
                                    ANCHOJUGADOR * FACTORZOOM, ALTOJUGADOR * FACTORZOOM);

    // Iterar sobre enemigos y verificar colisi�n
    for (cada enemigo) {
        Rectangle enemigoRect = Rectangle(enemigo->getX(), enemigo->getY(),
                                        ancho_enemigo, alto_enemigo);
        if (jugadorRect.IntersectsWith(enemigoRect)) {
            jugador->disminuirVida();
            // Opcional: mover jugador a posici�n segura o reiniciar posici�n
            break;
        }
    }
    */
}

void CControladora::verificarColisionHabilidad() {
    Rectangle jugadorRect = Rectangle(jugador->getX(), jugador->getY(),
        ANCHOJUGADOR * FACTORZOOM,
        ALTOJUGADOR * FACTORZOOM);

    for (CHabilidad* habilidad : habilidades->getHabilidades()) {
        if (habilidad->getEstado() == EEstadoHabilidad::Visible) {
            Rectangle habilidadRect = Rectangle(habilidad->getColumna() * ANCHOIMAGEN,
                habilidad->getFila() * ALTOIMAGEN,
                ANCHOIMAGEN, ALTOIMAGEN);

            if (jugadorRect.IntersectsWith(habilidadRect)) {
                // Activar habilidad seg�n tipo
                if (habilidad->getTipoHabilidad() == 1) { // Bomba Adicional
                    jugador->activarBombaAdicional();
                }
                // Hacer que la habilidad desaparezca
                habilidad->consumir();
                break;
            }
        }
    }
}

void CControladora::dibujarCancha(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpIrrompible, Bitmap^ bmpRrompible, Bitmap^ bmpHabilidad) {
    cancha->PintarCancha(g, bmpPiso);
    cancha->PintarMatriz(g, bmpIrrompible, bmpRrompible);
    habilidades->dibujar(g, bmpHabilidad, cancha->getCancha());
}

void CControladora::dibujarJugador(Graphics^ g, Bitmap^ bmpJugador) {
    jugador->dibujar(g, bmpJugador, cancha->getCancha());
}

void CControladora::dibujarVidas(Graphics^ g, Bitmap^ bmpVida) {
    jugador->dibujarVidas(g, bmpVida);
}

void CControladora::adicionarBomba() {
    if (bombas->getBombas().size() < 1 || jugador->puedeUsarBombaAdicional()) {
        bombas->crearBomba(jugador->getX(), jugador->getY());
        if (jugador->puedeUsarBombaAdicional()) {
            jugador->usarBombaAdicional();
        }
    }
}

// Actualizaci�n del m�todo moverJugador en Controladora.cpp
void CControladora::moverJugador(Graphics^ g, Bitmap^ bmpJugador, Bitmap^ bmpBomba, Bitmap^ bmpExplosion, Bitmap^ bmpEnemigo, Bitmap^ bmpVida) {
    if (!juegoTerminado) {
        bombas->dibujarBomba(g, bmpBomba, bmpExplosion, jugador->getX(), jugador->getY(), cancha->getCancha());

        // Verificar si se destruyeron bloques y revelar habilidades
        vector<pair<int, int>> bloquesDestruidos = bombas->getBloquesDestruidosRecientes();
        for (auto bloque : bloquesDestruidos) {
            habilidades->revelarHabilidadEnPosicion(bloque.first, bloque.second);
        }

        // Actualizar invulnerabilidad del jugador
        jugador->actualizarInvulnerabilidad();

        // Verificar colisiones
        verificarColisionExplosion();
        verificarColisionEnemigo();
        verificarColisionHabilidad();

        // Verificar si el jugador perdi� todas las vidas
        if (!jugador->estaVivo()) {
            juegoTerminado = true;
            // Reiniciar el juego despu�s de un breve delay
            reiniciarJuego();
        }

        jugador->mover(g, bmpJugador, cancha->getCancha());
        enemigos->dibujar(g, bmpEnemigo, cancha->getCancha());
    }

    // Siempre dibujar las vidas
    dibujarVidas(g, bmpVida);
}

CJugador* CControladora::getJugador() {
    return jugador;
}

void CControladora::crearHabilidades() {
    while (habilidades->getHabilidades().size() < 15) {
        habilidades->crearHabilidad();
    }
}

void CControladora::crearEnemigos() {
    while (enemigos->getEnemigos().size() < 3) {
        enemigos->crearEnemigo();
    }
}

CColeccionHabilidad* CControladora::getColeccionHabilidades() {
    return habilidades;
}

CColeccionEnemigo* CControladora::getColeccionEnemigos() {
    return enemigos;
}