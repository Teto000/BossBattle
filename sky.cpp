//===================================================
//
// メッシュの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "sky.h"
#include "application.h"
#include "renderer.h"

//=======================
// コンストラクタ
//=======================
CSky::CSky() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_VtxPos[nMaxVtx] = {};						//頂点の位置
	m_Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//面法線ベクトル格納用
	m_VtxNor[nMaxVtx] = {};						//頂点の法線ベクトル格納用
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

	m_nNumVtx = 0;			//頂点数
	m_nNumPolygon = 0;		//ポリゴン数
	m_nNumIndex = 0;		//インデックス数
	m_vertical = 0;			//垂直方向の分割数

	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ
	m_pIdxBuff = nullptr;	//インデックスバッファへのポインタ
}

//=======================
// デストラクタ
//=======================
CSky::~CSky()
{

}

//=======================
// 初期化
//=======================
HRESULT CSky::Init(D3DXVECTOR3 pos)
{
	m_vertical = 20;		//垂直方向の分割数

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//----------------------------------
	// メッシュを構成する情報の設定
	//----------------------------------
	m_nNumVtx = (m_vertical + 1) * (m_vertical + 1);						//頂点数
	m_nNumPolygon = m_vertical * m_vertical * 2 + (m_vertical - 1) * 4;		//ポリゴン数
	m_nNumIndex = (m_vertical + 1) * 2 * m_vertical + (m_vertical - 1) * 2;	//インデックス数

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
	float rotDiffZ = (D3DX_PI * 2.0f / (m_vertical - 1));	//Z方向の分割した一片

	for (int z = 0; z < m_vertical; z++)
	{
		float rotDiffX = (D3DX_PI * 2.0f / (m_vertical - 1));	//X方向の分割した一片
		float rotZ = rotDiffZ * (z + 1);			//Zの角度を設定

		for (int x = 0; x < m_vertical; x++)
		{
			float rotX = rotDiffX * (x + 1);		//xの角度を設定

			//頂点座標の設定
			pVtx[0].pos.x = sinf(rotX) * sinf(rotZ) * 100.0f;
			pVtx[0].pos.y = cosf(rotX) * 100.0f;
			pVtx[0].pos.z = sinf(rotX) * cosf(rotZ) * 100.0f;

			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(cosf(rotX), 0.0f, sinf(rotX));

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			pVtx++;
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

	for (int Z = 0; Z < m_vertical; Z++)
	{
		for (int X = 0; X < m_vertical; X++)
		{
			//インデックスバッファの設定
			pIdx[nCnt] = (WORD(X + Z * (m_vertical)+(m_vertical)));
			pIdx[nCnt + 1] = (WORD(X + Z * (m_vertical)));

			nCnt += 2;

			//縮退ポリゴンの追加
			if (X == m_vertical)
			{
				pIdx[nCnt] = X + Z * m_vertical;
				pIdx[nCnt + 1] = (Z + 2) * m_vertical;

				nCnt += 2;
			}
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=======================
// 終了
//=======================
void CSky::Uninit()
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
void CSky::Update()
{

}

//=======================
// 描画
//=======================
void CSky::Draw()
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

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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
								  m_nNumVtx,			//頂点数
								  0,
								  m_nNumPolygon);		//描画するプリミティブ数

	//テクスチャのリセット
	pDevice->SetTexture(0, NULL);

	//ライトの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================
// 生成
//===========================
CSky* CSky::Create(CTexture::TEXTURE texture)
{
	CSky *pSky = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pSky = new CSky;	//生成

	if (pSky != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pSky->m_texture = texture;
		
		//初期化
		pSky->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pSky->SetObjType(OBJTYPE_PLAYER);
	}

	return pSky;
}

//===========================
// 頂点の法線を設定
//===========================
void CSky::SetVtxNor(VERTEX_3D* pVtx, WORD* pIdx)
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