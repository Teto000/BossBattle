//===================================================
//
// 2Dポリゴンヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_	

//--------------------------------
// インクルード
//--------------------------------
#include "object2D.h"

//--------------------------------
// バレットクラス
//--------------------------------
class CPolygon : public CObject2D
{
public:
	enum POLYGONTYPE
	{
		POLYGONTYPE_NONE = 0,
		POLYGONTYPE_AIM,		//照準
		POLYGONTYPE_MAX
	};

	CPolygon();				//コンストラクタ
	~CPolygon() override;	//デストラクタ

	//-------------------
	// メンバ関数
	//-------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()	  override;

	//-------------------
	// 静的メンバ関数
	//-------------------
	static CPolygon* Create(POLYGONTYPE type);	//生成
	
private:
	//-------------------
	// メンバ変数
	//-------------------
	float		m_fRadius;	//半径
	POLYGONTYPE m_type;		//種類
};

#endif // !_BG_H_