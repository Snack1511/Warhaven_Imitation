#pragma once
#include "CImGui_Window.h"

BEGIN(Client)
class CWindow_Map final
	: public CImGui_Window
{
private:
	CWindow_Map();
	virtual ~CWindow_Map();

public:
	static CWindow_Map* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
protected:
private:
	void Ready_FileArray(const char* lpFolderPath, vector<tuple<char*, bool>>& szDataArr, bool bClear = true);
	void Update_FileArray(const char* lpFolderPath, vector<tuple<char*, bool>>& szDataArr);
	_bool Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);

private:
	vector<tuple<const char*, bool>> m_arrFilesCombo;
	string m_strPath;
};
END
