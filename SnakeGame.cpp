#include <iostream>
#include <windows.h>
#include <sstream>

using namespace std;

struct SnakePosition
{
    int x; // X coordinate
    int y; // Y coordinate
};

class Family // Game area
{
    static const int Height; // Height of the game area
    static const int Width;  // Width of the game area
    char **F;
    Family(const Family &);
    Family operator=(const Family &);

public:
    Family()
    {
        F = new char *[Height];
        for (int c = 0; c < Height; ++c)
        {
            F[c] = new char[Width];
        }
    }
    ~Family()
    {
        for (int c = 0; c < Height; ++c)
        {
            delete[] F[c];
        }
        delete[] F;
    }
    void print() // Print the game area
    {
        for (int c = 0; c < Height; ++c)
        {
            for (int r = 0; r < Width; ++r)
            {
                cout << F[c][r];
            }
            cout << endl;
        }
    }
    void clear() // Clear the game area
    {
        for (int c = 0; c < Height; ++c)
        {
            for (int r = 0; r < Width; ++r)
            {
                if (c == 0 || c == Height - 1 || r == 0 || r == Width - 1) // Check if it's a border
                {
                    F[c][r] = '#'; // Border character
                }
                else
                {
                    F[c][r] = ' '; // Inner characters to space
                }
            }
        }
    }
    void DrawTextMessage(int y, int x, const string &message)
    {
        for (size_t i = 0; i < message.size(); ++i)
        {
            if (y >= 0 && y < Height && x + static_cast<int>(i) >= 0 && x + static_cast<int>(i) < Width)
            {
                F[y][x + i] = message[i];
            }
        }
    }
    int getHeight() const
    {
        return Height;
    }
    int getWidth() const
    {
        return Width;
    }
    void Draw(int Y, int X, char what)
    {
        if (Y >= 1 && Y < Height - 1 && X >= 1 && X < Width - 1)
        {
            F[Y][X] = what;
        }
    }
} F;

class FoodPosition
{
    SnakePosition position;
    char symbol; // Symbol representing the food

public:
    FoodPosition() : symbol('$'), position() // Constructor initializing the food symbol and position
    {
        position.x = position.y = -1;
    }
    void setPosition(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
    void reposition(const Family &F) // Method to reposition the food randomly within the game area
    {
        position.x = rand() % (F.getWidth() - 2) + 1;  // Random X position
        position.y = rand() % (F.getHeight() - 2) + 1; // Random Y position
    }
    int getX() const // Method to get the X coordinate of the food
    {
        return position.x;
    }
    int getY() const // Method to get the Y coordinate of the food
    {
        return position.y;
    }
    char getSymbol() const // Method to get the symbol of the food
    {
        return symbol;
    }
} Food;

class Snake
{
    enum // Define directions for the snake
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    } dir;
    char symbol, headSymbol;
    SnakePosition Position[100]; // Array to hold the positions of the snake segments
    SnakePosition &head;         // Reference to the head of the snake
    int Speed;                   // Speed of the snake
    int Size;                    // Size of the snake
    bool can_turn;               // Flag to check if the snake can turn
    int score;                   // Score of the player

public:
    Snake(int X, int Y) : Position(), dir(RIGHT), symbol('#'), headSymbol('O'), Speed(1), Size(1), can_turn(true), head(Position[0]), score(0) // Constructor initializing the snake
    {
        Position[0].x = X;
        Position[0].y = Y;
    }
    bool CheckFood(const FoodPosition &food) // Method to check if the snake has eaten the food
    {
        if (food.getX() == head.x && food.getY() == head.y) // Check if head position matches food position
        {
            Size += 1;   // Increase the size of the snake
            score += 10; // Increase the score
            return true; // Return true indicating food was eaten
        }
        return false; // Return false if food was not eaten
    }
    void GetInput(const Family &F) // Method to get user input for snake direction
    {
        if (GetAsyncKeyState(VK_UP) && dir != DOWN && can_turn) // Check if UP key is pressed and snake is not going DOWN
        {
            dir = UP;
            can_turn = false; // Set can_turn to false to prevent immediate direction change
        }
        else if (GetAsyncKeyState(VK_DOWN) && dir != UP && can_turn) // Check if DOWN key is pressed and snake is not going UP
        {
            dir = DOWN;
            can_turn = false;
        }
        else if (GetAsyncKeyState(VK_LEFT) && dir != RIGHT && can_turn) // Check if LEFT key is pressed and snake is not going RIGHT
        {
            dir = LEFT;
            can_turn = false;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && dir != LEFT && can_turn) // Check if RIGHT key is pressed and snake is not going LEFT
        {
            dir = RIGHT;
            can_turn = false;
        }
        else
        {
            can_turn = true; // Allow turning again if no key is pressed
        }
    }
    void Move(const Family &F) // Method to move the snake
    {
        SnakePosition next = {0, 0}; // Create a new position for the next move
        switch (dir)                 // Determine the next position based on the current direction
        {
        case UP:
            next.y = -Speed; // Move up
            break;
        case DOWN:
            next.y = Speed; // Move down
            break;
        case LEFT:
            next.x = -Speed; // Move left
            break;
        case RIGHT:
            next.x = Speed; // Move right
            break;
        }
        for (int c = Size - 1; c > 0; --c) // Move each segment to the position of the segment in front of it
        {
            Position[c] = Position[c - 1]; // Update segment position
        }
        head.x += next.x; // Update head position
        head.y += next.y; // Update head position

        if (head.x < 1 || head.y < 1 || head.x >= F.getWidth() - 1 || head.y >= F.getHeight() - 1) // Check if the head is out of bounds
        {
            throw "Game Over!!!"; // Throw an exception to indicate game over
        }
    }
    void Draw(Family &F) // Method to draw the snake on the game area
    {
        for (int c = 0; c < Size; ++c) // Loop through each segment of the snake
        {
            if (c == 0) // Check if it's the head
            {
                F.Draw(Position[c].y, Position[c].x, headSymbol); // Draw the head symbol
            }
            else
            {
                F.Draw(Position[c].y, Position[c].x, symbol); // Draw the body symbol
            }
        }
    }
    int GetX() const // Method to get the X coordinate of the snake's head
    {
        return head.x;
    }
    int GetY() const // Method to get the Y coordinate of the snake's head
    {
        return head.y;
    }
    char GetSymbol() const // Method to get the symbol of the snake
    {
        return symbol;
    }
    int GetScore() const // Method to get the current score
    {
        return score;
    }
} Snake(1, 1); // Create an instance of the Snake class with initial position (1, 1)

const int Family::Height = 24; // Define the height of the game area
const int Family::Width = 80;  // Define the width of the game area

int main()
{
    F.clear();              // Clear the game area
    Food.setPosition(5, 5); // Set initial position of the food

    while (1)
    {
        F.clear();
        Snake.GetInput(F); // Get user input for snake direction
        try
        {
            Snake.Move(F); // Move the snake
        }
        catch (const char *error) // Catch any game over exceptions
        {
            F.clear();

            stringstream gameOverMsg;                                     // Create a string stream for the message
            gameOverMsg << "GAME OVER! Your score: " << Snake.GetScore(); // Format the game over message

            // Display the message in the center of the screen
            int msgPosX = (F.getWidth() - gameOverMsg.str().length()) / 2; // Calculate X position for centering
            int msgPosY = F.getHeight() / 2;                               // Calculate Y position for centering

            F.DrawTextMessage(msgPosY, msgPosX, gameOverMsg.str()); // Draw the game over message
            F.print();                                              // Print the game area with the message

            system("pause"); // Pause the system to allow the player to see the message
            return -1;       // Exit the program
        }
        Snake.Draw(F);                                      // Draw the snake on the game area
        F.Draw(Food.getY(), Food.getX(), Food.getSymbol()); // Draw the food on the game area

        if (Snake.CheckFood(Food)) // Check if the snake has eaten the food
        {
            Food.reposition(F); // Reposition the food randomly
        }

        cout << "Score: " << Snake.GetScore() << endl; // Print the current score

        F.print();
        Sleep(1000 / 30); // Sleep for a short duration to control the game speed
        system("cls");
    }
}
