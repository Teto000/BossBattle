//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "sky.h"
#include "application.h"
#include "renderer.h"

//========================
// コンストラクタ
//========================
CSky::CSky() : CObjectX(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
}

//========================
// デストラクタ
//========================
CSky::~CSky()
{

}

//========================
// 初期化
//========================
HRESULT CSky::Init(D3DXVECTOR3 pos)
{
	CObjectX::Init(pos);

	CObjectX::SetFireName("data\\MODEL\\sky.x");

	return S_OK;
}

//========================
// 終了
//========================
void CSky::Uninit()
{
	CObjectX::Uninit();
}

//========================
// 更新
//========================
void CSky::Update()
{
	CObjectX::Update();
}

//========================
// 描画
//========================
void CSky::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ライトを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CObjectX::Draw();

	//ライトの有効化
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================
// 生成
//========================
CSky* CSky::Create()
{
	CSky *pSky = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pSky = new CSky;	//生成

	if (pSky != nullptr)
	{//NULLチェック
		//初期化
		pSky->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//pSky->SetObjType(OBJTYPE_PLAYER);
	}

	return pSky;
}