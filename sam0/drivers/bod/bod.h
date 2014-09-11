/**
 * \file
 *
 * \brief SAM Brown Out Detector Driver
 *
 * Copyright (C) 2013-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef BOD_H_INCLUDED
#define BOD_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <bod_feature.h>

/**
 * \defgroup asfdoc_sam0_bod_group SAM Brown Out Detector Driver (BOD)
 *
 * This driver for SAM devices provides an interface for the configuration
 * and management of the device's Brown Out Detector (BOD) modules, to detect
 * and respond to under-voltage events and take an appropriate action.
 *
 * The following peripherals are used by this module:
 * - SYSCTRL (System Control)
 *
 * The following devices can use this module:
 *  - SAM D20/D21
 *  - SAM R21
 *  - SAM D10/D11
 *  - SAM L21
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_bod_prerequisites
 *  - \ref asfdoc_sam0_bod_module_overview
 *  - \ref asfdoc_sam0_bod_special_considerations
 *  - \ref asfdoc_sam0_bod_extra_info
 *  - \ref asfdoc_sam0_bod_examples
 *  - \ref asfdoc_sam0_bod_api_overview
 *
 *
 * \section asfdoc_sam0_bod_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_bod_module_overview Module Overview
 *
 * The SAM devices contain a number of Brown Out Detector (BOD) modules.
 * Each BOD monitors the supply voltage for any dips that go below the set
 * threshold for the module. In case of a BOD detection the BOD will either reset
 * the system or raise a hardware interrupt so that a safe power-down sequence can
 * be attempted.
 *
 *
 * \section asfdoc_sam0_bod_special_considerations Special Considerations
 *
 * The time between a BOD interrupt being raised and a failure of the processor
 * to continue executing (in the case of a core power failure) is system
 * specific; care must be taken that all critical BOD detection events can
 * complete within the amount of time available.
 *
 * \section asfdoc_sam0_bod_extra_info Extra Information
 *
 * For extra information see \ref asfdoc_sam0_bod_extra. This includes:
 *  - \ref asfdoc_sam0_bod_extra_acronyms
 *  - \ref asfdoc_sam0_bod_extra_dependencies
 *  - \ref asfdoc_sam0_bod_extra_errata
 *  - \ref asfdoc_sam0_bod_extra_history
 *
 *
 * \section asfdoc_sam0_bod_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_bod_exqsg.
 *
 *
 * \section asfdoc_sam0_bod_api_overview API Overview
 * @{
 */


/**
 * @}
 */


/**
 * \page asfdoc_sam0_bod_extra Extra Information for BOD Driver
 *
 * \section asfdoc_sam0_bod_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>BOD</td>
 *      <td>Brownout detector</td>
 *  </tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_bod_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_sam0_bod_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_bod_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Added support for SAMD21 and removed BOD12 reference</td>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam0_bod_exqsg Examples for BOD Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_bod_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_bod_basic_use_case
 *
 *  - \subpage asfdoc_sam0_bod_application_use_case
 *
 * \page asfdoc_sam0_bod_application_use_case Application Use Case for BOD - Application
 * The preferred method of setting BOD33 levels and settings is trough the fuses.
 * when it is desirable to set it in software, please see the below use case.
 *
 * In this use case, a new BOD33 level might be set in SW if the clock settings
 * are adjusted up after a battery has charged to a higher level. When the battery
 * discharges, the chip will reset when the battery level is below SW BOD33 level.
 * Now the chip will run at a lower clock rate and the BOD33 level from fuse.
 * The chip should always measure the voltage before adjusting the frequency up.
 *
 * \page asfdoc_sam0_bod_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>E</td>
 *		<td>09/2014</td>
 *		<td>Added support for SAML21.</td>
 *	</tr>
 *	<tr>
 *		<td>D</td>
 *		<td>04/2014</td>
 *		<td>Added support for SAMR21 and SAMD10/D11.</td>
 *	</tr>
 *	<tr>
 *		<td>C</td>
 *		<td>01/2014</td>
 *		<td>Added support for SAMD21.</td>
 *	</tr>
 *	<tr>
 *		<td>B</td>
 *		<td>06/2013</td>
 *		<td>Corrected documentation typos.</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>06/2013</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#ifdef __cplusplus
}
#endif

#endif /* BOD_H_INCLUDED */
