#pragma once

namespace Client
{
	static void Call_MsgBox_Index(const _tchar* strMessage, const _uint& iIdx)
	{
#ifdef _DEBUG

		wstring strFullMsg = strMessage;
		strFullMsg += L" (Index : ";
		strFullMsg += to_wstring(iIdx);
		strFullMsg += L")";

		Engine::Call_MsgBox(strFullMsg.c_str());

#endif // _DEBUG
	}

}