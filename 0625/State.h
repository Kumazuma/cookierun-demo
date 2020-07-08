#pragma once
class CObj;
class CPlayer;
struct IPlayeMoveState
{
	virtual void OnLoaded(CObj* const) = 0;
	virtual IPlayeMoveState* Update(CObj* const, float timedelta) = 0;
	virtual ~IPlayeMoveState() = default;
};
struct IPlayerCollisionState
{
	virtual void OnLoaded(CPlayer* const) = 0;
	virtual IPlayerCollisionState* Update(CPlayer* const, float timedelta) = 0;
	virtual IPlayerCollisionState* PostUpdate(CPlayer* const) = 0;
	virtual void Render(CPlayer* const, HDC) = 0;
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
			//�߷¿� ���� �ӵ��� ����Ǵ� ���� �����Ѵ�. 
			void GravityMove(CObj* const, float timedalta);
			//���� ���� ������� true�� ��ȯ�Ѵ�.
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
		//���� �߻� Ŭ����
		class Jump : public Gravity
		{
		public:
			void OnLoaded(CObj* const) override;
		protected:
			//�ν��Ͻ�ȭ�Ǹ� �ȵȴ�.
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
		void OnLoaded(CPlayer* const) override;
		IPlayerCollisionState* Update(CPlayer* const, float timedelta)override { return this; }
		IPlayerCollisionState* PostUpdate(CPlayer* const)override;
		void Render(CPlayer* const, HDC)override;
	};
	class BumpState : public IPlayerCollisionState
	{
	public:
		constexpr static float Interval = 1.f;
		void OnLoaded(CPlayer* const pObject)override;
		IPlayerCollisionState* Update(CPlayer* const, float timedelta)override;
		IPlayerCollisionState* PostUpdate(CPlayer* const) override { return this; };
		void Render(CPlayer* const, HDC)override;
		bool m_bTicToc = true;
		float m_fTime = Interval;
	};
}
