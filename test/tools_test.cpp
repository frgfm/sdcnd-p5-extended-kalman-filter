#include <iostream>
#include "../src/tools.h"
#include "catch.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

SCENARIO("RMSE computation") {
  GIVEN("estimations and ground truth") {
    Tools tools;
    vector<VectorXd> estimations;
    vector<VectorXd> ground_truth;

    WHEN("estimations are empty") {
      // Add a single ground truth sample
      VectorXd a_ground_truth(4);
      a_ground_truth << 1, 2, 3, 4;
      ground_truth.push_back(a_ground_truth);

      VectorXd rmse = tools.CalculateRMSE(estimations, ground_truth);

      THEN("RMSE is not computed") {
        REQUIRE(rmse.size() == 4);
        REQUIRE(rmse.sum() == 0);
      }
    }

    WHEN("ground-truths are empty") {
      // Add a single estimatin sample
      VectorXd an_estimation(4);
      an_estimation << 1, 2, 3, 4;
      estimations.push_back(an_estimation);

      VectorXd rmse = tools.CalculateRMSE(estimations, ground_truth);

      THEN("RMSE is not computed") {
        REQUIRE(rmse.size() == 4);
        REQUIRE(rmse.sum() == 0);
      }
    }

    WHEN("estimations and ground-truth doesn't have the same size") {
      // Add 2 estimation samples
      VectorXd an_estimation(4);
      an_estimation << 1, 2, 3, 4;
      estimations.push_back(an_estimation);
      estimations.push_back(an_estimation);
      // Add a single ground truth sample
      VectorXd a_ground_truth(4);
      a_ground_truth << 1, 2, 3, 4;
      ground_truth.push_back(a_ground_truth);

      VectorXd rmse = tools.CalculateRMSE(estimations, ground_truth);

      THEN("RMSE is not computed") {
        REQUIRE(rmse.size() == 4);
        REQUIRE(rmse.sum() == 0);
      }
    }

    WHEN("estimation and ground-truth are good") {
      // Two samples with both estimation and ground truth
      VectorXd an_estimation(4);
      an_estimation << 1, 2, 3, 4;
      estimations.push_back(an_estimation);
      estimations.push_back(an_estimation);
      VectorXd a_ground_truth(4);
      a_ground_truth << 4, 3, 2, 1;
      ground_truth.push_back(a_ground_truth);
      ground_truth.push_back(a_ground_truth);

      VectorXd rmse = tools.CalculateRMSE(estimations, ground_truth);

      THEN("no error is thrown") {
        REQUIRE(rmse.size() == 4);
        REQUIRE(rmse(0) == 3);
        REQUIRE(rmse(1) == 1);
        REQUIRE(rmse(2) == 1);
        REQUIRE(rmse(3) == 3);
      }
    }
  }
}

SCENARIO("Jacobian computation") {
  GIVEN("a state vector") {
    Tools tools;
    VectorXd state(4);

    WHEN("state vector size != 4") {
      // Invalid state vector size for radar update
      state.resize(2);
      state << 1, 2;
      MatrixXd Hj = tools.CalculateJacobian(state);

      THEN("Jacobian matrix is not computed") {
        REQUIRE(Hj.rows() == 3);
        REQUIRE(Hj.cols() == 4);
        REQUIRE(round(Hj.sum()) == 0);
      }
    }

    WHEN("px and py are zero") {
      // Division by zero
      state.resize(4);
      state << 0, 0, 3, 4;
      MatrixXd Hj = tools.CalculateJacobian(state);

      THEN("Jacobian matrix is not computed") {
        REQUIRE(Hj.rows() == 3);
        REQUIRE(Hj.cols() == 4);
        REQUIRE(round(Hj.sum()) == 0);
      }
    }

    WHEN("with other values") {
      // Correct state vector
      state.resize(4);
      state << 1, 2, 3, 4;
      MatrixXd Hj = tools.CalculateJacobian(state);

      THEN("no error is thrown") {
        REQUIRE(Hj.rows() == 3);
        REQUIRE(Hj.cols() == 4);
        REQUIRE(round(Hj(1, 1) * 10) == 2);
        REQUIRE(round(Hj(1, 0) * 10) == -4);
      }
    }
  }
}