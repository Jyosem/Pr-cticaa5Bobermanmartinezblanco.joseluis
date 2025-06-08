#pragma once
#include "Items.h"
#include "Cancha.h"
#define VELOCIDADENEMIGO 5

using namespace System::Drawing;

enum EEstadoEnemigo {
	Vivo = 0,
	Eliminado
};

class CEnemigo {
	int x;
	int y;
	int indiceX;
	int indiceY;
	int dx;
	int dy;
	bool ubicado;
	EEstadoEnemigo estado;
public:
	CEnemigo();
	~CEnemigo();

	bool buscarLugar(const CanchaArray& cancha, int& i, int& j);
	void dibujar(Graphics^ g, Bitmap^ bmpEnemigo, const CanchaArray& cancha);
	void animar();

};
