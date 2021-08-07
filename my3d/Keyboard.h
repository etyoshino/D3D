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
		Event() noexcept
			:
			mtype(Type::Invalid),
			mcode(0u)
		{}
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
	bool KeyIsPressed(unsigned char keycode)const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty()const noexcept;
	void FlushKey() noexcept;
	//char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	//auto-reapeat control
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutoRepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buff) noexcept;
private:
	static constexpr unsigned int nKeys = 265u;
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};
