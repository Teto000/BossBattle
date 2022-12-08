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
	m_pos = pos;
	m_nCntMotion = 1;

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
	if (CGame::GetPlayer()->GetCollisionPlayer())
	{//プレイヤーと当たっているなら
		CGame::SetEnemyState();

		//消去
		Uninit();

		//リザルト画面に移行
		//CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
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
	//線の色
	D3DXCOLOR lineCol(1.0f, 0.0f, 0.0f, 1.0f);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11] = CLine::Create(m_pos, start, end, lineCol);
}

//========================
// 線の情報の更新
//========================
void CEnemy::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//最大最小を求めなおす

	//剣を含まないでけいさんする

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3]->SetLine(start, end);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7]->SetLine(start, end);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11]->SetLine(start, end);
}

//===========================
// 位置の設定
//===========================
void CEnemy::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CEnemy::GetPosition()
{
	return m_pos;
}

//========================
// 向きの取得
//========================
D3DXVECTOR3 CEnemy::GetRot()
{
	return m_rot;
}

//===========================
// 幅の取得
//===========================
float CEnemy::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CEnemy::GetHeight()
{
	return 0.0f;
}

//===========================
// ワールドマトリックスの取得
//===========================
D3DXMATRIX CEnemy::GetmtxWorld()
{
	return m_mtxWorld;
}