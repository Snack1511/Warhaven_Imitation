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
	enum CLASS_TYPE
	{
		CLASS_WARRIOR,
		CLASS_SPEAR,
		CLASS_ARCHER,
		CLASS_PALADIN,
		CLASS_PRIEST,
		CLASS_ENGINEER,
		CLASS_FIONA,
		CLASS_QANDA,
		CLASS_HOEDT,
		CLASS_LANCER,
		CLASS_END
	};

	// 클래스 타입에 따라 포트레이트 크로스헤어를 활성화한다.
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

	CUI_Object* Load_UI(string m_wstrName);

private:
	void Load_UI(wstring m_wstrName);
};

END