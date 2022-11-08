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
		char szBuf[MIN_STR];
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

	_uint m_iSelectIndex = 999;
	wstring m_iSelectPath;


private:

	void Show_Transform(_uint iIndex);
	void Show_Shader(_uint iIndex);

	void Show_TextureWindow(_uint iIndex);

private:
	void Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void Show_TreeTexture(TREE_DATA& tTree, _uint iIndex);
};

END