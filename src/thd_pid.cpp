/*
 * thd_pid.cpp: pid implementation
 *
 * Copyright (C) 2013 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 or later as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 *
 * Author Name <Srinivas.Pandruvada@linux.intel.com>
 *
 */
#include "thd_pid.h"

cthd_pid::cthd_pid() {
	kp = 0.0005;
	ki = kd = 0.0001;
	last_time = 0;
	err_sum = 0.0;
	last_err = 0.0;
	target_temp = 0;
}

int cthd_pid::pid_output(unsigned int curr_temp) {
	double output;
	double d_err = 0;

	time_t now;
	time(&now);
	if (last_time == 0)
		last_time = now;
	time_t timeChange = (now - last_time);

	int error = curr_temp - target_temp;
	err_sum += (error * timeChange);
	if (timeChange)
		d_err = (error - last_err) / timeChange;
	else
		d_err = 0.0;

	/*Compute PID Output*/
	output = kp * error + ki * err_sum + kd * d_err;

	/*Remember some variables for next time*/
	last_err = error;
	last_time = now;
	thd_log_info("pid_output %d:%d %g:%d\n", curr_temp, target_temp, output,
			(int) output);
	return (int) output;
}