#ifndef ELEVATOR_H
#define ELEVATOR_H


#include "linalg.h"
#include "seq.h"
#include "gpuProgram.h"


class elevatorFace {
public:
    unsigned int v[3];
    elevatorFace() {}
    elevatorFace(int v0, int v1, int v2) {
        v[0] = v0; v[1] = v1; v[2] = v2;
    }
};


class elevator {

public:

    elevator() {


        //Triangle 1:
        verts.add(vec3(0.5, 0.5, 0.5));         // 0
        verts.add(vec3(0.5, 0.5, -0.5));        // 1
        verts.add(vec3(-0.5, 0.5, 0.5));        // 2
        faces.add(elevatorFace(0, 1, 2));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));

        //Triangle 2:
        verts.add(vec3(-0.5, 0.5, -0.5));       // 3
        faces.add(elevatorFace(1, 2, 3));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));

        gpu.init(vertShader, fragShader, "in elevator.cpp");

        setupVAO();
    };


    ~elevator() {}

    // The cylinder drawing provides to the shader (a) the OCS vertex
    // position as attribute 0 and (b) the OCS vertex normal as
    // attribute 1.
    //
    // The cylinder is drawn at the origin with radius 1 and height 1
    // (in [-0.5,+0.5]).  You must provide mat4 MV and mat4 MVP matrices,
    // as well as vec3 lightDir and vec3 colour.

    void draw(mat4& MV, mat4& MVP, vec3 lightDir, vec3 colour);

private:

    seq<vec3>         verts;
    seq<vec3>         normals;
    seq<elevatorFace>         faces;
    GLuint            VAO;

    GPUProgram        gpu;

    static const char* vertShader;
    static const char* fragShader;

    void setupVAO();
};

#endif
