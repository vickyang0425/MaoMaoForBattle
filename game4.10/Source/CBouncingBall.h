namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 硂class矗ㄑ穦紆铬瞴
	// 来碞э糶Θ祘Α
	/////////////////////////////////////////////////////////////////////////////

	class CBouncingBall
	{
	public:
		CBouncingBall();
		void LoadBitmap();		//载入地图
		void OnMove();			// 移动
		void OnShow();			//将图形贴到画面
		void SetFloor(int);     //设定弹回的水平面
		void SetXY(int, int);    //设定起始上升坐标
		void SetVelocity(int);    //设定上升起始速度

	private:
		int x, y;				// 图形坐标
		int floor;				// 地板的Y坐标
		bool rising;			// true表上升 false表下降
		int initial_velocity;	//初始速度
		int velocity;			// 目前的速度（点/次）
		CAnimation animation;	// 利用动画作图形
	};
}