# SlidingPuzzle

## 초기설정
```
#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;
```
## main함수
```
int main()
{
	srand(time(NULL)); //랜덤초기화

	auto scene = Scene::create("4x4 퍼즐", "Images/배경.png");

	ObjectPtr game_board[16];
	ObjectPtr init_board[16];
	int blank = 15;
```
퍼즐블럭 이미지를 넣을 배열과 초기값 복사해서 저장해 둘 배열 생성

```
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
```
for 루프를 이용해서 1~16까지의 퍼즐 이미지 배열에 넣어주고 초기 배열 값을 넣어줄 배열에도 똑같이 값을 넣어줌. 

```
 		if ((j % 4 > 0 && j - 1 == blank) ||
				(j % 4 < 3 && j + 1 == blank) ||
				(j > 3 && j - 4 == blank) ||
				(j < 12 && j + 4 == blank))
			{
				game_board[j]->locate(scene, 500 + (blank % 4) * 80, 470 - (blank / 4) * 80);
				game_board[blank]->locate(scene, 500 + (j % 4) * 80, 470 - (j / 4) * 80);
```
퍼즐을 클릭하면 blank처리한 마지막 퍼즐(흰색으로 표시됨)과 다른 퍼즐의 배열 위치를 바꾸는 식으로 퍼즐을 움직이는데 
blank 퍼즐이 클릭한 퍼즐의 상하좌우로 접해있을 경우에만 위치 바꾸는 것이 가능하도록 설정. 

```
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
```
움직인 배열(game_board)과 초기값을 저장해둔 배열(init_board)이 같아질 경우 "COMPLETED"메세지 출력

```
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
```
게임 맨 처음 시작할 때 퍼즐을 섞어주도록 함. 위에서 사용했던 조건들(상하좌우에 blank가 접해있을 때)을 적용하고 상/하/좌/우 중 랜덤으로 움직이도록 함. 
섞을 때는 timer를 사용해 타이머가 한번 작동될때마다 퍼즐 섞기가 실행되도록 함. 
```
		count++;
		if (count < 100) {
			t->set(0.1f);
			t->start();
		}
		return true;
	});
	timer->start();
 ```
 100번 반복해줌(게임창을 실행하자마자 바로 움직임)

 ```
	auto startButton = Object::create("Images/시작.png", scene, 200, 370);
	auto endButton = Object::create("Images/종료.png", scene, 200, 300);
	auto game_timer = Timer::create(210.f);

```
시작버튼, 종료버튼, 게임화면에 보여줄 타이머 생성

```
	showTimer(game_timer);
	game_timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("Game Over");

		startButton->show();
		endButton->show();

		return true;
		});

```
시간이 다 흐르면 Game Over 메세지 출력, 시작버튼과 종료버튼 보이게 함.

```
	startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
		startButton->hide();
		endButton->hide();

		game_timer->set(210.f);
		game_timer->start();
		
		return true;

		});
    
  ```
  시작버튼을 누르면 타이머가 작동되고 시작버튼과 종료버튼은 숨김. 시작버튼을 다시 누르면 타이머가 다시 처음으로 세팅됨.
  ```

	endButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
```
종료버튼을 누르면 즉시 게임 종료. 
```
		return true;
		});

	startGame(scene);

	return 0;
}
```
