#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CUI_Object;
class CTexture;
END

BEGIN(Client)

class CWindow_UI : public CImGui_Window
{
	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

private:
	CWindow_UI() = default;
	virtual ~CWindow_UI() = default;

public:
	static CWindow_UI* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	TREE_DATA	m_TextureRootNode;

	CTexture* m_pTexture;

private:
	void Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);

	void Show_TreeTexture(TREE_DATA& tTree);

	const _tchar* StringToChar(string string);
};

END