//===================================================
//
// ビルボードの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "billboard.h"
#include "main.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"

//------------------------
// マクロ定義
//------------------------
#define	ROT		(0.05f)		//回転角度の増加量
#define	SCAL	(0.01f)		//拡縮の値の増加量

//===========================
// コンストラクタ
//===========================
CBillBoard::CBillBoard() : CBillBoard(0)
{
}

CBillBoard::CBillBoard(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_fRot = 0.0f;		//回転角度
	m_fScal = 0.0f;		//大きさ
}

//===========================
// デストラクタ
//===========================
CBillBoard::~CBillBoard()
{
	assert(m_pVtxBuff == nullptr);
}

//===========================
// 初期化
//===========================
HRESULT CBillBoard::Init(D3DXVECTOR3 pos)
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
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, 0.0f);

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
void CBillBoard::Uninit()
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
void CBillBoard::Update()
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点座標の設定
	//------------------------
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 描画
//===========================
void CBillBoard::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//------------------------------------
	// マトリックス
	//------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスの取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ライトを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

	//ライトをの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//テクスチャの引継ぎオフ
	pDevice->SetTexture(0, NULL);
}

//===========================
// 移動量の加算
// 引数：移動量
//===========================
D3DXVECTOR3 CBillBoard::AddMove(D3DXVECTOR3 move)
{
	m_pos += move;

	return m_pos;
}

//===========================
// 大きさの設定
// 引数：幅、高さ
//===========================
void CBillBoard::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;		//幅の設定
	m_fHeight = fHeight;	//高さの設定
}

//===========================
// テクスチャ座標の設定
// 引数：左、右
//===========================
void CBillBoard::SetTexCIE(float left, float fRight)
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

//=========================================
// アニメーションの設定
// 引数：数値、テクスチャの分割数
//=========================================
void CBillBoard::SetAnimation(float fNumber, int nPattern)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	float PatternSize = 1.0f / nPattern;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//texの設定
	pVtx[0].tex = D3DXVECTOR2(fNumber * PatternSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fNumber * PatternSize + PatternSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fNumber * PatternSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fNumber * PatternSize + PatternSize, 1.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 頂点座標の設定(ゲージ)
// 引数：左、右、上、下
//===========================
void CBillBoard::SetVtxCIE_Gauge(float fLeft, float fRight, float fUp, float fDown)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(fLeft, fUp, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft, fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 頂点カラーの設定
// 引数：色
//===========================
void CBillBoard::SetColor(D3DXCOLOR col)
{
	//色の設定
	m_col = col;

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// テクスチャの設定
// 引数：テクスチャ名
//===========================
void CBillBoard::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}