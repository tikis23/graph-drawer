#include "CoordinateManager.h"

#include "Input.h"
#include "WindowManager.h"
#include "Settings.h"
#include "GraphRenderer.h"

double CoordinateManager::GetCenterX()
{
    return centerX;
}

double CoordinateManager::GetCenterY()
{
    return centerY;
}

double CoordinateManager::GetScaleLevel()
{
    return scaleLevel;
}

double CoordinateManager::ScreenToWorldX(int screenX)
{
    return (centerX + (-scaleLevel + ((scaleLevel + scaleLevel) / (WindowManager::GetWindow("main")->GetWidth())) * (screenX))) / Settings::globalScale;
}

double CoordinateManager::ScreenToWorldY(int screenY)
{
    return (centerY + (-scaleLevel + ((scaleLevel + scaleLevel) / (WindowManager::GetWindow("main")->GetHeight())) * (screenY))) / Settings::globalScale;
}

int CoordinateManager::WorldToScreenX(double worldX)
{
    return (WindowManager::GetWindow("main")->GetWidth() / ((centerX + scaleLevel) - (centerX - scaleLevel))) * (Settings::globalScale * worldX - (centerX - scaleLevel));
}

int CoordinateManager::WorldToScreenY(double worldY)
{
    return (WindowManager::GetWindow("main")->GetHeight() / ((centerY + scaleLevel) - (centerY - scaleLevel))) * (Settings::globalScale * worldY - (centerY - scaleLevel));
}

float CoordinateManager::ScreenNormalizedX(int screenX)
{
    return 2.0f * (screenX + 0.5f) / WindowManager::GetWindow("main")->GetWidth() - 1.0f;
}
float CoordinateManager::ScreenNormalizedY(int screenY)
{
    return 2.0f * (screenY + 0.5f) / WindowManager::GetWindow("main")->GetHeight() - 1.0f;
}

void CoordinateManager::Update()
{
    GLFWwindow* window = WindowManager::GetWindow("main")->GetHandle();

    // panning
    int tempX, tempY;
    Input::GetMousePosition(window, tempX, tempY);
    if (Input::GetMouseButton(window, Input::Mouse::LEFT, Input::Action::HOLD))
    {
        double panX = ScreenToWorldX(tempX);
        double panY = ScreenToWorldY(tempY);
        double panDiffX = panX - panOldX;
        double panDiffY = panY - panOldY;
        centerX -= panDiffX * Settings::globalScale * Settings::panSpeed * 0.1;
        centerY -= panDiffY * Settings::globalScale * Settings::panSpeed * 0.1;
        for (auto& it : GraphRenderer::graphs)
            it.generate = true;
    }
    panOldX = ScreenToWorldX(tempX);
    panOldY = ScreenToWorldY(tempY);

    // zooming
    if (Input::GetScrollOffsetY(window) != 0)
    {
        // check for offset
        Input::GetMousePosition(window, tempX, tempY);
        double oldMouseWorldX = ScreenToWorldX(tempX);
        double oldMouseWorldY = ScreenToWorldY(tempY);

        // scale
        scaleLevel -= (double)Input::GetScrollOffsetY(window) * scaleLevel * (double)Settings::zoomSpeed * 0.001;

        // remove offset
        Input::GetMousePosition(window, tempX, tempY);
        double mouseWorldX = ScreenToWorldX(tempX);
        double mouseWorldY = ScreenToWorldY(tempY);
        centerX += oldMouseWorldX - mouseWorldX;
        centerY += oldMouseWorldY - mouseWorldY;
        for (auto& it : GraphRenderer::graphs)
            it.generate = true;
    }
}

double CoordinateManager::centerX = 0;
double CoordinateManager::centerY = 0;
double CoordinateManager::scaleLevel = 10;
double CoordinateManager::panOldX;
double CoordinateManager::panOldY;