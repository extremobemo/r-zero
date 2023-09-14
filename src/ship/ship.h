// Copyright (c) 2023 Ben Morris (@extremobemo)

#ifndef SHIP_H
#define SHIP_H

#include "../../include/raylib.h"
#include "../../include/raymath.h"


class Ship {
public:
    Ship(const char *fileName, Vector3 ship_pos);
    void DrawShip();
    void DrawBoosters(Vector3 pos);

    Model model;
    Vector3 pos = {0.0f, 0.0f, 3.0f};

};

#endif // SHIP