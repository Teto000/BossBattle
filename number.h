//===================================================
//
// ���l�\���p�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"

//================================
// �i���o�[�N���X
//================================
class CNumber : public CObject2D
{
public:
	CNumber();				//�R���X�g���N�^
	~CNumber() override;	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// �Z�b�^�[
	//------------------
	void Set(int nDigit);	//���l�̐ݒ�

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CNumber* Create(D3DXVECTOR3 pos, int nValue);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	int m_nValue;	//���l
	int m_nDigit;	//����
};

#endif