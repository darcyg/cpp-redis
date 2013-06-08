#ifndef MUTEX_GUARD_HPP
#define MUTEX_GUARD_HPP

namespace redis {

class mutex_guard
{
public:
	mutex_guard(pthread_mutex_t& mutex)
		: mutex_(&mutex)
	{
		pthread_mutex_lock(mutex_);
	}

	~mutex_guard()
	{
		pthread_mutex_unlock(mutex_);
		mutex_ = NULL;
	}

private:
	mutex_guard() {}

private:
	pthread_mutex_t* mutex_;
};

}

#endif
