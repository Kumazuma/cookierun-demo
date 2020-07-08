#pragma once
class CObj;
struct IPlayeMoveState
{
	virtual void OnLoaded(CObj* const) = 0;
	virtual IPlayeMoveState* Update(CObj* const, float timedelta) = 0;
	virtual ~IPlayeMoveState() = default;
};
struct IPlayerCollisionState
{
	virtual void OnLoaded(CObj* const) = 0;
	virtual IPlayerCollisionState* Update(CObj* const, float timedelta) = 0;
	virtual IPlayerCollisionState* PostUpdate(CObj* const) = 0;
	virtual void Render(CObj* const, HDC) = 0;
	virtual ~IPlayerCollisionState() = default;
};
namespace State
{
	namespace Move
	{
		class Gravity : public IPlayeMoveState
		{
		public:
			IPlayeMoveState* Update(CObj* const, float timedelta) override;
		protected:
			Gravity();
			//중력에 따라 속도가 제어되는 것을 관리한다. 
			void GravityMove(CObj* const, float timedalta);
			//만약 땅에 닿았으면 true를 반환한다.
			bool IsReturn(CObj* const);
			float m_fSpeed;
		};
		class Default : public Gravity
		{
		public:
			void OnLoaded(CObj* const) override;
			IPlayeMoveState* Update(CObj* const, float timedelta) override;
		private:
		};
		//점프 추상 클래스
		class Jump : public Gravity
		{
		public:
			void OnLoaded(CObj* const) override;
		protected:
			//인스턴스화되면 안된다.
			Jump() = default;

		};
		class SingleJump : public Jump
		{
		public:
			SingleJump();
			virtual IPlayeMoveState* Update(CObj* const, float timedelta)override;
		private:
			bool m_bReachedTop;
		};
		class DoubleJump : public Jump
		{
		public:
			void OnLoaded(CObj* const) override;
			IPlayeMoveState* Update(CObj* const, float timedelta) override;
		};
		class Slide : public Gravity
		{
		public:
			void OnLoaded(CObj* const) override;
			IPlayeMoveState* Update(CObj* const, float timedelta)override;
		private:
			float m_fPrevHeight;
			float m_fPrevPosY;
			float m_fprevWidth;
		};
	}
	//
	class NormalState : public IPlayerCollisionState
	{
	public:
		void OnLoaded(CObj* const) override {}
		IPlayerCollisionState* Update(CObj* const, float timedelta)override { return this; }
		IPlayerCollisionState* PostUpdate(CObj* const)override;
		void Render(CObj* const, HDC)override;
	};
	class BumpState : public IPlayerCollisionState
	{
	public:
		constexpr static float Interval = 3.f;
		void OnLoaded(CObj* const)override {}
		IPlayerCollisionState* Update(CObj* const, float timedelta)override;
		IPlayerCollisionState* PostUpdate(CObj* const) override { return this; };
		void Render(CObj* const, HDC)override;
		bool m_bTicToc = true;
		float m_fTime = Interval;
	};
}
