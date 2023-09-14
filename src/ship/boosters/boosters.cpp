// Copyright (c) 2023 Ben Morris (@extremobemo)

#include "boosters.h"

Boosters::Boosters(Ship* ship) {
    Mesh cone_mesh = GenMeshCone(99.5f, 13.0f, 8);
    Model cone_model = LoadModelFromMesh(cone_mesh);
    pos = ship->pos;
}

Vector3 generateParticleVelocity() {
  return {(float)GetRandomValue(-100, 100) / 100 + 10, .20f,
          (float)GetRandomValue(-100, 100) / 100 + 2};
}

void Boosters::DrawBoosters(Vector3 pos) {
  // pos.x = pos.x + 0.1;
  Mesh cone_mesh = GenMeshCone(.5f, 3.0f, 8);
  Model cone_model = LoadModelFromMesh(cone_mesh);
  cone_model.transform = MatrixRotateXYZ((Vector3){DEG2RAD * 90.0f, 0.0f, DEG2RAD * -90.0f});

  for (int i = 0; i <= 100; i++) {
    DrawModel(cone_model, generateParticleVelocity(), 1.0f, RED);
  }
}