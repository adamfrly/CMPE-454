// ctrlPoints.cpp

#include "tree.h"
#include "main.h"
#include "ctrlPoints.h"
#include "seq.h"

#define CYLINDER_WIDTH 5.0              //Used for novel feature
#define CYLINDER_HEIGHT 40.0
#define CYLINDER_LENGTH 5.0
#define SPHERE_RADIUS 13.0
#define SPHERE_COLOUR 34/255.0, 139/255.0, 34/255.0
#define CYLINDER_COLOUR 165/255.0, 42/255.0, 42/255.0
#define NUM_TREES 50
#define COLS 6
#define ROWS 6

void Tree::draw(mat4& WCStoVCS, mat4& WCStoCCS, vec3 lightDir, vec3 colour, int trees[]) {

    
    if (treesDrawn == false) {
        mat4 M, MV, MVP;
        int offsetDistX = (NUM_TREES / COLS) + 70;
        int offsetDistY = (NUM_TREES / ROWS) + 70;
        //int randArray[20] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                //int randIndex = rand() % 19;
                //offsetDistX += randArray[randIndex];
                //offsetDistY += randArray[randIndex];

                M = translate(vec3((i * offsetDistX) + (i*6), (j * offsetDistY) + (j*6), 40)) * scale(SPHERE_RADIUS, SPHERE_RADIUS, SPHERE_RADIUS);
                MV = WCStoVCS * M;
                MVP = WCStoCCS * M;

                sphere->draw(MV, MVP, lightDir, vec3(SPHERE_COLOUR));

                M = translate(vec3((i * offsetDistX) + (i*6), (j * offsetDistY) + (j * 6), 25)) * scale(3.0, 3.0, CYLINDER_HEIGHT);
                MV = WCStoVCS * M;
                MVP = WCStoCCS * M;
                cylinder->draw(MV, MVP, lightDir, vec3(CYLINDER_COLOUR));

            }

        }
        treesDrawn = true;
    }

}

int Tree::genRandTreeLoc() {
    return (rand() % 150);
}
