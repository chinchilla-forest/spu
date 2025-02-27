// Copyright 2022 Ant Group Co., Ltd.
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

#include <algorithm>

#include "libspu/core/pt_buffer_view.h"
#include "libspu/kernel/hlo/casting.h"
#include "libspu/kernel/hlo/utils.h"

namespace spu::kernel::hlo {

spu::Value Constant(HalContext *ctx, const PtBufferView &view,
                    absl::Span<const int64_t> out_shape);

spu::Value Iota(HalContext *ctx, DataType dtype, int64_t numel);

// Returns the SPU epsilon, the positive distance between two fixed point value.
//
// The result visibility is public.
spu::Value Epsilon(HalContext *ctx);

}  // namespace spu::kernel::hlo
