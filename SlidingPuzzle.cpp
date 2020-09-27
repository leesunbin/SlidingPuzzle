#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;

int main()
{
	srand(time(NULL)); //랜덤초기화

	auto scene = Scene::create("4x4 퍼즐", "Images/배경.png");

	ObjectPtr game_board[16];
	ObjectPtr init_board[16];
	int blank = 15;

	for (int i = 0; i < 16; i++) {
		string filename = "Images/" + to_string(i + 1) + ".png";
		//cout << filename << endl;

		game_board[i] = Object::create(filename, scene, 500 + (i % 4) * 80, 470 - (i / 4) * 80);
		init_board[i] = game_board[i];
		game_board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction) -> bool {
			int j;
			for (j = 0; j < 16; j++) {
				if (game_board[j] == object)
					break;
			}

			if ((j % 4 > 0 && j - 1 == blank) ||
				(j % 4 < 3 && j + 1 == blank) ||
				(j > 3 && j - 4 == blank) ||
				(j < 12 && j + 4 == blank))
			{
				game_board[j]->locate(scene, 500 + (blank % 4) * 80, 470 - (blank / 4) * 80);
				game_board[blank]->locate(scene, 500 + (j % 4) * 80, 470 - (j / 4) * 80);

				game_board[j] = game_board[blank];
				game_board[blank] = object;
				blank = j;

				int k;
				for (k = 0; k < 16; k++) {
					if (game_board[k] != init_board[k]) 
						break;
				}
				if (k == 16) {
					showMessage("COMPLETED!!!");
				}

			}

			return true;
			});
	}

	game_board[blank]->hide();

	auto count = 0;
	auto timer = Timer::create(0.1);
	timer->setOnTimerCallback([&](TimerPtr t)->bool {
		// cout << "timeout" << count << endl;

		int j = 0;
		do {
			switch (rand() % 4) {
			case 0: j = blank - 1; break; //좌
			case 1: j = blank + 1; break; //우
			case 2: j = blank - 4; break; //상 
			case 3: j = blank + 4; break; //하
			}
		} while (j < 0 || j > 15 ||!  ( (j % 4 > 0 && j - 1 == blank) ||
			(j % 4 < 3 && j + 1 == blank) ||
			(j > 3 && j - 4 == blank) ||
			(j < 12 && j + 4 == blank))); 
		

		game_board[j]->locate(scene, 500 + (blank % 4) * 80, 470 - (blank / 4) * 80);
		game_board[blank]->locate(scene, 500 + (j %  4) * 80, 470 - (j / 4) * 80);
			 
		auto object = game_board[j];
		game_board[j] = game_board[blank]; //swap
		game_board[blank] = object;
		blank = j;
 
		


		count++;
		if (count < 100) {
			t->set(0.1f);
			t->start();
		}
		return true;
	});
	timer->start();

	auto startButton = Object::create("Images/시작.png", scene, 200, 370);
	auto endButton = Object::create("Images/종료.png", scene, 200, 300);
	auto game_timer = Timer::create(210.f);

	showTimer(game_timer);
	game_timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("Game Over");

		startButton->show();
		endButton->show();

		return true;
		});

	
	startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
		startButton->hide();
		endButton->hide();

		game_timer->set(210.f);
		game_timer->start();
		
		return true;

		});

	endButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();

		return true;
		});

	startGame(scene);

	return 0;
}