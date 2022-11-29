//===================================================
//
// ライトの処理
// Author:Teruto Sato
//
//===================================================

//-----------------------
// インクルード
//-----------------------
#include "light.h"
#include "renderer.h"

//========================
// コンストラクタ
//========================
CLight::CLight()
{
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ライト方向のベクトル
}

//========================
// デストラクタ
//========================
CLight::~CLight()
{

}

//========================
// 初期化
//========================
void CLight::Init(LPDIRECT3DDEVICE9 pDevice)
{
	//ライトをクリアする
	ZeroMemory(&m_Light, sizeof(m_Light));

	//--------------------------------------------
	// 1つ目のライト
	//--------------------------------------------
	//ライトの種類を設定
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定(色)
	m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を決定
	m_vecDir = D3DXVECTOR3(0.4f, -0.4f, -0.8f);

	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_Light[0].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &m_Light[0]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);

	//--------------------------------------------
	// 2つ目のライト
	//--------------------------------------------
	//ライトの種類を設定
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定(色)
	m_Light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を決定
	m_vecDir = D3DXVECTOR3(0.4f, 0.8f, -0.4f);

	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_Light[1].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(1, &m_Light[1]);

	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);

	//--------------------------------------------
	// 3つ目のライト
	//--------------------------------------------
	//ライトの種類を設定
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定(色)
	m_Light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を決定
	m_vecDir = D3DXVECTOR3(-0.4f, -0.8f, 0.4f);

	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_Light[2].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(2, &m_Light[2]);

	//ライトを有効にする
	pDevice->LightEnable(2, TRUE);
}

//========================
// 終了
//========================
void CLight::Uninit()
{

}

//========================
// 更新
//========================
void CLight::Update()
{

}

//========================
// ライト方向の取得
//========================
D3DXVECTOR3 CLight::GetVecDir()
{
	return m_vecDir;
}