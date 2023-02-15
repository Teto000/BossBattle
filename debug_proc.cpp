//===================================================
//
// デバッグの処理
// Author : Sato Teruto
//			Arita Meigen
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <stdarg.h>
#include <string>
#include "debug_proc.h"
#include "main.h"
#include "renderer.h"
#include "application.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
LPD3DXFONT CDebugProc::m_pFont = nullptr;
std::string CDebugProc::m_Str = {};
int CDebugProc::m_nCntData = 0;

//===========================
// コンストラクタ
//===========================
CDebugProc::CDebugProc()
{

}

//===========================
// デストラクタ
//===========================
CDebugProc::~CDebugProc()
{

}

//===========================
// 初期化
//===========================
HRESULT CDebugProc::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);

	return S_OK;
}

//===========================
// 終了
//===========================
void CDebugProc::Uninit()
{
	//文字列の解放
	m_pFont = nullptr;
	m_Str[0] = NULL;
}

//===========================
// 文字列の登録
//===========================
void CDebugProc::Print(const char* pFormat, ...)
{
	va_list ap;	//可変引数構造体の定義

	va_start(ap, pFormat);	//可変引数構造体の初期化

	m_Str += "\n";

	for (int i = 0; i < (signed)strlen(pFormat); i++)
	{//文字列の数だけ回す
		if (pFormat[i] == '%')
		{//文字が%なら
			if (pFormat[i + 1] == 'd')
			{//次の値がdなら
				//-------------------
				// int型の値を結合
				//-------------------
				int type = va_arg(ap, int);	//引数から値を取り出す
				m_Str += std::to_string(type);
			}
			else if (pFormat[i + 1] == 'f')
			{//次の値がfなら
				//-------------------
				// float型の値を結合
				//-------------------
				double type = va_arg(ap, double);
				m_Str += std::to_string(type);
			}

			i++;	//文字番号の加算
		}
		else
		{
			m_Str += pFormat[i];	//文字を結合
		}
	}

	va_end(ap);	//可変引数構造体の処分
}

//===========================
// 描画
//===========================
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[1024];

	wsprintf(str, &m_Str[0]);

	// テキスト描画
	if (m_pFont != nullptr)
	{
		m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	//テキストリセット
	m_Str = {};
}