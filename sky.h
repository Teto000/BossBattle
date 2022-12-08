//===================================================
//
// �X�J�C�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _SKY_H_
#define _SKY_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "objectX.h"

//================================
// �X�J�C�w�b�_�[�̒�`
//================================
class CSky : public CObjectX
{
public:
	CSky();				//�R���X�g���N�^
	~CSky() override;	//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition()override { return m_pos; }	//�ʒu�̎擾
	float GetWidth() override { return 0; };			//���̎擾
	float GetHeight() override { return 0; };			//�����̎擾

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CSky* Create();

private:
	//----------------
	// �����o�ϐ�
	//----------------
	D3DXVECTOR3 m_pos;		//�ʒu
};

#endif#pragma once
