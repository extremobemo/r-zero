/*******************************************************************************************
*
*   R-Zero
*
*   Originally created with raylib 4.2.0
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*   Copyright (c) 2023 Ben Morris (@extremobemo)
*
********************************************************************************************/

#include "engine_utils.h"
#include "ship/ship.h"

#define RLIGHTS_IMPLEMENTATION
#include "../include/rlights.h"

#define FLT_MAX     340282346638528859811704183484516925440.0f     
// Maximum value of a float, from bit pattern 01111111011111111111111111111111

int main(void) {
  InitWindow(constants::screenWidth, constants::screenHeight, "R-Zero");

  Camera camera = {0};
  camera.position = (Vector3){20.0f, 20.0f, 20.0f}; // Camera position
  camera.target = (Vector3){0.0f, 8.0f, 0.0f};      // Camera looking at point
  camera.up = (Vector3){0.0f, 1.6f, 0.0f};          // Camera up vector (rotation towards target)
  camera.fovy = 120.0f;                             // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type

  Ray ray = { 0 }; // Picking ray

  SetCameraMode(camera, CAMERA_FREE);
  SetTargetFPS(constants::fps_target); 

  Shader outline = LoadShader(NULL, "src/shader/data/outline.fs");
  Ship ship = Ship("resources/r-zero/rship.obj", {0.0f, 0.0f, 0.0f});
  Ship city = Ship("resources/r-zero/Track.obj", {0.0f, 0.0f, 0.0f});

  BoundingBox towerBBox = GetMeshBoundingBox(ship.model.meshes[0]);
  BoundingBox towerBBox2 = GetMeshBoundingBox(city.model.meshes[0]);

  RenderTexture2D target = LoadRenderTexture(constants::screenWidth, constants::screenHeight);

  // Main game loop
  while (!WindowShouldClose()) { // Detect window close button or ESC key

    UpdateCamera(&camera);
    // Display information about closest hit
    RayCollision collision = { 0 };
    char *hitObjectName = "None";
    collision.distance = FLT_MAX;
    collision.hit = false;
    Color cursorColor = WHITE;

    // Get ray and test against objects
    ray = GetMouseRay(GetMousePosition(), camera);

    // Check ray collision against ground quad
    

    // Check ray collision against bounding box first, before trying the full ray-mesh test
    //RayCollision boxHitInfo = GetRayCollisionBox(ray, towerBBox);
    RayCollision boxHitInfo = GetRayCollisionBox(ray, towerBBox2);

    if ((boxHitInfo.hit) && (boxHitInfo.distance < collision.distance))
    {
        collision = boxHitInfo;
        cursorColor = ORANGE;
        hitObjectName = "Box";

        // Check ray collision against model meshes
        RayCollision meshHitInfo = { 0 };
        for (int m = 0; m < city.model.meshCount; m++)
        {
            // NOTE: We consider the model.transform for the collision check but 
            // it can be checked against any transform Matrix, used when checking against same
            // model drawn multiple times with multiple transforms
            meshHitInfo = GetRayCollisionMesh(ray, city.model.meshes[m], city.model.transform);
            if (meshHitInfo.hit)
            {
                // Save the closest hit mesh
                if ((!collision.hit) || (collision.distance > meshHitInfo.distance)) collision = meshHitInfo;
                
                break;  // Stop once one mesh collision is detected, the colliding mesh is m
            }
        }

        if (meshHitInfo.hit)
        {
            collision = meshHitInfo;
            cursorColor = ORANGE;
            hitObjectName = "Mesh";
        }
    }


    // draw the scene with shader
    BeginDrawing();
      BeginTextureMode(target);
        ClearBackground((Color){255, 255, 255, 255});
        BeginMode3D(camera);
          ship.DrawShip();
          city.DrawShip();
          ship.DrawBoosters(ship.pos);
          //DrawGrid(10.0f, 10.0f);
        //   for (int i = 0; i <= 100; i++) {
        //     DrawModel(cone_model, generateParticleVelocity(), 1.0f, RED);
        //     DrawGrid(10.0f, 10.0f);
        //  }
         
        EndMode3D();
      EndTextureMode();

    // draw the scene but with banded light effect
      ClearBackground((Color){32, 64, 255, 255});
      BeginMode3D(camera);
       ship.DrawShip();
       city.DrawShip();

        //DrawGrid(10.0f, 10.0f);
        // for (int i = 0; i <= 100; i++) {
        //   DrawModel(cone_model, generateParticleVelocity(), 1.0f, ORANGE);
        //   DrawGrid(10.0f, 10.0f);
        // }

        if (boxHitInfo.hit) DrawBoundingBox(towerBBox2, LIME);
        if (collision.hit)
                {   ship.pos = {0,0,0};
                    //ship.pos = collision.point;
                    //ship.pos.y += 2.0f;
                    //ship.model.transform = MatrixRotateXYZ((Vector3){0.0f, DEG2RAD * -90.0f, 0.0f});
                    
                    
                    //DrawCube(collision.point, 0.3f, 0.3f, 0.3f, cursorColor);
                    DrawCubeWires(collision.point, 0.3f, 0.3f, 0.3f, RED);

                    Vector3 normalEnd;
                    normalEnd.x = collision.point.x + collision.normal.x;
                    normalEnd.y = collision.point.y + collision.normal.y;
                    normalEnd.z = collision.point.z + collision.normal.z;

                    // calculate the angle between the normal vector and the up vector
                    float angle = acos(Vector3DotProduct(normalEnd, (Vector3){0,1,0}));

                    // calculate the axis of rotation using the cross product of the normal vector and the up vector
                    Vector3 axis = Vector3CrossProduct(normalEnd, (Vector3){0,1,0});

                    // rotate the model's transform matrix by the calculated angle around the calculated axis
                    //ship.model.transform = MatrixRotate(axis, DEG2RAD * angle);


                    if (collision.normal.x < 0) {
                    ship.model.transform = MatrixRotateXYZ((Vector3){-collision.normal.x, DEG2RAD * -120.0 , -collision.normal.z});
                    }
                    ship.DrawShip();
                    DrawGrid(10.0f, 10.0f);
                    DrawLine3D(collision.point, normalEnd, RED);

                  
                }

      EndMode3D();

      BeginShaderMode(outline);
        DrawTexturePro(target.texture,
                       (Rectangle){0, 0, (float)target.texture.width,
                                  (float)-target.texture.height},
                       (Rectangle){0, 0, (float)target.texture.width,
                                  (float)target.texture.height},
                       (Vector2){0, 0}, 0, WHITE);
      EndShaderMode();

        if (collision.hit)
            {
                int ypos = 70;

                DrawText(TextFormat("Distance: %3.2f", collision.distance), 10, ypos, 10, BLACK);

                DrawText(TextFormat("Hit Pos: %3.2f %3.2f %3.2f",
                                    collision.point.x,
                                    collision.point.y,
                                    collision.point.z), 10, ypos + 15, 10, BLACK);

                DrawText(TextFormat("Hit Norm: %3.2f %3.2f %3.2f",
                                    collision.normal.x,
                                    collision.normal.y,
                                    collision.normal.z), 10, ypos + 30, 10, BLACK);
            }

      DrawFPS(10, 10);
    EndDrawing();
  }

  // De-Initialization
  //---------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //----------------------------------------------------------

  return 0;
}
