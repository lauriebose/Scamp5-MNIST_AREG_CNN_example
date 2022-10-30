//Author: Laurie Bose
//Date: 2021

#define filter_size 4

#include <math.h>
#include <scamp5.hpp>
#include <list>
#include <string>

#include "TIMING_STATS.hpp"

using namespace SCAMP5_PE;
using namespace std;

void MAIN_MNIST_SINGLE_LAYER_16();

void scamp5_main(){

    vs_on_gui_update(VS_GUI_FRAME_RATE,[&](int32_t new_value){
        uint32_t framerate = new_value;
        if(framerate > 0){
            vs_frame_trigger_set(1,framerate);
            vs_enable_frame_trigger();
            vs_post_text("frame trigger: 1/%d\n",(int)framerate);
        }else{
            vs_disable_frame_trigger();
            vs_post_text("frame trigger disabled\n");
        }
    });

    vs_on_host_connect([&](){
    	vs_post_text("Scamp5d Image Storage\n");
    	vs_post_text("loop_counter: %d\n",(int)vs_loop_counter_get());
        scamp5_kernel::print_debug_info();
        vs_led_on(VS_LED_2);
    });

    vs_on_host_disconnect([&](){
        vs_led_off(VS_LED_2);
    });

	auto slider_vxc = vs_gui_add_slider("vxc: ",0,4095,2900);
	  vs_on_gui_update(slider_vxc,[&](int32_t new_value){
			vs_scamp5_configure_voltage(3,new_value);
			});

	  MAIN_MNIST_SINGLE_LAYER_16();
}


int main(){

	// initialise M0 system
	vs_init();

	// make default output to be USB
	vs_post_bind_io_agent(vs_usb);

	scamp5_bind_io_agent(vs_usb);

    vs_on_shutdown([&](){
    	vs_post_text("M0 shutdown\n");
    });

    // run the vision algorithm
	scamp5_main();

	return 0;
}

