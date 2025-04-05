#pragma once

#define EVENT_OneParam(_Event_TypeName, _ParamType)                                \
	struct _Event_TypeName {                                                       \
		using EventCallbackFn = std::function<void(_ParamType)>;                   \
		void Broadcast(_ParamType Param) {                                         \
			if (InvocationItem) {                                                  \
				InvocationItem(std::forward<_ParamType>(Param)); } }               \
		void Subscribe(const EventCallbackFn& InCallback) {                        \
			InvocationItem = InCallback; }                                         \
		EventCallbackFn InvocationItem = nullptr; };                               
