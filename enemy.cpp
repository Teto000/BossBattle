//===================================================
//
// エネミーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <cmath>
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

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CEnemy::fDefGravity = 0.8f;	//基本の重力

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
	m_nMoveTime = 0;		//移動までの時間
	m_fLife = 0.0f;			//体力
	m_fRemLife = 0.0f;		//残り体力(%)
	m_fMaxLife = 0.0f;		//最大体力
	m_fGravity = 0.0f;		//重力の値
	m_bNockBack = false;	//ノックバックしたか
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
	m_fLife = 300.0f;			//体力
	m_fRemLife = 100.0f;		//残り体力(%)
	m_fMaxLife = m_fLife;		//最大体力
	m_fGravity = fDefGravity;	//重力の値

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
	//SetLine();

	//-----------------------
	// HPの生成
	//-----------------------
	{
		D3DXVECTOR3 hpPos(980.0f, 50.0f, 0.0f);
		m_pHP = CHP::Create(hpPos, 500.0f, 30.0f, CHP::HPTYPE_ENEMY);
		m_pHP->SetLife(m_fLife, m_fRemLife);	//HPの設定
	}

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

	//------------------------
	// 攻撃処理
	//------------------------
	Attack();

	//-------------------
	// モーション
	//-------------------
	SetMotion(true);
	
	//-------------------------
	// 線の更新
	//-------------------------
	//UpdateLine();

	//----------------------------
	// 攻撃を受けた処理
	//----------------------------
	//プレイヤーとの当たり判定
	CGame::GetPlayer()->GetCollisionPlayer(m_pos, m_size, m_mtxWorld);

	if (CGame::GetPlayer()->GetHitAttack())
	{//プレイヤーが攻撃を当てた状態なら
		//リセット
		m_nAttackTime = 0;			//攻撃までの時間
		m_fGravity = fDefGravity;	//重力の値

		//--------------------------------
		// ノックバックする処理
		//--------------------------------
		//プレイヤーの位置を取得
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

		//敵とプレイヤー間のベクトルを計算
		D3DXVECTOR3 vec = playerPos - m_pos;

		D3DXVec3Normalize(&vec,&vec);	//ベクトルの正規化

		if (!m_bNockBack)
		{//ノックバックしていないなら
			m_pos += -vec * 7.0f;	//逆ベクトル方向に移動
			m_pos.y += 50.0f;		//上昇
			m_bNockBack = true;
		}
	}
	else
	{
		m_bNockBack = false;
	}

	//---------------------
	// 重力の加算
	//---------------------
	if (m_pos.y >= 0.0f)
	{//飛んでいるなら
		m_pos.y -= m_fGravity;
		m_fGravity += 0.2f;
	}
	else
	{//地面に着いたら
		//重力の値をリセット
		m_fGravity = fDefGravity;
	}

	//----------------------------
	// 死亡時処理
	//----------------------------
	if (m_fLife <= 0)
	{//体力が尽きたら
		CGame::SetEnemyState();	//敵が死んだ状態

		//HPバーの消去
		if (m_pHP != nullptr)
		{//nullじゃないなら
			m_pHP->Uninit();
		}

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

//========================
// HP減少時の処理
//========================
void CEnemy::SubLife(float fDamage)
{
	m_fLife -= fDamage;	//体力の減少

	//残り体力を計算
	m_fRemLife = m_fLife * 100 / m_fMaxLife;

	//HPの設定
	m_pHP->SetLife(m_fLife, m_fRemLife);
}

//========================
// 攻撃処理
//========================
void CEnemy::Attack()
{
	//変数宣言
	int nMaxAttackTime = 120;				//攻撃時間
	float fAttackArea = 120.0f;				//敵の攻撃範囲
	float fMoveArea = fAttackArea - 50.0f;	//敵の移動範囲

	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	//敵とプレイヤー間の距離を計算
	D3DXVECTOR3 vec = playerPos - m_pos;

	//距離の絶対値を計算
	float fDistance = fabs((vec.x + vec.z) / 2);

	//-------------------------
	// プレイヤーまで移動
	//-------------------------
	if (fDistance >= fMoveArea)
	{//プレイヤーが範囲内にいないなら
		Move();
	}

	//-------------------------
	// 攻撃処理
	//-------------------------
	if (fDistance <= fAttackArea)
	{//プレイヤーが範囲内にいるなら
		//攻撃までの時間を加算
		m_nAttackTime++;

		if (m_nAttackTime >= nMaxAttackTime)
		{//攻撃時間が値に達したら
			CGame::GetPlayer()->SubLife(1);
		}
	}
}

//========================
// 移動処理
//========================
void CEnemy::Move()
{
	//------------------------------
	// プレイヤーの方を向く
	//------------------------------
	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos(CGame::GetPlayer()->GetPosition());

	//2点間の距離を求める
	float X = m_pos.x - playerPos.x;
	float Z = m_pos.z - playerPos.z;

	//角度の設定
	float angle = atan2f(X, Z);

	//向きの設定
	m_rotDest = D3DXVECTOR3(0.0f, angle, 0.0f);

	//-------------------------------
	// 目的の角度の正規化
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 目的の角度まで回転する
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.03f;	//減衰処理

	//-------------------------------
	// 角度の正規化
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//角度が180以上なら
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度が-180以下なら
		m_rot.y += D3DX_PI * 2;
	}

	//------------------------------
	// プレイヤーに向かって移動
	//------------------------------
	//プレイヤーと敵のベクトルを求める
	D3DXVECTOR3 vec(playerPos - m_pos);

	//ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	//プレイヤーに向かって移動
	m_move = vec * 1.5f;
	m_pos += m_move;
}