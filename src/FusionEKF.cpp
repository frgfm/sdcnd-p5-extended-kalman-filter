#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cos;
using std::cout;
using std::endl;
using std::sin;

/**
 * Constructor.
 */
FusionEKF::FusionEKF() {
  is_initialized_ = false;

  previous_timestamp_ = 0;

  // Sensor configuration
  radar_enabled_ = true;
  laser_enabled_ = true;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);

  // Measurement covariance matrix - laser
  R_laser_ << 0.0225, 0, 0, 0.0225;

  // Measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0, 0, 0.0009, 0, 0, 0, 0.09;

  // Measurement transition function
  H_laser_ << 1, 0, 0, 0, 0, 1, 0, 0;

  // State covariance matrix
  MatrixXd P_(4, 4);
  // State transition function
  MatrixXd F_(4, 4);
  // Process noise covariance matrix
  MatrixXd Q_(4, 4);
  // State vector
  VectorXd x_(4);

  // Initialize the Kalman filter attributes
  ekf_.Init(x_, P_, F_, H_laser_, R_laser_, Q_);
}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() = default;

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {
  /**
   * Initialization
   */
  if (!is_initialized_) {
    // first measurement
    cout << "EKF: " << endl;
    ekf_.x_ << 0, 0, 0, 0;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      // measurement: rho, phi, rho_dot
      float phi = measurement_pack.raw_measurements_(1);

      // Ensure phi is between -Pi & Pi
      while (phi > M_PI) {
        phi -= 2.0 * M_PI;
      }
      while (phi < -M_PI) {
        phi += 2.0 * M_PI;
      }

      ekf_.x_ << measurement_pack.raw_measurements_(0) * cos(phi),
          measurement_pack.raw_measurements_(0) * sin(phi),
          measurement_pack.raw_measurements_(2) * cos(phi),
          measurement_pack.raw_measurements_(2) * sin(phi);
    } else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      // Laser does not pick motion
      // measurement : px, py
      ekf_.x_ << measurement_pack.raw_measurements_(0),
          measurement_pack.raw_measurements_(1), 0, 0;
    } else {
      cout << "Unknown sensor type" << endl;
    }
    previous_timestamp_ = measurement_pack.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /**
   * Prediction
   */

  // Convert to seconds
  float dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;
  // Update timestamp
  previous_timestamp_ = measurement_pack.timestamp_;

  // Update the state transition matrix
  ekf_.F_ << 1, 0, dt, 0, 0, 1, 0, dt, 0, 0, 1, 0, 0, 0, 0, 1;

  // Update the process noise covariance matrix
  float noise_ax = 9;
  float noise_ay = 9;
  float dt2 = dt * dt;
  float dt3 = dt2 * dt;
  float dt4 = dt3 * dt;

  ekf_.Q_ << dt4 / 4 * noise_ax, 0, dt3 / 2 * noise_ax, 0, 0,
      dt4 / 4 * noise_ay, 0, dt3 / 2 * noise_ay, dt3 / 2 * noise_ax, 0,
      dt2 * noise_ax, 0, 0, dt3 / 2 * noise_ay, 0, dt2 * noise_ay;

  ekf_.Predict();

  /**
   * Update
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR && radar_enabled_) {
    // Radar updates
    ekf_.R_ = R_radar_;
    Tools tools;
    Hj_ = tools.CalculateJacobian(ekf_.x_);
    ekf_.H_ = Hj_;
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);

  } else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER && laser_enabled_) {
    // Laser updates
    ekf_.R_ = R_laser_;
    ekf_.H_ = H_laser_;
    ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}
