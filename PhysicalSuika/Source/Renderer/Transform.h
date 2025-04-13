#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CTransform final
{
public:
	CTransform() = default;
	CTransform(glm::vec2 InPos, float ZOrder = 0.0f) : Pos(InPos.x, InPos.y, ZOrder) {}


	// Position

	inline void SetPos(glm::vec2 NewPos)
	{
		Pos.x = NewPos.x;
		Pos.y = NewPos.y;
	}
	inline void Translate(glm::vec2 Shift)
	{
		Pos.x += Shift.x;
		Pos.y += Shift.y;
	}
	inline void SetZOrer(float ZOrder)
	{
		Pos.z = ZOrder;
	}
	inline glm::vec2 GetPos() const { return { Pos.x, Pos.y }; };


	// Rotation

	inline void SetRotation(float InAngle)
	{
		Rotation = glm::radians(InAngle);
	}
	// Rotates actor by Angle radians CCW
	inline void Rotate(float InAngle)
	{
		Rotation += InAngle;
	}
	// Rotate() but angle is in radians
	inline void RotateDeg(float InAngle)
	{
		Rotation += glm::radians(InAngle);
	}
	inline float GetRotation() const { return Rotation; };

	// Scale

	inline void SetScale(glm::vec2 InScale)
	{
		Scale = { InScale.x, InScale.y, 1.0f };
	}


	// For rendering
	glm::mat4x4 GetModel() const
	{
		glm::mat4 T = glm::translate(glm::mat4(1.0f), Pos);
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), Rotation, { 0.f, 0.f, 1.f });
		glm::mat4 S = glm::scale(glm::mat4(1.0f), Scale);

		return T * R * S;
	}

private:
	glm::vec3 Pos{ 0.f, 0.f, 0.f };
	float Rotation = 0.0f;
	glm::vec3 Scale{ 1.f, 1.f, 1.f };

};
