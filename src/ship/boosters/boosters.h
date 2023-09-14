// Copyright (c) 2023 Ben Morris (@extremobemo)

#ifndef BOOSTERS_H
#define BOOSTERS_H

#include "../ship.h"
#include "../../../include/raylib.h"
#include "../../../include/raymath.h"


class Boosters {
public:
    Boosters(Ship* ship);
    void DrawBoosters(Vector3 pos);
    
    Model cone_model;
    Vector3 pos;

};

#endif // SHIP