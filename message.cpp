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

	CObject2D::SetTexture(CTexture::TEXTURE_CLEAR);

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
	if (CGame::GetFinish())
	{//終了フラグが立っているなら
		m_nCntTime++;
	}

	//-------------------------
	// 透明にする
	//-------------------------
	if (m_nCntTime >= 100)
	{//カウントが60以上なら
		m_col.a -= 0.02f;	//透明度の減少
	}

	//色の設定
	CObject2D::SetColor(m_col);

	if (m_col.a <= 0.0f)
	{//完全に透明になったら
		Uninit();
	}
}

//=======================
// 描画
//=======================
void CMessage::Draw()
{
	if (CGame::GetFinish())
	{//終了フラグが立っているなら
		CObject2D::Draw();
	}
}

//=======================
// 生成
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