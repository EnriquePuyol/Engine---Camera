#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "MathGeoLib.h"

class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();

private:
    unsigned vbo        = 0;

	Frustum frustum;
	float aspect = 16 / 9;
	float3 eye = {0.0f, 0.0f, 6.0f};
	float4x4 view_matrix;

};

#endif /* __ModuleRenderExercise_h__ */
