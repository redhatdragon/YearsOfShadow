#pragma once
#include "HAL.h"

/*
NOTES:
All angles are taken in the  form of degrees.
+Z == further away from the view, -Z == closer to the view.
Attempts to keep pure C compatability.

TODO: consider swapping floats for fixed point for portability?
*/

#ifdef __cplusplus
extern "C" {
#endif

struct EE_ColorR8G8B8 {
	uint8_t buffer[3];
};
struct EE_Point2Df {
	float x, y;
};
struct EE_Point3Df {
	float x, y, z;
};

void EE_drawTriangle2DRGB(const EE_Point2Df points[3], EE_ColorR8G8B8 color);
void EE_drawTriangle2DRGBVertex(const EE_Point2Df points[3], const EE_ColorR8G8B8 colors[3]);
void* EE_getNewMesh(const char* filepath);
void EE_drawMesh(void* mesh);
//WARNING: may be replaced later with a better alternative.
void EE_setTextureSubmesh(void* mesh, uint8_t submeshIndex, const char* textureType, const char* path);
void EE_setPositionMesh(void* mesh, float x, float y, float z);
void EE_setRotationMesh(void* mesh, float x, float y, float z);
void EE_setScaleMesh(void* mesh, float x, float y, float z);
void EE_releaseMesh(void* mesh);

void* EE_getNewInstancedMesh(const char* filePath);
void EE_drawInstancedMesh(void* meshID);
void EE_releaseInstancedMesh(void* meshID);
void EE_setInstancedMeshPositions(void* meshID, const EE_Point3Df* posBuffer, uint32_t count);
void EE_setInstancedMeshScale(void* meshID, EE_Point3Df scale);
void EE_setInstancedSubmeshTexture(void* meshID, uint8_t submeshIndex, const char* textureType, const char* path);

//TODO: consider implenting these later
void EE_translate(float x, float y, float z);
void EE_rotate(float x, float y, float z);
void EE_pushMatrix();
void EE_popMatrix();

void* EE_getNewCamera();
void EE_cameraLookAt(void* self, float x, float y, float z);
void EE_setCameraRotation(void* self, float x, float y, float z);
void EE_setCameraPos(void* self, float x, float y, float z);
void EE_useCamera(void* self);
void EE_releaseCamera(void* self);

#ifdef __cplusplus
}
#endif