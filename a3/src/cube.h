#ifndef CUBE_H
#define CUBE_H


#include "linalg.h"
#include "seq.h"
#include "gpuProgram.h"


class CubeFace {
public:
    unsigned int v[3];
    CubeFace() {}
    CubeFace(int v0, int v1, int v2) {
        v[0] = v0; v[1] = v1; v[2] = v2;
    }
};


class Cube {

public:

    Cube() {


        //Triangle 1:
        verts.add(vec3(0.5, 1, 1.75));         // 0 (Makes train longer)
        verts.add(vec3(0.5, 1, -0.5));        // 1
        verts.add(vec3(-0.5, 1, 1.75));        // 2 (Makes train longer)
        faces.add(CubeFace(0, 1, 2));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));

        //Triangle 2:
        verts.add(vec3(-0.5, 1, -0.5));       // 3
        faces.add(CubeFace(1, 2, 3));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));
        normals.add(vec3(0.0, 0.5, 0.0));

        //Triangle 3:
        verts.add(vec3(0.5, 0, -0.5));       // 4
        faces.add(CubeFace(0, 1, 4));
        normals.add(vec3(0.5, 0.0, 0.0));
        normals.add(vec3(0.5, 0.0, 0.0));
        normals.add(vec3(0.5, 0.0, 0.0));

        //Triangle 4:
        verts.add(vec3(0.5, 0, 1.75));        // 5
        faces.add(CubeFace(0, 4, 5));
        normals.add(vec3(0.5, 0.0, 0.0));
        normals.add(vec3(0.5, 0.0, 0.0));
        normals.add(vec3(0.5, 0.0, 0.0));

        //Triangle 5:
        verts.add(vec3(-0.5, 0, -0.5));      // 6
        faces.add(CubeFace(4, 5, 6));
        normals.add(vec3(0.0, -0.5, 0.0));
        normals.add(vec3(0.0, -0.5, 0.0));
        normals.add(vec3(0.0, -0.5, 0.0));

        //Triangle 6:
        verts.add(vec3(-0.5, 0, 1.75));       // 7   (Makes train longer)
        faces.add(CubeFace(5, 6, 7));
        normals.add(vec3(0.0, -0.5, 0.0));
        normals.add(vec3(0.0, -0.5, 0.0));
        normals.add(vec3(0.0, -0.5, 0.0));

        //Triangle 7:
        faces.add(CubeFace(3, 6, 7));
        normals.add(vec3(-0.5, 0.0, 0.0));
        normals.add(vec3(-0.5, 0.0, 0.0));
        normals.add(vec3(-0.5, 0.0, 0.0));

        //Triangle 8:
        faces.add(CubeFace(3, 7, 2));
        normals.add(vec3(-0.5, 0.0, 0.0));
        normals.add(vec3(-0.5, 0.0, 0.0));
        normals.add(vec3(-0.5, 0.0, 0.0));

        //Triangle 9:
        faces.add(CubeFace(2, 7, 5));
        normals.add(vec3(0.0, 0.0, 0.5));
        normals.add(vec3(0.0, 0.0, 0.5));
        normals.add(vec3(0.0, 0.0, 0.5));

        //Triangle 10:
        faces.add(CubeFace(5, 2, 0));
        normals.add(vec3(0.0, 0.0, 0.5));
        normals.add(vec3(0.0, 0.0, 0.5));
        normals.add(vec3(0.0, 0.0, 0.5));

        //Triangle 11:
        faces.add(CubeFace(6, 3, 4));
        normals.add(vec3(0.0, 0.0, -0.5));
        normals.add(vec3(0.0, 0.0, -0.5));
        normals.add(vec3(0.0, 0.0, -0.5));

        //Triangle 12:
        faces.add(CubeFace(4, 1, 3));
        normals.add(vec3(0.0, 0.0, -0.5));
        normals.add(vec3(0.0, 0.0, -0.5));
        normals.add(vec3(0.0, 0.0, -0.5));


        gpu.init(vertShader, fragShader, "in cylinder.cpp");

        setupVAO();
    };


    ~Cube() {}

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
    seq<CubeFace>         faces;
    GLuint            VAO;

    GPUProgram        gpu;

    static const char* vertShader;
    static const char* fragShader;

    void setupVAO();
};

#endif
