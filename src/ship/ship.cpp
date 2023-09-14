// Copyright (c) 2023 Ben Morris (@extremobemo)

#include "ship.h"
#include "boosters/boosters.h"

Ship::Ship(const char *fileName, Vector3 ship_pos) {
    model = LoadModel(fileName);
    pos = ship_pos;
}

void Ship::DrawShip() {
  pos.x = pos.x + 0.1;
  DrawModel(model, pos, 1.0f, GRAY);
  DrawBoosters(pos);
}

void Ship::DrawBoosters(Vector3 pos) {
  auto boosters = Boosters(this);
  boosters.DrawBoosters(pos);
}