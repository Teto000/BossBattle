//==============================================
//
// バレットの処理
// Author : Sato Teruto
//
//==============================================

//----------------------------
// インクルード
//----------------------------
#include "bullet.h"
#include "application.h"
#include "renderer.h"

//====================================
// コンストラクタ
//====================================
CBullet::CBullet() : CBillBoard(0)
{
}
CBullet::CBullet(int nPriority) : CBillBoard(nPriority)
{

}

//====================================
// デストラクタ
//====================================
CBullet::~CBullet()
{

}

//====================================
// 初期化
//====================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fWidth = 50.0f;
	m_fHeight = 50.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CBillBoard::Init(pos);

	CBillBoard::SetPos(pos);
	CBillBoard::SetMove(m_move);
	CBillBoard::SetSize(m_fWidth, m_fHeight);
	CBillBoard::SetColor(m_col);
	CBillBoard::SetTexture(CTexture::TEXTURE_AIM);

	return S_OK;
}

//====================================
// 終了
//====================================
void CBullet::Uninit()
{
	CBillBoard::Uninit();
}

//====================================
// 更新
//====================================
void CBullet::Update()
{
	CBillBoard::Update();

	//位置に移動量を加算
	m_pos = CBillBoard::AddMove(m_move);
	CBillBoard::SetPos(m_pos);
}

//====================================
// 描画
//====================================
void CBullet::Draw()
{
	//オブジェクトの描画
	CBillBoard::Draw();
}

//====================================
// 生成
//====================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot)
{
	CBullet *pBullet = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pBullet = new CBullet;	//生成

	if (pBullet != nullptr)
	{//NULLチェック
		//移動量
		pBullet->m_move = D3DXVECTOR3(sinf(playerRot.x) * sinf(playerRot.y) * 5,
									  cosf(playerRot.x) * 5,
									  sinf(playerRot.x) * cosf(playerRot.y) * 5);

		//初期化
		pBullet->Init(D3DXVECTOR3(pos));
	}

	return pBullet;
}