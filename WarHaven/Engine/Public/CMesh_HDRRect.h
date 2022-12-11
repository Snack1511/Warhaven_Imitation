#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_HDRRect
	: public CMesh
{
	DECLARE_PROTOTYPE(CMesh_HDRRect);

protected:
	CMesh_HDRRect(_uint iGroupIDX);
	virtual ~CMesh_HDRRect();

	friend class CRender_Manager;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	static CMesh_HDRRect* Create(_uint iGroupIdx);

public:
	// CMesh을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};

END
