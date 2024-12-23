#include "camera.h"
#include "math_utils.h"
#include "entities.h"
#include "camera.h"
#include "framebuffer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Mesh *CreateMesh(size_t vertex_count, size_t edge_count, size_t face_count) {
    Mesh *mesh = malloc(sizeof(Mesh));
    if (!mesh) {
        fprintf(stderr, "Failed to allocate memory for Mesh\n");
        exit(1);
    }

    mesh->vertex_count = vertex_count;
    mesh->edge_count = edge_count;
    mesh->face_count = face_count;

    mesh->vertices = malloc(sizeof(Vertice) * vertex_count);
    if (!mesh->vertices) {
        fprintf(stderr, "Failed to allocate memory for vertices\n");
        free(mesh);
        exit(1);
    }

    mesh->edges = malloc(sizeof(Edge) * edge_count);
    if (!mesh->edges) {
        fprintf(stderr, "Failed to allocate memory for vertices\n");
        free(mesh->vertices);
        free(mesh);
        exit(1);
    }

    mesh->faces = malloc(sizeof(Face) * face_count);
    if (!mesh->faces) {
        fprintf(stderr, "Failed to allocate memory for faces\n");
        free(mesh->edges);
        free(mesh->vertices);
        free(mesh);
        exit(1);
    }

    memset(mesh->vertices, 0, sizeof(Vertice) * vertex_count);
    memset(mesh->edges, 0, sizeof(Edge) * edge_count);
    memset(mesh->faces, 0, sizeof(Face) * face_count);

    return mesh;
}

void DestroyMesh(Mesh *mesh) {
    if (mesh) {
        if (mesh->faces)
            free(mesh->faces);
        if (mesh->edges)
            free(mesh->edges);
        if (mesh->vertices)
            free(mesh->vertices);

        free(mesh);
    }
}

void AddVertsToMesh(Mesh *mesh, Vec3 *verts) {
    memcpy(mesh->vertices, verts, sizeof(Vec3) * mesh->vertex_count);
}

void AddEdgesToMesh(Mesh *mesh, Edge *edges) {
    memcpy(mesh->edges, edges, sizeof(Edge) * mesh->vertex_count);
}

void AddFacesToMesh(Mesh *mesh, Face *faces);

Object3D *CreateObject3D(Mesh *mesh, Vec3 position, Vec3 eulerRotation, Vec3 scale) {
    Object3D *obj = malloc(sizeof(Object3D));
    if (!obj) {
        fprintf(stderr, "Failed to allocate memory for Object3D\n");
        exit(1);
    }
    
    obj->mesh = mesh;

    obj->transformedMesh = CreateMesh(mesh->vertex_count, mesh->edge_count, mesh->face_count);
    memcpy(obj->transformedMesh->vertices, mesh->vertices, sizeof(Vertice) * mesh->vertex_count); 

    for (size_t i = 0; i < mesh->edge_count; i++) {
        obj->transformedMesh->edges[i].v0 = mesh->edges[i].v0;
        obj->transformedMesh->edges[i].v1 = mesh->edges[i].v1;
    }

    for (size_t i = 0; i < mesh->face_count; i++) {
        obj->transformedMesh->faces[i].v0 = mesh->faces[i].v0;
        obj->transformedMesh->faces[i].v1 = mesh->faces[i].v1;
        obj->transformedMesh->faces[i].v2 = mesh->faces[i].v2;
    }

    obj->position = position;
    obj->eulerRotation = eulerRotation;
    obj->scale = scale;

    return obj;
}

void DestroyObject3D(Object3D *obj) {
    if (obj) {
        if (obj->mesh) {
            DestroyMesh(obj->mesh);
        }
        free(obj);
    }
}

void TranslateObject3D(Object3D *obj, Vec3 translation) {
    obj->position = Vec3Add(obj->position, translation);
}

void RotateObject3D(Object3D *obj, Vec3 rotation) {
    obj->eulerRotation = Vec3Add(obj->eulerRotation, rotation);
}

void ScaleObject3D(Object3D *obj, Vec3 scale) {
    obj->scale = Vec3Mult(obj->scale, scale);
}

void TransformObject3D(Object3D *obj) {
    for (size_t i = 0; i < obj->mesh->vertex_count; i++) {
        Vec3 localVertex = obj->mesh->vertices[i].pos;

        Vec3 scaledVertex = Vec3Mult(localVertex, obj->scale);
        
        Matrice3x3 rotationMatrix = MultiplyMat3(MultiplyMat3(RyAxis(obj->eulerRotation.y), RxAxis(obj->eulerRotation.x)), RzAxis(obj->eulerRotation.z));
        Vec3 rotatedVertex = MultiplyMat3Vec3(rotationMatrix, scaledVertex);

        Vec3 translatedVertex = Vec3Add(rotatedVertex, obj->position);

        obj->transformedMesh->vertices[i].pos = translatedVertex;
    }
}

void RenderMesh(Mesh *mesh);
void RenderMeshWireframe(Mesh *mesh, Camera cam) {
    for (size_t i = 0; i < mesh->face_count; i++) {
        Vec2 v0 = ProjectVert(mesh->vertices[mesh->faces[i].v0].pos, cam);
        Vec2 v1 = ProjectVert(mesh->vertices[mesh->faces[i].v1].pos, cam);
        Vec2 v2 = ProjectVert(mesh->vertices[mesh->faces[i].v2].pos, cam);

        ScreenEdge e0 = {v0,v1};
        ScreenEdge e1 = {v1,v2};
        ScreenEdge e2 = {v0,v2};

        line(e0, 0xFFFFFFFF);
        line(e1, 0xFFFFFFFF);
        line(e2, 0xFFFFFFFF);
    }
}
