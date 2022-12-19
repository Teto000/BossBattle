//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "enemy.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "model.h"
#include "line.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "hp.h"

//------------------------
// グローバル変数
//------------------------
CEnemy::KEY_SET g_aKeySet[] =	//キーセット情報
{
	//----------------------
	// キー1
	//----------------------
	{ 40,	//フレーム数
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },},
	},

	//----------------------
	// キー2
	//----------------------
	{ 40,	//フレーム数
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }, },	//剣
	},
};

//========================
// コンストラクタ
//========================
CEnemy::CEnemy() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最大値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最小値
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最大値
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最小値
	m_fLife = 0.0f;			//体力
	m_fRemLife = 0.0f;		//残り体力(%)
	m_fMaxLife = 0.0f;		//最大体力
	m_pHP = nullptr;		//HP

	/* ↓ モデル情報 ↓ */
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = nullptr;
	}

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
}

//========================
// デストラクタ
//========================
CEnemy::~CEnemy()
{

}

//========================
// 初期化
//========================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_nCntMotion = 1;
	m_fLife = 300.0f;		//体力
	m_fRemLife = 100.0f;	//残り体力(%)
	m_fMaxLife = m_fLife;	//最大体力

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//--------------------
	// モデルの生成
	//--------------------
	SetModel();

	//-----------------------
	// モデルの大きさを設定
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-40.0f, 0.0f, -40.0f);
	m_vtxMax = D3DXVECTOR3(40.0f, 130.0f, 40.0f);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//-----------------------
	// 線の表示
	//-----------------------
	SetLine();

	//-----------------------
	// HPの生成
	//-----------------------
	m_pHP = CHP::Create(D3DXVECTOR3(0.0f, 50.0f, 200.0f), 200.0f, 10.0f);
	m_pHP->SetLife(m_fLife, m_fRemLife);	//HPの設定

	return S_OK;
}

//========================
// 終了
//========================
void CEnemy::Uninit()
{
	//-------------------------
	// モデルの終了
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// 更新
//========================
void CEnemy::Update()
{
	//-------------------------
	// モデルの更新
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}

	//-------------------
	// モーション
	//-------------------
	SetMotion(true);

	//-------------------------
	// 線の更新
	//-------------------------
	UpdateLine();

	//----------------------------
	// プレイヤーとの当たり判定
	//----------------------------
	/*if (CGame::GetPlayer()->GetCollisionPlayer())
	{//プレイヤーと当たっているなら
		m_fLife--;	//体力の減少

		//残り体力を計算
		m_fRemLife = m_fLife * 100 / m_fMaxLife;

		//HPの設定
		m_pHP->SetLife(m_fLife, m_fRemLife);
	}*/

	//----------------------------
	// 剣との当たり判定
	//----------------------------
	if (CGame::GetPlayer()->GetMotion() == CPlayer::MOTION_TYPE_ATTACK
		&& CGame::GetPlayer()->GetModel(4)->GetCollisionAttack())
	{//プレイヤーが攻撃中 & 剣と当たっているなら
		m_fLife--;	//体力の減少

		//残り体力を計算
		m_fRemLife = m_fLife * 100 / m_fMaxLife;

		//HPの設定
		m_pHP->SetLife(m_fLife, m_fRemLife);
	}

	//----------------------------
	// 死亡時処理
	//----------------------------
	if (m_fLife <= 0)
	{//体力が尽きたら
		CGame::SetEnemyState();	//敵が死んだ状態

		//消去
		Uninit();
	}
}

//========================
// 描画
//========================
void CEnemy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// モデルの描画
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// 生成
//========================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pEnemy = new CEnemy;	//生成

	if (pEnemy != nullptr)
	{//NULLチェック
		//変数に代入
		pEnemy->m_rot = rot;

		//初期化
		pEnemy->Init(pos);
		pEnemy->SetObjType(OBJTYPE_ENEMY);
	}

	return pEnemy;
}

//========================
// モデルの生成
//========================
void CEnemy::SetModel()
{
	//モデル0
	m_pModel[0] = CModel::Create("data\\MODEL\\enemy.x", nullptr,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//========================
// モーションの設定
//========================
void CEnemy::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_ENEMY_KEY)
	{//キーが最大数に達したら
		if (bLoop)
		{
			m_nCurrentKey = 0;	//キー番号の初期化
		}
		else
		{
			return;
		}
	}

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
	 //-----------------------------------------
	 // NULLチェック
	 //-----------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// 現在値を取得
		//-----------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-----------------------------------------
		//位置
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//向き
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-----------------------------------------
		//位置
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//向き
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// モデル情報の設定
		//-----------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(D3DXToRadian(fRotX), D3DXToRadian(fRotY), D3DXToRadian(fRotZ)));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//-------------------------
	// 初期化
	//-------------------------
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//========================
// 線の設置
//========================
void CEnemy::SetLine()
{
	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = CLine::CreateAll(m_pLine[i], i, m_pos, min, max);
	}
}

//========================
// 線の情報の更新
//========================
void CEnemy::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		if (m_pLine[i])
		{
			m_pLine[i]->SetLinePos(i, min, max);
		}
	}
}