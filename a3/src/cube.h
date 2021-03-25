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

        // all vertices of a 1x1x1 cube
        verts.add(vec3(0.0, 0.0, 0.0));
        verts.add(vec3(1.0, 0.0, 0.0));
        verts.add(vec3(0.0, 1.0, 0.0));
        verts.add(vec3(0.0, 0.0, 1.0));
        verts.add(vec3(0.0, 1.0, 1.0));
        verts.add(vec3(1.0, 0.0, 1.0));
        verts.add(vec3(1.0, 1.0, 0.0));
        verts.add(vec3(1.0, 1.0, 1.0));

        // all normals of a 1x1x1 cube
        normals.add(vec3(1.0, 0.0, 0.0));
        normals.add(vec3(1.0, 0.0, 0.0));
        normals.add(vec3(0.0, 1.0, 0.0));
        normals.add(vec3(0.0, 1.0, 0.0));
        normals.add(vec3(0.0, 0.0, 1.0));
        normals.add(vec3(0.0, 0.0, 1.0));
        normals.add(vec3(-1.0, 0.0, 0.0));
        normals.add(vec3(-1.0, 0.0, 0.0));
        normals.add(vec3(0.0, -1.0, 0.0));
        normals.add(vec3(0.0, -1.0, 0.0));
        normals.add(vec3(0.0, 0.0, -1.0));
        normals.add(vec3(0.0, 0.0, -1.0));


        // all faces of a 1x1x1 cube
        faces.add(CubeFace(1, 6, 7));
        faces.add(CubeFace(1, 5, 7));
        faces.add(CubeFace(2, 4, 7));
        faces.add(CubeFace(2, 5, 7));
        faces.add(CubeFace(4, 6, 7));
        faces.add(CubeFace(4, 3, 6));
        faces.add(CubeFace(2, 4, 0));
        faces.add(CubeFace(0, 4, 3));
        faces.add(CubeFace(0, 3, 1));
        faces.add(CubeFace(3, 1, 6));
        faces.add(CubeFace(0, 1, 2));
        faces.add(CubeFace(1, 2, 5));


        //// bottom vertices

        //verts.add(vec3(0, 0, -0.5));
        //normals.add(vec3(0, 0, -1));
        //for (int i = 0; i < numSlices; i++) {
        //    float theta = i / (float)numSlices * 2 * M_PI;
        //    verts.add(vec3(cos(theta), sin(theta), -0.5));
        //    normals.add(vec3(0, 0, -1));
        //}

        //// bottom faces

        //for (int i = 0; i < numSlices; i++)
        //    faces.add(CubeFace((i + 1) % numSlices, i, 0));

        //// top vertices

        //verts.add(vec3(0, 0, +0.5));
        //normals.add(vec3(0, 0, +1));
        //for (int i = 0; i < numSlices; i++) {
        //    float theta = i / (float)numSlices * 2 * M_PI;
        //    verts.add(vec3(cos(theta), sin(theta), +0.5));
        //    normals.add(vec3(0, 0, +1));
        //}

        //// top faces

        //for (int i = 0; i < numSlices; i++)
        //    faces.add(CubeFace(i + (numSlices + 1), (i + 1) % (numSlices + 1) + (numSlices + 1), 0 + (numSlices + 1)));

        //// side vertices (same as others, but with *different* normals)

        //int baseIndex = 2 * (numSlices + 1);

        //for (int i = 0; i < numSlices; i++) {
        //    float theta = i / (float)numSlices * 2 * M_PI;
        //    verts.add(vec3(cos(theta), sin(theta), -0.5));
        //    normals.add(vec3(cos(theta), sin(theta), 0));
        //}

        //for (int i = 0; i < numSlices; i++) {
        //    float theta = i / (float)numSlices * 2 * M_PI;
        //    verts.add(vec3(cos(theta), sin(theta), +0.5));
        //    normals.add(vec3(cos(theta), sin(theta), 0));
        //}

        //// side faces

        //for (int i = 0; i < numSlices; i++) {

        //    faces.add(CubeFace(baseIndex + i,
        //        baseIndex + (i + 1) % numSlices,
        //        baseIndex + (i + 1) % numSlices + numSlices));

        //    faces.add(CubeFace(baseIndex + (i + 1) % numSlices + numSlices,
        //        baseIndex + i + numSlices,
        //        baseIndex + i));
        //}

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
