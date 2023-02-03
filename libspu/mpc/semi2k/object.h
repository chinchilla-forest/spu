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

#include "libspu/mpc/common/communicator.h"
#include "libspu/mpc/semi2k/beaver/beaver_tfp.h"

namespace spu::mpc {

// TODO(jint) split this into individual states.
class Semi2kState : public State {
  std::unique_ptr<semi2k::BeaverTfpUnsafe> beaver_;

 public:
  static constexpr char kBindName[] = "Semi2kState";

  explicit Semi2kState(std::shared_ptr<yacl::link::Context> lctx) {
    beaver_ = std::make_unique<semi2k::BeaverTfpUnsafe>(lctx);
  }

  semi2k::BeaverTfpUnsafe* beaver() { return beaver_.get(); }

  std::unique_ptr<State> fork() override {
    return std::make_unique<Semi2kState>(
        dynamic_cast<semi2k::BeaverTfpUnsafe*>(beaver())->GetLink()->Spawn());
  }
};

}  // namespace spu::mpc