#pragma once

//#define HASHING(Type, Value) hash<Type>()(Value)

#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
class CCamera;
class CMesh;
END

BEGIN(Client)

class CTerrain;
class CDrawable_Terrain;
class CFunc_ObjectControl;
class CWindow_Map final
	: public CImGui_Window
{
public:
	enum TUPLEDATA { Tuple_CharPtr, Tuple_Bool, Tuple_Index };
	enum PICKINGTYPE {PICK_ANCHOR, PICK_OBJECT, PICK_CLONE, PICK_TERRAINVERT, PICK_TERRAINTEX, PICK_INSTANCEOBJECT, PICK_NONE};
	enum CAMERATYPE {CAM_RIGHT, CAM_UP, CAM_LOOK, CAM_FREE};
	enum PICKOUTTYPE {PICK_OUTPOS, PICK_OUTLOCALPOS, PICK_OUTNORM};
	struct MAPDATA
	{
		wstring TerrainDataPath;
		wstring ObjectDataPath;
		wstring InstanceDataPath;
		wstring NavDataPath;
		wstring LightDataPath;

	public:
		void Initialize();
		void Make_Path(string BasePath, string DataName);
		HRESULT SaveData(ofstream& rhsWriteFile, string BasePath, string DataName);
		HRESULT LoadData(ifstream& rhsReadFile, string FilePath);
		HRESULT SavePath(ofstream& rhsWriteFile, wstring strPath);
		HRESULT LoadPath(ifstream& rhsReadFile, wstring& strPath);

	};



	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

	typedef struct tagMapToolTerrainData
	{
		wstring strTileTexturePath;
		_uint iNumVerticesX;
		_uint iNumVerticesZ;
		_float3* pCurTerrainVertPos;
		_float4* pCurTerrainColor;
	public:
		typedef tuple<wstring, _uint, _uint, _float3*, _float4*> Terrain_TUPLE;
		enum TupleType
		{
			Tuple_TileTexture,
			Tuple_VerticesX,
			Tuple_VerticesZ,
			Tuple_TerrainPosPtr,
			Tuple_TerrainColorPtr,
		};
	public:
		void Initialize();
		void Make_Data(Terrain_TUPLE& tTerrainData);
	}MTT_DATA;

	typedef struct tagMapToolInstanceObjData
	{
		wstring strInstanceGorupName;
		wstring strMeshPath;
		_uint iInstanceNums;
		_float4 InstancePosition;
		VTXINSTANCE* ArrInstanceVTX;
	public:
		typedef tuple<wstring, wstring, _uint, _float4, VTXINSTANCE*> InstancingTuple;
		enum TupleType
		{
			Tuple_GroupName,
			Tuple_MeshPath,
			Tuple_InstanceNums,
			Tuple_Positon,
			Tuple_VTXArrayPtr,
		};
	public:
		void Initialize();
		void Make_Data(InstancingTuple& tTerrainData);
		void Save(ofstream& wirteFile);
		void Load(ifstream& readFile);
	}MTINSTANCE_DATA;

	typedef tuple<_float4, _float4, _float4> PICKDATA;
	typedef vector<tuple<char*, bool>> DataComboArr;

	const char* ArrObjectGroup[GROUP_END] =
	{
		"Static",
		"Default",
		"Enemy",
		"Decoration",
		"Prop",
		"UI",
		"Player",
		"Loading",
		"Effect",
	};
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
public:
	void Change_CurPickMode(PICKINGTYPE eType);
	_bool Is_CurPickMode(PICKINGTYPE eType);
	_bool Is_PickTerrain() { return m_bTerrainPick; }
	PICKDATA Get_PickData();
	void SetUp_PickMesh(CMesh* pMesh);
protected:

protected:

private:
	typedef struct tagMapToolObjectData MTO_DATA;

	typedef tuple<char*, bool> SelectComboData;
private:
	HRESULT Functions_Maptool();
#pragma region Private 파일컨트롤
private:
	void Func_FileControl();
	void Ready_FileArray();
	void Ready_ObjectGroupID();    

	void Update_FileArray();
	void SetUp_FilePath(string& strFilePath, char* szData, string strExt = "");
	void SetUp_CurPickingGroup();

#pragma endregion


#pragma region Private 라이트컨트롤
private:
	void Func_LightControl();
	void Ready_LightType();

	void Add_Light();
	void Delete_Light();
	void Clone_Light();

	void Set_LightTag(string strTag);
	void Set_LightType();
	void Set_LightPos(float* PosArr);
	void Set_LightOffset(float* OffsetArr);
	void Set_LightDir(float* DirArr);
	void Set_LightRange(float fRange);
	void Set_LightRandomRange(float fRandomRange);
	void Set_LightDifColor(float* ColorValue);
	void Set_LightAmbColor(float* ColorValue);
	void Set_LightSpecColor(float* ColorValue);
	void Set_LightOption();

	void Update_Light();


#pragma endregion

#pragma region Priavate 파일 입출력 함수
private:
	void Save_MapData(string BasePath, string SaveName);
	void Load_MapData(string FilePath);

	void Save_TerrainData(string BasePath, string SaveName);
	void Load_TerrainData(string FilePath);

	void Save_InstanceData(string BasePath, string SaveName);
	void Save_SpliteData(string BasePath, string SaveName);
	void Save_MergeData(string BasePath, string SaveName);
	void Load_InstanceData(string FilePath);


	void Save_NavGroup(string BasePath, string SaveName);
	void Load_NavGroup(string FilePath);

	void Save_LightGroup(string BasePath, string SaveName);
	void Load_LightGroup(string FilePath);
#pragma endregion

#pragma region Private 터레인 컨트롤
private:
	void Ready_TerrainBrushType();
	HRESULT Disable_DefaultTerrain();
	void Func_TerrainControl();
	void Generate_Terrain();

	void Change_TileTexture();
	void Increase_Height();

	void Edit_TerrainVert();
	void Edit_TerrainTex();
	void Edit_TerrainData();
	void Add_Brush(const char* BrushName);

	list<_uint> Select_Vertices();
	_bool Check_InBrush(_float3* CompVert, _float4 vCenterPos);
	_float3 Easing_Vertices(_float3* pArrVertPos);

#pragma endregion

private:
	void Func_InstanceObjectControl();
	void Make_InstanceObject();
	void Delete_InstanceObject();
	void Clear_InstanceGroup();
	void Merge_Instance();
	void Split_Instance();
private:
	_bool Calculate_Pick();


#pragma region Private 기타 함수
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
	_bool		Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
	void		Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
public:
	void		DebugData(const char* szTitleName, string& strData, ImVec4 Color = ImVec4(1.f, 1.f, 1.f, 1.f));
	string		CutOut_Ext(string& Origin, string& Ext);
	void		Show_TreeData(void* tTree, function<void(void*)> SelectFunction);
	void		Read_Folder_ForTree(const char* pFolderPath, TREE_DATA& tRootTree);
private:
	list<string>Read_Folder_ToStringList(const char* pFolderPath);
	void		Routine_InstanceMeshSelect(void* tTreeNode);

	HRESULT		SetUp_Cameras();
	void		Select_Camera();
	void		Change_Camera(_int Index);

	void		Set_BrushInform();
	_bool		Picked_VertList(list<_uint>& VertsList, _float4 vPosition, _float4& OutPos, _float4& OutNormal);
	void		EmptyFunction() {}
#pragma endregion

#pragma region Value 파일컨트롤
private:
	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
	string m_strPath = string("");

	_int m_SelectObjectGroupIDIndex = 0;
	vector<tuple<char*, bool>> m_arrObjectGroupId;
#pragma endregion

#pragma region Value 터레인컨트롤
private:
	CTerrain* m_pDefaultTerrain = nullptr;
	CDrawable_Terrain* m_pCurTerrain = nullptr;
	MTT_DATA m_CurTerrainData;
	vector<tuple<char*, bool>> m_arrBrushType;
	vector<tuple<char*, bool>> m_arrTileTextureName;
	_int m_iCurSelectTerrainBrush = 0;
	_uint3 m_i3PickedIndex = _uint3(0, 0, 0);
	PICKDATA m_OutDatas;
	_float m_fBrushSize = 1.f;
	_float m_fBrushWeight = 1.f;

	//Diffuse, Normal
	_float4 m_vTileTypeFlag = _float4(0.f, 0.f, 1.f, 1.f);

#pragma endregion

private:
	typedef map<size_t, vector<tuple<MTINSTANCE_DATA, CGameObject*>>> INSTANCEGROUPING;
	typedef vector<tuple<MTINSTANCE_DATA, CGameObject*>> INSTANCEVECTOR;
	typedef tuple<MTINSTANCE_DATA, CGameObject*> INSTANCETUPLE;
	_int m_iDrawInstanceObjectNums = 0;
	_float m_fRandomRatio = 0.f;
	string m_strCurSelectInstanceMeshPath = "";
	string m_strCurSelectInstanceMeshName = "";
	TREE_DATA m_tInstanceMeshDataRoot;
	CGameObject* m_pCurSelectInstanceObject = nullptr;
	vector<string> m_strArrInstanceMeshName;
	map<size_t, vector<tuple<MTINSTANCE_DATA, CGameObject*>>> m_InstanceMap;
	map<size_t, CGameObject*> m_MergeObjects;
	_int m_iCurSelectInstanceNameIndex = 0;
	_int m_iCurSelectInstanceObjectIndex = 0;

#pragma region Value 라이트컨트롤
private:
	vector<tuple<char*, bool>> m_arrLightTypeCombo;
	vector<tuple<string, LIGHTDESC>> m_LightDescs;
	_uint m_iCurSelectLight = 0;
	_bool m_bLightControl = false;
	_int m_iLightTypeIndex = 0;
	_int m_iLightPadding = -1;
#pragma endregion

	public:
		TREE_DATA* Get_MeshRoot() {
			return &m_MeshRootNode;
		}
#pragma region 기타 필요변수
public:
	_bool Is_HoveredWindow() { return m_bHoverWindow; }

private:

	TREE_DATA			m_MeshRootNode;

	list<CGameObject*>* m_pCurObjectList = nullptr;

	_bool m_bHoverWindow = false;
	_bool m_TerrainWireFrame = false;

	PICKINGTYPE m_ePickingType = PICK_NONE;

	string m_curSelectBGTextureName;
	string m_curSelectSourTextureName;
	string m_curSelectDestTextureName;
	_int m_iSourIndex = 0;
	_int m_iDestIndex = 0;
	_int m_iBGIndex = 0;

	vector<tuple<wstring, CCamera*, _float4>> m_ArrCams;
	_float4x4 m_matTerrain;
private:
	CFunc_ObjectControl* m_pObjectController = nullptr;
	_int m_iTileIndexFlag = 0;
	_bool m_bTerrainPick = true;
	CMesh* m_PickTargetMesh = nullptr;
#pragma endregion


};
END
