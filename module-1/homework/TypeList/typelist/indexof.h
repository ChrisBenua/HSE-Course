#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct IndexOf;

template<typename T>
struct IndexOf<NullType, T> {
    static constexpr int pos = -1;
};

template<typename T, typename Tail>
struct IndexOf<TypeList<T, Tail>, T>{
    static constexpr int pos = 0;
};

template<typename Head, typename Tail, typename T>
struct IndexOf<TypeList<Head, Tail>, T> {
    static constexpr int pos = IndexOf<Tail, T>::pos == -1 ? -1 : 1 + IndexOf<Tail, T>::pos;
};

