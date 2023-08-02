#pragma once
#include "Allocator.h"

//인자를 가변적으로 받게해줌 ...
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));
	// placement new(생성자 호출 방법)
	new(memory)Type(forward<Args>(args)...);

	return memory;
}

template<typename Type>
void xdelete(Type Obj)
{
	Obj->~Type();
	xrelease(Obj);
}