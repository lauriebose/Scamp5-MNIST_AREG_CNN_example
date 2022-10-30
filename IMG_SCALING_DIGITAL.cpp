//Author: Laurie Bose
//Date: 2021

#include "IMG_SCALING_DIGITAL.hpp"

static const int scaling_rowcol_order[] = {
		64, 32, 98, 16, 83, 50, 118, 8, 77, 43, 113, 26, 97, 62, 4, 77, 41, 114, 23, 97, 61, 14, 90, 53, 34, 113, 74, 2,
		83, 43, 125, 23, 105, 64, 13, 97, 55, 35, 121, 79, 8, 97, 52, 31, 121, 76, 20, 113, 67, 44, 92, 2, 98, 50, 26, 125,
		76, 14, 115, 65, 40, 92, 8, 113, 61, 35, 89, 22, 77, 50, 106, 5, 118, 62, 34, 91, 20, 78, 50, 110, 13, 74, 44, 106,
		29, 92, 61, 125,
};


namespace IMGTF
{
	namespace SCALING
	{
		namespace DIGITAL
		{

			void STEP_SCALE_UP_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R1,R3);
				scamp5_kernel_end();
				scamp5_load_rect(R2,0,0,255,128-i);
				scamp5_load_rect(R4,0,i+128,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R2,R4);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R2,R4);
				scamp5_kernel_end();
				scamp5_load_rect(R1,0,0,128-i,255);
				scamp5_load_rect(R3,i+128,0,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R1,R3);
					MOV(R12,R11);
				scamp5_kernel_end();


				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void STEP_SCALE_DOWN_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R1,R3);
				scamp5_kernel_end();
				scamp5_load_rect(R4,0,0,255,128-i);
				scamp5_load_rect(R2,0,i+128,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R2,R4);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R2,R4);
				scamp5_kernel_end();
				scamp5_load_rect(R3,0,0,128-i,255);
				scamp5_load_rect(R1,i+128,0,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R1,R3);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void STEP_SCALE_UPY_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R2,R4);
				scamp5_kernel_end();
				scamp5_load_rect(R1,0,0,128-i,255);
				scamp5_load_rect(R3,i+128,0,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R1,R3);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void STEP_SCALE_DOWNY_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R2,R4);
				scamp5_kernel_end();
				scamp5_load_rect(R3,0,0,128-i,255);
				scamp5_load_rect(R1,i+128,0,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R1,R3);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void STEP_SCALE_UPX_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R1,R3);
				scamp5_kernel_end();
				scamp5_load_rect(R2,0,0,255,128-i);
				scamp5_load_rect(R4,0,i+128,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R2,R4);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void STEP_SCALE_DOWNX_R11(int step_number)
			{
				unsigned char i = scaling_rowcol_order[step_number];

				scamp5_kernel_begin();
					SET(R0);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin()
							CLR(R1,R3);
				scamp5_kernel_end();
				scamp5_load_rect(R4,0,0,255,128-i);
				scamp5_load_rect(R2,0,i+128,255,255);

				scamp5_kernel_begin();
					DNEWS0(R11,R12);
					OR(R0,R2,R4);
					MOV(R12,R11);
				scamp5_kernel_end();

				scamp5_kernel_begin();
					MOV(R11,R12);
				scamp5_kernel_end();
				return;
			}

			void SCALE_Y(dreg_t reg,int scaling_mag,bool scale_down)
			{
				scamp5_dynamic_kernel_begin();
					MOV(R11,reg);
				scamp5_dynamic_kernel_end();
				if(!scale_down)
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_UPY_R11(n);
					}
				}
				else
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_DOWNY_R11(n);
					}
				}
				scamp5_dynamic_kernel_begin();
					MOV(reg,R11);
				scamp5_dynamic_kernel_end();
			}


			void SCALE_X(dreg_t reg,int scaling_mag,bool scale_down)
			{
				scamp5_dynamic_kernel_begin();
					MOV(R11,reg);
				scamp5_dynamic_kernel_end();
				if(!scale_down)
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_UPX_R11(n);
					}
				}
				else
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_DOWNX_R11(n);
					}
				}
				scamp5_dynamic_kernel_begin();
					MOV(reg,R11);
				scamp5_dynamic_kernel_end();
			}


			void SCALE(dreg_t reg,int scaling_mag,bool scale_down)
			{
//				scamp5_dynamic_kernel_begin();
//					MOV(R11,reg);
//				scamp5_dynamic_kernel_end();
				if(!scale_down)
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_UP_R11(n);
					}
				}
				else
				{
					for(unsigned char n = 0 ; n < scaling_mag ; n++)
					{
						STEP_SCALE_DOWN_R11(n);
					}
				}
//				scamp5_dynamic_kernel_begin();
//					MOV(reg,R11);
//				scamp5_dynamic_kernel_end();
			}



			int STEP_SCALE(dreg_t reg,int current_scaling_value, bool scale_DOWN)
			{
				scamp5_dynamic_kernel_begin();
					MOV(R11,reg);
				scamp5_dynamic_kernel_end();
				if(current_scaling_value > 0)
				{
					if(!scale_DOWN)
					{
						STEP_SCALE_UPX_R11(current_scaling_value);
						STEP_SCALE_UPY_R11(current_scaling_value);
						current_scaling_value = current_scaling_value + 1;
					}
					else
					{
						current_scaling_value = current_scaling_value - 1;
						STEP_SCALE_DOWNY_R11(current_scaling_value);
						STEP_SCALE_DOWNX_R11(current_scaling_value);
					}
				}
				else
				{
					if(current_scaling_value < 0)
					{
						if(!scale_DOWN)
						{
							current_scaling_value = current_scaling_value + 1;
							STEP_SCALE_UPY_R11(-current_scaling_value);
							STEP_SCALE_UPX_R11(-current_scaling_value);
						}
						else
						{
							STEP_SCALE_DOWNX_R11(-current_scaling_value);
							STEP_SCALE_DOWNY_R11(-current_scaling_value);
							current_scaling_value = current_scaling_value - 1;
						}
					}
					else
					{
						if(!scale_DOWN)
						{
							STEP_SCALE_UPX_R11(current_scaling_value);
							STEP_SCALE_UPY_R11(current_scaling_value);
							current_scaling_value = current_scaling_value + 1;
						}
						else
						{
							STEP_SCALE_DOWNX_R11(current_scaling_value);
							STEP_SCALE_DOWNY_R11(current_scaling_value);
							current_scaling_value = current_scaling_value - 1;
						}
					}
				}
				scamp5_dynamic_kernel_begin();
					MOV(reg,R11);
				scamp5_dynamic_kernel_end();
				return current_scaling_value;
			}



			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			void HALF_SCALE(dreg_t reg) //USES R11 R0 R12
			{
				scamp5_dynamic_kernel_begin();
					SET(R0);
					MOV(R12,reg);
				scamp5_dynamic_kernel_end();

				scamp5_kernel_begin();
					CLR(R1,R2,R3,R4);
				scamp5_kernel_end();

				uint8_t x = 128;
				uint8_t y = 0;
				uint8_t w = 127;
				uint8_t h = 255;
				scamp5_load_rect(R0,y,x,y+h,x+w);
				for(int n = 0; n < 128 ; n++)
				{
					scamp5_kernel_begin();
						SET(R2);
						WHERE(R12);
						DNEWS0(R11,FLAG);
						MOV(R12,R11);
						CLR(R2);

						SET(R4);
						WHERE(R0);
						DNEWS0(R0,FLAG);
						CLR(R4);
					scamp5_kernel_end();
				}

				x = 0;
				y = 0;
				w = 127;
				h = 255;
				scamp5_load_rect(R0,y,x,y+h,x+w);
				for(int n = 0; n < 128 ; n++)
				{
					scamp5_kernel_begin();
						SET(R4);
						WHERE(R12);
						DNEWS0(R11,FLAG);
						MOV(R12,R11);
						CLR(R4);

						SET(R2);
						WHERE(R0);
						DNEWS0(R0,FLAG);
						CLR(R2);
					scamp5_kernel_end();
				}


				x = 0;
				y = 128;
				w = 255;
				h = 127;
				scamp5_load_rect(R0,y,x,y+h,x+w);
				for(int n = 0; n < 128 ; n++)
				{
					scamp5_kernel_begin();
						SET(R1);
						WHERE(R12);
						DNEWS0(R11,FLAG);
						MOV(R12,R11);
						CLR(R1);

						SET(R3);
						WHERE(R0);
						DNEWS0(R0,FLAG);
						CLR(R3);
					scamp5_kernel_end();
				}

				x = 0;
				y = 0;
				w = 255;
				h = 127;
				scamp5_load_rect(R0,y,x,y+h,x+w);
				for(int n = 0; n < 128 ; n++)
				{
					scamp5_kernel_begin();
						SET(R3);
						WHERE(R12);
						DNEWS0(R11,FLAG);
						MOV(R12,R11);
						CLR(R3);

						SET(R1);
						WHERE(R0);
						DNEWS0(R0,FLAG);
						CLR(R1);
					scamp5_kernel_end();
				}

				scamp5_dynamic_kernel_begin();
					MOV(reg,R12);
				scamp5_dynamic_kernel_end();

				scamp5_kernel_begin();
					ALL();
				scamp5_kernel_end();
			}
		}
	}
}
