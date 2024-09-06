#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> body) {
  for (unsigned int i = 0; i < body.size(); i++) {
    if (Vector2Equals(body[i], element))
      return true;
  }
  return false;
}

bool eventTriggered(double interval) {
  double currentTime = GetTime();

  if (currentTime - lastUpdateTime >= interval) {
    lastUpdateTime = currentTime;
    return true;
  }

  return false;
}

class Snake {
public:
  deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
  Vector2 direction = {1, 0};
  bool addSegment = false;

  void Draw() {
    for (int i = 0; i < body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      // DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize,
      // darkGreen);

      Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize,
                                    (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, darkGreen);
    }
  }

  void Update() {
    body.push_front(Vector2Add(body[0], direction));

    if (addSegment)
      addSegment = false;
    else
      body.pop_back();
  }
};

class Food {
public:
  Vector2 position; // this doesn't represent the pixels but cell, so {5, 6} is
                    // 6th column and 7th row
  Texture2D texture;

  Food(deque<Vector2> snakeBody) {
    Image image = LoadImage("Graphics/apple.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position =
        GenerateRandomPos(snakeBody); // generate a random position for the food
  }

  ~Food() { UnloadTexture(texture); }

  void Draw() {
    // DrawRectangle(x, y, w, h, color);
    // DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize,
    // cellSize, darkGreen);

    DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
  }

  Vector2 GenerateRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    Vector2 position = {x, y};

    return position;
  }

  Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
    Vector2 position = GenerateRandomCell();

    while (ElementInDeque(position, snakeBody))
      position = GenerateRandomCell();

    return position;
  }
};

class Game {
public:
  Snake snake = Snake();
  Food food = Food(snake.body);

  void Draw() {
    food.Draw();
    snake.Draw();
  }

  void Update() {
    snake.Update();
    CheckCollisionWithFood();
  }

  void CheckCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.position)) {
      food.position = food.GenerateRandomPos(snake.body);
      snake.addSegment = true;
    }
  }
};

int main() {

  cout << "Starting the game..." << endl;
  InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
  SetTargetFPS(60);

  Game game = Game();

  while (WindowShouldClose() == false) {
    BeginDrawing();

    if (eventTriggered(0.2)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
      game.snake.direction = {
          0, -1}; // x remains same and y gets decremented as the coordinate
                  // system being used here is the 'Top-Left
    }
    if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
      game.snake.direction = {0, 1}; // x remains same and y gets incremented
    }
    if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
      game.snake.direction = {-1, 0}; // x = -1 and y remains same
    }
    if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
      game.snake.direction = {1, 0}; // x = 1 and y remains same
    }

    // Drawing
    ClearBackground(green);
    game.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}