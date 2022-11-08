#include "stdafx.h"
#include "CWindow_Effect.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"
#include "Model.h"
#include "MeshContainer.h"

#include "Functor.h"

#include "CDefault_Effect.h"

CWindow_Effect::CWindow_Effect()
{
}

CWindow_Effect::~CWindow_Effect()
{
}

CWindow_Effect* CWindow_Effect::Create()
{
	CWindow_Effect* pInstance = new CWindow_Effect;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Effect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Effect::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Effect).name(), ImVec2(400.f, 600.f), window_flags);

	m_MeshRootNode.strFolderPath = "../bin/resources/meshes";
	m_MeshRootNode.strFileName = "Effects";
	m_MeshRootNode.strFullPath = "../bin/resources/meshes/effects";
	Read_Folder("../bin/resources/meshes/effects/FBX", m_MeshRootNode);

	m_TextureRootNode.strFolderPath = "../bin/resources/textures";
	m_TextureRootNode.strFileName = "Effects";
	m_TextureRootNode.strFullPath = "../bin/resources/textures/effects";
	Read_Folder("../bin/resources/textures/effects", m_TextureRootNode);

	m_EffectRootNode.strFolderPath = "../bin";
	m_EffectRootNode.strFileName = "Effects";
	m_EffectRootNode.strFullPath = "../bin/Effects/";
	Read_Folder("../bin/effects", m_EffectRootNode);

	return S_OK;
}

void CWindow_Effect::Tick()
{
}

HRESULT CWindow_Effect::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	Show_MainList();

	if (m_iCurrentIdx != 9999)
	{
		if (m_vecEffects[m_iCurrentIdx].iEffectType == 0)
			Show_EffectTab();
		/*else
			Show_ParticleTab();*/
	}

	__super::End();

	return S_OK;
}

void CWindow_Effect::Show_MainList()
{
	//이펙트들 리스트박스
	if (ImGui::Button("Save_Effects"))
	{
		Save_CurEffect();
	}
	static _uint g_iEffectIndex = 0;
	if (ImGui::Button("Create Mesh Effect"))
	{
		CDefault_Effect* pEffect = CDefault_Effect::Create();

		EFFECT_ITEM tItem;
		tItem.bSelected = false;
		tItem.pEffect = pEffect;
		tItem.strName = "Default_Effect_";
		tItem.strName += to_string(g_iEffectIndex++);
		tItem.iEffectType = 0;

		m_vecEffects.push_back(tItem);

		CREATE_GAMEOBJECT(pEffect, GROUP_EFFECT);
	}

	ImGui::SameLine();

	/*if (ImGui::Button("Create Particle Effect"))
	{
		CRectEffects* pEffect = CRectEffects::Create(PLAYER->Get_Transform()->Get_World(WORLD_POS));

		EFFECT_ITEM tItem;
		tItem.bSelected = false;
		tItem.pEffect = pEffect;
		tItem.strName = "Default_Particle_";
		tItem.strName += to_string(g_iEffectIndex++);
		tItem.iEffectType = 1;
		m_vecEffects.push_back(tItem);

		CREATE_GAMEOBJECT(pEffect, GROUP_EFFECT);
	}*/

	/*static _bool	g_bCamMatrix = false;
	if (ImGui::RadioButton("CameraMatrix", g_bCamMatrix))
		g_bCamMatrix = !g_bCamMatrix;*/

	ImGui::SameLine();

	//if (ImGui::RadioButton("PlayerMatrix", !g_bCamMatrix))
	//	g_bCamMatrix = !g_bCamMatrix;

	/*ImGui::SameLine();
	static _bool g_bEnableAll = false;
	if (ImGui::RadioButton("ENABLE_ALL", g_bEnableAll))
		g_bEnableAll = !g_bEnableAll;

	if (ImGui::Button("EN / DIS ABLE_EFFECT(7)") || KEY(NUM7, TAP))
	{
		if (g_bEnableAll)
		{
			for (auto& elem : m_vecEffects)
			{

				if (elem.iEffectType == 0)
				{
					elem.pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS));
				}
				else
				{
					_float4x4 matTrans;
					if (g_bCamMatrix)
						matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
					else
						matTrans = PLAYER->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);

					elem.pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS), matTrans);
				}

			}
		}
		else
		{
			if (m_iCurrentIdx != 9999)
			{
				if (m_vecEffects[m_iCurrentIdx].pEffect->Is_Disable())
				{
					if (m_vecEffects[m_iCurrentIdx].iEffectType == 0)
					{
						m_vecEffects[m_iCurrentIdx].pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS));
					}
					else
					{
						_float4x4 matTrans;
						if (g_bCamMatrix)
							matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
						else
							matTrans = PLAYER->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);

						m_vecEffects[m_iCurrentIdx].pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS), matTrans);
					}



				}
				else
					DISABLE_GAMEOBJECT(m_vecEffects[m_iCurrentIdx].pEffect);

			}
		}


	}*/

	if (ImGui::Button("DELETE_ALL"))
	{
		m_vecEffects.clear();
		m_iCurrentIdx = 9999;
	}

	ImGui::SameLine();

	//if (ImGui::Button("RE-Initialize"))
	//{
	//	if (m_iCurrentIdx != 9999)
	//	{
	//		m_vecEffects[m_iCurrentIdx].pEffect->Re_Initialize();
	//	}
	//}


	if (ImGui::BeginListBox("CurrentEffects_List", ImVec2(360.f, 300.f)))
	{
		for (_uint i = 0; i < m_vecEffects.size(); ++i)
		{
			if (ImGui::Selectable(m_vecEffects[i].strName.c_str(), m_vecEffects[i].bSelected))
			{
				for (_uint j = 0; j < m_vecEffects.size(); ++j)
					m_vecEffects[j].bSelected = false;

				m_iCurrentIdx = i;
				m_vecEffects[i].bSelected = true;
			}

			if (m_vecEffects[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			if (m_vecEffects[i].pEffect->Is_Disable())
			{
				ImGui::SameLine(0.f, 70.f);
				ImGui::Text("disable");
			}
		}

		ImGui::EndListBox();
	}
	if (ImGui::CollapsingHeader(" - BINARY FILE - "))
	{
		if (ImGui::Button("Load_Effects"))
		{
			if (!m_CurSelectedEffectFileKey.empty())
			{
				Load_SelectedEffect();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Refresh"))
		{
			Read_Folder("../bin/effects", m_EffectRootNode);
		}

		if (ImGui::BeginListBox("Bin_Files_List", ImVec2(360.f, 300.f)))
		{
			Show_TreeData_Effect(m_EffectRootNode);

			ImGui::EndListBox();
		}
	}

}

void CWindow_Effect::Show_EffectTab()
{
	/* Effect Info */
	if (m_iCurrentIdx != 9999)
	{
		CEffect* pCurEffect = m_vecEffects[m_iCurrentIdx].pEffect;
		ImGui::Text("Selected Effect Name : ");
		ImGui::SameLine();
		if (ImGui::InputText("EFFECT_NAME", m_szCurEffectName, sizeof(m_szCurEffectName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_vecEffects[m_iCurrentIdx].strName = m_szCurEffectName;
		}

		if (ImGui::CollapsingHeader(" - MESH - "))
		{
			if (ImGui::Button("Change_Mesh(C)") || KEY(C, TAP))
			{
				if (!m_CurSelectedMeshFilePath.empty())
				{
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					Change_Model(pModelCom);

				}
			}

			if (ImGui::BeginListBox("FBX_Files_List", ImVec2(360.f, 300.f)))
			{
				Show_TreeData(m_MeshRootNode);

				ImGui::EndListBox();
			}
		}
		if (ImGui::CollapsingHeader(" - TEXTURE - "))
		{
			if (ImGui::Button("Bind_MaskMap(V)") || KEY(V, TAP))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrMaskMapFilePath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					pModelCom->Get_Materials().front().second.pTextures[1]->Add_Texture(
						CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());
					pModelCom->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(

						pModelCom->Get_Materials().front().second.pTextures[1]->Get_TextureSize() - 1
					);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Bind_ColorMap(B)") || KEY(B, TAP))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrColorMapFilePath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					CTexture* pTexture = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];

					pTexture->Add_Texture(CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());

					pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
				}
			}

			if (ImGui::BeginListBox("Texture_Files_List", ImVec2(360.f, 300.f)))
			{
				Show_TreeData_Texture(m_TextureRootNode);

				ImGui::EndListBox();
			}
		}

		if (ImGui::CollapsingHeader(" - SHADER PASS -"))
		{
			static _bool	bSelect[VTXEFFECT_PASS_END] = {};
			memset(bSelect, 0, sizeof(_bool) * VTXEFFECT_PASS_END);
			bSelect[pCurEffect->m_eEffectPassType] = true;
			CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);

			if (ImGui::Selectable("DEFAULT", &bSelect[VTXEFFECT_PASS_DEFAULT]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_DEFAULT;
			if (ImGui::Selectable("LINEX", &bSelect[VTXEFFECT_PASS_LINEX]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_LINEX;
			if (ImGui::Selectable("LINEY", &bSelect[VTXEFFECT_PASS_LINEY]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_LINEY;
			if (ImGui::Selectable("TEXT", &bSelect[VTXEFFECT_PASS_TEXT]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_TEXT;
			if (ImGui::Selectable("CARTOON", &bSelect[VTXEFFECT_PASS_CARTOON]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_CARTOON;
			if (ImGui::Selectable("DISTORTION", &bSelect[VTXEFFECT_PASS_DISTORTION]))
			{
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_DISTORTION;
				GET_COMPONENT_FROM(pCurEffect, CRenderer)->Set_RenderGroup(RENDER_DISTORTION);

			}
			if (ImGui::Selectable("NOISE", &bSelect[VTXEFFECT_PASS_NOISE]))
				pCurEffect->m_eEffectPassType = VTXEFFECT_PASS_NOISE;

			pModelCom->Set_ShaderPassToAll(pCurEffect->m_eEffectPassType);

		}
		if (ImGui::CollapsingHeader(" - TRANSLATION MATRIX - "))
		{
			static _float	fTurnDegrees[3] = {};
			if (ImGui::InputFloat3("Turn Degrees", fTurnDegrees, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f)
					* XMMatrixRotationX(XMConvertToRadians(fTurnDegrees[0]))
					* XMMatrixRotationY(XMConvertToRadians(fTurnDegrees[1]))
					* XMMatrixRotationZ(XMConvertToRadians(fTurnDegrees[2]));

				//모델 다시 로드
				CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
				Change_Model(pModelCom, pCurEffect->m_matTrans);
			}
		}
		if (ImGui::CollapsingHeader(" - EFFECT TYPE FLAG -"))
		{
			const static _uint iFlagCnt = 4;
			_bool	bFlagSelect[iFlagCnt] = {};

			if (pCurEffect->m_bEffectFlag & EFFECT_FOLLOWTARGET)
				bFlagSelect[0] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_TARGETPOS)
				bFlagSelect[1] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_COLLIDER)
				bFlagSelect[2] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_BILLBOARD)
				bFlagSelect[3] = true;

			if (ImGui::Selectable("FOLLOW_TARGET", &bFlagSelect[0]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_FOLLOWTARGET)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_FOLLOWTARGET;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_FOLLOWTARGET;
				}
			}

			if (ImGui::Selectable("GO_TARGETPOS", &bFlagSelect[1]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_TARGETPOS)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_TARGETPOS;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_TARGETPOS;
				}
			}

			if (ImGui::Selectable("COLLIDER", &bFlagSelect[2]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_COLLIDER)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_COLLIDER;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_COLLIDER;
				}
			}

			if (ImGui::Selectable("BILLBOARD", &bFlagSelect[3]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_BILLBOARD)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_BILLBOARD;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_BILLBOARD;
				}
			}

		}
		if (ImGui::CollapsingHeader(" - EFFECT DISALBE TYPE -"))
		{
			static _bool	bSelect[CEffect::DISABLE_END] = {};

			if (ImGui::Selectable("FADE", &bSelect[CEffect::FADE]))
			{
				pCurEffect->m_eDisableType = CEffect::FADE;
			}
			if (ImGui::Selectable("UV", &bSelect[CEffect::UV]))
			{
				pCurEffect->m_eDisableType = CEffect::UV;
			}
			if (ImGui::Selectable("WALL", &bSelect[CEffect::WALL]))
			{
				pCurEffect->m_eDisableType = CEffect::WALL;
			}
			if (ImGui::Selectable("NONE", &bSelect[CEffect::NONE]))
			{
				pCurEffect->m_eDisableType = CEffect::NONE;
			}

			memset(bSelect, 0, sizeof(_bool) * CEffect::DISABLE_END);
			bSelect[pCurEffect->m_eDisableType] = true;
		}
		if (ImGui::CollapsingHeader(" - EFFECT SHADER FLAG -"))
		{
			const static _uint iShFlagCnt = 3;
			_bool	bShFlagSelect[iShFlagCnt] = {};

			if (pCurEffect->m_vEffectFlag.x > 0.99f)
			{
				bShFlagSelect[0] = true;
			}
			else
			{
				bShFlagSelect[0] = false;
			}

			if (pCurEffect->m_vEffectFlag.y > 0.99f)
			{
				bShFlagSelect[1] = true;
			}
			else
			{
				bShFlagSelect[1] = false;
			}

			if (pCurEffect->m_vEffectFlag.w < 1.f)
			{
				bShFlagSelect[2] = false;
			}
			else
			{
				bShFlagSelect[2] = true;
			}

			/*if (pCurEffect->m_vEffectFlag.z > 0.99f)
			{
				bShFlagSelect[2] = true;
			}
			else
			{
				bShFlagSelect[2] = false;
			}*/

			if (ImGui::Selectable("GLOW", &bShFlagSelect[0]))
			{
				if (pCurEffect->m_vEffectFlag.x > 0.99f)
				{
					pCurEffect->m_vEffectFlag.x = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.x = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);
			}

			if (ImGui::Selectable("BLOOM", &bShFlagSelect[1]))
			{
				if (pCurEffect->m_vEffectFlag.y > 0.99f)
				{
					pCurEffect->m_vEffectFlag.y = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.y = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

			}

			if (ImGui::Selectable("BLUR", &bShFlagSelect[2]))
			{
				if (pCurEffect->m_vEffectFlag.w > 0.99f)
				{
					pCurEffect->m_vEffectFlag.w = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.w = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

			}

			/*	if (ImGui::Selectable("DISTORTION", &bShFlagSelect[2]))
				{
					if (pCurEffect->m_vEffectFlag.z > 0.99f)
					{
						pCurEffect->m_vEffectFlag.z = 0.95f;
					}
					else
					{
						pCurEffect->m_vEffectFlag.z = 1.f;
					}

					GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

				}*/

		}
		if (ImGui::CollapsingHeader(" - GLOW VECTOR "))
		{
			_float vGlowFlags[4] = { pCurEffect->m_vGlowFlag.x, pCurEffect->m_vGlowFlag.y, pCurEffect->m_vGlowFlag.z, pCurEffect->m_vGlowFlag.w };
			memcpy(vGlowFlags, &pCurEffect->m_vGlowFlag, sizeof(_float4));

			if (ImGui::InputFloat4("vGlowFlag", vGlowFlags, "%.2f"))
			{
				memcpy(&pCurEffect->m_vGlowFlag, vGlowFlags, sizeof(_float4));
			}
		}
		if (ImGui::CollapsingHeader(" - VARIABLES "))
		{
			_float	vUVSpeed[2] = { pCurEffect->m_fUVSpeedX , pCurEffect->m_fUVSpeedY };

			if (ImGui::InputFloat2("UV_SPEED XY", vUVSpeed, "%.2f"))
			{
				pCurEffect->m_fUVSpeedX = vUVSpeed[0];
				pCurEffect->m_fUVSpeedY = vUVSpeed[1];
			}

			if (ImGui::InputFloat("TURN_SPEED", &pCurEffect->m_fTurnSpeed, 0.1f))
			{
			}

			_float	vOffsetPos[3] = { pCurEffect->m_vOffsetPos.x, pCurEffect->m_vOffsetPos.y, pCurEffect->m_vOffsetPos.z };
			if (ImGui::InputFloat3("OFFSET POS", vOffsetPos, "%.2f"))
			{
				pCurEffect->m_vOffsetPos = _float4(vOffsetPos[0], vOffsetPos[1], vOffsetPos[2], 1.f);
			}

			_float	fTurnDir[3] = { pCurEffect->m_vTurnDir.x, pCurEffect->m_vTurnDir.y, pCurEffect->m_vTurnDir.z };
			if (ImGui::InputFloat3("Turn Direction", fTurnDir, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vTurnDir = _float4(fTurnDir[0], fTurnDir[1], fTurnDir[2], 0.f);
				pCurEffect->m_vTurnDir.Normalize();
			}

			/* FADE */
			if (ImGui::InputFloat("FadeIn StartTime", &pCurEffect->m_fFadeInStartTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeIn Time", &pCurEffect->m_fFadeInTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeOut StartTime", &pCurEffect->m_fFadeOutStartTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeOut Time", &pCurEffect->m_fFadeOutTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("Target Alpha", &pCurEffect->m_fTargetAlpha, 0.1f))
			{
			}

			/* Scale*/
			_float	vStartScale[3] = { pCurEffect->m_vStartScale.x , pCurEffect->m_vStartScale.y, pCurEffect->m_vStartScale.z };
			_float	vFadeInTargetScale[3] = { pCurEffect->m_vFadeInTargetScale.x, pCurEffect->m_vFadeInTargetScale.y, pCurEffect->m_vFadeInTargetScale.z };
			_float	vFadeOutTargetScale[3] = { pCurEffect->m_vFadeOutTargetScale.x, pCurEffect->m_vFadeOutTargetScale.y, pCurEffect->m_vFadeOutTargetScale.z };

			if (ImGui::InputFloat3("vStartScale", vStartScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vStartScale.x = vStartScale[0];
				pCurEffect->m_vStartScale.y = vStartScale[1];
				pCurEffect->m_vStartScale.z = vStartScale[2];
			}

			if (ImGui::InputFloat3("vFadeInTargetScale", vFadeInTargetScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vFadeInTargetScale.x = vFadeInTargetScale[0];
				pCurEffect->m_vFadeInTargetScale.y = vFadeInTargetScale[1];
				pCurEffect->m_vFadeInTargetScale.z = vFadeInTargetScale[2];
			}

			if (ImGui::InputFloat3("vFadeOutTargetScale", vFadeOutTargetScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vFadeOutTargetScale.x = vFadeOutTargetScale[0];
				pCurEffect->m_vFadeOutTargetScale.y = vFadeOutTargetScale[1];
				pCurEffect->m_vFadeOutTargetScale.z = vFadeOutTargetScale[2];
			}


		}


	}


}
