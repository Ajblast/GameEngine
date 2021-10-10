#pragma once
#include <atomic>
#include <utility>
#include <thread>

namespace GRAVEngine
{
	namespace Locks
	{
#pragma region Explicit Locking/Unlocking
		// Unlock all of the locks
		template<size_t... indices, typename... L>
		void unlockExplicitLock(const int first, const int last, std::index_sequence<indices...>, L&... locks)
		{
			// Create a dummy array from the varaible list
			int dummy[] = {
				(
					// The unlock function is only ran when the lock index is between first and last
					(first <= static_cast<int>(indices) && static_cast<int>(indices) < last ? (void)locks.unlock() : void()),	// The actul function to be ran
					0																											// Make sure there is at least 1 item, I think
				)... };

			(void)dummy;
		}
		// Try to lock an explicity lock from a template parameter pack
		template<size_t... indices, typename... L>
		void lockExplicitLock(const int index, std::index_sequence<indices...>, L&... locks)
		{
			int dummy[] = {
				(
					// Indicies is set to each individual value found in index_sequence after expansion. i.e., {1, 2, 3} turns into 1 == index, 2 == index, 3 == index
					// This function will only run the lock if it is the index we are looking for in the template parameter pack
					(static_cast<int>(indices) == index ? (void)(result = locks.lock()) : void()),	// The actul function to be ran
					0																				// Make sure there is at least 1 item, I think
				)... };

			(void)dummy;
		}
		// Try to lock an explicity lock from a template parameter pack
		template<size_t... indices, typename... L>
		bool tryLockExplicitLock(const int index, std::index_sequence<indices...>, L&... locks)
		{
			bool result;
			int dummy[] = {
				(
					// Indicies is set to each individual value found in index_sequence after expansion. i.e., {1, 2, 3} turns into 1 == index, 2 == index, 3 == index
					// This function will only run the try lock if it is the index we are looking for in the template parameter pack
					(static_cast<int>(indices) == index ? (void)(result = locks.try_lock()) : void()),	// The actul function to be ran
					0																					// Make sure there is at least 1 item, I think
				)... };

			(void)dummy;

			return result;

		}
#pragma endregion

#pragma region Try Lock
		// Attempt to lock of the given locks. Report which lock was not able to be acquired. -1 for success.
		template<typename... L>
		int try_lockMore(const int startIndex, const int endIndex, L&... locks)
		{
			// Indices is an index_sequence for all of the given lock types
			// It holds the index from 0...sizeof(L)
			using indices = std::index_sequence_for<L...>;

			size_t i = startIndex;
			try
			{
				// Attempt to lock each lock
				for (; i != endIndex; i++)
				{
					// Try to lock the explicit lock at i. This is done this way because locks is a parameter pack
					if (tryLockExplicitLock(i, indices{}, locks...) == false)
					{
						// Failed to get a lock, unlock all of the currently acquired locks
						unlockExplicitLock(startIndex, i, indices{}, locks...);
						return i;
					}
				}
			}
			catch (...)
			{
				// Unlock all of the currently locked locks if any exceptions happened
				unlockExplicitLock(startIndex, i, indices{}, locks...);
				throw;
			}

			// Success. All locks acquired.
			return -1;
		}
		// Attemp to lock three or more locks. Report which lock was not able to be acquired. -1 for success.
		template<typename L1, typename L2, typename L3, typename... L>
		int try_locks(L1& lock1, L2& lock2, L3& lock3, L&... locks)
		{
			return try_lockMore(0, sizeof...(L) + 3, lock1, lock2, lock3, locks...);
		}
		// Attemp to lock the two given locks. Report which lock was not able to be acquired. -1 for success.
		template<typename L1, typename L2>
		int try_locks(L1 lock1, L2 lock2)
		{
			// Try to lock the first one
			if (lock1.try_lock() == false)
				return 0;

			try
			{
				// Try to unlock the second one
				if (lock2.try_lock() == false)
				{
					lock1.unlock();
					return 1;
				}
			}
			catch (...)
			{
				// If there was an exception trying to get the second lock, make sure to unlock the first lock
				lock1.unlock();
				throw;
			}

			// Success. All locks acquired
			return -1;
		}
#pragma endregion

		// Try to lock multiple locks
		template<typename L1, typename L2, typename... L>
		int try_lock(L1& lock1, L2& lock2, L&... locks)
		{
			try_locks(lock1, lock2, locks...);
		}

#pragma region Lock
		// Attempt to lock two locks
		template<typename... L>
		int lockAttempt(const int index, L&... locks)
		{
			using indices = std::index_sequence_for<L...>;

			// Lock this lock
			lockExplicitLock(index, indicies{}, locks...);

			int failed = -1;
			int backoutIndex = index;

			try
			{
				// Try to lock from the first to starting lock
				failed = try_lockMore(0, index, locks...);
				if (failed == -1)
				{
					backoutIndex = 0;										// Unlock [0, startIndex] if an exception occures
					failed = try_lockMore(index + 1, sizeof...(L), locks...);	// Lock the rest of the locks

					// All of the locks were acquired
					if (failed == -1)
						return -1;
				}
			}
			catch (...)
			{
				unlockExplicitLock(backoutIndex, index + 1, indices{}, locks...);
				throw;
			}

			// Not all of the locks were acquired, so unlock everything we just acquired
			unlockExplicitLock(backoutIndex, index + 1, indices{}, locks...);
			std::this_thread::yield();
			return false;
		}
		// Lock 3 ore more locks
		template<typename L1, typename L2, typename L3, typename... L>
		void lockLocks(L1& lock1, L2& lock2, L3& lock3, L&... locks)
		{
			int startIndex = 0;
			// Keep trying to get the locks until the attempt returns that all of the locks passing
			while (startIndex != -1)
				startIndex = lockAttempt(startIndex, lock1, lock2, lock3, locks...);
		}
		// Attempt to lock two locks
		template<typename L1, typename L2>
		bool lockSmallAttempt(L1& lock1, L2& lock2)
		{
			// Lock the first one
			lock1.lock();
			try
			{
				// Try to lock the second one, and the attempt fails if you can't get it
				if (lock2.try_lock())
					return true;
			}
			catch (...)
			{
				// Unlock the first lock if anything bad happends
				lock1.unlock();
				throw;
			}

			// Unlock the first lock because we couldn't get both locks
			lock1.unlock();
			std::this_thread::yield();
			return false;
		}
		// Lock the locks
		template<typename L1, typename L2>
		void lockLocks(L1& lock1, L2& lock2)
		{
			// Keep attempting to get both locks
			while (lockSmallAttempt(lock1, lock2) == false && lockSmallAttempt(lock2, lock1) == false) {}
		}
#pragma endregion

		// Lock multiple locks
		template<typename L1, typename L2, typename... L>
		void lock(L1& lock1, L2& lock2, L&... locks)
		{
			lockLocks(lock1, lock2, locks...);
		}

		// Unlock multiple locks
		template<typename... L>
		void unlock(L&... locks)
		{
			std::apply([](L&... args) {(..., (void)args.unlock()); }, locks...);
		}
	}
}
