#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Client)
class CScript_FixedCam :
    public CScriptable
{
	DECLARE_PROTOTYPE(CScript_FixedCam);
private:
	CScript_FixedCam(_uint iGroupIdx);
	virtual ~CScript_FixedCam();

public:
	static CScript_FixedCam* Create(_uint iGroupIdx);

public:
	void	Enter_UnityLevel() { m_bUnityLevel = true; }

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
public:
	void Regist_KeyInput(KEY eKeyInput, function<void(_float4&)> function);

private:
	_bool	m_bTurnOn = true;
	_bool	m_bUnityLevel = false;
	function<void(_float4&)> m_W_InputFunction;
	function<void(_float4&)> m_A_InputFunction;
	function<void(_float4&)> m_S_InputFunction;
	function<void(_float4&)> m_D_InputFunction;
private:
	void	Key_Input();
};
END
