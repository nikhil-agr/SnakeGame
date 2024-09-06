#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

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

      Rectangle segment =
          Rectangle{offset + x * cellSize, offset + y * cellSize,
                    (float)cellSize, (float)cellSize};
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

  void Reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
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

    DrawTexture(texture, offset + position.x * cellSize,
                offset + position.y * cellSize, WHITE);
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
  bool running = true;
  int score = 0;
  Sound eatSound;
  Sound wallSound;

  Game() {
    InitAudioDevice();
    eatSound  = LoadSound("Sounds/eat.mp3");
    wallSound = LoadSound("Sounds/wall.mp3");
  }

  ~Game(){
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
  }

  void Draw() {
    food.Draw();
    snake.Draw();
  }

  void Update() {
    if (running) {
      snake.Update();
      CheckCollisionWithFood();
      CheckCollisionWithEdges();
      CheckCollisionWithTail();
    }
  }

  void CheckCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.position)) {
      food.position = food.GenerateRandomPos(snake.body);
      snake.addSegment = true;
      score++;
      PlaySound(eatSound);
    }
  }

  void CheckCollisionWithEdges() {
    if (snake.body[0].x == cellSize - 5 || snake.body[0].x == -1 ||
        snake.body[0].y == cellSize - 5 || snake.body[0].y == -1) {
      GameOver();
    }
  }

  void GameOver() {
    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
    running = false;
    score = 0;
    PlaySound(wallSound);
  }

  void CheckCollisionWithTail() {
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();

    if (ElementInDeque(snake.body[0], headlessBody)) {
      GameOver();
    }
  }
};

int main() {

  cout << "Starting the game..." << endl;
  InitWindow(2 * offset + cellSize * cellCount,
             2 * offset + cellSize * cellCount, "Retro Snake");
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
      game.running = true;
    }
    if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
      game.snake.direction = {0, 1}; // x remains same and y gets incremented
      game.running = true;
    }
    if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
      game.snake.direction = {-1, 0}; // x = -1 and y remains same
      game.running = true;
    }
    if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
      game.snake.direction = {1, 0}; // x = 1 and y remains same
      game.running = true;
    }

    // Drawing
    ClearBackground(green);
    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                   (float)cellSize * cellCount + 10,
                                   (float)cellSize * cellCount + 10},
                         5, darkGreen);
    DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
    DrawText(TextFormat("%i", game.score), offset + cellSize * cellCount - 15,
             25, 40, darkGreen);
    game.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}