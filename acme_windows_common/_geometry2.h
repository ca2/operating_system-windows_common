#pragma once


rectangle_type(const D2D1_RECT_F & t) noexcept { ::copy(this, &t); }


inline void copy(::float_rectangle & prectDst, const D2D1_RECT_F & prectSrc) { ::copy_rect(prectDst, prectSrc); }




