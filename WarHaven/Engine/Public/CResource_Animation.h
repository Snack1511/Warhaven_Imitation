#pragma once
#include "CResource.h"

BEGIN(Engine)



class CResource_Animation
	: public CResource
{
private:
	CResource_Animation();
	virtual ~CResource_Animation();

public:
	static CResource_Animation* Create(ifstream* pReadFile, string strName);

public:
	ANIM_DESC& Get_AnimDesc() { return m_tAnimDesc; }

public:
	// CResource��(��) ���� ��ӵ�
	virtual HRESULT Initialize(ifstream* pReadFile, string strName) override;

private:
	ANIM_DESC	m_tAnimDesc;

};

END