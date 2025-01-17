#include <raylib.h>
#include <iostream>
#include <deque>
#include <string>

using namespace std;

// The cell width in pixels
const int cellSize = 30;
// The cells number
const int n_cell = 25;

// The offset of screnn
const int off_set = 75;

// Screen Size
const int screenHeight = 2*off_set+cellSize*n_cell;
const int screenWidth = 2*off_set+cellSize*n_cell;

// Cell count
const int count_y_size = screenHeight/cellSize;
const int count_x_size = screenWidth/cellSize;

// Colors
Color gray = { 201, 201, 201, 255 };
Color dark_gray = { 73, 73, 73, 255 };

// Check if two vectors are equal
bool VectorsAreEqual(Vector2 vec1, Vector2 vec2) {
    if (vec1.x == vec2.x && vec1.y == vec2.y) {
        return true;
    }
    return false;
}

// Check a Vector in deque
bool VectorInDeque(Vector2 target, deque<Vector2> itr) {
    for (int i = 0; i < itr.size(); i++){
        if (VectorsAreEqual(itr[i], target)) {
            return true; 
        };
    };
    return false;
};

// Calculates if the time has passed
double last_time = 0;

bool EventSleep(double seconds) {
    double cur_time = GetTime();
    if (cur_time - last_time >= seconds) {
        // If the time is passed, so return true
        last_time = cur_time;
        return true;
    }
    return false;
}

// Food Object
class Food {
public:
    // Food position
    Vector2 pos = {5, 6};

    // Func to draw food
    void Draw() {
        // DrawRectangleRounded(Rectangle ret, float rounded, int segments, Color color)
        Rectangle rec = Rectangle{pos.x*cellSize+off_set, pos.y*cellSize+off_set, cellSize, cellSize};
        DrawRectangleRounded(rec, 0.5, 6, dark_gray);
    };

    void ChangePos() {
        pos.x = GetRandomValue(0, n_cell-1);
        pos.y = GetRandomValue(0, n_cell-1);
    };
};

class Snake {
public:
    // head position
    // snake body
    deque<Vector2> body = {Vector2{10, 10}, Vector2{9, 10}, Vector2{8, 10}};
    // snake direction
    Vector2 direction = {0, 1};
    bool running = true;

    void Draw() {
        for (int i = 0; i < body.size(); i++) {
            // DrawRectangleRounded(Rectangle ret, float rounded, int segments, Color color)
            Rectangle rec = Rectangle{
                body[i].x*cellSize+off_set, body[i].y*cellSize+off_set, cellSize, cellSize
            };
            DrawRectangleRounded(rec, 0.5, 6, dark_gray);
        };
    };

    void Eat() {
        Vector2 new_component = {
            body[body.size()-1].x,
            body[body.size()-1].y
        };
        body.push_back(new_component);
    };

    void Run() {
        // Create new head local and check if it is on body
        Vector2 new_head = {body[0].x+direction.x, body[0].y+direction.y};
        //Check Death # n_cell-1 because 
        if (Collision(new_head) || new_head.x > n_cell-1 || new_head.x < 0 || new_head.y > n_cell-1 || new_head.y < 0) {
            running = false;
        };

        // head update
        body.push_front(new_head);
        // delete body last element
        body.pop_back();
    };

    bool Collision(Vector2 next_pos) {
        if (VectorInDeque(next_pos, body)) {
            return true;
        };
        return false;
    };
};

int main() {
    // Score
    int score = 0;
    // Sounds
    Sound eat;
    Sound wall;

    InitAudioDevice();
    eat = LoadSound("sounds/eat.mp3");
    wall = LoadSound("sounds/wall.mp3");
    // INIT APP
    InitWindow(screenWidth, screenHeight, "snake_game");

    // run app
    SetTargetFPS(60);
    
    // Creating the food
    Food food = Food();
    // Creating the snake
    Snake snake = Snake();

    while (!WindowShouldClose()) {

        // Check the running
        if (!snake.running){
            break;
        };
        // Controls
        if (IsKeyDown(KEY_RIGHT)){
            snake.direction = {1, 0};
        };
        if (IsKeyDown(KEY_LEFT)){
            snake.direction = {-1, 0};
        };
        if (IsKeyDown(KEY_UP)){
            snake.direction = {0, -1};
        };
        if (IsKeyDown(KEY_DOWN)){
            snake.direction = {0, 1};
        };

        if (VectorsAreEqual(snake.body[0], food.pos)) {
            snake.Eat();
            while (VectorInDeque(food.pos, snake.body)) {
                PlaySound(eat);
                score++;
                food.ChangePos();
            };
        };

        BeginDrawing();
        // all drawing happens
            ClearBackground(gray);


            // Screen area # The off_set-5 is because the weidth of line is 5
            Rectangle area = Rectangle{
                (float)off_set-5,
                (float)off_set-5,
                (float)cellSize*n_cell+10,
                (float)cellSize*n_cell+10,
            };
            DrawRectangleLinesEx(area, 5, dark_gray);
            DrawText("Snake Game", off_set, off_set-40, 40, dark_gray);
            DrawText(TextFormat("%i", score), off_set, off_set+cellSize*n_cell+cellSize, 30, dark_gray);
            food.Draw();
            snake.Draw();
           // Add snake step
            if (EventSleep(0.2)) {
                snake.Run();
            };
            
        EndDrawing();
    };
    CloseWindow();
    return 0;
};
