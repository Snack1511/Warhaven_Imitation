#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CUI_Object;

class CWindow_UI : public CImGui_Window
{
	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

public:
	struct UI_Object
	{
		CUI_Object* pUI = nullptr;
		_bool bSelected = false;
	};

private:
	CWindow_UI();
	virtual ~CWindow_UI();

public:
	static CWindow_UI* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	TREE_DATA	m_TextureRootNode;

	vector<UI_Object> m_vecUI;

	_uint m_iSelectIndex = 9999;
	wstring m_iSelectPath;

	_float m_fScale = 1.f;

private:
	CUI_Object* Add_UI();
	CUI_Object* Clone_UI();

	void Show_UIList();
	void Show_Inspector(CUI_Object* pUI);

	void Set_Object_Info(CUI_Object* pUI);

	void Show_Shader(_uint iIndex);

private:
	void Show_Texture();
	void Show_Color();

private:
	// 현재 리스트로 저장 중
	// 객체마다 하나씩 저장할 수 있게 함수로 빼놓기
	void Save_UI_Info();
	void Load_UI_Info();

private:
	void Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void Show_TreeTexture(TREE_DATA& tTree, _uint iIndex);
};

END