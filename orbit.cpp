//===================================================
//
// 軌跡の処理
// Author:Teruto Sato
//
//===================================================

//-----------------------
// インクルード
//-----------------------
//
#include <assert.h>
#include "orbit.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "model.h"

//==========================
// コンストラクタ
//==========================
COrbit::COrbit() : CObject(1)
{
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//オフセット座標
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ワールド座標
	m_initCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色の初期値
	m_nNumVtx = 0;		//頂点数

	D3DXMatrixIdentity(&m_mtxWorld);	//ワールドマトリックス
	D3DXMatrixIdentity(&m_pMtxParent);	//親のマトリックス
	m_pVtxBuff = nullptr;				//頂点バッファへのポインタ
}

//==========================
// デストラクタ
//==========================
COrbit::~COrbit()
{

}

//==========================
// 初期化
//==========================
HRESULT COrbit::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_offsetPos = pos;	//位置
	m_initCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f);	//色の初期値

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//----------------------------------
	// 頂点バッファの生成
	//----------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxVtx,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------------
	// 頂点情報の設定
	//----------------------------------
	for (int i = 0; i < nMaxVtx; i++)
	{
		//------------------------
		// 頂点座標の設定
		//------------------------
		pVtx[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//------------------------
		// 各頂点の法線の設定
		//------------------------
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//------------------------
		// 頂点カラーの設定
		//------------------------
		pVtx[i].col = m_initCol;

		//------------------------
		// テクスチャ座標の設定
		//------------------------
		//pVtx[i].tex = D3DXVECTOR2(0.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	pVtx += m_nNumVtx;

	return S_OK;
}

//==========================
// 終了
//==========================
void COrbit::Uninit()
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
void COrbit::Update()
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// 頂点座標を後ろからコピーする
	//--------------------------------------
	for (int i = m_nNumVtx - 1; i > 0; i--)
	{//頂点数分降順に回す
		pVtx[i].pos = pVtx[i - 2].pos;
		pVtx[i].col = pVtx[i - 2].col;

	}

	//-------------------------------
	// 色の変更
	//-------------------------------
	for (int i = 0; i < m_nNumVtx; i++)
	{
		D3DXCOLOR col = pVtx[i].col;
		col.a -= 0.005f;
		pVtx[i].col = col;
	}

	//--------------------------------------
	// 空いた0と1に軌跡の新しい座標を代入
	//--------------------------------------
	//親モデルの原点からオフセット分移動した座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &m_offsetPos, &m_pMtxParent);
	pVtx[1].pos = m_worldPos;
	pVtx[1].col = m_initCol;

	//親モデルの原点の座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, -20.0f, 0.0f), &m_pMtxParent);
	pVtx[0].pos = m_worldPos;
	pVtx[0].col = m_initCol;

	if (m_nNumVtx < nMaxVtx - 2)
	{//頂点の数が最大値を超えないなら
		m_nNumVtx += 2;	//頂点数を加算
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================
// 描画
//==========================
void COrbit::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//------------------------------------
	// マトリックス
	//------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//------------------------------------
	// 頂点
	//------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//裏面から見えるようにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Zバッファのクリア
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
						   0,					//描画する最初の頂点インデックス
						   m_nNumVtx - 4);		//描画するプリミティブ数

	// Zバッファの有効化設定
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//裏面から見えなくする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================
// 生成
//==========================
COrbit* COrbit::Create(D3DXVECTOR3 offset, D3DXMATRIX mtx)
{
	COrbit* pOrbit = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pOrbit = new COrbit;	//生成

	if (pOrbit != nullptr)
	{//NULLチェック
		//引数の代入
		pOrbit->m_pMtxParent = mtx;	//親のマトリックス

		//初期化
		pOrbit->Init(offset);
		pOrbit->SetObjType(OBJTYPE_MESHORBIT);
	}

	return pOrbit;
}