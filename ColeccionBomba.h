// ColeccionBomba.h
#pragma once
#include <vector>
#include "Bomba.h";
#define TOTALBOMBAS 3

using namespace std;

class CColeccionBomba {
    vector<CBomba*> bombas;
    int totalBombas;

public:
    CColeccionBomba();
    vector<CBomba*>& getBombas() { return bombas; }

    void crearBomba(int x, int y);
    void dibujarBomba(Graphics^ g, Bitmap^ bmpBomba, Bitmap^ bmpExplosion, int jugadorX, int jugadorY, const CanchaArray& cancha);
    vector<pair<int, int>> getBloquesDestruidosRecientes(); // Nueva función
};
