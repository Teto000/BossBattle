//===================================================
//
// カメラの処理
// Author:Teruto Sato
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "title_camera.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debug_proc.h"
#include "utility.h"

//===========================
// コンストラクタ
//===========================
CTitleCamera::CTitleCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TSPEED = 0.0f;
	m_bLockOn = true;

	//カメラの振動
	m_nQuakeFreamCount = 0;
	m_fQuakeMagnitude = 0.0f;
}

//===========================
// デストラクタ
//===========================
CTitleCamera::~CTitleCamera()
{

}

//========================
// カメラの初期化処理
//========================
void CTitleCamera::Init(void)
{
	//---------------------------------
	// 視点・注視点・上方向を設定する
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 1000.0f, -1000.0f);
	m_posR = D3DXVECTOR3(0.0f, 800.0f, 200.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_TSPEED = 0.02f;		//カメラの回転速度(Turn)
}

//========================
// カメラの終了処理
//========================
void CTitleCamera::Uninit(void)
{

}

//========================
// カメラの更新処理
//========================
void CTitleCamera::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//回転
	m_rot.y -= m_TSPEED / 3.0f;

	// 左右の角度の正規化
	m_rot.y = CUtility::GetNorRot(m_rot.y);

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//行列に回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//行列に移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ワールド変換行列を使ってposV,posRを求める
	D3DXVec3TransformCoord(&m_worldPosR, &m_posR, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldPosV, &m_posV, &m_mtxWorld);
}

//========================
// カメラの設定処理
//========================
void CTitleCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//--------------------------------
	// カメラ情報の設定
	//--------------------------------
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_worldPosV,		//視点
					   &m_worldPosR,		//注視点
					   &m_vecU);	//上の向き

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							   D3DXToRadian(45.0f),	//視野角
							   (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//アスペクト比
							   10.0f,		//near
							   100000.0f);	//far

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}