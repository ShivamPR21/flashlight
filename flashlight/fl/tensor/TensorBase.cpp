/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "flashlight/fl/tensor/TensorBase.h"

#include <stdexcept>
#include <utility>

#include "flashlight/fl/tensor/TensorAdapter.h"
#include "flashlight/fl/tensor/TensorBackend.h"

#define FL_TENSOR_BACKENDS_MATCH_CHECK(...)             \
  if (!detail::areBackendsEqual(__VA_ARGS__)) {         \
    throw std::invalid_argument(                        \
        std::string(__func__) +                         \
        " called with tensors of different backends."); \
  }

namespace fl {

Tensor::Tensor(std::unique_ptr<TensorAdapterBase> adapter)
    : impl_(std::move(adapter)) {}

Tensor::~Tensor() {}

Tensor::Tensor(const Tensor& tensor) : impl_(tensor.impl_->clone()) {}

Tensor::Tensor() : impl_(detail::getDefaultAdapter()) {}

const Shape& Tensor::shape() const {
  return impl_->shape();
}

dtype Tensor::type() const {
  return impl_->type();
}

Tensor Tensor::astype(const dtype type) {
  return impl_->astype(type);
}

Tensor Tensor::operator()(const std::vector<Index>& indices) const {
  return impl_->index(indices);
}

TensorBackendType Tensor::backendType() const {
  return impl_->backendType();
}

TensorBackend& Tensor::backend() const {
  return impl_->backend();
}

/* --------------------------- Tensor Operators --------------------------- */

/************************** Unary Operators ***************************/
Tensor exp(const Tensor& tensor) {
  return tensor.backend().exp(tensor);
}

Tensor log(const Tensor& tensor) {
  return tensor.backend().log(tensor);
}

Tensor negative(const Tensor& tensor) {
  return tensor.backend().negative(tensor);
}

Tensor logicalNot(const Tensor& tensor) {
  return tensor.backend().logicalNot(tensor);
}

Tensor log1p(const Tensor& tensor) {
  return tensor.backend().log1p(tensor);
}

Tensor sin(const Tensor& tensor) {
  return tensor.backend().sin(tensor);
}

Tensor cos(const Tensor& tensor) {
  return tensor.backend().cos(tensor);
}

Tensor sqrt(const Tensor& tensor) {
  return tensor.backend().sqrt(tensor);
}

Tensor tanh(const Tensor& tensor) {
  return tensor.backend().tanh(tensor);
}

Tensor absolute(const Tensor& tensor) {
  return tensor.backend().absolute(tensor);
}

Tensor clip(const Tensor& tensor, const Tensor& low, const Tensor& high) {
  FL_TENSOR_BACKENDS_MATCH_CHECK(tensor, low, high);
  return tensor.backend().clip(tensor, low, high);
}

Tensor clip(const Tensor& tensor, const Tensor& low, const double& high) {
  return clip(tensor, low, full(tensor.shape(), high));
}

Tensor clip(const Tensor& tensor, const double& low, const Tensor& high) {
  return clip(tensor, full(tensor.shape(), low), high);
}

Tensor clip(const Tensor& tensor, const double& low, const double& high) {
  return clip(tensor, full(tensor.shape(), low), full(tensor.shape(), high));
}

Tensor isnan(const Tensor& tensor) {
  return tensor.backend().isnan(tensor);
}

/************************** Binary Operators ***************************/

Tensor minimum(const Tensor& lhs, const Tensor& rhs) {
  FL_TENSOR_BACKENDS_MATCH_CHECK(lhs, rhs);
  return lhs.backend().minimum(lhs, rhs);
}

Tensor maximum(const Tensor& lhs, const Tensor& rhs) {
  FL_TENSOR_BACKENDS_MATCH_CHECK(lhs, rhs);
  return lhs.backend().maximum(lhs, rhs);
}

Tensor minimum(const Tensor& lhs, const double& rhs) {
  return minimum(lhs, full(lhs.shape(), rhs));
}

Tensor minimum(const double& lhs, const Tensor& rhs) {
  return minimum(full(rhs.shape(), lhs), rhs);
}

Tensor maximum(const Tensor& lhs, const double& rhs) {
  return maximum(lhs, full(lhs.shape(), rhs));
}

Tensor maximum(const double& lhs, const Tensor& rhs) {
  return maximum(full(rhs.shape(), lhs), rhs);
}

Tensor power(const Tensor& lhs, const Tensor& rhs) {
  FL_TENSOR_BACKENDS_MATCH_CHECK(lhs, rhs);
  return lhs.backend().power(lhs, rhs);
}

/************************** Reductions ***************************/

Tensor amin(const Tensor& input, const std::vector<int>& axes) {
  return input.backend().amin(input, axes);
}

Tensor amax(const Tensor& input, const std::vector<int>& axes) {
  return input.backend().amax(input, axes);
}

Tensor sum(const Tensor& input, const std::vector<int>& axes) {
  return input.backend().sum(input, axes);
}

Tensor mean(const Tensor& input, const std::vector<int>& axes) {
  return input.backend().mean(input, axes);
}

Tensor var(const Tensor& input, const std::vector<int>& axes, bool bias) {
  return input.backend().var(input, axes, bias);
}

double norm(const Tensor& input) {
  return input.backend().norm(input);
}

} // namespace fl