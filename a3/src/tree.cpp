// ctrlPoints.cpp

#include "tree.h"
#include "main.h"
#include "ctrlPoints.h"
#include "seq.h"

#define CYLINDER_WIDTH 3.0              //Used for novel feature
#define CYLINDER_HEIGHT 40.0
#define CYLINDER_LENGTH 3.0
#define SPHERE_RADIUS 15.0
#define SPHERE_COLOUR 34/255.0, 139/255.0, 34/255.0
#define CYLINDER_COLOUR 165/255.0, 42/255.0, 42/255.0
#define NUM_TREES 50
#define COLS 6
#define ROWS 6

void Tree::draw(mat4& WCStoVCS, mat4& WCStoCCS, vec3 lightDir, vec3 colour) {


    mat4 M, MV, MVP;
    int offsetDistX = (NUM_TREES / COLS) + 70;
    int offsetDistY = (NUM_TREES / ROWS) + 70;
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {

            if ((rand() % 2) == 0) {
                int randNum = rand() % 25;
                int randNum2 = rand() % 25;
                M = translate(vec3((i * offsetDistX) + (i+randNum), (j * offsetDistY) + (j+randNum2), 40)) * scale(SPHERE_RADIUS, SPHERE_RADIUS, SPHERE_RADIUS);
                MV = WCStoVCS * M;
                MVP = WCStoCCS * M;

                sphere->draw(MV, MVP, lightDir, vec3(SPHERE_COLOUR));

                M = translate(vec3((i * offsetDistX) + (i+randNum), (j * offsetDistY) + (j+randNum2), 25)) * scale(CYLINDER_LENGTH, CYLINDER_WIDTH, CYLINDER_HEIGHT);
                MV = WCStoVCS * M;
                MVP = WCStoCCS * M;
                cylinder->draw(MV, MVP, lightDir, vec3(CYLINDER_COLOUR));
            }
        }

    }
    

}

int Tree::genRandTreeLoc() {
    return (rand() % 150);
}
