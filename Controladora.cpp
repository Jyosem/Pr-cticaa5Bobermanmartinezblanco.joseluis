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
    // Obtener la posición del jugador
    int jugadorX = jugador->getX();
    int jugadorY = jugador->getY();

    // Crear el rectángulo de colisión del jugador
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

            // Verificar colisión con el centro de la explosión
            Rectangle centroExplosion = Rectangle(bombaX, bombaY, newAnchoImagen, newAltoImagen);
            if (jugadorRect.IntersectsWith(centroExplosion)) {
                jugador->disminuirVida();
                return; // Solo quitar una vida por explosión
            }

            // Verificar colisión con explosión hacia la izquierda
            if (cellX > 0 && cancha->getCancha()[cellY][cellX - 1]->getTipo() != EElementos::paredes) {
                Rectangle izquierda = Rectangle(bombaX - newAnchoImagen, bombaY, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(izquierda)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisión con explosión hacia la derecha
            if (cellX < COLUMNAS - 1 && cancha->getCancha()[cellY][cellX + 1]->getTipo() != EElementos::paredes) {
                Rectangle derecha = Rectangle(bombaX + newAnchoImagen, bombaY, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(derecha)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisión con explosión hacia arriba
            if (cellY > 0 && cancha->getCancha()[cellY - 1][cellX]->getTipo() != EElementos::paredes) {
                Rectangle arriba = Rectangle(bombaX, bombaY - newAltoImagen, newAnchoImagen, newAltoImagen);
                if (jugadorRect.IntersectsWith(arriba)) {
                    jugador->disminuirVida();
                    return;
                }
            }

            // Verificar colisión con explosión hacia abajo
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
    // Verificar si el jugador colisiona con algún enemigo
    // Esta función debe implementar la detección de colisión
    // entre el jugador y los enemigos

    // Por ahora, es una función placeholder que debes completar
    // según tu lógica de detección de colisiones con enemigos

    // Ejemplo de cómo podría verse:
    /*
    Rectangle jugadorRect = Rectangle(jugador->getX(), jugador->getY(),
                                    ANCHOJUGADOR * FACTORZOOM, ALTOJUGADOR * FACTORZOOM);

    // Iterar sobre enemigos y verificar colisión
    for (cada enemigo) {
        Rectangle enemigoRect = Rectangle(enemigo->getX(), enemigo->getY(),
                                        ancho_enemigo, alto_enemigo);
        if (jugadorRect.IntersectsWith(enemigoRect)) {
            jugador->disminuirVida();
            // Opcional: mover jugador a posición segura o reiniciar posición
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
                // Activar habilidad según tipo
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

// Actualización del método moverJugador en Controladora.cpp
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

        // Verificar si el jugador perdió todas las vidas
        if (!jugador->estaVivo()) {
            juegoTerminado = true;
            // Reiniciar el juego después de un breve delay
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