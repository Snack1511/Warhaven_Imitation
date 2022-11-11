#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
	enum UI_TYPE { BG, Port, Key, Effect, Type_End };

	DECLARE_PROTOTYPE(CUI_Portrait);
	DECLARE_GAMEOBJECT(CUI_Portrait);

private:
	CUI_Portrait();
	CUI_Portrait(const CUI_Portrait& Prototype);
	virtual ~CUI_Portrait();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_ShaderEffect(CShader* pShader, const char* constName);

public:
	void Set_Portrait(_uint iIndex);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrPortraitUI[5][Type_End] = {};

	_float m_fEffectValue = 0.f;

private:
	void Enable_Portrait();

	void Set_Pass();
	void Bind_Shader();

	// ����� ��Ʈ�� ���鼭 ��, ��Ÿ��

	// ������ �� ���鼭 �ڱ�� ������ ���� ���̵��ϸ� ���ٴ���
};

END