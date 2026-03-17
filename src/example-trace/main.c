/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2024, 2025 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdlib.h>

#include <rtems.h>
#include <rtems/cpuuse.h>
#include <rtems/bspIo.h>
#include <rtems/test.h>

#include "appl-config.h"

/*
 * This example application runs a couple worker tasks for three seconds with
 * event recording enabled.
 */

/*
 * In SMP configurations, we use an EDF scheduler.  The barrier prevents that
 * the started worker task starve the initialization task.
 */
static rtems_id barrier_id;

/*
 * The worker task identifiers are stored to let the timer suspend the worker
 * tasks.
 */
static rtems_id worker_task_ids[APPL_PROCESSOR_COUNT][TASK_COUNT_PER_CPU];

/*
 * This value is used to utilize the processor in the worker tasks for a
 * specific amount of time.
 */
uint_fast32_t one_clock_tick_busy;

/*
 * The worker tasks have an individual period.  Each worker task should utilize
 * the processor for approximately 9%.
 */
static void worker_task(rtems_task_argument arg)
{
	rtems_status_code sc;
	rtems_name name;
	rtems_id period_id;
	rtems_interval period_length;

	period_length = (rtems_interval)arg;

	sc = rtems_object_get_classic_name(rtems_task_self(), &name);
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_rate_monotonic_create(name, &period_id);
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_barrier_wait(barrier_id, RTEMS_NO_TIMEOUT);
	assert(sc == RTEMS_SUCCESSFUL);

	while (true) {
		rtems_interval i;

		(void)rtems_rate_monotonic_period(period_id, period_length);
		assert(sc == RTEMS_SUCCESSFUL);

		for (i = 0; i < period_length / TASK_COUNT_PER_CPU; ++i) {
			T_busy(one_clock_tick_busy);
		}
	}
}

/*
 * The timer fires after three seconds.  It suspends all worker tasks, prints
 * some statistics, and terminates the program.  The record fatal handler dumps
 * the event records (see CONFIGURE_RECORD_FATAL_DUMP_BASE64).
 */
static void timer(rtems_id id, void *arg)
{
	uint32_t cpu_count;
	uint32_t cpu;

	(void)id;
	(void)arg;

	cpu_count = rtems_scheduler_get_processor_maximum();

	for (cpu = 0; cpu < cpu_count; ++cpu) {
		size_t i;

		for (i = 0; i < TASK_COUNT_PER_CPU; ++i) {
			(void)rtems_task_suspend(worker_task_ids[cpu][i]);
		}
	}

	rtems_cpu_usage_report();
	rtems_rate_monotonic_report_statistics();
	(void)printk("\n*** END OF TEST " APPL_NAME " ***\n\n");
	rtems_fatal(RTEMS_FATAL_SOURCE_EXIT, 0);
}

int main(void)
{
	rtems_task_config task_config = {.initial_priority = 2,
					 .storage_size = APPL_TASK_STORAGE_SIZE,
					 .maximum_thread_local_storage_size = APPL_MAX_TLS_SIZE,
					 .initial_modes = RTEMS_DEFAULT_MODES,
					 .attributes = APPL_TASK_ATTRIBUTES};
	rtems_id id;
	rtems_status_code sc;
	uint32_t cpu_count;
	uint32_t cpu;

	(void)printk("initialize event recording and tracing example\n");

	one_clock_tick_busy = 90 * (T_get_one_clock_tick_busy() / 100);
	cpu_count = rtems_scheduler_get_processor_maximum();

	sc = rtems_barrier_create(rtems_build_name('I', 'N', 'I', 'T'),
				  RTEMS_BARRIER_AUTOMATIC_RELEASE,
				  1 + cpu_count * TASK_COUNT_PER_CPU, &barrier_id);
	assert(sc == RTEMS_SUCCESSFUL);

	for (cpu = 0; cpu < cpu_count; ++cpu) {
		rtems_task_priority i;
		rtems_task_argument arg_0;
		rtems_task_argument arg_1;

		arg_0 = 0;
		arg_1 = 10;

		for (i = 0; i < TASK_COUNT_PER_CPU; ++i) {
			rtems_task_argument arg_3;

			task_config.initial_priority = 1 + i;
			task_config.name =
				rtems_build_name(' ', 'T', (char)('A' + cpu), (char)('A' + i));
			task_config.storage_area = aligned_alloc(RTEMS_TASK_STORAGE_ALIGNMENT,
								 task_config.storage_size);
			assert(task_config.storage_area != NULL);

			sc = rtems_task_construct(&task_config, &id);
			assert(sc == RTEMS_SUCCESSFUL);

			worker_task_ids[cpu][i] = id;

			sc = rtems_task_start(id, worker_task, arg_1);
			assert(sc == RTEMS_SUCCESSFUL);

			arg_3 = arg_0 + arg_1;
			arg_0 = arg_1;
			arg_1 = arg_3;
		}
	}

	sc = rtems_timer_create(rtems_build_name('S', 'T', 'O', 'P'), &id);
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_timer_fire_after(id, 3000, timer, NULL);
	assert(sc == RTEMS_SUCCESSFUL);

	rtems_cpu_usage_reset();

	(void)printk("run worker tasks for three seconds\n");

	sc = rtems_barrier_wait(barrier_id, RTEMS_NO_TIMEOUT);
	assert(sc == RTEMS_SUCCESSFUL);

	rtems_task_exit();
}
