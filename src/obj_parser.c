#include "obj_parser.h"
#include "entities.h"
#include "math_utils.h"
#include <string.h>
#include <stdio.h>

Mesh *LoadOBJ(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file");
        return NULL;
    }

    Mesh *mesh = CreateMesh(0,0,0);
    char line[256];
    Vec3 *normals = NULL;
    size_t normal_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            AddVertsToMesh(mesh, vertex);
        } 
        else if (line[0] == 'v' && line[1] == 't') {
            continue;
        }
        else if (line[0] == 'v' && line[1] == 'n') {
            Vec3 normal;
            sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);

            normals = realloc(normals, sizeof(Vec3) * (normal_count + 1));
            if (!normals) {
                fprintf(stderr, "Memory allocation failed for normals\n");
                fclose(file);
                return NULL;
            }

            normals[normal_count++] = normal;
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            if (strstr(line, "//")) {
                int vn1, vn2, vn3;
                sscanf(line, "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);

                mesh->vertices[v1].normal = normals[vn1];
                mesh->vertices[v2].normal = normals[vn2];
                mesh->vertices[v3].normal = normals[vn3];
            } else if (strchr(line, '/')) {
                int vn1, vn2, vn3;
                sscanf(line, "f %d/%*d/%d %d/%*d/%d %d/%*d/%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);

                if (v1 >= 0 && (size_t)v1 < mesh->vertex_count)
                    mesh->vertices[v1].normal = normals[vn1];
                if (v2 >= 0 && (size_t)v2 < mesh->vertex_count)
                    mesh->vertices[v2].normal = normals[vn2];
                if (v3 >= 0 && (size_t)v3 < mesh->vertex_count)
                    mesh->vertices[v3].normal = normals[vn3];
            } else {
                // Handle format: v1 v2 v3
                sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            }
            AddFacesToMesh(mesh, v1 - 1, v2 - 1, v3 - 1);
        }
    }

    free(normals);

    return mesh;
}
