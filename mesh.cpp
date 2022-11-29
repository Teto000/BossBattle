//===================================================
//
// メッシュの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include <assert.h>
#include <time.h>
#include "mesh.h"
#include "main.h"
#include "renderer.h"
#include "application.h"
#include "game.h"
#include "player.h"

//=======================
// コンストラクタ
//=======================
CMesh::CMesh() : CMesh(0)
{
}
CMesh::CMesh(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_VtxPos[nMaxVtx] = {};						//頂点の位置
	m_Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//面法線ベクトル格納用
	m_VtxNor[nMaxVtx] = {};						//頂点の法線ベクトル格納用
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

	m_nNumDivision = 0;		//ポリゴンの分割数
	m_nCntVtx = 0;			//頂点の分割数
	m_nNumVtx = 0;			//頂点数
	m_nNumPolygon = 0;			//ポリゴン数
	m_nNumIndex = 0;			//インデックス数
	m_fMaxWidth = 0.0f;		//メッシュフィールドの最大幅
	m_fMeshWidth = 0.0f;	//メッシュフィールドの幅
	m_fTexSize = 0.0f;		//テクスチャの分割サイズ

	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ
	m_pIdxBuff = nullptr;	//インデックスバッファへのポインタ
}

//=======================
// デストラクタ
//=======================
CMesh::~CMesh()
{
	assert(m_pVtxBuff == nullptr);
	assert(m_pIdxBuff == nullptr);
}

//=======================
// 初期化
//=======================
HRESULT CMesh::Init(D3DXVECTOR3 pos)
{
	//時刻をもとにしたランダムな値を生成
	//srand((unsigned int)time(NULL));

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//----------------------------------
	// メッシュを構成する情報の設定
	//----------------------------------
	m_nNumDivision = 12;				//ポリゴンの分割数
	m_nCntVtx = m_nNumDivision + 1;		//頂点の分割数
	m_nNumVtx = (m_nNumDivision + 1) * (m_nNumDivision + 1);							//頂点数
	m_nNumPolygon = m_nNumDivision * m_nNumDivision * 2 + (m_nNumDivision - 1) * 4;		//ポリゴン数
	m_nNumIndex = (m_nNumDivision + 1) * 2 * m_nNumDivision + (m_nNumDivision - 1) * 2;	//インデックス数
	m_fMaxWidth = 1200.0f;								//メッシュフィールドの最大幅
	m_fMeshWidth = (m_fMaxWidth / m_nNumDivision);		//メッシュフィールドの幅
	m_fTexSize = (1.0f / m_nNumDivision);				//テクスチャの分割サイズ


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
				//ランダムな位置の設定
				int nRand = rand() % 80;

				D3DXVECTOR3 pos(-(m_fMaxWidth / 2) + (m_fMeshWidth * x), 0.0f, (m_fMaxWidth / 2) - (m_fMeshWidth * z));
				pVtx[x + (z * (m_nCntVtx))].pos = pos;
			}

			//各頂点の法線の設定(※ベクトルの大きさは必ず1にする必要がある)
			pVtx[x + (z * (m_nCntVtx))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[x + (z * (m_nCntVtx))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[x + (z * (m_nCntVtx))].tex = D3DXVECTOR2(0.0f + m_fTexSize * x, 0.0f + m_fTexSize * z);
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
			pIdx[nCnt] = (WORD(X + Z * (m_nCntVtx) + (m_nCntVtx)));
			pIdx[nCnt + 1] = (WORD(X + Z * (m_nCntVtx)));

			nCnt += 2;

			//縮退ポリゴンの追加
			if (X == m_nNumDivision)
			{
				pIdx[nCnt] = X + Z * m_nCntVtx;
				pIdx[nCnt + 1] = (Z + 2) * m_nCntVtx;

				nCnt += 2;
			}
		}
	}

	//頂点の法線を設定
	SetVtxNor(pVtx, pIdx);

	pVtx += m_nNumVtx;
	pIdx += m_nNumIndex;

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=======================
// 終了
//=======================
void CMesh::Uninit()
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

//=======================
// 更新
//=======================
void CMesh::Update()
{
	//-------------------------
	// メッシュの当たり判定
	//-------------------------
	CollisionMesh();
}

//=======================
// 描画
//=======================
void CMesh::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//------------------------------------
	// マトリックス
	//------------------------------------
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

	//------------------------------------
	// 頂点
	//------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//------------------------------------
	// テクスチャ
	//------------------------------------
	//テクスチャの取得
	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
								  0,
								  0,
								  m_nNumVtx,	//頂点数
								  0,
								  m_nNumPolygon);	//描画するプリミティブ数

	//テクスチャのリセット
	pDevice->SetTexture(0, NULL);
}

//==================================
// メッシュフィールドの当たり判定
//==================================
void CMesh::CollisionMesh()
{
	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D*pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------
	// 頂点の位置を取得
	//--------------------------
	for (int nCnt = 0; nCnt < m_nNumIndex; nCnt++)
	{//インデックス数分回す
		m_VtxPos[nCnt] = pVtx[pIdx[nCnt]].pos;
	}

	//--------------------------
	// 色の初期化
	//--------------------------
	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{//頂点数分回す
		//通常の色にする
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//------------------------------------
	// プレイヤーがいるポリゴンを求める
	//------------------------------------
	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	for (int nNumIdx = 0; nNumIdx < (m_nNumIndex - 2); nNumIdx++)
	{
		//ポリゴンの頂点の位置を取得
		D3DXVECTOR3 P1 = m_VtxPos[nNumIdx + 0];
		D3DXVECTOR3 P2 = m_VtxPos[nNumIdx + 1];
		D3DXVECTOR3 P3 = m_VtxPos[nNumIdx + 2];

		//縮退ポリゴンの除外
		if (P1 == P2 || P2 == P3 || P3 == P1)
		{
			nNumIdx++;
			continue;
		}

		//ポリゴンの頂点同士のベクトルの計算
		D3DXVECTOR3 vecA = P2 - P1;
		D3DXVECTOR3 vecB = P3 - P2;
		D3DXVECTOR3 vecC = P1 - P3;

		//プレイヤーと頂点のベクトルの計算
		D3DXVECTOR3 vec1 = playerPos - P1;
		D3DXVECTOR3 vec2 = playerPos - P2;
		D3DXVECTOR3 vec3 = playerPos - P3;

		//外積計算の式変数
		float a = vecA.x * vec1.z - vecA.z * vec1.x;
		float b = vecB.x * vec2.z - vecB.z * vec2.x;
		float c = vecC.x * vec3.z - vecC.z * vec3.x;

		//外積計算(2次元)
		if ((a * b >= 0) && (b * c >= 0) && (c * a >= 0))
		{//3つのベクトルの内側にある(外積が全て+)なら
			//赤色にする
			/*pVtx[pIdx[nNumIdx + 0]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[pIdx[nNumIdx + 1]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[pIdx[nNumIdx + 2]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);*/

			//-------------------------
			// 面法線ベクトルを計算
			//-------------------------
			D3DXVECTOR3 V1 = P2 - P1;
			D3DXVECTOR3 V2 = P3 - P2;

			//外積計算(3次元)
			D3DXVec3Cross(&m_Normal, &V1, &V2);

			//ベクトルの正規化
			D3DXVec3Normalize(&m_Normal, &m_Normal);

			//-----------------------------
			// プレイヤーのY座標を求める
			//-----------------------------
			playerPos.y = P1.y - ((playerPos.x - P1.x) * m_Normal.x
						   + (playerPos.z - P1.z) * m_Normal.z)
						   / m_Normal.y;

			//プレイヤーの位置の設定
			CGame::GetPlayer()->SetPosition(playerPos);
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 頂点の法線を設定
//===========================
void CMesh::SetVtxNor(VERTEX_3D* pVtx, WORD* pIdx)
{
	//--------------------------
	// 頂点の位置を取得
	//--------------------------
	for (int nCnt = 0; nCnt < m_nNumIndex; nCnt++)
	{//インデックス数分回す
		m_VtxPos[nCnt] = pVtx[pIdx[nCnt]].pos;
		//各頂点のベクトルを格納
		m_VtxNor[pIdx[nCnt]] = pVtx[pIdx[nCnt]].nor;
	}

	//-----------------------------
	// 頂点の法線ベクトルを設定
	//-----------------------------
	for (int nNumIdx = 0; nNumIdx < (m_nNumIndex - 2); nNumIdx++)
	{
		//ポリゴンの頂点の位置を取得
		D3DXVECTOR3 P1 = m_VtxPos[nNumIdx + 0];
		D3DXVECTOR3 P2 = m_VtxPos[nNumIdx + 1];
		D3DXVECTOR3 P3 = m_VtxPos[nNumIdx + 2];

		//縮退ポリゴンの除外
		if (P1 == P2 || P2 == P3 || P3 == P1)
		{
			nNumIdx++;
			continue;
		}

		//-------------------------
		// 面法線ベクトルを計算
		//-------------------------
		D3DXVECTOR3 V1 = P2 - P1;
		D3DXVECTOR3 V2 = P3 - P2;

		//外積計算(3次元)
		D3DXVec3Cross(&m_Normal, &V1, &V2);

		//ベクトルの正規化
		D3DXVec3Normalize(&m_Normal, &m_Normal);

		//-------------------------
		// 法線の向きを揃える
		//-------------------------
		if (nNumIdx % 2 != 0)
		{//奇数なら
			m_Normal *= -1;
		}

		//-------------------------
		// 頂点ベクトルを計算
		//-------------------------
		//面法線ベクトルを加算
		m_VtxNor[pIdx[nNumIdx]] += m_Normal;

		//ベクトルの正規化
		D3DXVec3Normalize(&m_VtxNor[pIdx[nNumIdx]], &m_VtxNor[pIdx[nNumIdx]]);

		//ベクトルの設定
		pVtx[pIdx[nNumIdx]].nor = m_VtxNor[pIdx[nNumIdx]];
	}
}

//===========================
// 分割数と幅の設定
//===========================
void CMesh::SetMeshSize(int nDivision, float fWidth)
{
	m_nNumDivision = nDivision;	//ポリゴンの分割数の設定
	m_fMaxWidth = fWidth;		//メッシュフィールドの最大幅の設定

	//----------------------------
	// メッシュの情報の再設定
	//----------------------------
	m_nCntVtx = m_nNumDivision + 1;		//頂点の分割数
	m_nNumVtx = (m_nNumDivision + 1) * (m_nNumDivision + 1);							//頂点数
	m_nNumPolygon = m_nNumDivision * m_nNumDivision * 2 + (m_nNumDivision - 1) * 4;		//ポリゴン数
	m_nNumIndex = (m_nNumDivision + 1) * 2 * m_nNumDivision + (m_nNumDivision - 1) * 2;	//インデックス数
	m_fMeshWidth = (m_fMaxWidth / m_nNumDivision);		//メッシュフィールドの幅
	m_fTexSize = (1.0f / m_nNumDivision);				//テクスチャの分割サイズ
}

//===========================
// テクスチャの設定
//===========================
void CMesh::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CMesh::GetPosition()
{
	return m_pos;
}

//===========================
// 幅の取得
//===========================
float CMesh::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CMesh::GetHeight()
{
	return 0.0f;
}