
// ColeccionBomba.cpp
#include "ColeccionBomba.h"

vector<pair<int, int>> bloquesDestruidosGlobales;

CColeccionBomba::CColeccionBomba() {
    totalBombas = 1;
}

void CColeccionBomba::crearBomba(int x, int y) {
    

    // Por esta nueva condición:
    if (bombas.size() < totalBombas) {
        CBomba* nuevaBomba = new CBomba(x, y);
        bombas.push_back(nuevaBomba);
    }
}

vector<pair<int, int>> CColeccionBomba::getBloquesDestruidosRecientes() {
    vector<pair<int, int>> resultado = bloquesDestruidosGlobales;
    bloquesDestruidosGlobales.clear(); // Limpiar después de obtener
    return resultado;
}

void CColeccionBomba::dibujarBomba(Graphics^ g, Bitmap^ bmpBomba, Bitmap^ bmpExplosion, int jugadorX, int jugadorY, const CanchaArray& cancha) {
    for (int i = 0; i < bombas.size(); ++i) {
        switch (bombas.at(i)->getEstado()) {
        case EBomba::puesto:
            bombas.at(i)->dibujar(g, bmpBomba, jugadorX, jugadorY, cancha);
            bombas.at(i)->animar();
            break;

        case EBomba::explosion:
            bombas.at(i)->dibujarExplosion(g, bmpExplosion, cancha);
            bombas.at(i)->animarExplosion();
            // Recopilar bloques destruidos
            {
                auto bloquesDestruidos = bombas.at(i)->getBloquesDestruidos();
                for (auto& bloque : bloquesDestruidos) {
                    bloquesDestruidosGlobales.push_back(bloque);
                }
            }
            break;

        case EBomba::explotado:
            delete bombas.at(i);               // liberar memoria
            bombas.erase(bombas.begin() + i); // borrar del vector
            --i;                              // ajustar el índice
            break;
        }
    }
}