//===================================================
//
// メッセージの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "message.h"
#include "renderer.h"
#include "game.h"
#include "enemy.h"

//=======================
// コンストラクタ
//=======================
CMessage::CMessage() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
	m_nCntTime = 0;		//時間カウント
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_message = MESSAGE_NONE;
}

//=======================
// デストラクタ
//=======================
CMessage::~CMessage()
{

}

//=======================
// 初期化
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 800.0f;
	m_fHeight = 400.0f;

	CObject2D::Init(m_pos);

	CObject2D::SetSize(m_fWidth, m_fHeight);

	switch (m_message)
	{
	case MESSAGE_CLEAR:
		CObject2D::SetTexture(CTexture::TEXTURE_CLEAR);
		break;

	case MESSAGE_BREAK:
		CObject2D::SetTexture(CTexture::TEXTURE_BREAK);
		break;

	default:
		break;
	}

	return S_OK;
}

//=======================
// 終了
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CMessage::Update()
{
	CObject2D::Update();

	//時間カウント
	if (CGame::GetFinish()
		|| CGame::GetEnemy()->GetState() == CEnemy::ENEMYSTATE_BREAK)
	{//終了フラグが立っている || 敵がブレイク状態なら
		m_nCntTime++;
	}

	//-------------------------
	// 透明にする
	//-------------------------
	if (m_nCntTime >= 100)
	{//カウントが60以上なら
		m_col.a -= 0.02f;	//透明度の減少
	}

	if (m_col.a < 0.0f)
	{//完全に透明なら
		m_nCntTime = 0;
	}

	//色の設定
	CObject2D::SetColor(m_col);
}

//=======================
// 描画
//=======================
void CMessage::Draw()
{
	switch (m_message)
	{
	case MESSAGE_CLEAR:
		if (CGame::GetFinish())
		{//終了フラグが立っているなら
			CObject2D::Draw();
		}
		break;

	case MESSAGE_BREAK:
		if (CGame::GetEnemy()->GetState() == CEnemy::ENEMYSTATE_BREAK)
		{//敵がブレイク状態なら
			CObject2D::Draw();
		}
		else
		{//敵がブレイク状態じゃなくなったら
			m_col.a = 1.0f;	//透明度を最大にする
			CObject2D::SetColor(m_col);	//色の設定
		}
		break;

	default:
		break;
	}
}

//=======================
// 生成
// 引数：位置、種類
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos, MESSAGE message)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pMessage = new CMessage;	//生成

	if (pMessage != nullptr)
	{//NULLチェック
		//引数の代入
		pMessage->m_message = message;	//列挙型

		//初期化
		pMessage->Init(D3DXVECTOR3(pos));
		pMessage->SetObjType(OBJTYPE_MESSAGE);
	}

	return pMessage;
}