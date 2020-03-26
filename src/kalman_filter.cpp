#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() = default;

KalmanFilter::~KalmanFilter() = default;

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
    // Prediction step
    x_ = F_ * x_;
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
    // Update step
    VectorXd z_pred = H_ * x_;
    // Use the result of measurement function
    VectorXd y = z - z_pred;
    MeasurementUpdate(y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
    // Switch from cartesian to polar coords
    float rho = sqrt(x_(0) * x_(0) + x_(1) * x_(1));
    float phi = atan2(x_(1), x_(0));
    float rho_dot = 0;
    if (rho != 0) {
        rho_dot = (x_(0) * x_(2) + x_(1) * x_(3)) / rho;
    }

    // Update step
    VectorXd z_pred(3);
    z_pred << rho, phi, rho_dot;

    // Use the result of measurement function
    VectorXd y = z - z_pred;
    // Ensure the angle is between -Pi and Pi
    while (y(1) > M_PI) {y(1) -= 2.0 * M_PI;}
    while (y(1) < -M_PI) {y(1) += 2.0 * M_PI;}

    MeasurementUpdate(y);
}

void KalmanFilter::MeasurementUpdate(const VectorXd &y) {
    // Update step
    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;

    // New estimate
    x_ = x_ + (K * y);
    uint x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}
