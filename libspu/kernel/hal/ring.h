// Copyright 2021 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "libspu/kernel/context.h"
#include "libspu/kernel/value.h"

// !!please read [README.md] for api naming conventions.
// this module implements ops x ring 2k space WITHOUT dtype check.
//
// for example, when multiply sfxp with sint, it first dispatch to `_mul`, which
// multiply the underline data x a typed unchecked way, then set the result
// dtype to fxp.

namespace spu::kernel::hal {

Type _common_type(HalContext* ctx, const Type& a, const Type& b);

Value _cast_type(HalContext* ctx, const Value& x, const Type& to);

Value _negate(HalContext* ctx, const Value& x);

// Return 1 when x >= 0 else -1.
Value _sign(HalContext* ctx, const Value& x);

Value _add(HalContext* ctx, const Value& x, const Value& y);

Value _sub(HalContext* ctx, const Value& x, const Value& y);

Value _mul(HalContext* ctx, const Value& x, const Value& y);

// Note: there is no div (multiplicative inverse) in ring 2^k.
// Value _div(HalContext* ctx, const Value& x, const Value& y);

Value _mmul(HalContext* ctx, const Value& x, const Value& y);

Value _conv2d(HalContext* ctx, const Value& x, const Value& y,
              absl::Span<const int64_t> window_strides,
              absl::Span<const int64_t> result_shape);

Value _and(HalContext* ctx, const Value& x, const Value& y);

Value _xor(HalContext* ctx, const Value& x, const Value& y);

Value _or(HalContext* ctx, const Value& x, const Value& y);

Value _not(HalContext* ctx, const Value& in);

Value _msb(HalContext* ctx, const Value& in);

// Return 1{x == y}
Value _equal(HalContext* ctx, const Value& x, const Value& y);

Value _less(HalContext* ctx, const Value& x, const Value& y);

Value _lshift(HalContext* ctx, const Value& in, size_t bits);

Value _rshift(HalContext* ctx, const Value& in, size_t bits);

Value _arshift(HalContext* ctx, const Value& in, size_t bits);

Value _trunc(HalContext* ctx, const Value& x, size_t bits = 0);

Value _trunc_with_sign(HalContext* ctx, const Value& x, size_t bits = 0,
                       bool is_positive = true);

Value _bitrev(HalContext* ctx, const Value&, size_t start_idx, size_t end_idx);

// Expect pred is either {0, 1}.
Value _mux(HalContext* ctx, const Value& pred, const Value& a, const Value& b);

// Make a public value from uint128_t init value.
//
// If the current working field has less than 128bit, the lower sizeof(field)
// bits are used.
Value _constant(HalContext* ctx, uint128_t init,
                absl::Span<const int64_t> shape);

// Return the parity of bits, that is
// - 1 if there are odd number of 1s.
// - 0 if there are even number of 1s.
Value _bit_parity(HalContext* ctx, const Value& x, size_t bits);

Value _popcount(HalContext* ctx, const Value& x, size_t bits);

// out[i] = OR(in[0..i])
Value _prefix_or(HalContext* ctx, const Value& x);

// separate even and odd bits. e.g.
//   xAyBzCwD -> xyzwABCD
Value _bitdeintl(HalContext* ctx, const Value& in);

// Return value in arithmetic shared form if it's not.
//
// Note: theoretically, we should not leak `share` concept to hal layer.
Value _prefer_a(HalContext* ctx, const Value& x);

// Return value in binary shared form if it's not.
// Note: theoretically, we should not leak `share` concept to hal layer.
Value _prefer_b(HalContext* ctx, const Value& x);

}  // namespace spu::kernel::hal
