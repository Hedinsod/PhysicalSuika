#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CTransform
{
public:
	CTransform()
		: TRS(T* R* S)
	{
	}

	inline void SetFullPos(glm::vec3 NewPos)
	{
		Pos = NewPos;
		UpdateTranslation();
	}
	inline void SetPos(glm::vec2 NewPos)
	{
		Pos.x = NewPos.x;
		Pos.y = NewPos.y;
		UpdateTranslation();
	}
	inline void Translate(glm::vec2 Shift)
	{
		Pos.x += Shift.x;
		Pos.y += Shift.y;
		UpdateTranslation();
	}
	inline glm::vec2 GetPos() const { return { Pos.x, Pos.y }; };

	inline void SetRot(float InAngle)
	{
		Angle = glm::radians(InAngle);
		UpdateRotation();
	}
	inline void Rotate(float InAngle)
	{
		Angle += glm::radians(InAngle);
		UpdateRotation();
	}

	glm::mat4x4 GetModel() const
	{
		return TRS;
	}
private:

	inline void UpdateTranslation()
	{
		T = glm::translate(glm::mat4(1.0f), Pos);
		TRS = T * R * S;
	}
	inline void UpdateRotation()
	{
		R = glm::rotate(glm::mat4(1.0f), Angle, { 0.f, 0.f, 1.f });
		TRS = T * R * S;
	}

	glm::mat4x4 T = glm::mat4(1.0f);
	glm::mat4x4 R = glm::mat4(1.0f);
	glm::mat4x4 S = glm::mat4(1.0f);
	glm::mat4x4 TRS;

	glm::vec3 Pos{ 0.f, 0.f, 0.f };
	float Angle = 0.0f;
};
