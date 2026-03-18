/*
  main.c - An embedded CNC Controller with rs274/ngc (g-code) support
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"


// Declare system global variable structure
system_t sys;
int32_t sys_position[N_AXIS];      // Real-time machine (aka home) position vector in steps.   存储坐标的数组
int32_t sys_probe_position[N_AXIS]; // Last probe position in machine coordinates and steps.
volatile uint8_t sys_probe_state;   // Probing state value.  Used to coordinate the probing cycle with stepper ISR.
volatile uint8_t sys_rt_exec_state;   // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
volatile uint8_t sys_rt_exec_alarm;   // Global realtime executor bitflag variable for setting various alarms.
volatile uint8_t sys_rt_exec_motion_override; // Global realtime executor bitflag variable for motion-based overrides.
volatile uint8_t sys_rt_exec_accessory_override; // Global realtime executor bitflag variable for spindle/coolant overrides.
#ifdef DEBUG
  volatile uint8_t sys_rt_exec_debug;
#endif


int grbl_Start(void)
{
	// Initialize system upon power-up.
	serial_init();   // Setup serial baud rate and interrupts
	settings_init(); // Load Grbl settings from EEPROM
	stepper_init();  // Configure stepper pins and interrupt timers
	system_init();   // Configure pinout pins and pin-change interrupt

	memset(sys_position,0,sizeof(sys_position)); // Clear machine position.
	sei(); // Enable interrupts

	// Initialize system state.
	#ifdef FORCE_INITIALIZATION_ALARM
	  // Force Grbl into an ALARM state upon a power-cycle or hard reset.
	  sys.state = STATE_ALARM;
	#else
	  sys.state = STATE_IDLE;
	#endif

	// Check for power-up and set system alarm if homing is enabled to force homing cycle
	// by setting Grbl's alarm state. Alarm locks out all g-code commands, including the
	// startup scripts, but allows access to settings and internal commands. Only a homing
	// cycle '$H' or kill alarm locks '$X' will disable the alarm.
	// NOTE: The startup script will run after successful completion of the homing cycle, but
	// not after disabling the alarm locks. Prevents motion startup blocks from crashing into
	// things uncontrollably. Very bad.
	#ifdef HOMING_INIT_LOCK
	  if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { sys.state = STATE_ALARM; }
	#endif

	// Grbl initialization loop upon power-up or a system abort. For the latter, all processes
	// will return to this loop to be cleanly re-initialized.
	for(;;) {

	  // Reset system variables.
	  uint8_t prior_state = sys.state;
	  memset(&sys, 0, sizeof(system_t)); // Clear system struct variable.
	  sys.state = prior_state;
	  sys.f_override = DEFAULT_FEED_OVERRIDE;  // Set to 100%
	  sys.r_override = DEFAULT_RAPID_OVERRIDE; // Set to 100%
	  sys.spindle_speed_ovr = DEFAULT_SPINDLE_SPEED_OVERRIDE; // Set to 100%
	memset(sys_probe_position,0,sizeof(sys_probe_position)); // Clear probe position.
	  sys_probe_state = 0;
	  sys_rt_exec_state = 0;
	  sys_rt_exec_alarm = 0;
	  sys_rt_exec_motion_override = 0;
	  sys_rt_exec_accessory_override = 0;


	  serial_reset_read_buffer(); // Clear serial read buffer
	  gc_init(); // Set g-code parser to default state
	  spindle_init();
	  coolant_init();
	  limits_init();
	  probe_init();
	  plan_reset(); // Clear block buffer and planner variables
	  st_reset(); // Clear stepper subsystem variables.


	  plan_sync_position();
	  gc_sync_position();

	  /*----*/
//	  extern settings_t settings;  // 如果settings在其他文件
//
	    // 设置关键参数
//	    settings.pulse_microseconds = 10;
//	    settings.steps_per_mm[0] = 80.0;  // X  82
//	    settings.steps_per_mm[1] = 80.0;  // Y  82
//	    settings.steps_per_mm[2] = 80.0;  // Z
//
//	    // 其他写字机参数
//	    settings.max_rate[0] = 8000.0;
//	    settings.max_rate[1] = 8000.0;
//	    settings.max_rate[2] = 1000.0;
//
//	    settings.acceleration[0] = 1500;
//	    settings.acceleration[1] = 1500; //10800
//	    settings.acceleration[2] = 200.0;
//
//	    settings.max_travel[0] = 300.0;
//	    settings.max_travel[1] = 200.0;
//	    settings.max_travel[2] = (100.0*60*60);
//
//	    // 方向设置
//	    settings.dir_invert_mask = 0;
//	    settings.step_invert_mask = 0;
//	    settings.flags = BITFLAG_INVERT_ST_ENABLE;  // 低电平使能
	    /*----*/

	  report_init_message();

	  //serial_manual_Write("$RST=*\n");
//	  serial_manual_Write("G0 X-100\n");
//	  serial_manual_Write("G0 Y-100\n");
//	  serial_manual_Write("G92 X0 Y0\n");
//	  serial_manual_Write("$?\n");


	  protocol_main_loop();

	}
	return 0;   /* Never reached */
}
