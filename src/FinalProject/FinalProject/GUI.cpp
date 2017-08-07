//---------------------------------------------------------------------------
// Matthew Cameron
// 300261699
// COMP 308
//---------------------------------------------------------------------------


#include <GL/glut.h>
#include "GUI.h"


GUI::GUI(void(*callback)(int))
{
    this->callback = callback;
    buttons = new std::vector<Button>();
    labels = new std::vector<Label>();
}


GUI::~GUI()
{
    delete buttons;
    delete labels;
}

void GUI::AddButton(float x, float y, float width, float height, int callbackNumber, char* text)
{
    Button result;
    result.posX = x;
    result.posY = y;
    result.width = width;
    result.height = height;
    result.number = callbackNumber;
    result.text = text;
    result.hovered = false;

    buttons->push_back(result);
}

int GUI::AddLabel(float x, float y, char* text)
{
    Label result;
    result.posX = x;
    result.posY = y;
    result.text = text;

    labels->push_back(result);
    return labels->size() - 1;
}

void GUI::SetLabelText(int label, char* text)
{
    labels->at(label).text = text;
}

bool GUI::HandleMouseDown(int x, int y)
{
    for (std::vector<Button>::iterator it = buttons->begin(); it != buttons->end(); ++it)
    {
        if (x >= it->posX && x <= it->posX + it->width)
        {
            if (y >= it->posY && y <= it->posY + it->height)
            {
                callback(it->number);
                return true;
            }
        }
    }

    return false;
}

void GUI::HandleMouseMove(int x, int y)
{
    for (std::vector<Button>::iterator it = buttons->begin(); it != buttons->end(); ++it)
    {
        if (x >= it->posX && x <= it->posX + it->width && y >= it->posY && y <= it->posY + it->height)
        {
            it->hovered = true;
        }
        else
        {
            it->hovered = false;
        }
    }
}

void GUI::Draw()
{
    glMatrixMode(GL_MODELVIEW);

    // Draw buttons
    for (std::vector<Button>::iterator it = buttons->begin(); it != buttons->end(); ++it)
    {
        float top, left, right, bottom;

        left = it->posX;
        right = left + it->width;
        top = it->posY;
        bottom = top + it->height;

        glBegin(GL_QUADS);

        if (it->hovered)
        {
            glColor3f(1, 1, 0.2f);
        }
        else
        {
            glColor3f(0, 1, 0);
        }
        glVertex3f(left, top, -0.01f);
        glVertex3f(right, top, -0.01f);
        glVertex3f(right, bottom, -0.01f);
        glVertex3f(left, bottom, -0.01f);
        glEnd();


        glColor3f(1, 0, 1);
        glRasterPos2f(left, top + 24);

        char* toDraw = it->text;
        while (*toDraw != '\0')
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *toDraw);
            toDraw++;
        }
    }

    // Draw labels
    for (std::vector<Label>::iterator it = labels->begin(); it != labels->end(); ++it)
    {
        glColor3f(1, 1, 1);
        glRasterPos2f(it->posX, it->posY + 24);

        char* toDraw = it->text;
        while (*toDraw != '\0')
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *toDraw);
            toDraw++;
        }
    }
}
