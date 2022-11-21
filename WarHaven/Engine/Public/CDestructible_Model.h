#pragma once
#include "Model.h"

BEGIN(Engine)


class CDestructible_Model
	: public CModel
{
	DECLARE_PROTOTYPE(CDestructible_Model)

private:
	CDestructible_Model();
	virtual ~CDestructible_Model();
	
public:
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix);


	//메쉬컨테이너마다 PhysX 만들기
	//꺼놓고
	//충돌 들어오면 켜

private:
	vector<PxRigidDynamic*>	m_vecConvex;

private:
	HRESULT	SetUp_ConvexMeshes();
};

END