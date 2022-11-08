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
	void Create_UI();

	CUI_Object* Add_UI();
	CUI_Object* Clone_UI();

	void Show_UIList();

	void Show_Inspector(CUI_Object* pUI);

	void Set_Object_Info(CUI_Object* pUI);

	void Show_Shader(_uint iIndex);

	void Show_Texture();
	void Show_Color();

	void UI_IO();

	void Save_UI_List();
	void Save_UI_Info(CUI_Object* pUI);

	void Load_UI_List();
	void Load_UI_Info(CUI_Object* pUI);

	void Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void Show_TreeTexture(TREE_DATA& tTree, _uint iIndex);
};

END