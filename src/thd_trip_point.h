/*
 * thd_trip_point.h: thermal zone trip points class interface
 *
 * Copyright (C) 2012 Intel Corporation. All rights reserved.
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

#ifndef THD_TRIP_POINT_H
#define THD_TRIP_POINT_H

#include "thd_common.h"
#include "thd_sys_fs.h"
#include "thd_preference.h"
#include "thd_cdev.h"

#include <vector>
#include <algorithm>    // std::sort
typedef enum {
	CRITICAL, MAX, PASSIVE, ACTIVE, POLLING, INVALID_TRIP_TYPE
} trip_point_type_t;

typedef enum {
	PARALLEL,  // All associated cdevs are activated together
	SEQUENTIAL  // one after other once the previous cdev reaches its max state
} trip_control_type_t;

typedef struct {
	cthd_cdev *cdev;
	int influence;
} trip_pt_cdev_t;

#define DEFAULT_SENSOR_ID	0xFFFF

static bool trip_cdev_sort(trip_pt_cdev_t cdev1, trip_pt_cdev_t cdev2) {
	return (cdev1.influence > cdev2.influence);
}

class cthd_trip_point {
private:
	int index;
	trip_point_type_t type;
	unsigned int temp;
	unsigned int hyst;
	std::vector<trip_pt_cdev_t> cdevs;
	trip_control_type_t control_type;
	int zone_id;
	int sensor_id;
	bool trip_on;
public:
	static const int default_influence = 100;
	cthd_trip_point(int _index, trip_point_type_t _type, unsigned int _temp,
			unsigned int _hyst, int _zone_id, int _sensor_id,
			trip_control_type_t _control_type = PARALLEL);
	bool thd_trip_point_check(int id, unsigned int read_temp, int pref, bool *reset);

	void thd_trip_point_add_cdev(cthd_cdev &cdev, int influence);

	void thd_trip_cdev_state_reset();
	int thd_trip_point_value() {
		return temp;
	}
	void thd_trip_update_set_point(unsigned int new_value) {
		temp = new_value;
	}
	int thd_trip_point_add_cdev_index(int _index, int influence);
	void thd_trip_point_set_control_type(trip_control_type_t type) {
		control_type = type;
	}
	trip_point_type_t get_trip_type() {
		return type;
	}
	unsigned int get_trip_temp() {
		return temp;
	}
	void trip_cdev_add(trip_pt_cdev_t trip_cdev) {
		cdevs.push_back(trip_cdev);
		std::sort(cdevs.begin(), cdevs.end(), trip_cdev_sort);
	}
};

#endif
