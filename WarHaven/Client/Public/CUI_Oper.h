#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Oper : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Oper);
	DECLARE_GAMEOBJECT(CUI_Oper);

private:
	CUI_Oper();
	virtual ~CUI_Oper();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

private:	// ����ȸ�� ���
	enum OperBG { OB_BG, OB_Smoke, OB_Black, OB_End };
	CUI_Object* m_pOperBG[OB_End];

private:
	void Create_OperBG();

private:	// ����ȸ�� ������
	CUI_Object* m_pOperProfile = nullptr;
	CUI_Object* m_pArrOperProfile[4];

private:
	void Create_OperProfile();

private:	

private:
	virtual void My_Tick();
};

END