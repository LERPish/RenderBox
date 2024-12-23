#ifndef ENTITIES_H
#define ENTITIES_H
#include "math_utils.h"

typedef struct {
    Vertice *vertices;
    Edge *edges;
    Face *faces;
    size_t vertex_count;
    size_t edge_count;
    size_t face_count;
} Mesh;

typedef struct {
    Vec3 position;
    Vec3 eulerRotation; // Euler Angles
    Vec3 scale;
    Mesh *mesh;
    Mesh *transformedMesh;
} Object3D;

Mesh *CreateMesh(size_t vertex_count, size_t edge_count, size_t face_count);
void DestroyMesh(Mesh *mesh);

void AddVertsToMesh(Mesh *mesh, Vec3 *verts);
void AddEdgesToMesh(Mesh *mesh, Edge *edges);
void AddFacesToMesh(Mesh *mesh, Face *faces);

Object3D *CreateObject3D(Mesh *mesh, Vec3 position, Vec3 eulerRotation, Vec3 scale);
void DestroyObject3D(Object3D *obj);

void TranslateObject3D(Object3D *obj, Vec3 translation);
void RotateObject3D(Object3D *obj, Vec3 rotation);
void ScaleObject3D(Object3D *obj, Vec3 scale);

void TransformObject3D(Object3D *obj);

void RenderMesh(Mesh *mesh);
void RenderMeshWireframe(Mesh *mesh);

#endif