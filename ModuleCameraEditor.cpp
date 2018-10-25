#include "ModuleCameraEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "GL/glew.h"
#include "SDL.h"



ModuleCameraEditor::ModuleCameraEditor()
{
}


ModuleCameraEditor::~ModuleCameraEditor()
{
}

bool ModuleCameraEditor::Init()
{
	// https://github.com/d0n3val/Edu-Game-Engine/blob/master/Source/ModuleEditorCamera.cpp

	float3 vertex_buffer_data[] = {
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ 0.0f,  1.0f, 0.0f },
	};

	// Creacion de matrices para el ejercicio
	float3 target(0.0f, 0.0f, 0.0f);
	float3 up(0.0f, 1.0f, 0.0f);

	// View matrix
	float3 f(target - eye); f.Normalize();
	float3 s(f.Cross(up));
	float3 u(s.Cross(f));

	view_matrix[0][0] = s.x;	view_matrix[0][1] = s.y;	view_matrix[0][2] = s.z;	view_matrix[0][3] = -s.Dot(eye);
	view_matrix[1][0] = u.x;	view_matrix[1][1] = u.y;	view_matrix[1][2] = u.z;	view_matrix[1][3] = -u.Dot(eye);
	view_matrix[2][0] = -f.x;	view_matrix[2][1] = -f.y;	view_matrix[2][2] = -f.z;	view_matrix[2][3] = f.Dot(eye);
	view_matrix[3][0] = 0.0f;	view_matrix[3][1] = 0.0f;	view_matrix[3][2] = 0.0f;	view_matrix[3][3] = 1.0f;

	// Projection matrix
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * aspect;
	math::float4x4 proj = frustum.ProjectionMatrix();

	// Transformacion del buffer data
	for (int i = 0; i < 3; i++)
	{
		// proj * view * model
		float4 res = proj * view_matrix * float4(vertex_buffer_data[i], 1.0f);
		vertex_buffer_data[i] = res.xyz() / res.w;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

update_status ModuleCameraEditor::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleCameraEditor::Update()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	glColor3f(0.5f, 0.5f, 0.5f);

	Move();

	// ---------------------
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleCameraEditor::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleCameraEditor::CleanUp()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	return true;
}

void ModuleCameraEditor::Move()
{
	float speed = cameraSpeed;
	float3 moveDir(float3::zero);

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		moveDir = float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		moveDir = -float3::unitY;

	frustum.Translate(moveDir * speed);
}
