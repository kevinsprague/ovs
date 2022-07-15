/*
 * Copyright (c) 2022 Intel.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ODP_EXTRACT_PRIVATE
#define ODP_EXTRACT_PRIVATE 1

#include "dp-packet.h"
#include "odp-execute.h"
#include "odp-netlink.h"
#include "ovs-atomic.h"

/* Forward declaration for typedef. */
struct odp_execute_action_impl;

/* Typedef for an initialization function that can initialize each
 * implementation, checking requirements such as CPU ISA. */
typedef int (*odp_execute_action_init_func)
                    (struct odp_execute_action_impl *self);

/* Structure represents an implementation of the odp actions. */
struct odp_execute_action_impl {
    /* When set, the CPU ISA required for this implementation is available
     * and the implementation can be used. */
    bool available;

    /* Name of the implementation. */
    const char *name;

    /* Function is used to detect if this CPU has the ISA required
     * to run the optimized action implementation and if available, initializes
     * the implementation for use. */
    odp_execute_action_init_func init_func;

    /* An array of callback functions, one for each action. */
    ATOMIC(odp_execute_action_cb) funcs[__OVS_ACTION_ATTR_MAX];
};

/* Order of Actions implementations. */
enum odp_execute_action_impl_idx {
    ACTION_IMPL_SCALAR,
    /* See ACTION_IMPL_BEGIN below, for "first to-be-validated" impl.
     * Do not change the autovalidator position in this list without updating
     * the define below. */

    ACTION_IMPL_MAX,
};

/* Index to start verifying implementations from. */
BUILD_ASSERT_DECL(ACTION_IMPL_SCALAR == 0);

/* Odp execute init handles setting up the state of the actions functions at
 * initialization time. It cannot return errors, as it must always succeed in
 * initializing the scalar/generic codepath. */
void odp_execute_action_init(void);

/* Init functions for the action implementations. Initializes the function
 * pointers for optimized action types. */
int odp_action_scalar_init(struct odp_execute_action_impl *self);

struct odp_execute_action_impl * odp_execute_action_set(const char *name);

#endif /* ODP_EXTRACT_PRIVATE */
