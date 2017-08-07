//---------------------------------------------------------------------------
// Matthew Cameron
// 300261699
// COMP 308
//---------------------------------------------------------------------------


#pragma once

#include <vector>

typedef struct Button
{
    char* text;
    float posX;
    float posY;
    float width;
    float height;
    int number;
    bool hovered;
} Button;

typedef struct Label
{
    char* text;
    float posX;
    float posY;
} Label;

class GUI
{

private:
    void(*callback)(int);
    std::vector<Button>* buttons;
    std::vector<Label>* labels;

public:
    GUI(void(*callback)(int));
    ~GUI();

    void AddButton(float x, float y, float width, float height, int callbackNumber, char* text);
    int AddLabel(float x, float y, char* text);
    void SetLabelText(int label, char* text);
    bool HandleMouseDown(int x, int y);
    void HandleMouseMove(int x, int y);
    void Draw();
    int width;
    int height;
};

