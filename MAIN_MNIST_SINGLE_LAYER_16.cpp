//Author: Laurie Bose
//Date: 2021

#include <math.h>
#include <scamp5.hpp>
#include <list>
#include <string>
#include <sstream>
#include <bitset>
#include <limits.h>

#include "AREG_POOLING.hpp"
#include "MISC_FUNCTIONS.hpp"

#include "FUNCS_DIGIT_EXTRACTION.hpp"
#include "FUNCS_WEIGHT_LOADING.hpp"
#include "WEIGHTS_MNIST_4x4_16CONVOLS_64x64INPUT_95ACC.hpp"

void load_dreg_image(DREG target_dreg,const uint8_t*image_buffer,uint16_t n_rows);
void refresh_dreg_storage();

extern int default_convolution_filters[];

const int filters = 16;
const int filter_size = 4;

#define final_neuron_activations_log_length 3
int final_neuron_activations_log_index = 0;
int final_neuron_activations_log [final_neuron_activations_log_length][10];
int final_neuron_activations_averages [10];

//void change_digit(int img_digit, int extracted_digit_img_size,int grid_posx,int grid_posy,int input_img_size,int weight_grid_width,int grid_cell_padding)
//{
//	 if(vs_gui_request_done() && vs_gui_is_on()){
//
//		std::string tmp_string;
//		std::stringstream ss;
//		ss << "C:/Users/lauri/Desktop/mnist_data/";
//		ss << img_digit;
//		ss << ".bmp";
//		tmp_string = ss.str();
//		vs_gui_request_image(tmp_string.c_str(),1,[&](vs_dotmat const& dotmat,int s){
//			if(s<0)
//			{
//				return;
//			}
//			load_dreg_image(R11,(const uint8_t*)dotmat.get_buffer(),dotmat.get_height());
//		});
//		do{
//			scamp5_kernel_begin();
//				REFRESH(R11);
//			scamp5_kernel_end();
//			vs_process_message();
//		}while(!vs_gui_request_done());
//	}
//	MISC_FUNCTIONS::load_DREG_into_F(DENUM::R11,120,-120);
//
//	extract_character_from_F_into_R11(1,100,extracted_digit_img_size,grid_posx,grid_posy-1);
//	extract_digit_and_duplicate_into_grid_R10(input_img_size, weight_grid_width,grid_cell_padding);
//
//	scamp5_kernel_begin();
//		MOV(R5,R10);
//	scamp5_kernel_end();
//}

void MAIN_MNIST_SINGLE_LAYER_16()
{
	vs_stopwatch timer;

	// Initialization
	vs_init();

	int predictions = 0;
	int correct_predictions = 0;
	int current_prediction = 0;
	int current_true_digit = 0;

	int auto_evaluation = 0;
	int auto_evaluation_max_counter = final_neuron_activations_log_length*2;
	int auto_evaluation_counter = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SETUP DISPLAYS

	const int display_size = 1;

	auto display00 = vs_gui_add_display("",0,0,display_size);
	auto display01 = vs_gui_add_display("",0,display_size,display_size);
	auto display02 = vs_gui_add_display("",0,display_size*2,display_size,"preset_pgnr");
//	auto display03 = vs_gui_add_display("",0,display_size*3,display_size);

	auto display10 = vs_gui_add_display("",display_size,0,display_size,"preset_pgnr");
	auto display11 = vs_gui_add_display("",display_size,display_size,display_size,"preset_pgnr");
	auto display12 = vs_gui_add_display(" 0  1  2  3  4   5  6  7  8  9",display_size,display_size*2,display_size);
//	auto display13 = vs_gui_add_display("",display_size,display_size*3,display_size);

	vs_gui_set_barplot(display12,0,100,15);

	bool print_timings = true;
	auto print_timings_but = vs_gui_add_button("print timings");
	 vs_on_gui_update(print_timings_but,[&](int32_t new_value){
		 print_timings = true;
		 predictions = 0;
		 correct_predictions = 0;
	    });

	int show_debug_displays = 1;
//	vs_gui_add_switch("show_debug_displays",true,&show_debug_displays);

	int img_threshold = 100;
	vs_gui_add_slider("img_threshold",-120,120,100,&img_threshold);

//	int weights_display = 3;
//	vs_gui_add_slider("weights_display",0,3,3,&weights_display);

//	vs_gui_add_switch("auto eval",0,&auto_evaluation);

	int grid_posx = 256-(1+64/2), grid_posy = 64/2;
//	vs_gui_add_slider("grid_posx",0,256,256-(1+64/2),&grid_posx);
//	vs_gui_add_slider("grid_posy",0,256,64/2,&grid_posy);
	int weight_grid_width = 256/64;

	int input_img_size = 64;
//	vs_gui_add_slider("input_img_size",0,64,64,&input_img_size);

	int grid_cell_padding = 0;
//	vs_gui_add_slider("grid_cell_padding",0,28,0,&grid_cell_padding);

	int convol_steps = filters-1;
//	vs_gui_add_slider("convol_steps",0,filters-1,filters-1,&convol_steps);

	int input_white = 120;
	vs_gui_add_slider("input_white",-120,120,15,&input_white);

	int input_black = 0;
//	vs_gui_add_slider("input_black",-120,120,0,&input_black);

	int apply_relu = 1;
//	vs_gui_add_switch("apply_relu",1,&apply_relu);

	int maxpool = 4;
//	vs_gui_add_slider("maxpool",1,4,4,&maxpool);

	int maxpool_dirx = 0,maxpool_diry = 1;
//	vs_gui_add_switch("maxpool_dirx",false,&maxpool_dirx);
//	vs_gui_add_switch("maxpool_diry",true,&maxpool_diry);

	int maxpool_blocking = 1;
//	vs_gui_add_switch("maxpool_blocking",1,&maxpool_blocking);

//	int convol_readout_display = 0;
//	vs_gui_add_slider("convol_readout_display",0,filters-1,0,&convol_readout_display);

	int FC_weights_orderx = 0,FC_weights_ordery = 1;
//	vs_gui_add_switch("FC_weights_orderx",false,&FC_weights_orderx);
//	vs_gui_add_switch("FC_weights_ordery",true,&FC_weights_ordery);

	int FC_switch_xy = 0;
//	vs_gui_add_switch("FC_switch_xy",false,&FC_switch_xy);

	int extracted_digit_img_size = 45;
	auto digit_size_slider = vs_gui_add_slider("extracted_digit_img_size",0,128,45,&extracted_digit_img_size);

//	int img_digit = 1;
//	auto digit_selection_slider = vs_gui_add_slider("img_digit",0,10000,std::rand()%8800,&img_digit);

//	 vs_on_gui_update(digit_size_slider,[&](int32_t new_value)
//	 {
//		 change_digit( img_digit,  extracted_digit_img_size, grid_posx, grid_posy, input_img_size, weight_grid_width, grid_cell_padding);
//	});

//	 vs_on_gui_update(digit_selection_slider,[&](int32_t new_value)
//	 {
//		 predictions++;
//		 if(current_prediction == current_true_digit)
//		 {
//			 correct_predictions++;
//		 }
//		 current_true_digit = new_value%10;
//		 vs_post_text("%d %d %d \n", (1000*correct_predictions)/predictions,correct_predictions,predictions);
//
//		 change_digit( img_digit,  extracted_digit_img_size, grid_posx, grid_posy, input_img_size, weight_grid_width, grid_cell_padding);
//
//	});

	 int FC_weight_debug = -1;
	 vs_gui_add_slider("FC weight debug",-1,9,-1,&FC_weight_debug);

	 auto load_fc_weights_button = vs_gui_add_button("load_fc_weights_button");
	 vs_on_gui_update(load_fc_weights_button,[&](int32_t new_value)
	 {
		load_FC_weights_into_grid_E(FC_weight_debug+1,filters,10,FC_layer_weights_MNIST_4x4,FC_switch_xy,FC_weights_orderx,FC_weights_ordery,input_img_size,grid_posx, grid_posy, weight_grid_width,grid_cell_padding);
		scamp5_kernel_begin();
			mov(A,E);
		scamp5_kernel_end();

		load_filter_weights_into_grid_F(filters,filter_size,TW_CONVOL_weights_MNIST_4x4,input_img_size,grid_posx, grid_posy, weight_grid_width,grid_cell_padding);
		scamp5_kernel_begin();
			mov(B,F);
		scamp5_kernel_end();
	 });

	int gsum_order = 0;
//	vs_gui_add_slider("gsum_order",0,9,0,&gsum_order);

	int tweak;
	vs_gui_add_slider("tweak",0,20,1,&tweak);

  auto slider_vxc = vs_gui_add_slider("vxc: ",0,4095,2450);
  vs_on_gui_update(slider_vxc,[&](int32_t new_value){
		vs_scamp5_configure_voltage(3,new_value);
   });

	int min_score = INT_MAX;
	int max_score = INT_MIN;

	while(true)
	{
		scamp5_get_image(F,E,vs_gui_read_slider(VS_GUI_FRAME_GAIN));

		scamp5_output_image(F,display00);
		extract_character_from_F_into_R11(0,img_threshold,extracted_digit_img_size,grid_posx,grid_posy-1);
		duplicate_digit_into_grid_R10(input_img_size, weight_grid_width,grid_cell_padding);
		scamp5_kernel_begin();
			MOV(R5,R10);
		scamp5_kernel_end();
		scamp5_output_image(R5,display01);

//////////////////////////////////////////////////////////////////////////////////////////////

		//REFRESH DIGIT DUPLICATES
		scamp5_kernel_begin();
			REFRESH(R5);
		scamp5_kernel_end();

		//REFRESH FC WEIGHTS IN AREG A AND CONVOL WEIGHTS IN B
		vs_process_message();
		vs_wait_frame_trigger();

		scamp5_load_in(E,0);
		scamp5_load_in(50);
		scamp5_kernel_begin();
			mov(F,A);
			sub(F,F,IN);
			where(F);
				add(E,E,IN);
				add(E,E,IN);
			all();
			bus(F,A);
			sub(F,F,IN);
			where(F);
				sub(E,E,IN);
				sub(E,E,IN);
			all();
			mov(A,E);
		scamp5_kernel_end();

		scamp5_load_in(E,0);
		scamp5_load_in(80);
		scamp5_kernel_begin();
			mov(F,B);
			sub(F,F,IN);
			where(F);
				add(E,E,IN);
				add(E,E,IN);
			all();
			bus(F,B);
			sub(F,F,IN);
			where(F);
				sub(E,E,IN);
				sub(E,E,IN);
			all();
			mov(B,E);
		scamp5_kernel_end();

		scamp5_output_image(B,display10);
		scamp5_output_image(A,display11);

//////////////////////////////////////////////////////////////////////////////////////////////
		//PERFORM CONVOLUTION LAYER

		load_DREG_into_F(DENUM::R5,input_white,input_black);
		scamp5_kernel_begin();
			MOV(R8,R5);
		scamp5_kernel_end();

		scamp5_load_in(0);
		scamp5_kernel_begin();
			mov(C,IN);
		scamp5_kernel_end();
		scamp5_load_in(40);
		scamp5_kernel_begin();
			sub(D,B,IN);
			where(D);
				MOV(R9,FLAG);
			all();
		scamp5_kernel_end();

		scamp5_load_in(F,input_white);


		int shift2 = 1;
		if(shift2 > 0){
			while(shift2--){
				scamp5_kernel_begin();
					WHERE(R9);
					DNEWS(R9,FLAG,north);
				scamp5_kernel_end();
			}
		}else
		if(shift2 < 0){
			while(shift2++){
				scamp5_kernel_begin();
					WHERE(R9);
					DNEWS(R9,FLAG,south);
				scamp5_kernel_end();
			}
		}

		shift2 = 0;
		if(shift2 > 0){
			while(shift2--){
				scamp5_kernel_begin();
					WHERE(R9);
					DNEWS(R9,FLAG,east);
				scamp5_kernel_end();
			}
		}else
		if(shift2 < 0){
			while(shift2++){
				scamp5_kernel_begin();
					WHERE(R9);
					DNEWS(R9,FLAG,west);
				scamp5_kernel_end();
			}
		}

		timer.reset();

		////////////////////////////////////////////////////////////
		//CREATE PATTERN USED TO CONSTRAIN FLOODING WHEN SPREADING FILTER WEIGHTS

		//for 8x8 grid of 32x32 computation blocks
//		scamp5_load_pattern(R1,0,0,255,224);
//		scamp5_load_pattern(R2,0,0,224,255);
		//for 4x4 grid of 64x64 computation blocks
		scamp5_load_pattern(R1,0,0,255,192);
		scamp5_load_pattern(R2,0,0,192,255);
		scamp5_kernel_begin();
			OR(R3,R1,R2);
			NOT(R10,R3);
		scamp5_kernel_end();

		//////////////////////////////////////////////////////////////

		int weight_index = 0;
		for(int y = 0 ; y < filter_size ; y++)
		{
			for(int x = 0 ; x < filter_size ; x++)
			{
				if(weight_index > convol_steps)
				{
					y = filter_size;
					break;
				}

				scamp5_load_pattern(R11,8-weight_index/4,7-weight_index%4,224,224);

				scamp5_kernel_begin();

					AND(R1,R9,R11);
					MOV(R11,R1);
				scamp5_kernel_end();

				scamp5_flood(R11,R10,0,1);

				scamp5_kernel_begin();

					AND(R1,R8,R11);
					WHERE(R1);
						add(C,C,F);
					NOT(R1,R11);
					AND(R2,R8,R1);
					WHERE(R2);
						sub(C,C,F);
					all();
				scamp5_kernel_end();

				if(x != filter_size-1)
				{
					scamp5_kernel_begin();
						WHERE(R8);
						DNEWS(R8,FLAG,west);
						ALL();
					scamp5_kernel_end();
				}
				else
				{
					//TODO PUT WEIGHTS IN THE RIGHT ORDER RATHER THAN DOING THIS
					scamp5_kernel_begin();
						WHERE(R8);
						DNEWS(R8,FLAG,east);
						ALL();
						WHERE(R8);
						DNEWS(R8,FLAG,east);
						ALL();
						WHERE(R8);
						DNEWS(R8,FLAG,east);
						ALL();
						WHERE(R8);
						DNEWS(R8,FLAG,south);
						ALL();
					scamp5_kernel_end();
				}
				weight_index++;
			}
		}

		if(print_timings)
		{
			vs_post_text("CONVOL LAYER TIME %d \n",timer.get_usec());
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//PERFORM RELU

		timer.reset();

		if(apply_relu)
		{
			scamp5_load_in(F,0);
			scamp5_kernel_begin();
				where(C);
					mov(F,C);
				all();
				mov(C,F);
			scamp5_kernel_end();
		}

//		scamp5_load_in(F,convol_activation_thresh);
//		scamp5_kernel_begin();
//			sub(F,F,C);
//			where(F);
//				mov(C,IN);
//			all();
//		scamp5_kernel_end();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//PERFORM MAXPOOLING



		if(maxpool > 1)
		{
			scamp5_kernel_begin();
					mov(F,C);
			scamp5_kernel_end();
			AREG_POOLING::MAX_POOL_F(maxpool-1,maxpool_dirx == 1, maxpool_diry == 1, maxpool_blocking);
			scamp5_kernel_begin();
					mov(C,F);
			scamp5_kernel_end();
		}

		int xshift = 1;
		if(xshift > 0){
			while(xshift--){
				scamp5_kernel_begin();
					movx(C,C,west);
				scamp5_kernel_end();
			}
		}else
		if(xshift < 0){
			while(xshift++){
				scamp5_kernel_begin();
					movx(C,C,east);
				scamp5_kernel_end();
			}
		}

		int yshift = -1;
		if(yshift > 0){
			while(yshift--){
				scamp5_kernel_begin();
					movx(C,C,south);
				scamp5_kernel_end();
			}
		}else
		if(yshift < 0){
			while(yshift++){
				scamp5_kernel_begin();
					movx(C,C,north);
				scamp5_kernel_end();
			}
		}

		if(print_timings)
		{
			vs_post_text("RELU MAXPOOL TIME %d \n",timer.get_usec());
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//PPA FC layer

		timer.reset();

		scamp5_kernel_begin();
			REFRESH(R6);
			REFRESH(R7);
		scamp5_kernel_end();
//		scamp5_output_image(E,display11);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//PUT FC WEIGHTS INTO DREG

		scamp5_load_in(50);
		scamp5_kernel_begin();
			mov(F,A);
			sub(F,F,IN);
			where(F);
				MOV(R6,FLAG);
			all();

			bus(F,A);
			sub(F,F,IN);
			where(F);
				MOV(R7,FLAG);
			all();
		scamp5_kernel_end();

		scamp5_load_in(F,0);
		scamp5_kernel_begin();

			WHERE(R6);
				add(F,F,C);
			ALL();

			WHERE(R7);
				sub(F,F,C);
			ALL();
		scamp5_kernel_end();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//PERFORM FC LAYER

		int sums[] = {0,0,0,0,0,0,0,0,0,0};
		min_score = INT_MAX;
		max_score = INT_MIN;

		for(int j = 0 ; j < tweak ; j++)
		{
			scamp5_load_in(0);
			scamp5_kernel_begin();
				mov(F,IN);

				WHERE(R6);
					add(F,F,C);
				ALL();

				WHERE(R7);
					sub(F,F,C);
				ALL();

				mov(E,F);
			scamp5_kernel_end();
			gsum_order = (gsum_order+1)%10;
			for(int i = 0 ; i < 10 ; i++)
			{
				int n = (i+gsum_order)%10;

				int x = n%4-1;
				int y = n/4;

				sums[n] += scamp5_global_sum_sparse(E,y,x,124,124);
			}


			scamp5_load_in(0);
			scamp5_kernel_begin();
				mov(F,IN);

				WHERE(R6);
					add(F,F,C);
				ALL();

				WHERE(R7);
					sub(F,F,C);
				ALL();

				mov(E,F);
			scamp5_kernel_end();
			gsum_order = (gsum_order+1)%10;
			for(int i = 0 ; i < 10 ; i++)
			{
				int n = (i+gsum_order)%10;

				int x = n%4-1;
				int y = n/4;

				sums[n]  += scamp5_global_sum_sparse(E,y+128,x,124,124);
			}


			scamp5_load_in(0);
			scamp5_kernel_begin();
				mov(F,IN);

				WHERE(R6);
					add(F,F,C);
				ALL();

				WHERE(R7);
					sub(F,F,C);
				ALL();

				mov(E,F);
			scamp5_kernel_end();
			gsum_order = (gsum_order+1)%10;
			for(int i = 0 ; i < 10 ; i++)
			{
				int n = (i+gsum_order)%10;

				int x = n%4-1;
				int y = n/4;

				sums[n]  += scamp5_global_sum_sparse(E,y,x+128,124,124);
			}


			scamp5_load_in(0);
			scamp5_kernel_begin();
				mov(F,IN);

				WHERE(R6);
					add(F,F,C);
				ALL();

				WHERE(R7);
					sub(F,F,C);
				ALL();

				mov(E,F);
			scamp5_kernel_end();
			gsum_order = (gsum_order+1)%10;
			for(int i = 0 ; i < 10 ; i++)
			{
				int n = (i+gsum_order)%10;

				int x = n%4-1;
				int y = n/4;

				sums[n]  += scamp5_global_sum_sparse(E,y+128,x+128,124,124);
			}
		}


		for(int i = 0 ; i < 10 ; i++)
		{
			if(sums[i] > max_score)
			{
				max_score = sums[i];
			}
			if(sums[i] < min_score)
			{
				min_score = sums[i];
			}
		}

		if(print_timings)
		{
			vs_post_text("FC LAYER TIME %d \n",timer.get_usec());
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//UPDATE FINAL NEURON ACTIVATIONS AND PREDICTION

		for(int n = 0 ; n < 10 ; n++)
		{
			final_neuron_activations_log[final_neuron_activations_log_index][n] = 100*(sums[n]-min_score)/(max_score-min_score);
		}
		final_neuron_activations_log_index = (final_neuron_activations_log_index + 1)%final_neuron_activations_log_length;

		max_score = INT_MIN;
		for(int n = 0 ; n < 10 ; n++)
		{
			final_neuron_activations_averages[n] = 0;
			for(int i = 0 ; i < final_neuron_activations_log_length ; i++)
			{
				final_neuron_activations_averages[n] = final_neuron_activations_averages[n] + final_neuron_activations_log[i][n];
			}
			final_neuron_activations_averages[n] = final_neuron_activations_averages[n] / final_neuron_activations_log_length;
			if(final_neuron_activations_averages[n] > max_score)
			{
				current_prediction = n;
				max_score = final_neuron_activations_averages[n];
			}
		}

		int16_t bar_values_16b[10];
		for(int n = 0 ; n < 10 ; n++)
		{
			bar_values_16b[n] = final_neuron_activations_averages[n];
		}
		vs_post_set_channel(display12);
		vs_post_int16(bar_values_16b,1,10);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(auto_evaluation)
		{
			auto_evaluation_counter++;
			if(auto_evaluation_counter > auto_evaluation_max_counter)
			{
				auto_evaluation_counter = 0;
				int random_digit = std::rand()%8800;
//				vs_gui_move_slider(digit_selection_slider,random_digit,false);
			}
		}

		print_timings = false;

		if(vs_loop_counter_get()%25==0){
			vs_led_toggle(VS_LED_1);
		}
		vs_loop_counter_inc();
	}
}



