#pragma once
#include "Allocator.h"

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new(생성자 호출 방법)
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xRelease(obj);
}