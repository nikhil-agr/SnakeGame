#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

class Snake{
  public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5, 9}, Vector2{4, 9}};
    void Draw(){
      for(int i = 0; i < body.size(); i++){
        float x = body[i].x;
        float y = body[i].y;
        // DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, darkGreen);

        Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
        DrawRectangleRounded(segment, 0.5, 6, darkGreen);
      }
    }
};

class Food{
  public:
    Vector2 position; // this doesn't represent the pixels but cell, so {5, 6} is 6th column and 7th row
    Texture2D texture;

    Food(){
      Image image = LoadImage("Graphics/apple.png");
      texture = LoadTextureFromImage(image);
      UnloadImage(image);
      position = GenerateRandomPos(); // generate a random position for the food
    }

    ~Food(){
      UnloadTexture(texture);
    }

    void Draw(){
      // DrawRectangle(x, y, w, h, color);
      // DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);

      DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
      DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);

    }

    Vector2 GenerateRandomPos(){
      float x = GetRandomValue(0, cellCount - 1);
      float y = GetRandomValue(0, cellCount - 1);

      return Vector2{x, y};
    }
};
int main() {

  cout << "Starting the game..." << endl;
  InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
  SetTargetFPS(60); 

  Food food;
  Snake snake;

  while(WindowShouldClose() == false){
    BeginDrawing();

    // Drawing
    ClearBackground(green);
    food.Draw();
    snake.Draw();

    EndDrawing();
  }

  CloseWindow(); 
  return 0;
}