#pragma once
#include "Component.h"
#include "Client_Defines.h"

#define	FADE_NONE	0
#define	FADE_TIME	0x01
#define	FADE_KEY	0x02
#define	FADE_COL	0x04
#define	FADE_TEMP	0x08

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CFader
	: public CComponent
{
public:
	enum FADE_STATE
	{
		FADEINREADY,
		FADEIN,
		FADEOUTREADY,
		FADEOUT,
	};

public:
	DECLARE_PROTOTYPE(CFader);

private:
	CFader(_uint iGroupIdx);
	~CFader();

public:
	static CFader* Create(COMPONENT_PIPELINE	ePipeLine, const FADEDESC& tFadeDesc);

public:
	FADEDESC& Get_FadeDesc() { return m_tFadeDesc; }
	FADE_STATE&	Get_FadeState() { return m_eState; }
	_float& Get_FastTime() { return m_fFastTime; }
	void	Set_FadeMessage() { m_bFadeMessage = true; }
	void	No_FadeMessage() { m_bFadeMessage = false; }

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName) override;

public:
	void			Force_KeyInput() { m_bKeyInput = true; }
	void			Re_FadeIn();
	void			Re_FadeOut();

public:
	// CComponent��(��) ���� ���
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDead()	override;

private:
	FADEDESC	m_tFadeDesc;
	FADE_STATE	m_eState = FADEINREADY;
	_float		m_fTimeAcc = 0.f;
	_float		m_fFastTime = 1.f;

	_float4		m_vTargetPos;
	_float		m_fOffsetX = 0.f;
	_float4		m_vOriginScale;
	_float		m_fOffsetScale = 1.f;

	_bool		m_bKeyInput = false;
	_bool		m_bFadeMessage = false;

private:
	void	FadeReady();
	void	FadeIn();
	void	FadeDelay();
	void	FadeOut();

	void	OnFadeStyle(_bool bFadeOut);

private:
	void	Change_State(FADE_STATE eState);
	void	OnFadeOut_Finish();

};
END
