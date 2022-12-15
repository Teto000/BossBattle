//===================================================
//
// 軌跡の処理
// Author:Teruto Sato
//
//===================================================

//-----------------------
// インクルード
//-----------------------
#include <assert.h>
#include "meshorbit.h"
#include "application.h"
#include "renderer.h"

//==========================
// コンストラクタ
//==========================
CMeshOrbit::CMeshOrbit() : CObject(0)
{
	m_localpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ローカル座標
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ワールド座標
	D3DXMatrixIdentity(m_pMtxParent);				//親のマトリックス
	m_pVtxBuff = nullptr;							//頂点バッファへのポインタ
}

//==========================
// デストラクタ
//==========================
CMeshOrbit::~CMeshOrbit()
{

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
}