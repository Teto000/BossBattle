//===================================================
//
// 軌跡の処理
// Author:Teruto Sato
//
//===================================================
/*
//-----------------------
// インクルード
//-----------------------
#include <assert.h>
#include "meshorbit.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "model.h"

//==========================
// コンストラクタ
//==========================
CMeshOrbit::CMeshOrbit() : CObject(0)
{
	m_localpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ローカル座標
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ワールド座標
	D3DXMatrixIdentity(&m_mtxWorld);	//ワールドマトリックス
	D3DXMatrixIdentity(m_pMtxParent);	//親のマトリックス
	m_pVtxBuff = nullptr;				//頂点バッファへのポインタ
	m_pIdxBuff = nullptr;				//インデックスバッファへのポインタ
}

//==========================
// デストラクタ
//==========================
CMeshOrbit::~CMeshOrbit()
{
	assert(m_pVtxBuff);
	assert(m_pIdxBuff);
}

//==========================
// 初期化
//==========================
HRESULT CMeshOrbit::Init(D3DXVECTOR3 pos)
{
	return S_OK;
}

//==========================
// 終了
//==========================
void CMeshOrbit::Uninit()
{
	//頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//==========================
// 更新
//==========================
void CMeshOrbit::Update()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// 頂点座標を後ろからコピーする
	//--------------------------------------
	for (int i = nMaxVtx; i > 1; i++)
	{//頂点数分降順に回す
		pVtx[i].pos = pVtx[i - 1].pos;
	}

	//--------------------------------------
	// 空いた0と1に軌跡の新しい座標を代入
	//--------------------------------------
	// 親モデルの原点からオフセット分移動した座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &m_localpos, m_pMtxParent);
	pVtx[1].pos = m_worldPos;

	// 親モデルの原点の座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pMtxParent);
	pVtx[0].pos = m_worldPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================
// 描画
//==========================
void CMeshOrbit::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//------------------------------------
	// マトリックス
	//------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//剣の向きを取得
	D3DXVECTOR3 swordRot(CGame::GetPlayer()->GetModel(6)->GetRot());

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, swordRot.y, swordRot.x, swordRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//剣の位置を取得
	D3DXVECTOR3 swordPos(CGame::GetPlayer()->GetModel(6)->GetPos());

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, swordPos.x, swordPos.y, swordPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//------------------------------------
	// 頂点
	//------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//==========================
// 生成
//==========================
CMeshOrbit* CMeshOrbit::Create(D3DXVECTOR3 pos)
{
	CMeshOrbit *pMeshOrbit = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pMeshOrbit = new CMeshOrbit;	//生成

	if (pMeshOrbit != nullptr)
	{//NULLチェック
		//初期化
		pMeshOrbit->Init(pos);
		pMeshOrbit->SetObjType(OBJTYPE_MESHORBIT);
	}

	return pMeshOrbit;
}*/