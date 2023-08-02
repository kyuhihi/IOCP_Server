#pragma once
#include "Allocator.h"

//���ڸ� ���������� �ް����� ...
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));
	// placement new(������ ȣ�� ���)
	new(memory)Type(forward<Args>(args)...);

	return memory;
}

template<typename Type>
void xdelete(Type Obj)
{
	Obj->~Type();
	xrelease(Obj);
}