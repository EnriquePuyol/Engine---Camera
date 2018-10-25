#ifndef __ModuleCameraEditor_h__
#define __ModuleCameraEditor_h__

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"

class ModuleCameraEditor : public Module
{
public:
	ModuleCameraEditor();
	~ModuleCameraEditor();

	bool            Init();
	update_status	PreUpdate();
	update_status   Update();
	update_status	PostUpdate();
	bool            CleanUp();

	void Move();

public:
	int cameraSpeed = 5;

private:
	unsigned vbo = 0;

	Frustum frustum;
	float aspect = 16 / 9;
	float3 eye = { 0.0f, 0.0f, 6.0f };
	float4x4 view_matrix;

};

#endif