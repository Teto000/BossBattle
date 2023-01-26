//===================================================
//
// カメラの処理
// Author:Teruto Sato
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "camera.h"
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
CCamera::CCamera()
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
CCamera::~CCamera()
{

}

//========================
// カメラの初期化処理
//========================
void CCamera::Init(void)
{
	//---------------------------------
	// 視点・注視点・上方向を設定する
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_TSPEED = 0.02f;		//カメラの回転速度(Turn)
}

//========================
// カメラの終了処理
//========================
void CCamera::Uninit(void)
{

}

//========================
// カメラの更新処理
//========================
void CCamera::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//回転
	Turn();

	//ロックオン状態の切り替え
	if (CInputKeyboard::Trigger(DIK_SPACE))
	{
		//ロックオン状態を切り替え
		m_bLockOn = !m_bLockOn;
	}

	if (CGame::GetFinish())
	{//終了フラグが立っているなら
		//ロックオンを解除
		m_bLockOn = false;
	}

	//----------------------------------------
	// 行列を使ったカメラ制御
	//----------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//行列に回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//行列に移動を反映
	D3DXMatrixTranslation(&mtxTrans, playerPos.x, playerPos.y, playerPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ワールド変換行列を使ってposV,posRを求める
	D3DXVec3TransformCoord(&m_posRDest, &m_posR, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_posVDest, &m_posV, &m_mtxWorld);

	//----------------------------------------
	// ロックオン処理
	//----------------------------------------
	//ロックオン位置
	D3DXVECTOR3 lockOnPos;
	if (CGame::GetEnemy())
	{//敵がnullじゃないなら
		lockOnPos = CGame::GetEnemy()->GetPosition();
	}

	if (m_bLockOn)
	{//ロックオン状態なら
		//注視点をロックオン位置に設定
		m_posRDest = lockOnPos;
		m_posRDest.y += 100.0f;	//注視点の座標を高くする

		//プレイヤー位置とロックオン位置間のベクトルを求める
		D3DXVECTOR3 vec = lockOnPos - playerPos;

		//正規化
		D3DXVec3Normalize(&vec, &vec);

		//視点の位置を設定
		{
			float posV_y = m_posVDest.y;				//y座標を保存
			m_posVDest = playerPos + (-vec) * 500.0f;	//視点の位置を変更
			m_posVDest.y = posV_y;						//y座標を戻す
		}

		//カメラの角度をロックオン先に合わせる
		m_rot.y = atan2f(m_worldPosR.x - m_worldPosV.x, m_worldPosR.z - m_worldPosV.z);
	}

	// 左右の角度の正規化
	m_rot.y = CUtility::GetNorRot(m_rot.y);

	//減衰
	m_worldPosR += (m_posRDest - m_worldPosR) * 0.1f;
	m_worldPosV += (m_posVDest - m_worldPosV) * 0.1f;

	//視点・注視点の表示
	CDebugProc::Print("視点：%f,%f,%f", m_worldPosV.x, m_worldPosV.y, m_worldPosV.z);
	CDebugProc::Print("注視点：%f,%f,%f", m_worldPosR.x, m_worldPosR.y, m_worldPosR.z);
	CDebugProc::Print("回転：%f,%f,%f", m_rot.x, m_rot.y, m_rot.z);

	//カメラの振動
	if (CInputKeyboard::Trigger(DIK_0))
	{
		ShakeCamera(60, 0.05f);
	}
}

//========================
// カメラの設定処理
//========================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//--------------------------------
	// カメラ情報の振動
	//--------------------------------
	D3DXVECTOR3 adjustPos(0.0f, 0.0f, 0.0f);

	if (m_nQuakeFreamCount > 0)
	{//揺らすフレーム数が0より大きいなら
		//フレーム数の減少
		m_nQuakeFreamCount--;

		//ランダムな値を設定
		adjustPos.x = m_fQuakeMagnitude * (rand() % 100 - 50);
		adjustPos.y = m_fQuakeMagnitude * (rand() % 100 - 50);
		adjustPos.z = m_fQuakeMagnitude * (rand() % 100 - 50);
	}

	//--------------------------------
	// カメラ情報の設定
	//--------------------------------
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &(m_worldPosV + adjustPos),	//視点
					   &(m_worldPosR + adjustPos),	//注視点
					   &m_vecU);					//上の向き

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

	//--------------------------------
	// フォグ
	//--------------------------------
	//フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//フォグカラーの設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//フォグモードの設定
	//範囲指定:D3DFOG_LINEAR , 密度指定:D3DFOG_EXP
	//pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);	//範囲指定
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);	//密度指定

	//----------------------------
	// 範囲指定時の処理
	//----------------------------
	/*float fFogStart = 100;		//開始位置
	float fFogEnd = 1000;		//終了位置
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fFogStart));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fFogEnd));*/

	//----------------------------
	// 密度指定時の処理
	//----------------------------
	float fFogDensity = 0.000f;		//密度(0.001でもかなり真っ白)
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fFogDensity));
}

//========================
// カメラの旋回
//========================
void CCamera::Turn()
{
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetJoypad();
	D3DXVECTOR3 stick = joypad->Stick(CInputJoypad::JOYKEY_RIGHT_STICK, 0);

	//スティックを動かす値の設定
	float fMoveValue = 0.5f;

	//------------------
	// 注視点の旋回
	//------------------
	if (CGame::GetFinish() == false)
	{//終了フラグが立っていないなら
		if (CInputKeyboard::Press(DIK_Q) || stick.x <= -fMoveValue)	//左回転
		{//Qキーが押された
			m_rot.y -= m_TSPEED;	//回転量の増加
		}
		else if (CInputKeyboard::Press(DIK_E) || stick.x >= fMoveValue)	//右回転
		{//Eキーが押された
			m_rot.y += m_TSPEED;
		}
		if (CInputKeyboard::Press(DIK_Y) || stick.y >= fMoveValue)	//上回転
		{//Yキーが押された
			m_rot.x += m_TSPEED;
		}
		else if (CInputKeyboard::Press(DIK_B) || stick.y <= -fMoveValue)	//下回転
		{//Bキーが押された
			m_rot.x -= m_TSPEED;
		}
	}

	if (CGame::GetFinish())
	{//終了フラグが立ってるなら
		m_rot.y -= m_TSPEED / 3;
	}

	//上下旋回の上限値追加
	if (m_rot.x >= 1.2f)
	{
		m_rot.x = 1.2f;
	}
	else if (m_rot.x <= -0.3f)
	{
		m_rot.x = -0.3f;
	}
}

//========================
// カメラの振動情報の設定
//========================
void CCamera::ShakeCamera(int fream, float magnitude)
{
	m_nQuakeFreamCount = fream;			//揺らすフレーム数
	m_fQuakeMagnitude = magnitude;		//揺れの量
}

//========================
// 視点の取得
//========================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//========================
// 角度の取得
//========================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}