//===================================================
//
// オブジェクト3Dの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "object3d.h"
#include "main.h"
#include "renderer.h"
#include "application.h"
#include "input_keyboard.h"
#include "texture.h"

//------------------------
// マクロ定義
//------------------------
#define	ROT		(0.05f)		//回転角度の増加量
#define	SCAL	(0.01f)		//拡縮の値の増加量

//===========================
// コンストラクタ
//===========================
CObject3D::CObject3D() : CObject3D(0)
{
}

CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_Depth = 0.0f;		//奥行き
	m_fRot = 0.0f;		//回転角度
	m_fScal = 0.0f;		//大きさ
}

//===========================
// デストラクタ
//===========================
CObject3D::~CObject3D()
{
	assert(m_pVtxBuff == nullptr);
}

//===========================
// 初期化
//===========================
HRESULT CObject3D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置
	m_pos = pos;

	//------------------------
	// 頂点座標の設定
	//------------------------
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, m_Depth / 2);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, m_Depth / 2);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, -m_Depth / 2);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, -m_Depth / 2);

	//-----------------------------------------------------------------
	// 各頂点の法線の設定(※ベクトルの大きさは必ず1にする必要がある)
	//-----------------------------------------------------------------
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//------------------------
	// 頂点カラーの設定
	//------------------------
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//------------------------
	// テクスチャ座標の設定
	//------------------------
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================
// 終了
//===========================
void CObject3D::Uninit()
{
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
void CObject3D::Update()
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点座標の設定
	//------------------------
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, m_Depth / 2);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, m_Depth / 2);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, -m_Depth / 2);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, -m_Depth / 2);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 描画
//===========================
void CObject3D::Draw()
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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
						   0,					//描画する最初の頂点インデックス
						   2);					//描画するプリミティブ数

	//テクスチャのリセット
	pDevice->SetTexture(0, NULL);
}

//===========================
// 位置の設定
//===========================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 向きの設定
//===========================
void CObject3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
// 色の設定
//===========================
void CObject3D::SetColor(D3DXCOLOR col)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点カラーの設定
	//------------------------
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 大きさの設定
//===========================
void CObject3D::SetSize(float fWidth, float fHeight, float fDepth)
{
	m_fWidth = fWidth;		//幅の設定
	m_fHeight = fHeight;	//高さの設定
	m_Depth = fDepth;		//奥行きの設定
}

//===========================
// 頂点情報の設定
//===========================
void CObject3D::SetVtx(VERTEX_3D* vtx)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx = vtx;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// テクスチャ座標の設定
//===========================
void CObject3D::SetTexCIE(float left, float fRight)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(left, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fRight, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(left, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fRight, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// テクスチャの設定
//===========================
void CObject3D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
// αブレンディングを減算合成に設定
//=========================================
void CObject3D::SetSubALPHA()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//=========================================
// αブレンディングを元に戻す
//=========================================
void CObject3D::ResetSubALPHA()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CObject3D::GetPosition()
{
	return m_pos;
}

//===========================
// 幅の取得
//===========================
float CObject3D::GetWidth()
{
	return m_fWidth;
}

//===========================
// 高さの取得
//===========================
float CObject3D::GetHeight()
{
	return m_fHeight;
}