//===================================
//
// オブジェクト2Dの処理
// Author : Sato Teruto
//
//===================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "object2D.h"
#include "main.h"
#include "renderer.h"
#include "application.h"
#include "input_keyboard.h"

//------------------------
// マクロ定義
//------------------------
#define	ROT		(0.05f)		//回転角度の増加量
#define	SCAL	(0.01f)		//拡縮の値の増加量

//===========================
// コンストラクタ
//===========================
CObject2D::CObject2D() : CObject2D(0)
{
}

CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_fRot = 0.0f;		//回転角度
	m_fScal = 0.0f;		//大きさ
}

//===========================
// デストラクタ
//===========================
CObject2D::~CObject2D()
{
	assert(m_pVtxBuff == nullptr);
}

//===========================
// 初期化
//===========================
HRESULT CObject2D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置
	m_pos = pos;

	//------------------------
	// 頂点座標の設定
	//------------------------
	pVtx[0].pos = m_pos + D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, 0.0f);

	//------------------------
	// rhwの設定
	//------------------------
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
void CObject2D::Uninit()
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
void CObject2D::Update()
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
  	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点座標の設定
	//------------------------
	pVtx[0].pos = m_pos + D3DXVECTOR3(-m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(m_fWidth / 2, -m_fHeight / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(m_fWidth / 2, m_fHeight / 2, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 描画
//===========================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
						   0,					//描画する最初の頂点インデックス
						   2);					//描画するプリミティブ数

	//テクスチャのリセット
	pDevice->SetTexture(0, NULL);
}

//===========================
// 位置の設定
//===========================
void CObject2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 移動量の加算
//===========================
D3DXVECTOR3 CObject2D::AddMove(D3DXVECTOR3 move)
{
	m_pos += move;

	return m_pos;
}

//===========================
// 大きさの設定
//===========================
void CObject2D::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;		//幅の設定
	m_fHeight = fHeight;	//高さの設定
}

//===========================
// 色の設定
//===========================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

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
// 左右画面端の設定
//===========================
void CObject2D::SetScreenX(float posX, float fLeft, float fRight)
{
	if (posX <= fLeft)
	{
		posX = fLeft;
	}
	else if (posX >= fRight)
	{
		posX = fRight;
	}

	m_pos.x = posX;
}

//===========================
// 上下画面端の設定
//===========================
void CObject2D::SetScreenY(float posY, float fUp, float fDown)
{
	if (posY <= fUp)
	{
		posY = fUp;
	}
	else if (posY >= fDown)
	{
		posY = fDown;
	}

	m_pos.y = posY;
}

//===========================
// 画面外に出た処理
//===========================
bool CObject2D::OutScreen(D3DXVECTOR3 pos)
{
	if (pos.x <= 0.0f || pos.x >= SCREEN_WIDTH)
	{
		return true;
	}
	else if (pos.y <= 0.0f || pos.y >= SCREEN_HEIGHT)
	{
		return true;
	}

	return false;
}

//===========================
// テクスチャ座標の設定
//===========================
void CObject2D::SetTexCIE(float left ,float fRight)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

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
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//===========================
// 頂点座標の設定
//===========================
void CObject2D::SetVtxCIE(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==================================
// 頂点座標の設定(回転)
//==================================
void CObject2D::SetVtxCIE_Rot(D3DXVECTOR3 pos, float rotX, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) / 2;	//対角線の長さを算出する
	float fAngle = atan2f(fWidth, fHeight);								//対角線の角度を算出

	//頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rotX + (D3DX_PI + fAngle)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rotX + (D3DX_PI + fAngle)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rotX + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rotX + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rotX - (0 + fAngle)) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rotX - (0 + fAngle)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rotX - (0 - fAngle)) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rotX - (0 - fAngle)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 頂点座標の設定(ゲージ)
//===========================
void CObject2D::SetVtxCIE_Gauge(D3DXVECTOR3 pos, 
	float fLeft,float fRight, float fUp, float fDown)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(fLeft, fUp, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fRight, fUp, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(fLeft, fDown, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fRight, fDown, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================
// 頂点座標の設定(スコア)
//===========================
void CObject2D::SetVtxCIE_Score(D3DXVECTOR3 pos, float fWidth, float fHeight,float fSpace)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);

	pos.x += fSpace;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
// αブレンディングを加算合成に設定
//=========================================
void CObject2D::SetAddALPHA()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//=========================================
// アニメーションの設定
// 引数：数値、テクスチャの分割数
//=========================================
void CObject2D::SetAnimation(float fNumber, int nPattern)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

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

//=========================================
// αブレンディングを元に戻す
//=========================================
void CObject2D::ResAddALPHA()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================
// 円形に動かす
//===========================
D3DXVECTOR3 CObject2D::MoveCircle(D3DXVECTOR3 CenterPos, float fAngle, float fLength)
{
	//角度の上側を0にする
	float fNewAngle = D3DXToRadian(fAngle) - D3DXToRadian(90);

	//円の位置までの距離を求める
	float fDistX = (float)(cos(fNewAngle) * fLength);
	float fDistY = (float)(sin(fNewAngle) * fLength);

	//円の位置に移動する
	m_pos.x = CenterPos.x + fDistX;
	m_pos.y = CenterPos.y + fDistY;

	return m_pos;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CObject2D::GetPosition()
{
	return m_pos;
}

//===========================
// 相手の位置の取得
//===========================
D3DXVECTOR3 CObject2D::GetTargetPos()
{
	return m_TargetPos;
}

//===========================
// 幅の取得
//===========================
float CObject2D::GetWidth()
{
	return m_fWidth;
}

//===========================
// 高さの取得
//===========================
float CObject2D::GetHeight()
{
	return m_fHeight;
}