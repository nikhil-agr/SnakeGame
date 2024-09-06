#include <iostream>
#include <raylib.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

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

  while(WindowShouldClose() == false){
    BeginDrawing();

    // Drawing
    ClearBackground(green);
    food.Draw();

    EndDrawing();
  }

  CloseWindow(); 
  return 0;
}