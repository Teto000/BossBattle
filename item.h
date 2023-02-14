//======================================================
//
// アイテムヘッダー
// Author : Sato Teruto
//
//======================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//-------------------------------
// インクルード
//-------------------------------
#include "billboard.h"

//======================================
// アイテムクラス
//======================================
class CItem : public CBillBoard
{
public:
	//アイテムの種類
	enum ITEMTYPE
	{
		ITEMTYPE_NONE = 0,
		ITEMTYPE_HEAL,
		ITEMTYPE_MAX
	};

	CItem();			//コンストラクタ
	~CItem() override;	//デストラクタ

	//-------------------
	// メンバ関数
	//-------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//-------------------
	// 静的メンバ関数
	//-------------------
	static CItem* Create(D3DXVECTOR3 pos, ITEMTYPE type);

private:
	//-------------------
	// メンバ変数
	//-------------------
	ITEMTYPE m_type;	//種類
};

#endif // !_ITEM_H_
