//===================================================
//
// ラインの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "line.h"
#include "main.h"
#include "renderer.h"
#include "application.h"

//===========================
// コンストラクタ
//===========================
CLine::CLine()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//始点
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//終点
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ
}

//===========================
// デストラクタ
//===========================
CLine::~CLine()
{
	assert(m_pVtxBuff == nullptr);
}

//===========================
// 初期化
//===========================
HRESULT CLine::Init()
{
	CObject3D::Init(m_pos);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								nullptr);

	//-------------------------------
	//	頂点情報の設定
	//-------------------------------
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各頂点の法線の設定(※ベクトルの大きさは必ず1にする必要がある)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報の設定
	CObject3D::SetVtx(pVtx);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================
// 終了
//===========================
void CLine::Uninit()
{
	CObject3D::Uninit();

	//頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//===========================
// 更新
//===========================
void CLine::Update()
{
	CObject3D::Update();
}

//===========================
// 描画
//===========================
void CLine::Draw()
{
	CObject3D::Draw();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST,	//プリミティブの種類
						   2,				//描画する最初の頂点インデックス
						   1);				//描画するプリミティブ数
}

//===========================
// 線の設定
// 引数：位置、始点、終点、色
//===========================
CLine* CLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine* pLine = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pLine = new CLine;	//生成

	if (pLine != nullptr)
	{//NULLチェック
		//変数に代入
		pLine->m_pos = pos;		//位置
		pLine->m_start = start;	//始点
		pLine->m_end = end;		//終点
		pLine->m_col = col;		//色

		//初期化
		pLine->Init();
	}

	return pLine;
}