#include "camera.h"
#include "math_utils.h"
#include "entities.h"
#include "camera.h"
#include "framebuffer.h"
#include <math.h>
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

void AddVertsToMesh(Mesh *mesh, Vec3 vert) {
    mesh->vertex_count += 1;
    mesh->vertices = realloc(mesh->vertices, mesh->vertex_count * sizeof(Vertice));
    mesh->vertices[mesh->vertex_count - 1] = (Vertice){.pos = vert, .normal = {0.0f, 0.0f, 0.0f}}; 
}

void AddEdgesToMesh(Mesh *mesh, Edge *edges) {
    memcpy(mesh->edges, edges, sizeof(Edge) * mesh->vertex_count);
}

void AddFacesToMesh(Mesh *mesh, size_t v0, size_t v1, size_t v2) {
    mesh->face_count += 1;
    mesh->faces = realloc(mesh->faces, mesh->face_count * sizeof(Face));
    Vec3 normal = CalculateFaceNormal(mesh->vertices[v0].pos,
                                      mesh->vertices[v1].pos,
                                      mesh->vertices[v2].pos);
    mesh->faces[mesh->face_count - 1] = (Face){ v0, v1, v2, normal };
}

Vec3 CalculateFaceNormal(Vec3 v0, Vec3 v1, Vec3 v2) {
    Vec3 edge1 = Vec3Sub(v1, v2);
    Vec3 edge2 = Vec3Sub(v2, v0);
    Vec3 ret = Vec3Normalize(Vec3Cross(edge1, edge2));
    return ret;
}

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
        obj->transformedMesh->faces[i].normal = mesh->faces[i].normal;
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

    for (size_t i = 0; i < obj->mesh->face_count; i++) {
        Face *face = &obj->transformedMesh->faces[i];

        Vec3 v0 = obj->transformedMesh->vertices[face->v0].pos;
        Vec3 v1 = obj->transformedMesh->vertices[face->v1].pos;
        Vec3 v2 = obj->transformedMesh->vertices[face->v2].pos;

        Vec3 normal = CalculateFaceNormal(v0, v1, v2);

        face->normal = normal;
    }
}

void RenderMesh(Mesh *mesh, Camera cam) {
    for (size_t i = 0; i < mesh->face_count; i++) {
        Face *face = &mesh->faces[i];
        Vertice *v0 = &mesh->vertices[face->v0];
        Vertice *v1 = &mesh->vertices[face->v1];
        Vertice *v2 = &mesh->vertices[face->v2];

        Vec3 viewDirection = Vec3Sub(cam.pos, v0->pos);

        float dot = Vec3DotProduct(face->normal, viewDirection);

        if (dot <= 0.0f)
            continue;

        float nDot = fmaxf(0, Vec3DotProduct(face->normal, Vec3Normalize((Vec3){ 1, 1, -1 })));
        Color color = CreateColorHex(0xFFFFFFFF);

        color.r *= nDot;
        color.g *= nDot;
        color.b *= nDot;

        color.r = fminf(fmaxf(color.r, 0.0f), 1.0f);
        color.g = fminf(fmaxf(color.g, 0.0f), 1.0f);
        color.b = fminf(fmaxf(color.b, 0.0f), 1.0f);
        color.a = 0.1f;

        RGBToHex(&color);

        RasterizeTriangle(v0->pos, v1->pos, v2->pos, cam, color.hex);
    }
}

void RenderMeshWireframe(Mesh *mesh, Camera cam) {
    for (size_t i = 0; i < mesh->face_count; i++) {
        Face *face = &mesh->faces[i];
        Vec3 viewDirection = Vec3Sub(cam.pos, mesh->vertices[face->v0].pos);

        float dot = Vec3DotProduct(face->normal, viewDirection);

        if (dot <= 0.0f)
            continue;

        Vec3 vv0 = ProjectVert(mesh->vertices[mesh->faces[i].v0].pos, &cam);
        Vec3 vv1 = ProjectVert(mesh->vertices[mesh->faces[i].v1].pos, &cam);
        Vec3 vv2 = ProjectVert(mesh->vertices[mesh->faces[i].v2].pos, &cam);
        Vec2 v0 = {vv0.x, vv0.y};
        Vec2 v1 = {vv1.x, vv1.y};
        Vec2 v2 = {vv2.x, vv2.y};

        line(v0, v1, 0x00000000);
        line(v1, v2, 0x00000000);
        line(v2, v0, 0x00000000);
    }
}
