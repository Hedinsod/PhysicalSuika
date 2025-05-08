#pragma once

#include <functional>
#include <vector>

template <class RetType, class ...Args>
struct TEvent final
{
	using EventCallbackFn = std::function<RetType(Args... args)>;
	std::vector<EventCallbackFn> InvocationList;

	inline void Subscribe(const EventCallbackFn& InCallback)
	{
		InvocationList.push_back(InCallback);
	}

	void Unsubscribe(const EventCallbackFn& InCallback)
	{
		InvocationList.erase(InCallback);
	}

	void Broadcast(Args... args)
	{
		for (auto& InvocationItem : InvocationList)
		{
			if (InvocationItem)
			{
				InvocationItem(std::forward<Args>(args)...);
			}
		}
	}

};
