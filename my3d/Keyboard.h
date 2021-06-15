#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class  Window;
public:
	class Event {
	public:
		enum class Type		//virtual-key status
		{
			Press,		
			Release,
			Invalid
		};
	private:
		Type mtype;
		unsigned char mcode;		//Virtual-Key Code
	public:
		Event(Type type, unsigned char mcode) noexcept
			:
			mtype(type),
			mcode(mcode)
		{};
		bool isPressed() noexcept 
		{
			return mtype == Type::Press;
		}
		bool isRelease() noexcept
		{
			return mtype == Type::Release;
		}
		unsigned char GetCode() noexcept
		{
			return mcode;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	//virtual-key event stuff
	bool KeyIsPressed(unsigned char keycode) noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() noexcept;
	void FlushKey() noexcept;
	//char event stuff
	Event ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	//auto-reapeat control
};

