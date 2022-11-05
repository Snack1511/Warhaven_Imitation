#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CFrustum_Manager final
{
	DECLARE_SINGLETON(CFrustum_Manager)

private:
	CFrustum_Manager();
	virtual ~CFrustum_Manager();

public:
	/* ������������ ����ü�� �����ϴ� ������ ���� ��� ���Ѵ�. */
	HRESULT Initialize();

	/*���� �������� ������ ����� ���Ѵ�. */
	void Update();

public:
	_bool isIn_Frustum_InWorldSpace(_vector vWorldPoint, _float fRange = 0.f);
	_bool isIn_Frustum_InLocalSpace(_vector vLocalPoint, _float fRange = 0.f);
	void Transform_ToLocalSpace(_matrix WorldMatrixInv);

private:
	_float4				m_vOriginalPoints[8];
	_vector				m_vWorldPoints[8];
	_float4				m_vWorldPlane[6];
	_float4				m_vLocalPlane[6];
};

END