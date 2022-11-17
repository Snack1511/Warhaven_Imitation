#pragma once



namespace Engine
{

	static _int random(_int _min, _int _max)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<_int> disx(_min, _max);

		return disx(gen);
	}

	static _float frandom(_float _min, _float _max)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<_float> disx(_min, _max);

		return disx(gen);
	}

	static void Call_MsgBox(const _tchar* strMessage)
	{
#ifdef _DEBUG
		MessageBox(0, strMessage, TEXT("System Error"), MB_OK);
#endif // DEBUG

	}

	static	size_t	Convert_ToHash(wstring strTemp)
	{
		hash<wstring>	strHash;
		return strHash(strTemp);
	}

	static	size_t	Convert_ToHash(string strTemp)
	{
		hash<string>	strHash;
		return strHash(strTemp);
	}

	static	size_t	Convert_ToHash(const _tchar* strTemp)
	{
		hash<wstring>	strHash;
		return strHash(strTemp);
	}

	static string	CutPath_R(string strPath, string strKey)
	{
		int iFind;
		string strFileName;

		iFind = (int)strPath.rfind(strKey);
		strFileName = strPath.substr(0, iFind);

		return strFileName;
	}

	static wstring	CutPath_R(wstring strPath, wstring strKey)
	{
		int iFind;
		wstring strFileName;

		iFind = (int)strPath.rfind(strKey);
		strFileName = strPath.substr(0, iFind);

		return strFileName;
	}



	template<typename T>
	void Safe_Delete(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)
		{
			dwRefCnt = Temp->Release();

			if(0 == dwRefCnt)
				Temp = nullptr;
		}

		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_release(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)
		{
			Temp->release();
		}
		Temp = nullptr;


		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_AddRef(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)	
			dwRefCnt = Temp->AddRef();		

		return dwRefCnt;
	}


	template<typename T>
	void Safe_Delete_Array(T& Temp)
	{
		if (Temp)
		{
			delete[] Temp;
			Temp = nullptr;
		}
	}

}
