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
#include "object3d.h"
#include "game.h"
#include "player.h"
#include "model.h"

//===========================
// コンストラクタ
//===========================
CLine::CLine() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//始点
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//終点
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ
	m_pObject3D = nullptr;
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
HRESULT CLine::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;

	if (m_pObject3D != nullptr)
	{
		m_pObject3D->Init(m_pos);
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * nMaxLine,	//確保するバッファのサイズ
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

	for (int i = 0; i < nMaxLine; i++)
	{
		//頂点座標の設定
		pVtx[0].pos = m_start;
		pVtx[1].pos = m_end;

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;

		pVtx += 2;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================
// 終了
//===========================
void CLine::Uninit()
{
	if (m_pObject3D != nullptr)
	{
		m_pObject3D->Uninit();
	}

	//頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}

//===========================
// 更新
//===========================
void CLine::Update()
{
	//-------------------------------
	//	頂点情報の設定
	//-------------------------------
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nMaxLine; i++)
	{
		//頂点座標の設定
		pVtx[0].pos = m_start;
		pVtx[1].pos = m_end;

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;

		pVtx += 2;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 描画
//===========================
void CLine::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	//ライトの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================
// 線の設定
// 引数：位置、始点、終点、色
//===========================
CLine* CLine::Create(
	D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine* pLine = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pLine = new CLine;	//生成
	pLine->m_pObject3D = new CObject3D(0);

	if (pLine != nullptr)
	{//NULLチェック
		//変数に代入
		pLine->m_start = start;	//始点
		pLine->m_end = end;		//終点
		pLine->m_col = col;		//色

		//初期化
		pLine->Init(pos);
	}

	return pLine;
}

//===========================
// 全ての線の生成
//===========================
CLine* CLine::CreateAll(CLine* line, int nNumber,
					D3DXVECTOR3 pos, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	//変数
	D3DXVECTOR3 start(0.0f, 0.0f, 0.0f);		//開始位置
	D3DXVECTOR3 end(0.0f, 0.0f, 0.0f);			//終了位置
	D3DXCOLOR lineCol(1.0f, 0.0f, 0.0f, 1.0f);	//線の色

	//-----------------------------------
	// 下辺
	//-----------------------------------
	switch (nNumber)
	{
	case 0:
		start = D3DXVECTOR3(min.x, min.y, min.z);
		end = D3DXVECTOR3(max.x, min.y, min.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 1:
		start = D3DXVECTOR3(min.x, min.y, min.z);
		end = D3DXVECTOR3(min.x, min.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 2:
		start = D3DXVECTOR3(max.x, min.y, min.z);
		end = D3DXVECTOR3(max.x, min.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 3:
		start = D3DXVECTOR3(min.x, min.y, max.z);
		end = D3DXVECTOR3(max.x, min.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	//-----------------------------------
	// 上辺
	//-----------------------------------
	case 4:
		start = D3DXVECTOR3(min.x, max.y, min.z);
		end = D3DXVECTOR3(max.x, max.y, min.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 5:
		start = D3DXVECTOR3(min.x, max.y, min.z);
		end = D3DXVECTOR3(min.x, max.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 6:
		start = D3DXVECTOR3(max.x, max.y, min.z);
		end = D3DXVECTOR3(max.x, max.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 7:
		start = D3DXVECTOR3(min.x, max.y, max.z);
		end = D3DXVECTOR3(max.x, max.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	case 8:
		start = D3DXVECTOR3(min.x, min.y, min.z);
		end = D3DXVECTOR3(min.x, max.y, min.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 9:
		start = D3DXVECTOR3(min.x, min.y, max.z);
		end = D3DXVECTOR3(min.x, max.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 10:
		start = D3DXVECTOR3(max.x, min.y, min.z);
		end = D3DXVECTOR3(max.x, max.y, min.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;

	case 11:
		start = D3DXVECTOR3(max.x, min.y, max.z);
		end = D3DXVECTOR3(max.x, max.y, max.z);
		line = CLine::Create(pos, start, end, lineCol);
		break;
	}

	return line;
}

//===========================
// 線の開始終了位置を設定
//===========================
void CLine::SetLinePos(int nNumber, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	//-----------------------------------
	// 下辺
	//-----------------------------------
	switch (nNumber)
	{
	case 0:
		m_start = D3DXVECTOR3(min.x, min.y, min.z);
		m_end = D3DXVECTOR3(max.x, min.y, min.z);
		break;

	case 1:
		m_start = D3DXVECTOR3(min.x, min.y, min.z);
		m_end = D3DXVECTOR3(min.x, min.y, max.z);
		break;

	case 2:
		m_start = D3DXVECTOR3(max.x, min.y, min.z);
		m_end = D3DXVECTOR3(max.x, min.y, max.z);
		break;

	case 3:
		m_start = D3DXVECTOR3(min.x, min.y, max.z);
		m_end = D3DXVECTOR3(max.x, min.y, max.z);
		break;

		//-----------------------------------
		// 上辺
		//-----------------------------------
	case 4:
		m_start = D3DXVECTOR3(min.x, max.y, min.z);
		m_end = D3DXVECTOR3(max.x, max.y, min.z);
		break;

	case 5:
		m_start = D3DXVECTOR3(min.x, max.y, min.z);
		m_end = D3DXVECTOR3(min.x, max.y, max.z);
		break;

	case 6:
		m_start = D3DXVECTOR3(max.x, max.y, min.z);
		m_end = D3DXVECTOR3(max.x, max.y, max.z);
		break;

	case 7:
		m_start = D3DXVECTOR3(min.x, max.y, max.z);
		m_end = D3DXVECTOR3(max.x, max.y, max.z);
		break;

		//-----------------------------------
		// 縦辺
		//-----------------------------------
	case 8:
		m_start = D3DXVECTOR3(min.x, min.y, min.z);
		m_end = D3DXVECTOR3(min.x, max.y, min.z);
		break;

	case 9:
		m_start = D3DXVECTOR3(min.x, min.y, max.z);
		m_end = D3DXVECTOR3(min.x, max.y, max.z);
		break;

	case 10:
		m_start = D3DXVECTOR3(max.x, min.y, min.z);
		m_end = D3DXVECTOR3(max.x, max.y, min.z);
		break;

	case 11:
		m_start = D3DXVECTOR3(max.x, min.y, max.z);
		m_end = D3DXVECTOR3(max.x, max.y, max.z);
		break;
	}
}