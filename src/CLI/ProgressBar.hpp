#pragma once
#include <cstdint>
#include <string>
#include <mutex>

/**
 * RAII implementation of a progress bar.
 */
class ProgressBar
{
public:
	/**
	 * Constructor.
	 * It takes two values: the expected number of iterations whose progress we
	 * want to monitor and an initial message to be displayed on top of the bar
	 * (which can be updated with updateLastPrintedMessage()).
	 */
	ProgressBar(
		size_t expectedIterations, const std::string& initialMessage = "");

	/**
	 * Destructor to guarantee RAII.
	 */
	~ProgressBar();

	// Make the object non-copyable
	ProgressBar(const ProgressBar& o) = delete;
	ProgressBar& operator=(const ProgressBar& o) = delete;

	/**
	 * Must be invoked when the progress bar is no longer needed to restore the
	 * position of the cursor to the end of the output.
	 * It is automatically invoked when the object is destroyed.
	 */
	void endProgressBar();

	/**
	 * Prints a new message under the last printed message, without overwriting
	 * it. This moves the progress bar down to be placed under the newly
	 * written message.
	 */
	void printNewMessage(const std::string& message);

	/**
	 * Prints a message while the progress bar is on the screen on top on the
	 * last printed message. Since the cursor is right at the beginning of the
	 * progress bar, it moves the cursor up by one line before printing, and
	 * then returns it to its original position.
	 */
	void updateLastPrintedMessage(const std::string& message);

	/**
	 * Overloaded prefix operator, used to indicate that the has been a new
	 * iteration.
	 */
	void operator++();

private:
	size_t mTotalIterations;
	size_t mNumberOfTicks;
	bool mEnded;
	size_t mLengthOfLastPrintedMessage;
	std::mutex g_mutex;
};
