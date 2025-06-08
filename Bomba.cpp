#include "Bomba.h"

CBomba::CBomba(int x, int y) {
    // Calculate the center of the grid cell where the player is
    int cellX = (x / ANCHOIMAGEN);
    int cellY = (y / ALTOIMAGEN);

    // Set bomb position to the center of the cell
    // Centering the bomb graphic in the middle of the cell
    this->x = cellX * ANCHOIMAGEN + (ANCHOIMAGEN / 2) - ((ANCHOIMAGEN * 0.7) / 2);
    this->y = cellY * ALTOIMAGEN + (ALTOIMAGEN / 2) - ((ALTOIMAGEN * 0.7) / 2);

    // Store grid coordinates for explosion calculations
    this->cellX = cellX;
    this->cellY = cellY;

    estado = EBomba::puesto;
    tiempoExplosion = 0;
    indiceX = 0;

    // para explosion
    indiceEX = 0;
    indiceEY = 0;
}

EBomba CBomba::getEstado() {
    return estado;
}

void CBomba::animarExplosion() {
    if (indiceEX >= 0 && indiceEX < 3) {
        indiceEX++;
    }
    else {
        estado = EBomba::explotado;
    }
}

bool CBomba::verificarLugar(int jugadorX, int jugadorY, const CanchaArray& cancha) {
    if ((cancha[jugadorY / ALTOIMAGEN][jugadorX / ANCHOIMAGEN]->getTipo() == EElementos::piso) || (cancha[jugadorY / ALTOIMAGEN][jugadorX / ANCHOIMAGEN]->getTipo() == EElementos::libre)) {
        return true;
    }

    return false;
}

void CBomba::dibujar(Graphics^ g, Bitmap^ bmpBomba, int jugadorX, int jugadorY, const CanchaArray& cancha) {
    if (verificarLugar(jugadorX, jugadorY, cancha)) {
        Rectangle rectangulo = Rectangle(indiceX * ANCHOBOMBA, 0, ANCHOBOMBA, ALTOBOMBA);
        Rectangle zoom = Rectangle(x, y, ANCHOIMAGEN * 0.7, ALTOIMAGEN * 0.7);
        g->DrawImage(bmpBomba, zoom, rectangulo, GraphicsUnit::Pixel);
        if (tiempoExplosion == TIEMPOEXPLOSION) {
            estado = EBomba::explosion;
        }
    }
}

void CBomba::animar() {
    if ((indiceX >= 0) && (indiceX < 2)) {
        indiceX++;
    }
    else {
        tiempoExplosion++;
        indiceX = 0;
    }
}

void CBomba::dibujarExplosion(Graphics^ g, Bitmap^ bmpExplosion, const CanchaArray& cancha) {
    const int newAnchoImagen = ANCHOIMAGEN - 10;
    const int newAltoImagen = ANCHOIMAGEN - 10;

    // Limpiar lista de bloques destruidos al inicio de cada explosión
    if (indiceEX == 0) {
        bloquesDestruidos.clear();
    }

    // Usar las coordenadas de pantalla para dibujar la explosión
    Rectangle centro = Rectangle(x, y, newAnchoImagen, newAltoImagen);
    Rectangle posicionCentro = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);
    g->DrawImage(bmpExplosion, centro, posicionCentro, GraphicsUnit::Pixel);

    Rectangle izquierdaDerecha = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY + 2 * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);
    Rectangle arribaAbajo = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY + 6 * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);

    // Verificar y modificar bloques usando las coordenadas lógicas de la cuadrícula
    // Verificar límites antes de acceder a los índices

    // Izquierda
    if (cellX > 0 && cancha[cellY][cellX - 1]->getTipo() != EElementos::paredes) {
        Rectangle izquierda = Rectangle(x - newAnchoImagen, y, newAnchoImagen, newAltoImagen);
        g->DrawImage(bmpExplosion, izquierda, izquierdaDerecha, GraphicsUnit::Pixel);
        if (cancha[cellY][cellX - 1]->getTipo() == EElementos::rrompible) {
            cancha[cellY][cellX - 1]->setTipo(EElementos::libre);
            bloquesDestruidos.push_back(make_pair(cellY, cellX - 1));
        }
    }

    // Derecha
    if (cellX < COLUMNAS - 1 && cancha[cellY][cellX + 1]->getTipo() != EElementos::paredes) {
        Rectangle derecha = Rectangle(x + newAnchoImagen, y, newAnchoImagen, newAltoImagen);
        g->DrawImage(bmpExplosion, derecha, izquierdaDerecha, GraphicsUnit::Pixel);
        if (cancha[cellY][cellX + 1]->getTipo() == EElementos::rrompible) {
            cancha[cellY][cellX + 1]->setTipo(EElementos::libre);
            bloquesDestruidos.push_back(make_pair(cellY, cellX + 1));
        }
    }

    // Arriba
    if (cellY > 0 && cancha[cellY - 1][cellX]->getTipo() != EElementos::paredes) {
        Rectangle arriba = Rectangle(x, y - newAltoImagen, newAnchoImagen, newAltoImagen);
        g->DrawImage(bmpExplosion, arriba, arribaAbajo, GraphicsUnit::Pixel);
        if (cancha[cellY - 1][cellX]->getTipo() == EElementos::rrompible) {
            cancha[cellY - 1][cellX]->setTipo(EElementos::libre);
            bloquesDestruidos.push_back(make_pair(cellY - 1, cellX));
        }
    }

    // Abajo
    if (cellY < FILAS - 1 && cancha[cellY + 1][cellX]->getTipo() != EElementos::paredes) {
        Rectangle abajo = Rectangle(x, y + newAltoImagen, newAnchoImagen, newAltoImagen);
        g->DrawImage(bmpExplosion, abajo, arribaAbajo, GraphicsUnit::Pixel);
        if (cancha[cellY + 1][cellX]->getTipo() == EElementos::rrompible) {
            cancha[cellY + 1][cellX]->setTipo(EElementos::libre);
            bloquesDestruidos.push_back(make_pair(cellY + 1, cellX));
        }
    }
}

// Función para obtener los bloques destruidos por esta bomba
vector<pair<int, int>> CBomba::getBloquesDestruidos() {
    return bloquesDestruidos;
}