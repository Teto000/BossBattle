//===================================================
//
// ���[�h�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _MODE_H_
#define _MODE_H_	

//-------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// �O���錾
//--------------------------------
class CGame;		//�Q�[��
class CTitle;		//�^�C�g��
class CTutorial;	//�`���[�g���A��
class CResult;		//���U���g
class CFade;		//�t�F�[�h

//================================
// ���[�h�N���X�̒�`
//================================
class CMode
{
public:
	//----------------------------
	// ���[�h�񋓌^�̒�`
	//----------------------------
	enum MODE
	{
		MODE_TITLE = 0,		//�^�C�g��
		MODE_TUTORIAL,		//�`���[�g���A��
		MODE_GAME,			//�Q�[��
		MODE_RESULT,		//���U���g
		MODE_FADE,			//�t�F�[�h
		MODE_MAX
	};

	CMode();	//�R���X�g���N�^
	~CMode();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	static HRESULT Init();
	static void Uninit();
	static void Update();

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static void SetMode(MODE mode);		//���[�h�̐ݒ�
	static MODE GetMode()		{ return m_mode; };		//���[�h�̎擾
	static CGame*	GetGame()	{ return m_pGame; }		//�Q�[���̎擾
	static CTitle*	GetTitle()	{ return m_pTitle; }	//�^�C�g���̎擾
	static CFade*	GetFade()	{ return m_pFade; }		//�t�F�[�h�̎擾

private:
	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CGame*		m_pGame;		//�Q�[���N���X
	static CTitle*		m_pTitle;		//�^�C�g���N���X
	static CTutorial*	m_pTutorial;	//�`���[�g���A��
	static CResult*		m_pResult;		//���U���g�N���X
	static CFade*		m_pFade;		//�t�F�[�h�N���X
	static MODE			m_mode;			//���[�h
};

#endif