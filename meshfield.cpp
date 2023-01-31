//===================================================
//
// メッシュフィールドの処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "meshfield.h"
#include "renderer.h"

//=======================
// コンストラクタ
//=======================
CMeshField::CMeshField() : CMesh(0)
{

}

//=======================
// デストラクタ
//=======================
CMeshField::~CMeshField()
{

}

//=======================
// 初期化
//=======================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	CMesh::Init(pos);

	//分割数、幅の設定
	//CMesh::SetMeshSize(8, 400);

	CMesh::SetTexture(CTexture::TEXTURE_GROUND);

	return S_OK;
}

//=======================
// 終了
//=======================
void CMeshField::Uninit()
{
	CMesh::Uninit();
}

//=======================
// 更新
//=======================
void CMeshField::Update()
{
	CMesh::Update();
}

//=======================
// 描画
//=======================
void CMeshField::Draw()
{
	CMesh::Draw();
}

//=======================
// 生成
//=======================
CMeshField *CMeshField::Create()
{
	CMeshField *pMeshField = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pMeshField = new CMeshField;	//生成

	if (pMeshField != nullptr)
	{//NULLチェック
		//初期化
		{
			D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			pMeshField->Init(D3DXVECTOR3(pos));
		}
		pMeshField->SetObjType(OBJTYPE_MESH);
	}

	return pMeshField;
}