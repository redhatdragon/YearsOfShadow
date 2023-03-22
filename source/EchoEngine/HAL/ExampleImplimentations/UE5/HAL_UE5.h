#pragma once

#include "../HAL_3D.h"
#include "CoreMinimal.h"
#include <Actor.h>

int bla = a;

void EE_drawTriangle2DRGB(const EE_Point2Df points[3], EE_ColorR8G8B8 color);
void EE_drawTriangle2DRGBVertex(const EE_Point2Df points[3], const EE_ColorR8G8B8 colors[3]);
void* EE_getNewMesh(const char* filepath);
void EE_drawMesh(void* mesh);
//WARNING: may be replaced later with a better alternative.
void EE_setTextureSubmesh(void* mesh, uint8_t submeshIndex, uint8_t textureIndex, const char* path);
void EE_setPositionMesh(void* mesh, float x, float y, float z);
void EE_setRotationMesh(void* mesh, float x, float y, float z);
void EE_setScaleMesh(void* mesh, float x, float y, float z);
void EE_releaseMesh(void* mesh);

//TODO: consider implenting these later
void EE_translate(float x, float y, float z);
void EE_rotate(float x, float y, float z);
void EE_pushMatrix();
void EE_popMatrix();

void* EE_getNewCamera();
void EE_setCameraRotation(void* self, float x, float y, float z);
void EE_setCameraPos(void* self, float x, float y, float z);
void EE_useCamera(void* self);
void EE_releaseCamera(void* self);