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


	//�޽������̳ʸ��� PhysX �����
	//������
	//�浹 ������ ��

private:
	vector<PxRigidDynamic*>	m_vecConvex;

private:
	HRESULT	SetUp_ConvexMeshes();
};

END