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

#include "libspu/mpc/api_test.h"

#include "gtest/gtest.h"

#include "libspu/core/shape_util.h"
#include "libspu/mpc/api.h"
#include "libspu/mpc/common/communicator.h"
#include "libspu/mpc/utils/ring_ops.h"
#include "libspu/mpc/utils/simulate.h"

namespace spu::mpc::test {
namespace {

constexpr int64_t kNumel = 1000;
const std::vector<size_t> kShiftBits = {0, 1, 2, 31, 32, 33, 64, 1000};

}  // namespace

#define TEST_BINARY_OP_SS(OP)                                             \
  TEST_P(ApiTest, OP##_ss) {                                              \
    const auto factory = std::get<0>(GetParam());                         \
    const RuntimeConfig& conf = std::get<1>(GetParam());                  \
    const size_t npc = std::get<2>(GetParam());                           \
                                                                          \
    utils::simulate(                                                      \
        npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {      \
          auto obj = factory(conf, lctx);                                 \
                                                                          \
          /* GIVEN */                                                     \
          auto p0 = rand_p(obj.get(), kNumel);                            \
          auto p1 = rand_p(obj.get(), kNumel);                            \
                                                                          \
          /* WHEN */                                                      \
          auto tmp =                                                      \
              OP##_ss(obj.get(), p2s(obj.get(), p0), p2s(obj.get(), p1)); \
          auto re = s2p(obj.get(), tmp);                                  \
          auto rp = OP##_pp(obj.get(), p0, p1);                           \
                                                                          \
          /* THEN */                                                      \
          EXPECT_TRUE(ring_all_equal(re, rp));                            \
        });                                                               \
  }

#define TEST_BINARY_OP_SP(OP)                                                \
  TEST_P(ApiTest, OP##_sp) {                                                 \
    const auto factory = std::get<0>(GetParam());                            \
    const RuntimeConfig& conf = std::get<1>(GetParam());                     \
    const size_t npc = std::get<2>(GetParam());                              \
                                                                             \
    utils::simulate(npc,                                                     \
                    [&](const std::shared_ptr<yacl::link::Context>& lctx) {  \
                      auto obj = factory(conf, lctx);                        \
                                                                             \
                      /* GIVEN */                                            \
                      auto p0 = rand_p(obj.get(), kNumel);                   \
                      auto p1 = rand_p(obj.get(), kNumel);                   \
                                                                             \
                      /* WHEN */                                             \
                      auto tmp = OP##_sp(obj.get(), p2s(obj.get(), p0), p1); \
                      auto re = s2p(obj.get(), tmp);                         \
                      auto rp = OP##_pp(obj.get(), p0, p1);                  \
                                                                             \
                      /* THEN */                                             \
                      EXPECT_TRUE(ring_all_equal(re, rp));                   \
                    });                                                      \
  }

#define TEST_BINARY_OP(OP) \
  TEST_BINARY_OP_SS(OP)    \
  TEST_BINARY_OP_SP(OP)

TEST_BINARY_OP(add)
TEST_BINARY_OP(mul)
TEST_BINARY_OP(and)
TEST_BINARY_OP(xor)

#define TEST_UNARY_OP_S(OP)                                                 \
  TEST_P(ApiTest, OP##_s) {                                                 \
    const auto factory = std::get<0>(GetParam());                           \
    const RuntimeConfig& conf = std::get<1>(GetParam());                    \
    const size_t npc = std::get<2>(GetParam());                             \
                                                                            \
    utils::simulate(                                                        \
        npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {        \
          auto obj = factory(conf, lctx);                                   \
                                                                            \
          /* GIVEN */                                                       \
          auto p0 = rand_p(obj.get(), kNumel);                              \
                                                                            \
          /* WHEN */                                                        \
          auto r_s = s2p(obj.get(), OP##_s(obj.get(), p2s(obj.get(), p0))); \
          auto r_p = OP##_p(obj.get(), p0);                                 \
                                                                            \
          /* THEN */                                                        \
          EXPECT_TRUE(ring_all_equal(r_s, r_p));                            \
        });                                                                 \
  }

#define TEST_UNARY_OP_P(OP)                                                 \
  TEST_P(ApiTest, OP##_p) {                                                 \
    const auto factory = std::get<0>(GetParam());                           \
    const RuntimeConfig& conf = std::get<1>(GetParam());                    \
    const size_t npc = std::get<2>(GetParam());                             \
                                                                            \
    utils::simulate(npc,                                                    \
                    [&](const std::shared_ptr<yacl::link::Context>& lctx) { \
                      auto obj = factory(conf, lctx);                       \
                                                                            \
                      /* GIVEN */                                           \
                      auto p0 = rand_p(obj.get(), kNumel);                  \
                                                                            \
                      /* WHEN */                                            \
                      auto r_p = OP##_p(obj.get(), p0);                     \
                      auto r_pp = OP##_p(obj.get(), p0);                    \
                                                                            \
                      /* THEN */                                            \
                      EXPECT_TRUE(ring_all_equal(r_p, r_pp));               \
                    });                                                     \
  }

#define TEST_UNARY_OP(OP) \
  TEST_UNARY_OP_S(OP)     \
  TEST_UNARY_OP_P(OP)

TEST_UNARY_OP(not )
TEST_UNARY_OP(msb)

#define TEST_UNARY_OP_WITH_BIT_S(OP)                                         \
  TEST_P(ApiTest, OP##S) {                                                   \
    const auto factory = std::get<0>(GetParam());                            \
    const RuntimeConfig& conf = std::get<1>(GetParam());                     \
    const size_t npc = std::get<2>(GetParam());                              \
                                                                             \
    utils::simulate(                                                         \
        npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {         \
          auto obj = factory(conf, lctx);                                    \
                                                                             \
          /* GIVEN */                                                        \
          auto p0 = rand_p(obj.get(), kNumel);                               \
                                                                             \
          for (auto bits : kShiftBits) {                                     \
            if (bits >= SizeOf(conf.field()) * 8) {                          \
              continue;                                                      \
            }                                                                \
            /* WHEN */                                                       \
            auto r_s =                                                       \
                s2p(obj.get(), OP##_s(obj.get(), p2s(obj.get(), p0), bits)); \
            auto r_p = OP##_p(obj.get(), p0, bits);                          \
                                                                             \
            /* THEN */                                                       \
            EXPECT_TRUE(ring_all_equal(r_s, r_p));                           \
          }                                                                  \
        });                                                                  \
  }

#define TEST_UNARY_OP_WITH_BIT_P(OP)                                        \
  TEST_P(ApiTest, OP##P) {                                                  \
    const auto factory = std::get<0>(GetParam());                           \
    const RuntimeConfig& conf = std::get<1>(GetParam());                    \
    const size_t npc = std::get<2>(GetParam());                             \
                                                                            \
    utils::simulate(npc,                                                    \
                    [&](const std::shared_ptr<yacl::link::Context>& lctx) { \
                      auto obj = factory(conf, lctx);                       \
                                                                            \
                      /* GIVEN */                                           \
                      auto p0 = rand_p(obj.get(), kNumel);                  \
                                                                            \
                      for (auto bits : kShiftBits) { /* WHEN */             \
                        if (bits >= SizeOf(conf.field()) * 8) {             \
                          continue;                                         \
                        }                                                   \
                        auto r_p = OP##_p(obj.get(), p0, bits);             \
                        auto r_pp = OP##_p(obj.get(), p0, bits);            \
                                                                            \
                        /* THEN */                                          \
                        EXPECT_TRUE(ring_all_equal(r_p, r_pp));             \
                      }                                                     \
                    });                                                     \
  }

#define TEST_UNARY_OP_WITH_BIT(OP) \
  TEST_UNARY_OP_WITH_BIT_S(OP)     \
  TEST_UNARY_OP_WITH_BIT_P(OP)

TEST_UNARY_OP_WITH_BIT(lshift)
TEST_UNARY_OP_WITH_BIT(rshift)
TEST_UNARY_OP_WITH_BIT(arshift)

TEST_P(ApiTest, TruncS) {
  const auto factory = std::get<0>(GetParam());
  const RuntimeConfig& conf = std::get<1>(GetParam());
  const size_t npc = std::get<2>(GetParam());

  // trunc_pr only work for smalle range.
  auto p0 = ring_rand_range(conf.field(), kNumel, 0, 10000);
  utils::simulate(npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {
    auto obj = factory(conf, lctx);

    const size_t bits = 2;
    auto r_s = s2p(obj.get(), trunc_s(obj.get(), p2s(obj.get(), p0), bits));
    auto r_p = arshift_p(obj.get(), p0, bits);

    /* THEN */
    EXPECT_TRUE(ring_all_equal(r_s, r_p, npc));
  });
}

TEST_P(ApiTest, MatMulSS) {
  const auto factory = std::get<0>(GetParam());
  const RuntimeConfig& conf = std::get<1>(GetParam());
  const size_t npc = std::get<2>(GetParam());

  const int64_t M = 70;
  const int64_t K = 400;
  const int64_t N = 60;
  const int64_t N2 = 90;
  const std::vector<int64_t> shape_A{M, K};
  const std::vector<int64_t> shape_B{K, N};
  const std::vector<int64_t> shape_B2{K, N2};

  utils::simulate(npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {
    auto obj = factory(conf, lctx);

    /* GIVEN */
    auto p0 = rand_p(obj.get(), calcNumel(shape_A));
    auto p1 = rand_p(obj.get(), calcNumel(shape_B));
    auto p2 = rand_p(obj.get(), calcNumel(shape_B2));

    /* WHEN */
    auto tmp =
        mmul_ss(obj.get(), p2s(obj.get(), p0), p2s(obj.get(), p1), M, N, K);
    auto tmp2 =
        mmul_ss(obj.get(), p2s(obj.get(), p0), p2s(obj.get(), p2), M, N2, K);

    auto r_ss = s2p(obj.get(), tmp);
    auto r_pp = mmul_pp(obj.get(), p0, p1, M, N, K);

    auto r_ss2 = s2p(obj.get(), tmp2);
    auto r_pp2 = mmul_pp(obj.get(), p0, p2, M, N2, K);

    /* THEN */
    EXPECT_TRUE(ring_all_equal(r_ss, r_pp));
    EXPECT_TRUE(ring_all_equal(r_ss2, r_pp2));
  });
}

TEST_P(ApiTest, MmulSP) {
  const auto factory = std::get<0>(GetParam());
  const RuntimeConfig& conf = std::get<1>(GetParam());
  const size_t npc = std::get<2>(GetParam());

  const int64_t M = 3;
  const int64_t K = 4;
  const int64_t N = 3;
  const std::vector<int64_t> shape_A{M, K};
  const std::vector<int64_t> shape_B{K, N};

  utils::simulate(npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {
    auto obj = factory(conf, lctx);

    /* GIVEN */
    auto p0 = rand_p(obj.get(), calcNumel(shape_A));
    auto p1 = rand_p(obj.get(), calcNumel(shape_B));

    /* WHEN */
    auto tmp = mmul_sp(obj.get(), p2s(obj.get(), p0), p1, M, N, K);
    auto r_ss = s2p(obj.get(), tmp);
    auto r_pp = mmul_pp(obj.get(), p0, p1, M, N, K);

    /* THEN */
    EXPECT_TRUE(ring_all_equal(r_ss, r_pp));
  });
}

TEST_P(ApiTest, P2S_S2P) {
  const auto factory = std::get<0>(GetParam());
  const RuntimeConfig& conf = std::get<1>(GetParam());
  const size_t npc = std::get<2>(GetParam());

  utils::simulate(npc, [&](const std::shared_ptr<yacl::link::Context>& lctx) {
    auto obj = factory(conf, lctx);

    /* GIVEN */
    auto p0 = rand_p(obj.get(), kNumel);

    /* WHEN */
    auto s = p2s(obj.get(), p0);
    auto p1 = s2p(obj.get(), s);

    /* THEN */
    EXPECT_TRUE(ring_all_equal(p0, p1));
  });
}

}  // namespace spu::mpc::test
