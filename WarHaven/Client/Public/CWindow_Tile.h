#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;

END

BEGIN(Client)

class CDebugObject;
class CTileDebugger;

class CWindow_Tile final
	: public CImGui_Window
{
private:
	CWindow_Tile();
	virtual ~CWindow_Tile();

public:
	static CWindow_Tile* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	char				m_szFileName[MIN_STR] = {};
	_bool				m_bRenderTile = true;

private:
	vector<CTileDebugger*> m_vecTileDebugger;

private:
	_float			m_fStepY = 0.f;

private:
	void		On_Picking(_uint iLayerIndex, _float4 vPickedPos);

};
END
