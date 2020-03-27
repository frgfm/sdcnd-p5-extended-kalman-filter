#include "tools.h"
#include <iostream>
#include "spdlog/spdlog.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::sqrt;
using std::vector;

Tools::Tools() = default;

Tools::~Tools() = default;

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  VectorXd rmse(4);
  rmse << 0, 0, 0, 0;

  // check the validity of the following inputs:
  //  * the estimation vector size should not be zero
  //  * the estimation vector size should equal ground truth vector size
  if (estimations.empty() || estimations.size() != ground_truth.size()) {
    spdlog::warn("Invalid argument dimensions - received sizes {} and {}", estimations.size(), ground_truth.size());
    return rmse;
  }

  // Compute the SSE
  VectorXd tmp_(4);
  for (uint i = 0; i < estimations.size(); ++i) {
    tmp_ = (estimations[i] - ground_truth[i]).array().pow(2);
    rmse += tmp_;
  }

  // Divide by the number of samples to get the MSE
  rmse /= estimations.size();
  // Take the squared root to get the RMSE
  rmse = rmse.array().sqrt();

  return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd &x_state) {
  // Initialize the Jacobian
  MatrixXd J_(3, 4);
  J_.setZero(3, 4);

  if ( x_state.size() != 4 ) {
    spdlog::warn("Invalid state vector size - expected 4, received {}", x_state.size());
    return J_;
  }
  // recover state parameters
  float px = x_state(0);
  float py = x_state(1);
  float vx = x_state(2);
  float vy = x_state(3);

  // check division by zero
  float denom = px * px + py * py;
  if (denom == 0) {
    spdlog::warn("Invalid measurement values - division by zero");
  } else {
    float sq_denom = sqrt(denom);
    float denom3_2 = denom * sq_denom;

    // compute the Jacobian matrix
    J_ << px / sq_denom, py / sq_denom, 0, 0, -py / denom, px / denom, 0, 0,
        py * (vx * py - vy * px) / denom3_2,
        px * (vy * px - vx * py) / denom3_2, px / sq_denom, py / sq_denom;
  }

  return J_;
}
