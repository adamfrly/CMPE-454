#ifndef TREE_H
#define TREE_H

#include "headers.h"
#include "linalg.h"
#include "seq.h"
#include "sphere.h"
#include "cylinder.h"


class Tree {

public:

    seq<vec3> points;             // points in the air
    seq<vec3> bases;              // base on the terrain

    bool treesDrawn;
    GLFWwindow* window;

    Tree() {
        treesDrawn = false;
    }

    Tree(GLFWwindow* w) {
        window = w;
        treesDrawn = false;
    }

    void clear() {
        points.clear();
        bases.clear();
    }
    int count() {
        return points.size();
    }
    void draw(mat4& WCStoVCS, mat4& WCStoCCS, vec3 lightDir, vec3 colour, int trees[]);
    int genRandTreeLoc();

    float maxHeight() {
        float max = -MAXFLOAT;
        for (int i = 0; i < points.size(); i++)
            if (points[i].z > max)
                max = points[i].z;
        return max;
    }

};


#endif
