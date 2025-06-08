#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include "Enemigo.h"

using namespace std;

class CColeccionEnemigo {
	vector<CEnemigo*> enemigos;

public:
	vector<CEnemigo*> getEnemigos() {
		return enemigos;
	}

	void crearEnemigo() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		CEnemigo* enemigo = new CEnemigo();
		enemigos.push_back(enemigo);
	}

	void dibujar(Graphics^ g, Bitmap^ bmpEnemigo, const CanchaArray& cancha) {
		for (int i = 0; i < enemigos.size(); i++) {
			enemigos.at(i)->dibujar(g, bmpEnemigo, cancha);
			enemigos.at(i)->animar();
		}
	}
};
