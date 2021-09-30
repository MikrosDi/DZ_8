#include<iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <thread>

using namespace std;

/*Запрограммировать игру крестики нолики на поле 3х3. 
Прислать архив всего проекта или ссылку на гит репозиторий.
Для джуниоров: не обязательно делать крутой искусственный интеллект в игре. 
Но у вас должны корректно работать все основные моменты в игре, как например и главный цикл игры в main.
Челендж для опытных и продвинутых: сделать решение на поле 4х4 или 5х5 или на динамическом поле, размер которого ввел пользователь. 
Также можно запросить у пользователя количество фигур необходимое для выигрыша, 3-5.*/

enum Kletka : char 
{
	CROSS = 'X', // крестик
	ZERO = '0',  // нолик
	EMPTY = '_' // пустая клетка
};

enum Progress : uint16_t // Выделяю больше потому что нет цели экономить
{
	IN_PROGRESS, // игра уже началась - но ещё не окончена
	WON_HUMAN, //выйграл человек
	WON_BOT,    // выйграл бот
	ALL_LOSE   // ничья/ все проиграли
};

struct Coordinat // координаты
{
	size_t x{ 0 };
	size_t y{ 0 };
};

#pragma pack(push, 1)
struct Game 
{
	Kletka** ppField = nullptr;
	const size_t SIZE = 3;
	size_t turn = 0;
	Kletka human = CROSS;
	Kletka bot = ZERO;
	Progress progress = IN_PROGRESS;
};
#pragma pack(pop)

inline void Сlear()
{
	system("cls");
}

	int32_t getRandomMove(int32_t min, int32_t max) // генератор случайных чисел
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
};

	void __fastcall initGame(Game& g)
	{
		g.ppField = new Kletka * [g.SIZE];
		for (size_t i = 0; i < g.SIZE; i++)
		{
			g.ppField[i] = new Kletka[g.SIZE];
		}

		for (size_t x = 0; x < g.SIZE; x++)
		{
			for (size_t y = 0; y < g.SIZE; y++)
			{
				g.ppField[x][y] = EMPTY;
			}
		}

		if (getRandomMove(0, 722) > 361) // условия для того что бы рандомный определитель первого хода работал
		{
			g.human = CROSS;
			g.bot = ZERO;
			g.turn = 0;
		}
		else
		{
			g.human = ZERO;
			g.bot = CROSS;
			g.turn = 1;
		}

	}

	void __fastcall deinitGame(Game& g)
	{
		for (size_t i = 0; i < g.SIZE; i++)
		{
			delete [] g.ppField[i];
		}
		delete[] g.ppField;
		g.ppField = nullptr;
	}

	void __fastcall PrintGame(const Game& g)
	{
		setlocale(LC_ALL, "rus");
		cout << "    ";
		for (size_t y = 0; y < g.SIZE; y++)
		{
			cout << y + 1 << "   ";
		}
		cout << endl;

		for (size_t x = 0; x < g.SIZE; x++)
		{
			cout << x + 1 << " | ";
			for (size_t y = 0; y < g.SIZE; y++)
			{
				cout << g.ppField[x][y] << " | ";
			}
			cout << endl;
		}
		cout << endl << "Человек: " << g.human << endl; 
		cout << "Компьютер: " << g.bot << endl;
	}
	void __fastcall GCwin(const Game& g)
	{
		setlocale(LC_ALL, "rus");
		if (g.progress == WON_HUMAN)
			cout << "Умница! С победой тебя)" << endl;
		if (g.progress == WON_BOT)
			cout << "Молодец! Скоро ты поработишь этих людишек)" << endl;
		if (g.progress == ALL_LOSE)
			cout << "Тренеруйтесь дальше, пока что вы не готовы!" << endl;
	}
	Progress getWon(const Game& g)
	{
		setlocale(LC_ALL, "rus");
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[x][0] == g.ppField[x][1] && g.ppField[x][0] == g.ppField[x][2])
			{
				if (g.ppField[x][0] == g.human)
					return WON_HUMAN;
				if (g.ppField[x][0] == g.bot)
					return WON_BOT;
			}
		}
		for (size_t y = 0; y < g.SIZE; y++)
		{
			if (g.ppField[0][y] == g.ppField[1][y] && g.ppField[0][y] == g.ppField[2][y])
			{
				if (g.ppField[0][y] == g.human)
					return WON_HUMAN;
				if (g.ppField[0][y] == g.bot)
					return WON_BOT;
			}
		}
		//диаганаль
		if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
		{
			if (g.ppField[0][0] == g.human)
				return WON_HUMAN;
			if (g.ppField[0][0] == g.bot)
				return WON_BOT;
		}
		if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[2][0] == g.ppField[1][1])
		{
			if (g.ppField[1][1] == g.human)
				return WON_HUMAN;
			if (g.ppField[1][1] == g.bot)
				return WON_BOT;
		}

		// проверка на ничью
		bool all_lose = true;
		for (size_t x = 0; x < g.SIZE; x++)
		{
			for (size_t y = 0; y < g.SIZE; y++)
				if (g.ppField[x][y] == EMPTY)
				{
					all_lose = false;
					break;
				}
			if (!all_lose)
				break;
		}
		if (all_lose)
			return ALL_LOSE;

		return IN_PROGRESS;
	}

	Coordinat getHumanCoordinat(const Game& g)
	{
		setlocale(LC_ALL, "rus");
		Coordinat c;
		do 
		{
			cout << "Введите X: ";
			cin >> c.x;
			cout << "Введите Y: ";
			cin >> c.y;
		}
		while(c.x == 0 || c.x > 3 || c.y == 0 || c.y > 3 || g.ppField[c.x - 1][c.y - 1] != EMPTY);
		c.x--;
		c.y--;
		return c;
	}
	Coordinat getBotCoordinat(Game & g)
	{
		setlocale(LC_ALL, "rus");
		// пытается победить

		for (size_t x = 0; x < g.SIZE; x++)
		{

			for (size_t y = 0; y < g.SIZE; y++)
			{
				if (g.ppField[x][y] == EMPTY)
				{
					g.ppField[x][y] = g.bot;
					if (getWon(g) == WON_BOT)
					{
						g.ppField[x][y] = EMPTY;
						return { x, y };
					}
					g.ppField[x][y] = EMPTY;
				}
			}
		}

		//мешает человеку
		for (size_t x = 0; x < g.SIZE; x++)
		{

			for (size_t y = 0; y < g.SIZE; y++)
			{
				if (g.ppField[x][y] == EMPTY)
				{
					

					g.ppField[x][y] = g.human;
					if (getWon(g) == WON_HUMAN)
					{
						g.ppField[x][y] = EMPTY;
						return { x, y };
					}
					g.ppField[x][y] = EMPTY;
				}
			}
		}
		
		//центр
		if (g.ppField[1][1] == EMPTY)
			return { 1, 1 };
		Coordinat arr[4];
		size_t num{ 0 };

		//углы
		if (g.ppField[0][0] == EMPTY)
		{
			arr[num] = { 0, 0 };
			num++;
		}
		if (g.ppField[2][2] == EMPTY)
		{
			arr[num] = { 2, 2 };
			num++;
		}
		if (g.ppField[0][2] == EMPTY)
		{
			arr[num] = { 0, 2 };
			num++;
		}
		if (g.ppField[2][0] == EMPTY)
		{
			arr[num] = { 2, 0 };
			num++;
		}
		if (num > 0)
		{
			const size_t index = getRandomMove(0, 4000) % num;
			return arr[index];
		}
		//не забывай
		num = 0;

		// не углы
		if (g.ppField[0][1] == EMPTY)
		{
			arr[num] = { 0, 1 };
			num++;
		}
		if (g.ppField[1][0] == EMPTY)
		{
			arr[num] = { 1, 0 };
			num++;
		}
		if (g.ppField[1][2] == EMPTY)
		{
			arr[num] = { 1, 2 };
			num++;
		}
		if (g.ppField[2][1] == EMPTY)
		{
			arr[num] = { 2, 1 };
			num++;
		}
		if (num > 0)
		{
			const size_t index = getRandomMove(0, 4000) % num;
			return arr[index];
		}

		return{};
	}
	



int main()
{
	setlocale(LC_ALL, "rus");

	Game g;
	initGame(g);

	Сlear();

	PrintGame(g);

	do
	{
		if (g.turn % 2 == 0)
		{
			Coordinat c = getHumanCoordinat(g);
			g.ppField[c.x][c.y] = g.human;
			
	    }
		else
		{
			Coordinat c = getBotCoordinat(g);
			g.ppField[c.x][c.y] = g.bot;
			std::this_thread::sleep_for(2500ms);
		}

		Сlear();
		PrintGame(g);
		g.progress = getWon(g);
		g.turn++;
	} 
	while (g.progress == IN_PROGRESS);

	GCwin(g);

	deinitGame(g);

	return 0;
}

