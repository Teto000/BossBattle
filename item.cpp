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
#include "debug_proc.h"
#include "application.h"
#include "renderer.h"

//===============================================
// コンストラクタ
//===============================================
CItem::CItem() : CBillBoard(0)
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
	m_fWidth = 100.0f;
	m_fHeight = 100.0f;

	CBillBoard::Init(pos);
	CBillBoard::SetSize(m_fWidth, m_fHeight);
	CBillBoard::SetTexture(CTexture::TEXTURE_ITEM_HEAL);
	CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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

	//CDebugProc::Print("アイテムの位置 %f %f %f", m_pos.x, m_pos.y, m_pos.z);

	//--------------------------------
	// プレイヤーの情報を取得
	//--------------------------------
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();
	D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();

	//--------------------------------
	// 当たり判定
	//--------------------------------
	if (Collision(playerPos, playerSize))
	{//プレイヤーと当たったら
		switch (m_type)
		{
		case ITEMTYPE_HEAL:
			CGame::GetPlayer()->AddLife(100);
			break;

		case ITEMTYPE_ATTACK:
			break;

		case ITEMTYPE_SPEED:
			break;

		case ITEMTYPE_COMBO:
			break;

		default:
			break;
		}

		//アイテムの消去
		Uninit();
	}
}

//===============================================
// 描画
//===============================================
void CItem::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Zバッファのクリア
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);

	//オブジェクトの描画
	CBillBoard::Draw();

	// Zバッファの有効化設定
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);
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

//===============================================
// 当たり判定
// 引数：相手の位置、相手の大きさ
//===============================================
bool CItem::Collision(D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize)
{
	//--------------------------------
	// プレイヤーの端の設定
	//--------------------------------
	float fLeft = targetPos.x - targetSize.x;
	float fRight = targetPos.x + targetSize.x;
	float fTop = targetPos.y + targetSize.y;
	float fBottom = targetPos.y - targetSize.y;
	float fFront = targetPos.z - targetSize.z;
	float fBack = targetPos.z + targetSize.z;

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
		return true;
	}

	return false;
}