#pragma once

#include "Transform.h"


class ACamera
{
public:
	ACamera(float Top, float Left, float Bottom, float Right, glm::vec2 InPos)
		: Projection(glm::ortho(Left, Right, Bottom, Top))
	{
		Trans.SetFullPos({ InPos.x, InPos.y, -1.0 });

		UpdateProjView();
	}

	inline void SetPos(glm::vec2 NewPos)
	{
		Trans.SetPos(NewPos);
		UpdateProjView();
	}
	inline void SetRot(float InAngle)
	{
		Trans.SetRot(InAngle);
		UpdateProjView();
	}

	glm::mat4x4 GetVP() const
	{
		return ProjView;
	}

private:
	void UpdateProjView()
	{
		ProjView = Projection * glm::inverse(Trans.GetModel());
	}

	CTransform Trans;

	glm::mat4x4 Projection;
	glm::mat4x4 ProjView;
};
