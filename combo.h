//===================================================
//
// �R���{�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _COMBO_H_
#define _COMBO_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object.h"

//-------------------------------
// �O���錾
//-------------------------------
class CNumber;	//���l

//================================
// �i���o�[�N���X
//================================
class CCombo : CObject
{
public:
	CCombo();	//�R���X�g���N�^
	~CCombo();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	int AddNumber(int nNum);	//���l�̉��Z

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }	//�ʒu�̎擾
	float GetWidth() override { return 0.0f; }				//���̎擾
	float GetHeight() override { return 0.0f; }				//�����̎擾

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CCombo* Create(D3DXVECTOR3 pos, int nNum);

private:
	void SetNumber();			//���l�̐ݒ�

private:
	//------------------
	// �萔
	//------------------
	static const int nMaxDigits = 2;	//�ő包��

	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_pos;				//�ʒu
	int m_nNumCombo;				//�R���{��
	int m_aPosTexU[nMaxDigits];		//���̌��̐��l
	CNumber* m_pNumber[nMaxDigits];	//���l
};

#endif