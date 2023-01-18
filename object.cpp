//===================================================
//
// オブジェクトの処理
// Author : Sato Teruto
//
//===================================================

//--------------------
// インクルード
//--------------------
#include "object.h"
#include "object3d.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CObject* CObject::m_Top[nMaxPriority] = {};		//最初のオブジェクト
CObject* CObject::m_Current[nMaxPriority] = {};	//最後のオブジェクト

//=============================
// コンストラクタ
//=============================
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;	//プライオリティの番号
	m_bDeath = false;			//死亡フラグ

	//-----------------------------
	// 先頭のオブジェクトを設定
	//-----------------------------
	if (m_Top[m_nPriority] == nullptr)
	{//先頭がないなら
		//先頭のオブジェクトに代入
		m_Top[m_nPriority] = this;
		m_pPrev = nullptr;
	} 

	//-----------------------------
	// 最後のオブジェクトを設定
	//-----------------------------
	if (m_pNext == nullptr)
	{//1つ後ろがnullなら
		//最後のオブジェクトに代入
		m_pPrev = m_Current[m_nPriority];
		m_Current[m_nPriority] = this;

		if (m_pPrev)
		{
			m_pPrev->m_pNext = this;
		}
	}
}

//=============================
// デストラクタ
//=============================
CObject::~CObject()
{

}

//=============================
// 全て解放
//=============================
void CObject::ReleaseAll(bool bFinish)
{
	for (int i = 0; i < nMaxPriority; i++)
	{
		if (!m_Top[i])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[i];

		while (pObj)
		{//pObjがnullじゃないなら
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			//終了処理
			pObj->Uninit();
			pObj->Release();

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}

		//-------------------
		// 死亡処理
		//-------------------
		pObj = m_Top[i];

		while (pObj)
		{
			if (bFinish == true)
			{
				//次のオブジェクトを保存
				CObject* pObjNext = pObj->m_pNext;

				if (pObj->m_bDeath == true)
				{
					//消去処理
					pObj->Death(pObj);
				}

				//次のオブジェクトのアドレスを代入
				pObj = pObjNext;
			}
		}
	}
}

//=============================
// 全て更新
//=============================
void CObject::UpdateAll()
{
	for (int i = 0; i < nMaxPriority; i++)
	{
		if (!m_Top[i])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[i];

		while (pObj)
		{//pObjがnullじゃないなら
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			//更新処理
			if (pObj->m_bDeath != true)
			{
				pObj->Update();
			}

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}

		//-------------------
		// 死亡処理
		//-------------------
		pObj = m_Top[i];

		while (pObj)
		{
			//次のオブジェクトを保存
			CObject* pObjNext = pObj->m_pNext;

			if (pObj->m_bDeath == true)
			{
				//消去処理
				pObj->Death(pObj);
			}

			//次のオブジェクトのアドレスを代入
			pObj = pObjNext;
		}
	}
}

//=============================
// 全て描画
//=============================
void CObject::DrawAll()
{
	for (int i = 0; i < nMaxPriority; i++)
	{
		if (!m_Top[i])
		{//Topがnullなら
			continue;
		}

		CObject* pObj = m_Top[i];

		while (pObj)
		{//pObjがnullじゃないなら
			//描画処理
			pObj->Draw();

			//次のオブジェクトのアドレスを代入
			pObj = pObj->m_pNext;
		}
	}
}

//=============================
// 解放する
//=============================
void CObject::Release()
{
	//死亡フラグを立てる
	m_bDeath = true;

	return;
}

//=============================
// 死亡処理
//=============================
void CObject::Death(CObject* pObj)
{
	//----------------------------------
	// 前後のオブジェクトの接続
	//----------------------------------
	if (m_pPrev != nullptr)
	{//1つ前がnullじゃないなら
	 //前のNextに次を代入
		m_pPrev->m_pNext = m_pNext;
	}

	if (m_pNext != nullptr)
	{//1つ後がnullじゃないなら
	 //次のPrevに前を代入
		m_pNext->m_pPrev = m_pPrev;
	}

	//----------------------------------
	// 先頭と最後のオブジェクトを設定
	//----------------------------------
	if (m_pPrev == nullptr)
	{//自分が先頭なら
		//次のオブジェクトを先頭にする
		m_Top[m_nPriority] = m_pNext;
	}

	if (m_pNext == nullptr)
	{//自分が最後なら
		//次のオブジェクトを最後にする
		m_Current[m_nPriority] = m_pPrev;
	}

	pObj->Uninit();

	delete pObj;
	pObj = nullptr;
}

//=============================
// 総数の取得
//=============================
int CObject::GetNumAll()
{
	return 0;
}

//========================================
// オブジェクトの取得
// 引数：目的の種類、プライオリティ番号
//========================================
CObject *CObject::GETObject(EObjType type, int nNum)
{
	CObject* pObj = m_Top[nNum];	//pObjを先頭にする

	while (pObj)
	{//pObjがnullじゃないなら
		//次のオブジェクトのアドレスを代入
		pObj = pObj->m_pNext;

		if (pObj && pObj->m_ObjType == type)
		{//オブジェクトがnullじゃない & 目的の種類なら
			return pObj;
		}
	}

	return nullptr;
}

//=============================
// オブジェクトの種類の設定
//=============================
void CObject::SetObjType(EObjType ObjType)
{
	m_ObjType = ObjType;
}

//=============================
// オブジェクトの種類の取得
//=============================
CObject::EObjType CObject::GetObjType()
{
	return m_ObjType;
}