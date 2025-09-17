#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

struct Vector2
{
	int x;
	int y;

	Vector2(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

enum Objects
{
	EMPTY, // Espacio vacío
	PLAYER, // Jugador (Ledian)
	WALL, // Pared
	BOX, // Caja
	HOLE, // Agujero
	GLASSES, // Lentes de Menethil
	EXIT // Salida
};

void drawMap(vector<vector<Objects>>& map, Vector2 playerPos, bool hasGlasses, int steps)
{
	system("cls");
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (playerPos.x == i && playerPos.y == j)
				cout << "L "; // La posición de Ledian.
			else
			{
				switch (map[i][j])
				{
				case WALL:
					cout << (hasGlasses ? "O " : "W "); // Uso un operador ternario que evalúa si Ledian tiene los Lentes. Si los tiene, muestra agujeros. Si no, muestra paredes.
					break;
				case BOX:
					cout << "X "; // Caja.
					break;
				case HOLE:
					cout << "O "; // Agujero.
					break;
				case GLASSES:
					cout << "G "; // Lentes de Menethil.
					break;
				case EXIT:
					cout << (hasGlasses ? "E " : "  "); // Uso otro operador ternario. Si Ledian tiene los Lentes, se revela la salida.
					break;
				default:
					cout << "  "; // Espacio vacío.
					break;
				}
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "Pasos: " << steps << endl;
	cout << "(W=Arriba, A=Izquierda, S=Abajo, D=Derecha)" << endl;
}

bool movePlayer(Vector2& playerPos, Vector2 direction, vector<vector<Objects>>& map, bool& hasGlasses, int& steps)
{
	// Calculo la nueva posición del jugador sumando dirección a su posición actual.
	int newX = playerPos.x + direction.x;
	int newY = playerPos.y + direction.y;

	// Si el movimiento está fuera de los límites del mapa, no es válido y retorna false.
	if (newX < 0 || newX >= map.size() || newY < 0 || newY >= map[0].size())
	{
		return false;
	}

	// Guardo en una variable de tipo Objects el objeto que se encuentra en la posición a la que el jugador quiere moverse para determinar con qué está interactuando.
	Objects& destination = map[newX][newY];

	////------- Condiciones para moverse -------////

	// Si el destino es una pared y el jugador no tiene los Lentes, el movimiento no es válido y retorna false.
	if (destination == WALL && !hasGlasses)
	{
		cout << endl;
		cout << "Hay una pared en tu camino, no puedes cruzarla. Intenta moverte en otra direccion." << endl;
		cout << endl;
		return false;
	}

	// Si el destino es un agujero, el jugador cae en él. Si es un muro y tiene los lentes, también. 
	if (destination == HOLE || (destination == WALL && hasGlasses))
	{
		cout << endl;
		cout << "Ledian cayo en un agujero. Fin del juego." << endl;
		cout << endl;
		return true;
	}

	// Si el destino es una caja, pueden ocurrir distintas interacciones.
	if (destination == BOX)
	{
		// Nueva posición de la caja según la direcciónd el movimiento.
		int boxNewX = newX + direction.x;
		int boxNewY = newY + direction.y;

		// Verifico si la caja puede moverse dentro de los límites. Si se mueve contra una pared o contra otra caja, el movimiento no es válido.
		if (boxNewX < 0 || boxNewX >= map.size() || boxNewY < 0 || boxNewY >= map[0].size() || map[boxNewX][boxNewY] == WALL || map[boxNewX][boxNewY] == BOX)
		{
			return false;
		}

		// Si la nueva posición de la caja es un agujero, la caja cae en el agujero y este se convierte en un espacio vacío (EMPTY). Si no, mueve la caja a la nueva posición.
		if (map[boxNewX][boxNewY] == HOLE)
		{
			map[boxNewX][boxNewY] = EMPTY;
		}
		else
		{
			map[boxNewX][boxNewY] = BOX;
		}
		map[newX][newY] = EMPTY; // Limpio la posición original de la caja en el mapa. 
	}

	// Si encuentra los Lentes...
	if (destination == GLASSES)
	{
		hasGlasses = true;
		map[newX][newY] = EMPTY; // Los lentes se recogen y queda un espacio vacio en ese lugar.
		cout << endl;
		cout << "Ledian encontro los Lentes de Menethil. Las paredes se han convertido en agujeros, ten mucho cuidado por donde vas." << endl;
		cout << endl;
		Sleep(2000);
	}

	// Si encuentra la salida y tiene los Lentes...
	if (destination == EXIT && hasGlasses)
	{
		steps++;
		map[newX][newY] = PLAYER;
		drawMap(map, Vector2(newX, newY), hasGlasses, steps);
		cout << endl;
		cout << "Felicitaciones. Ledian escapo con los Lentes de Menethil en " << steps << " pasos. Ganaste el juego." << endl;
		cout << endl;
		return true;
	}

	// Si el movimiento es válido, no termina el juego. Actualizo la posición del jugador a la nueva posición, incremento los pasos y vuelvo a dibujar el mapa.
	playerPos.x = newX;
	playerPos.y = newY;
	steps++;

	drawMap(map, playerPos, hasGlasses, steps);
	return false;
}

void playGame()
{
	// Inicialización del mapa.
	vector<vector<Objects>> map =
	{
		{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
		{WALL, PLAYER, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, WALL},
		{WALL, EMPTY, BOX, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, WALL},
		{WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, BOX, EMPTY, WALL},
		{WALL, EMPTY, WALL, HOLE, WALL, EMPTY, BOX, EMPTY, HOLE, WALL},
		{WALL, WALL, WALL, EMPTY, WALL, HOLE, WALL, WALL, EMPTY, WALL},
		{WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
		{WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, GLASSES, WALL},
		{WALL, EXIT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
		{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
	};

	// Posición inicial del jugador. Booleano que indica si tiene los Lentes. Contador de pasos en 0.
	Vector2 playerPos(1, 1);
	bool hasGlasses = false;
	int steps = 0;

	// Dibujo el mapa inicial.
	drawMap(map, playerPos, hasGlasses, steps);

	while (true)
	{
		if (_kbhit())
		{
			char input = _getch();
			Vector2 direction(0, 0);

			switch (tolower(input))
			{
			case 'w': direction = Vector2(-1, 0); // Arriba  
				break;
			case 's': direction = Vector2(1, 0); // Abajo
				break;
			case 'a': direction = Vector2(0, -1); // Izquierda
				break;
			case 'd': direction = Vector2(0, 1); // Derecha
				break;
			default:
				cout << "Entrada invalida. Usa W, A, S o D para moverte." << endl;
				continue; // Ignorar entradas no válidas
			}

			if (movePlayer(playerPos, direction, map, hasGlasses, steps))
			{
				// Juego terminado. Ganado o perdido
				break; // Salir del bucle del juego
			}
		}
	}
}

void showMenu()
{
	system("cls");

	cout << "*** Ledian y los Lentes de Menethil ***" << endl << endl;

	cout << "Objetivo del juego:" << endl;
	cout << "Ayuda a Ledian a encontrar y llevar los Lentes de Menethil (G) a la salida (E)." << endl;
	cout << "Pero cuidado, hay obstaculos en el camino y trampas que pueden hacerte perder." << endl << endl;

	cout << "Reglas del juego:" << endl;
	cout << "1. Ledian (L) puede caminar por los espacios vacios." << endl;
	cout << "2. No puedes atravesar paredes (W) a menos que tengas los Lentes de Menethil." << endl;
	cout << "3. Empuja las cajas (X) para cubrir los agujeros (O) y poder pasar." << endl;
	cout << "4. Si caes en un agujero (O), pierdes el juego." << endl;
	cout << "5. Una vez que recojas los lentes (G), las paredes se transformaran en agujeros (O)." << endl;
	cout << "6. Ganas el juego si alcanzas la salida (E) llevando los Lentes de Menethil." << endl << endl;

	cout << "Controles:" << endl;
	cout << "W: Moverse hacia arriba" << endl;
	cout << "A: Moverse hacia la izquierda" << endl;
	cout << "S: Moverse hacia abajo" << endl;
	cout << "D: Moverse hacia la derecha" << endl << endl;

	cout << "Presiona Enter para empezar tu aventura..." << endl;
	cin.get(); // Pausa para que el jugador presione Enter antes de comenzar
}

int main()
{
	showMenu();
	char restart;
	do {
		playGame(); // Jugar
		cout << "Quieres jugar de nuevo? (s/n): ";
		cin >> restart;
		if (tolower(restart) != 's')
		{
			cout << "Gracias por jugar. Hasta la proxima." << endl;
			break; // Salir del bucle si no quiere jugar de nuevo
		}
	} while (true);

	return 0;
}