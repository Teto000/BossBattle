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
COrbit::COrbit() : CObject(0)
{
	m_localpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ローカル座標
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ワールド座標
	D3DXMatrixIdentity(&m_mtxWorld);	//ワールドマトリックス
	D3DXMatrixIdentity(&m_pMtxParent);	//親のマトリックス
	m_pVtxBuff = nullptr;				//頂点バッファへのポインタ
	m_pIdxBuff = nullptr;				//インデックスバッファのポインタ
}

//==========================
// デストラクタ
//==========================
COrbit::~COrbit()
{
	/*assert(m_pVtxBuff);
	assert(m_pIdxBuff);*/
}

//==========================
// 初期化
//==========================
HRESULT COrbit::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_localpos = pos;

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//----------------------------------
	// メッシュを構成する情報の設定
	//----------------------------------
	m_nNumDivision = 12;				//ポリゴンの分割数
	m_nCntVtx = m_nNumDivision + 1;		//頂点の分割数
	m_nNumVtx = (m_nNumDivision + 1) * (m_nNumDivision + 1);							//頂点数
	m_nNumPolygon = m_nNumDivision * m_nNumDivision * 2 + (m_nNumDivision - 1) * 4;		//ポリゴン数
	m_nNumIndex = (m_nNumDivision + 1) * 2 * m_nNumDivision + (m_nNumDivision - 1) * 2;	//インデックス数
	m_fMaxWidth = 5500.0f;								//メッシュフィールドの最大幅
	m_fMeshWidth = (m_fMaxWidth / m_nNumDivision);		//メッシュフィールドの幅
	m_fTexSize = (5.0f / m_nNumDivision);				//テクスチャの分割サイズ


	//----------------------------------
	// 頂点バッファの生成
	//----------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//----------------------------------
	// インデックスバッファの生成
	//----------------------------------
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pIdxBuff,
								NULL);

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------------
	// 頂点情報の設定
	//----------------------------------
	for (int z = 0; z < m_nCntVtx; z++)
	{
		for (int x = 0; x < m_nCntVtx; x++)
		{
			//頂点座標の設定
			{
				D3DXVECTOR3 vtxPos(-(m_fMaxWidth / 2) + (m_fMeshWidth * x), 0.0f, (m_fMaxWidth / 2) - (m_fMeshWidth * z));
				pVtx[x + (z * (m_nCntVtx))].pos = vtxPos;
			}

			//各頂点の法線の設定(※ベクトルの大きさは必ず1にする必要がある)
			pVtx[x + (z * (m_nCntVtx))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[x + (z * (m_nCntVtx))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[x + (z * (m_nCntVtx))].tex = D3DXVECTOR2(0.0f, 1.0f);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//----------------------------------
	// インデックスの設定
	//----------------------------------
	int nCnt = 0;

	for (int Z = 0; Z < m_nNumDivision; Z++)
	{
		for (int X = 0; X < m_nCntVtx; X++)
		{
			//インデックスバッファの設定
			pIdx[nCnt] = (WORD(X + Z * (m_nCntVtx)+(m_nCntVtx)));
			pIdx[nCnt + 1] = (WORD(X + Z * (m_nCntVtx)));

			nCnt += 2;

			//縮退ポリゴンの追加
			if (X == m_nNumDivision)
			{
				pIdx[nCnt] = (WORD)(X + Z * m_nCntVtx);
				pIdx[nCnt + 1] = (WORD)((Z + 2) * m_nCntVtx);

				nCnt += 2;
			}
		}
	}

	pVtx += m_nNumVtx;
	pIdx += m_nNumIndex;

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

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

	//インデックスバッファの破壊
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
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
	for (int i = nMaxVtx; i > 1; i--)
	{//頂点数分降順に回す
		pVtx[i].pos = pVtx[i - 1].pos;
	}

	//--------------------------------------
	// 空いた0と1に軌跡の新しい座標を代入
	//--------------------------------------
	//親モデルの原点からオフセット分移動した座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &m_localpos, &m_pMtxParent);
	pVtx[1].pos = m_worldPos;

	//親モデルの原点の座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_pMtxParent);
	pVtx[0].pos = m_worldPos;

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