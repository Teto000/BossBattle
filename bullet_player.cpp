//==============================================
//
// バレットの処理
// Author : Sato Teruto
//
//==============================================

//----------------------------
// インクルード
//----------------------------
#include "bullet_player.h"
#include "bullet.h"
#include "camera.h"

//====================================
// コンストラクタ
//====================================
CBulletPlayer::CBulletPlayer() : CBullet(0)
{

}

//====================================
// デストラクタ
//====================================
CBulletPlayer::~CBulletPlayer()
{

}

//====================================
// 初期化
//====================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fWidth = 50.0f;
	m_fHeight = 50.0f;

	CBullet::Init(pos);

	CBullet::SetPos(pos);
	CBullet::SetMove(m_move);
	CBullet::SetSize(m_fWidth, m_fHeight);

	return S_OK;
}

//====================================
// 終了
//====================================
void CBulletPlayer::Uninit()
{
	CBullet::Uninit();
}

//====================================
// 更新
//====================================
void CBulletPlayer::Update()
{
	CBullet::Update();
}

//====================================
// 描画
//====================================
void CBulletPlayer::Draw()
{
	CBullet::Draw();
}

//====================================
// 生成
//====================================
CBulletPlayer* CBulletPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot)
{
	CBulletPlayer *pBulletPlayer = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pBulletPlayer = new CBulletPlayer;	//生成

	if (pBulletPlayer != nullptr)
	{//NULLチェック
		//移動量
		pBulletPlayer->m_move = D3DXVECTOR3(sinf(playerRot.x) * sinf(playerRot.y) * 5,
											cosf(playerRot.x) * 5,
											sinf(playerRot.x) * cosf(playerRot.y) * 5);

		//初期化
		pBulletPlayer->Init(D3DXVECTOR3(pos));
	}

	return pBulletPlayer;
}