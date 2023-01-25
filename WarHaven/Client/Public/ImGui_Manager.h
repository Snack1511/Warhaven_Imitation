#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CImGui_Window;

class CImGui_Manager
{
public:
	enum IMGUI_WINDOW_TYPE
	{
		IMGUI_DEFAULT,
		IMGUI_SELECT,
		IMGUI_UI,
		IMGUI_MAPTOOL,
		IMGUI_EFFECT,
		IMGUI_PhysX,
		IMGUI_POSITION,
		IMGUI_LEVEL,
		IMGUI_TILE,
		IMGUI_PATH,
		IMGUI_AI,
		IMGUI_SHADER,
		IMGUI_END
	};

	DECLARE_SINGLETON(CImGui_Manager)

private:
	CImGui_Manager();
	~CImGui_Manager();

public:
	void	Enable_Window(IMGUI_WINDOW_TYPE eType, _bool bEnable);
	void	Turn_Window(IMGUI_WINDOW_TYPE eType);
public:
	void Push_KorFont();
	void Pop_Font();
public:
	void		On_ToolTip(string strContext, string strToolTip = "(?)", _bool bSameLine = true);
public:
	HRESULT Initialize();
	void	Tick();
	HRESULT Render();

private:
	CImGui_Window* m_arrWindows[IMGUI_END];

	_bool m_bIsInit = false;
private:
	ImFont* m_pKorFont = nullptr;
private:
	void Release();
};

END