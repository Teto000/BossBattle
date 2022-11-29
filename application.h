//===================================================
//
// �A�v���P�[�V�����w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_	

//------------------------------
// �C���N���[�h
//------------------------------
#include <d3dx9.h>

//------------------------------
// �O���錾
//------------------------------
class CGame;		//�Q�[��
class CTitle;		//�^�C�g��
class CResult;		//���U���g
class CFade;		//�t�F�[�h

class CRenderer;	//�����_���[
class CInput;		//�C���v�b�g
class CInputJoypad;	//�W���C�p�b�h
class CTexture;		//�e�N�X�`��
class CSound;		//�T�E���h
class CLight;		//���C�g
class CDebugProc;	//�f�o�b�O�p����

//================================
// �A�v���P�[�V�����N���X�̒�`
//================================
class CApplication
{
public:
	//--------------------------------
	// �Q�[�����[�h�̗񋓌^�̒�`
	//--------------------------------
	enum MODE
	{
		MODE_TITLE = 0,		//�^�C�g��
		MODE_GAME,			//�Q�[��
		MODE_RESULT,		//���U���g
		MODE_FADE,			//�t�F�[�h
		MODE_MAX
	};

	enum GAMEMODE
	{
		GAMEMODE_TIME = 0,
		GAMEMODE_SCORE,
		GAMEMODE_MAX
	};

	CApplication();		//�R���X�g���N�^
	~CApplication();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static void SetMode(MODE mode);		//���[�h�̐ݒ�
	static MODE GetMode();				//���[�h�̎擾

	static CRenderer *GetRenderer();	//�����_���[�̎擾
	static CInput	 *GetInput();		//�C���v�b�g�̎擾
	static CInputJoypad* GetJoypad();	//�W���C�p�b�h�̎擾
	static CTexture  *GetTexture();		//�e�N�X�`���̎擾
	static CSound	 *GetSound();		//�T�E���h�̎擾
	static CLight	 *GetLight();		//���C�g�̎擾

private:
	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CGame*		m_pGame;		//�Q�[���N���X
	static CTitle*		m_pTitle;		//�^�C�g���N���X
	static CResult*		m_pResult;		//���U���g�N���X
	static CFade*		m_pFade;		//�t�F�[�h�N���X
	static MODE			m_mode;			//���[�h
	static GAMEMODE		m_gameMode;		//�Q�[�����[�h

	static CRenderer*	m_pRenderer;	//�����_���[�N���X
	static CInput*		m_pInput;		//�C���v�b�g�N���X
	static CInputJoypad*m_pJoypad;		//�W���C�p�b�h
	static CTexture*	m_pTexture;		//�e�N�X�`���N���X
	static CSound*		m_pSound;		//�T�E���h�N���X
	static CLight*		m_pLight;		//���C�g�N���X
	static CDebugProc*	m_pDebugproc;	//�f�o�b�O�p����
};

#endif // !_APPLICATION_H_