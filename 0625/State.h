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
	class Default : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta) override;
	private:
	};
	//점프 추상 클래스
	class Jump : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
		
	protected:
		//인스턴스화되면 안된다.
		Jump() = default;
		//중력에 따라 속도가 제어되는 것을 관리한다. 만약 땅에 닿았으면 true를 반환한다.
		bool GravityMove(CObj* const, float timedalta);
	};
	class SingleJump : public Jump
	{
	public:
		SingleJump();
		void OnLoaded(CObj* const) override;
		virtual IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		bool m_bReachedTop;
		float m_fDelay;
		float m_fPrevSpeed;
	};
	class DoubleJump : public Jump
	{
	public:
		IPlayerState* Update(CObj* const, float timedelta) override;
	};
	class Slide : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		float m_fPrevHeight;
		float m_fPrevPosY;
	};
}
