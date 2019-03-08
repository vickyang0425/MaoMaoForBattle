namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѷ|�@���u�����y
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class CBouncingBall
	{
	public:
		CBouncingBall();
		void LoadBitmap();		//�����ͼ
		void OnMove();			// �ƶ�
		void OnShow();			//��ͼ����������
		void SetFloor(int);     //�趨���ص�ˮƽ��
		void SetXY(int, int);    //�趨��ʼ��������
		void SetVelocity(int);    //�趨������ʼ�ٶ�

	private:
		int x, y;				// ͼ������
		int floor;				// �ذ��Y����
		bool rising;			// true������ false���½��U��
		int initial_velocity;	//��ʼ�ٶ�
		int velocity;			// Ŀǰ���ٶȣ���/�Σ�
		CAnimation animation;	// �Q���ö�����ͼ��
	};
}