//Author: Laurie Bose
//Date: 2021

#include <scamp5.hpp>
#include <math.h>

using namespace SCAMP5_PE;

#ifndef IMG_SCALING_DIGITAL
#define IMG_SCALING_DIGITAL

namespace IMGTF
{
	namespace SCALING
	{
		namespace DIGITAL
		{
			void STEP_SCALE_UP_R11(int step_number);
			void STEP_SCALE_DOWN_R11(int step_number);

			void STEP_SCALE_UPY_R11(int step_number);
			void STEP_SCALE_DOWNY_R11(int step_number);

			void STEP_SCALE_UPX_R11(int step_number);
			void STEP_SCALE_DOWNX_R11(int step_number);

			///////////////////////////////////////////

			int STEP_SCALE(dreg_t reg,int current_scaling_value, bool scale_down);
			void SCALE_Y(dreg_t reg,int scaling_mag, bool scale_down);
			void SCALE_X(dreg_t reg,int scaling_mag, bool scale_down);
			void SCALE(dreg_t reg,int scaling_mag, bool scale_down);

			///////////////////////////////////////////

			void HALF_SCALE(dreg_t reg);
		}
	}
}
#endif
