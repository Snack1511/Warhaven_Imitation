#pragma once
#include "CUI_Wrapper.h"
class CUI_Main : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Main);
	DECLARE_GAMEOBJECT(CUI_Main);

private:
	enum MainWindow { MW_Play,/* MW_Barracks,*/ MW_Profile, MW_End };

private:
	CUI_Main();
	virtual ~CUI_Main();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void On_PointEnter_TopBtn(const _uint& iEventNum);
	virtual void On_PointExit_TopBtn(const _uint& iEventNum);
	virtual void On_PointDown_TopBtn(const _uint& iEventNum);

	void Set_Shader_TopBtnEffect(CShader* pShader, const char* pConstName);



public:
	void SetActive_TopBtn(_bool value);
	void SetActive_PlayerNameText(_bool value);
	void SetActive_MainWindow(MainWindow eWindow);

private:
	enum TopBtn { TB_Play, /*TB_Barracks,*/ TB_Profile, TB_End };
	CUI_Object* m_pArrTopBtn[TB_End];

private:
	void Create_TopBtn();

private:
	CUI_Object* m_pTopBtnEffect = nullptr;

private:
	void Create_TopBtnEffect();

private:
	enum PlayerInfo { PI_Level, PI_Name, PI_End };
	CUI_Object* m_pPlayerInfo[PI_End];

	_float4 m_vColorWhite = _float4(1.f, 1.f, 1.f, 1.f);
	_float4 m_vColorGrey = _float4(0.5f, 0.5f, 0.5f, 1.f);

	_uint m_iCurEventNum = 0;
	_uint m_iPrvEnvetNum = 0;

private:
	void Create_PlayerNameText();

private:
	CUI_Wrapper* m_pMainWindow[MW_End];

	MainWindow m_eWindow = MainWindow::MW_Play;

private:
	void Create_MainWindow();

private:
	void Bind_Btn();
	void Bind_Shader();
};

