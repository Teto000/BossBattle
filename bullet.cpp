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
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "damage.h"
#include "sound.h"

//====================================
// コンストラクタ
//====================================
CBullet::CBullet() : CBillBoard(0)
{
}
CBullet::CBullet(int nPriority) : CBillBoard(nPriority)
{
	nLife = 0;	//寿命
	m_pDamage = nullptr;	//ダメージ
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
	nLife = 100;
	m_fWidth = 50.0f;
	m_fHeight = 50.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CBillBoard::Init(pos);

	CBillBoard::SetPos(pos);
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

	//------------------------------
	// 敵に向かって移動
	//------------------------------
	//敵の位置を取得
	D3DXVECTOR3 enemyPos = CGame::GetEnemy()->GetPosition();
	enemyPos.y += 200.0f;

	//ベクトルを求める
	D3DXVECTOR3 vec(enemyPos - m_pos);

	//ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	//プレイヤーに向かって移動
	m_move = vec * 10.0f;
	m_pos += m_move;

	//------------------------------
	// 敵との当たり判定
	//------------------------------
	//敵の大きさの取得
	D3DXVECTOR3 enemySize = CGame::GetEnemy()->GetSize();

	//敵の端の設定
	float fLeft = enemyPos.x - enemySize.x;
	float fRight = enemyPos.x + enemySize.x;
	float fTop = enemyPos.y + enemySize.y;
	float fBottom = enemyPos.y - enemySize.y;
	float fFront = enemyPos.z - enemySize.z;
	float fBack = enemyPos.z + enemySize.z;

	//--------------------------------
	// プレイヤーとの当たり判定
	//--------------------------------
	float fRange = 30.0f;	//当たり判定の範囲
	int nDamage = rand() % 3 + 1;		//ダメージ数
	
	//ダメージの表示位置をランダムに設定
	int nWidth = 50;
	int nRandX = rand() % nWidth - (nWidth / 2);
	int nRandY = rand() % nWidth - (nWidth / 2);
	D3DXVECTOR3 damagePos(m_pos.x + (float)nRandX
						  , m_pos.y + (float)nRandY
						  , m_pos.z);

	if (m_pos.x + fRange >= fLeft
		&& m_pos.x - fRange <= fRight
		&& m_pos.y + fRange >= fBottom
		&& m_pos.y - fRange <= fTop
		&& m_pos.z + fRange >= fFront
		&& m_pos.z - fRange <= fBack)
	{//プレイヤーが範囲内に入ったら
		CGame::GetEnemy()->SubGauge(nDamage, CEnemy::GAUGE_HP);	//敵の体力の減少
		CGame::GetPlayer()->AddCombo(1);	//コンボの加算

		//ダメージ数
		m_pDamage = CDamage::Create(damagePos, nDamage, CDamage::DAMAGE_TYPE_NONE);

		//SEの再生
		CSound::PlaySound(CSound::SOUND_LABEL_SE_HIT);

		Uninit();
		return;
	}

	//------------------------------
	// 弾が消える処理
	//------------------------------
	nLife--;

	if (nLife <= 0)
	{//寿命が尽きたら
		Uninit();
		return;
	}
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
CBullet* CBullet::Create(D3DXVECTOR3 pos)
{
	CBullet *pBullet = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pBullet = new CBullet;	//生成

	if (pBullet != nullptr)
	{//NULLチェック
		//初期化
		pBullet->Init(D3DXVECTOR3(pos));
	}

	return pBullet;
}