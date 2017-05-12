#include "filters.h"

namespace Project {
	namespace Filters {
		//KalmanFilter
		KalmanFilter::KalmanFilter(double f, double h, double r, double q) {
			Q = q;
			R = r;
			F = f;
			H = h;
			B = 0;
		}
		void KalmanFilter::SetState(double state, double covariance) {
			State = state;
			Covariance = covariance;
		}
		double KalmanFilter::GetState() {
			return State;
		}
		void KalmanFilter::Correct(double data) {
			X0 = F * State;
			P0 = F * Covariance * F + Q;

			double K = H * P0 / (H * P0 * H + R);
			State = X0 + K * (data - H * X0);
			Covariance = (1 - K * H) * P0;
		}
		//
	}
}
