//======================================================
//
// アイテムの処理
// Author : Sato Teruto
//
//======================================================

//-------------------------------
// インクルード
//-------------------------------
#include "item.h"
#include "utility.h"
#include "game.h"
#include "player.h"

//===============================================
// コンストラクタ
//===============================================
CItem::CItem()
{
	m_type = ITEMTYPE_NONE;		//種類
}

//===============================================
// デストラクタ
//===============================================
CItem::~CItem()
{

}

//===============================================
// 初期化
// 引数：位置
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fWidth = 70.0f;
	m_fHeight = 70.0f;

	CBillBoard::Init(pos);
	CBillBoard::SetSize(m_fWidth, m_fHeight);
	CBillBoard::SetTexture(CTexture::TEXTURE_CLEAR);

	return S_OK;
}

//===============================================
// 終了
//===============================================
void CItem::Uninit()
{
	CBillBoard::Uninit();
}

//===============================================
// 更新
//===============================================
void CItem::Update()
{
	CBillBoard::Update();

	//--------------------------------
	// プレイヤーの情報を取得
	//--------------------------------
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();
	D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();
	D3DXMATRIX playerMtx = CGame::GetPlayer()->GetmtxWorld();

	//--------------------------------
	// プレイヤーの端の設定
	//--------------------------------
	float fLeft = playerPos.x - playerSize.x;
	float fRight = playerPos.x + playerSize.x;
	float fTop = playerPos.y + playerSize.y;
	float fBottom = playerPos.y - playerSize.y;
	float fFront = playerPos.z - playerSize.z;
	float fBack = playerPos.z + playerSize.z;

	//--------------------------------
	// プレイヤーとの当たり判定
	//--------------------------------
	float fRange = 30.0f;	//当たり判定の範囲

	if (m_pos.x + fRange >= fLeft
		&& m_pos.x - fRange <= fRight
		&& m_pos.y + fRange >= fBottom
		&& m_pos.y - fRange <= fTop
		&& m_pos.z + fRange >= fFront
		&& m_pos.z - fRange <= fBack)
	{//プレイヤーが範囲内に入ったら
		Uninit();
	}
	
}

//===============================================
// 描画
//===============================================
void CItem::Draw()
{
	CBillBoard::Draw();
}

//===============================================
// 生成
// 引数：位置
//===============================================
CItem * CItem::Create(D3DXVECTOR3 pos, ITEMTYPE type)
{
	CItem* pItem = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pItem = new CItem;	//生成

	if (pItem != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pItem->m_type = type;

		//初期化
		pItem->Init(pos);
	}

	return pItem;
}
