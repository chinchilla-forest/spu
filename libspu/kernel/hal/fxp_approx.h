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
namespace spu::kernel::hal {
namespace detail {

Value log2_pade_approx(HalContext* ctx, const Value& x);

Value log_householder_approx(HalContext* ctx, const Value& x);

// Works for range [-500, 2.1]
Value exp_taylor_series(HalContext* ctx, const Value& x);

Value exp2_pade_approx(HalContext* ctx, const Value& x);

// Works for range [-12.0, 18.0]
Value exp_pade_approx(HalContext* ctx, const Value& x);

}  // namespace detail

Value f_exp(HalContext* ctx, const Value& x);

Value f_log1p(HalContext* ctx, const Value& x);

Value f_log(HalContext* ctx, const Value& x);

Value f_log2(HalContext* ctx, const Value& x);

Value f_exp2(HalContext* ctx, const Value& x);

Value f_tanh(HalContext* ctx, const Value& x);

Value f_rsqrt(HalContext* ctx, const Value& x);

Value f_sqrt(HalContext* ctx, const Value& x);

}  // namespace spu::kernel::hal
