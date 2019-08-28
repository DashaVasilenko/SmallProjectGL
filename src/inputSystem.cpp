#include "inputSystem.h"

bool InputSystem::keys[1024]; // нажатые(отпущенные) клавиши
bool InputSystem::draw_gui = false; // рисуем в гуи
bool InputSystem::firstMouseMove = true;
double InputSystem::lastCursPosX;
double InputSystem::lastCursPosY;
double InputSystem::deltaCursPosX; 
double InputSystem::deltaCursPosY;