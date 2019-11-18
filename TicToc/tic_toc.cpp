/*

Copyright (c) 2019 Macrobull

*/

#include "tic_toc.inl.hpp"

#define ImplUnitType std::chrono::seconds

template class TicToc<ImplUnitType>;

#undef ImplUnitType

#define ImplUnitType std::chrono::milliseconds

template class TicToc<ImplUnitType>;

#undef ImplUnitType

#define ImplUnitType std::chrono::microseconds

template class TicToc<ImplUnitType>;

#undef ImplUnitType

#define ImplUnitType std::chrono::nanoseconds

template class TicToc<ImplUnitType>;

#undef ImplUnitType
