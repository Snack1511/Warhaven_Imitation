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
	CUI_Wrapper(const CUI_Wrapper& Prototype);
	virtual ~CUI_Wrapper();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	wstring	m_wstrName;
	CUI_Object* m_pUI = nullptr;
	map<wstring, CUI_Object*> m_pUIMap;

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