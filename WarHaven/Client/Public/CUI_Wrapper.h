#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CUI_Object;

class CUI_Wrapper abstract : public CGameObject
{
protected:
	CUI_Wrapper();
	virtual ~CUI_Wrapper();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void	On_MouseInEvent(const _uint& iEventNum);
	virtual void	On_MouseExitEvent(const _uint& iEventNum);
	virtual void	On_ButtonClickEvent(const _uint& iEventNum);


protected:
	wstring	m_wstrName;
	CUI_Object* m_pUI = nullptr;
	map<wstring, CUI_Object*> m_pUIMap;

	_float m_fAccTime = 0.f;
	_float m_fMinValue = 0.01f;

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	void Read_UI(string key);
	void Read_Texture(string strFolderPath, string key);
	void Read_Texture(CUI_Object* pUI, string strFolderPath, string key);

	CUI_Object* Load_UI(string m_wstrName);

private:
	void Load_UI(wstring m_wstrName);
};

END