#pragma once
#include "Client_Defines.h"
#include "CUI.h"


BEGIN(Client)
class CUI_Animation : public CUI
{
	DECLARE_GAMEOBJECT(CUI_Animation);
	DECLARE_PROTOTYPE(CUI_Animation);

public:
	struct ANIMINFO
	{
		_bool		bPlayOnce = false;
		_bool		bBlackBackGround = false;
		_uint		iWidthSize = 1;
		_uint		iHeightSize = 1;
		_float		fDuration = 0.f;

		_float2		vPos;
		_float2		vScale;
	};

private:
	CUI_Animation();
	virtual ~CUI_Animation();

public:
	void Trun_UIAnimation(ANIMINFO tAniminfo);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start();

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();

public:
	virtual void My_Tick();

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName) override;

private:
	ANIMINFO	m_tAniminfo;
	_float		m_fTimeAcc = 0.f;
	_float		m_fRowX = 0.f;
	_float		m_fColY = 0.f;

	_bool		m_bPlayOnce = false;
	_bool		m_bBlackBackGround = false;
	_uint		m_iWidthSize = 1;
	_uint		m_iHeightSize = 1;
	_float		m_fDuration = 0.f;



};

END
