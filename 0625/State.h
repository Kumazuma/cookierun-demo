#pragma once
class CObj;
struct IPlayerState
{
	virtual void OnLoaded(CObj* const) = 0;
	virtual IPlayerState* Update(CObj* const, float timedelta) = 0;
	virtual ~IPlayerState() = default;
	bool IsNull() const { return this == nullptr; }
};
namespace State
{
	class Gravity : public IPlayerState
	{
	public:
		IPlayerState* Update(CObj* const, float timedelta) override;
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
		IPlayerState* Update(CObj* const, float timedelta) override;
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
		virtual IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		bool m_bReachedTop;
	};
	class DoubleJump : public Jump
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta) override;
	};
	class Slide : public Gravity
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		float m_fPrevHeight;
		float m_fPrevPosY;
		float m_fprevWidth;
	};
}
