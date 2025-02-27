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

#include <memory>

#include "absl/types/span.h"

#include "libspu/core/ndarray_ref.h"
#include "libspu/core/type.h"
#include "libspu/core/type_util.h"

namespace spu {

class Value final {
  NdArrayRef data_;
  DataType dtype_ = DT_INVALID;

 public:
  Value() = default;
  explicit Value(NdArrayRef data, DataType dtype);

  /// Forward ndarray methods.
  inline int64_t numel() const { return data_.numel(); }
  inline size_t elsize() const { return data_.elsize(); }
  std::vector<int64_t> const& strides() const { return data_.strides(); }
  std::vector<int64_t> const& shape() const { return data_.shape(); }

  // Get the concrete storage type.
  const Type& storage_type() const { return data_.eltype(); }

  // Access the underline storage data.
  const NdArrayRef& data() const { return data_; }
  NdArrayRef& data() { return data_; }

  // Get vtype, is readonly and decided by the underline secure compute engine.
  Visibility vtype() const;
  bool isPublic() const { return vtype() == VIS_PUBLIC; }
  bool isSecret() const { return vtype() == VIS_SECRET; }

  // Get dtype.
  DataType dtype() const { return dtype_; }
  bool isInt() const { return isInteger(dtype()); }
  bool isFxp() const { return isFixedPoint(dtype()); }

  // Set dtype.
  //
  // By default, we can only set dtype when it's not set yet(=DT_INVALID),
  // unless we forcely do it.
  Value& setDtype(DataType new_dtype, bool force = false);
  Value& asFxp() { return setDtype(DT_FXP); }

  // Serialize to protobuf.
  ValueProto toProto() const;
  ValueMeta toMetaProto() const;

  // Deserialize from protobuf.
  static Value fromProto(const ValueProto& proto);

  Value clone() const;
};

std::ostream& operator<<(std::ostream& out, const Value& v);

}  // namespace spu
