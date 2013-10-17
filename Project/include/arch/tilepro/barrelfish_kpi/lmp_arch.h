/*
 * lmp_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_KPI_LMP_H
#define ARCH_TILEPRO_BARRELFISH_KPI_LMP_H

/**
 * \brief Maximum total length of LMP and LRPC messages (payload)
 *
 * Determined by number of registers available to transfer messages.
 */
#define LMP_MSG_LENGTH          7
#define LRPC_MSG_LENGTH         0

#endif /* ARCH_TILEPRO_BARRELFISH_KPI_LMP_H */
