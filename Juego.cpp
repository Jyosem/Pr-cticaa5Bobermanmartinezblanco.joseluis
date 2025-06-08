#include "Juego.h"

using namespace Bomberman;

void main() {
	srand(static_cast<unsigned>(time(nullptr)));  // Semilla única
	Application::EnableVisualStyles();
	Application::Run(gcnew Juego());
}