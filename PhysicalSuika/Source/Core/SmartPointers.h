#pragma once

#include <memory>

// Just shorter names
template <class T>
using StdShared = std::shared_ptr<T>;

template <class T, class ...Args>
inline StdShared<T> MakeShared(Args... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T>
using StdScoped = std::unique_ptr<T>;

template <class T, class ...Args>
inline StdScoped<T> MakeScoped(Args... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}
