/*
 * thd_zone_surface.cpp: zone implementation for external surface
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

#include "thd_zone_surface.h"

cthd_zone_surface::cthd_zone_surface() :
		zone(NULL) {
}

int cthd_zone_surface::init() {
	zone = thd_engine->search_zone("TSKN");

	if (!zone) {
		thd_log_debug("cthd_zone_surface is not active \n");
		return THD_ERROR;
	}

	if (zone && zone->zone_cdev_binded()) {
		thd_log_debug("Zone is bound \n");
		return THD_ERROR;
	} else {
		thd_log_debug("Zone is not bound to cdev \n");

		cthd_cdev *cdev;
		cdev = thd_engine->search_cdev("rapl_enable");
		if (cdev) {
			zone->bind_cooling_device(PASSIVE, 0, cdev);
		}
		cdev = thd_engine->search_cdev("rapl_controller");
		if (cdev) {
			zone->bind_cooling_device(PASSIVE, 0, cdev);
		}
		zone->set_zone_active();
	}

	return THD_SUCCESS;
}

