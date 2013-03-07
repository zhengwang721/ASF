/**
 * \file
 *
 * \brief SAM D20 I2C Common Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef I2C_COMMON_H_INCLUDED
#define I2C_COMMON_H_INCLUDED

#include <sercom.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup asfdoc_samd20_i2c_group I2C Common
 *
 * \section asfdoc_samd20_i2c_introduction Introduction
 *
 * This is a driver for the SAM D20 devices. This driver provides an
 * interface for configuration and management of the SERCOM I<SUP>2</SUP>C&trade; module,
 * as well as data transfer via I<SUP>2</SUP>C&trade;.
 * The following peripheral is used by this module:
 * - SERCOM in I<SUP>2</SUP>C&trade; Master Mode:
 *     - \ref asfdoc_samd20_i2c_master_group "I2C&trade; Master"
 *     - \ref asfdoc_samd20_i2c_master_interrupt_group "I2C&trade; Master Interrupt"
 * - SERCOM in I<SUP>2</SUP>C&trade; Slave Mode:
 *     - \ref asfdoc_samd20_i2c_slave_interrupt_group "I2C&trade; Slave Interrupt"
 *
 * The outline of this documentation is as follows:
 * - \ref asfdoc_samd20_i2c_prerequisites
 * - \ref asfdoc_samd20_i2c_overview
 * - \ref asfdoc_samd20_i2c_special_considerations
 * - \ref asfdoc_samd20_i2c_extra
 * - \ref asfdoc_samd20_i2c_examples
 * - \ref asfdoc_samd20_i2c_api_overview
 *
 * \section asfdoc_samd20_i2c_prerequisites Prerequisites
 * There are no prerequisites.
 *
 * \section asfdoc_samd20_i2c_overview Module Overview
 * The outline of this section is as follows:
 * - \ref asfdoc_samd20_i2c_functional_desc
 * - \ref asfdoc_samd20_i2c_bus_topology
 * - \ref asfdoc_samd20_i2c_transactions
 * - \ref asfdoc_samd20_i2c_multi_master
 * - \ref asfdoc_samd20_i2c_bus_states
 * - \ref asfdoc_samd20_i2c_timeout
 * - \ref asfdoc_samd20_i2c_sleep_modes
 *
 * \subsection asfdoc_samd20_i2c_functional_desc Functional Description
 * The I<SUP>2</SUP>C&trade; provides a simple two-wire bidirectional bus consisting of a
 * wired-AND type serial clock line (SCA) and a wired-AND type serial data line
 * (SDA).
 *
 * The I<SUP>2</SUP>C&trade; bus provides a simple, but efficient method of interconnecting
 * multiple master and slave devices. An arbitration mechanism is provided for
 * resolving bus ownership between masters, as only one master device may own
 * the bus at any given time. The arbitration mechanism relies on the wired-AND
 * connections to avoid bus drivers short-circuiting.
 *
 * A unique address is assigned to all slave devices connected to the bus. A
 * device can contain both master and slave logic, and can emulate multiple
 * slave devices by responding to more than one address.
 *
 * \subsection asfdoc_samd20_i2c_bus_topology Bus Topology
 * The I<SUP>2</SUP>C&trade; bus topology is illustrated in the figure below. The pullup
 * resistors (Rs) will provide a high level on the bus lines when none of the
 * I<SUP>2</SUP>C&trade; devices are driving the bus. These are optional, and can be replaced
 * with a constant current source.
 * \dot
 * digraph bus_topology{
 *
 * splines = false;
 *
 * vcc [label="VCC", shape=none];
 * vcc_end [label="", style=invisible];
 * sda [label="SDA", shape=none];
 * sda_end [label="", style=invisible];
 * scl [label="SCL", shape=none];
 * scl_end [label="", style=invisible];
 *
 * res1 [label="Rp", shape=box, height=1, width=0.1];
 * res2 [label="Rp", shape=box, height=1, width=0.1];
 * res1dev1 [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 * res2dev1 [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 * res1dev2 [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 * res2dev2 [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 * res1devn [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 * res2devn [label="Rs", shape=box, style=dotted, height=1, width=0.1];
 *
 * i2c1 [label="I2C DEVICE 1", shape=box, height=1.5];
 * i2c2 [label="I2C DEVICE 2", shape=box, height=1.5];
 * i2cn [label="I2C DEVICE N", shape=box, height=1.5];
 * dev_invis [label="", style=invisible];
 * dev_invis_end [label="", style="invis"];
 *
 * vcc_res1 [label="", style="invis", height=0];
 * vcc_res2 [label="", style="invis", height=0];
 * vcc_dev1 [label="", style="invis", height=0];
 * vcc_dev2 [label="", style="invis", height=0];
 * vcc_devn [label="", style="invis", height=0];
 *
 * scl_res1 [label="", style="invis", height=0];
 * sda_res2 [label="", style="invis", height=0];
 * scl_res1dev1 [label="", style="invis", height=0];
 * sda_res2dev1 [label="", style="invis", height=0];
 * scl_res1dev2 [label="", style="invis", height=0];
 * sda_res2dev2 [label="", style="invis", height=0];
 * scl_res1devn [label="", style="invis", height=0];
 * sda_res2devn [label="", style="invis", height=0];
 * vcc_devn [label="", style="invis", height=0];
 *
 * {rank=same; dev_invis res1 res2 i2c1 i2c2 i2cn dev_invis_end}
 * {rank=same; res1dev1 res2dev1 res1dev2 res2dev2 res1devn res2devn}
 * {rank=same; vcc vcc_res1 vcc_res2 vcc_dev1 vcc_dev2 vcc_devn vcc_end}
 * {rank=same; sda sda_res2 sda_res2dev1 sda_res2dev2 sda_res2devn sda_end}
 * {rank=same; scl scl_res1 scl_res1dev1 scl_res1dev2 scl_res1devn scl_end}
 *
 * edge [dir=none];
 * vcc->vcc_res1:e;
 * vcc_res1->vcc_res2:e [constraint=true];
 * vcc_res1->res1 [constraint=true];
 * vcc_res2->vcc_dev1:e [constraint=true];
 * vcc_res2->res2 [constraint=true];
 * vcc_dev1->vcc_dev2:e [constraint=true];
 * vcc_dev2->vcc_devn:e [constraint=true];
 * vcc_dev2->i2c2 [constraint=true];
 * vcc_devn->vcc_end [constraint=true];
 * vcc_devn:c->i2cn [constraint=true];
 * vcc_dev1:c->i2c1 [constraint=true];
 *
 * vcc_end->sda_end [constraint=true, style="invis"];
 *
 * vcc->dev_invis [constraint=true, style="invis"];
 * dev_invis->sda [constraint=true, style="invis"];
 *
 * sda->sda_res2:e;
 * sda_res2->sda_res2dev1:e;
 * sda_res2dev1->sda_res2dev2:e;
 * sda_res2dev2->sda_res2devn:e;
 * sda_res2devn->sda_end;
 *
 * sda->scl [constraint=true, style="invis"];
 * scl->scl_res1:e;
 * scl_res1->scl_res1dev1:e;
 * scl_res1dev1->scl_res1dev2:e;
 * scl_res1dev2->scl_res1devn:e;
 * scl_res1devn->scl_end;
 *
 * res1->res2 [constraint=true, style="invis"];
 * res2->i2c1 [constraint=true, style="invis"];
 * i2c1->i2c2 [constraint=true, style="invis"];
 * i2c2->i2cn [constraint=true, style="dotted"];
 *
 * res1->scl_res1;
 * res2->sda_res2;
 *
 * i2c1->res1dev1;
 * i2c1->res2dev1;
 * res1dev1->scl_res1dev1;
 * res2dev1->sda_res2dev1;
 *
 * i2c2->res1dev2;
 * i2c2->res2dev2;
 * res1dev2->scl_res1dev2;
 * res2dev2->sda_res2dev2;
 *
 * i2cn->res1devn;
 * i2cn->res2devn;
 * res1devn->scl_res1devn;
 * res2devn->sda_res2devn;
 *
 * i2cn->dev_invis_end [constraint=true, style="invis"];
 * vcc_end->dev_invis_end [constraint=true, style="invis"];
 * dev_invis_end->sda_end [constraint=true, style="invis"];
 * sda_end->scl_end [constraint=true, style="invis"];
 * }
 * \enddot
 *
 * \subsection asfdoc_samd20_i2c_transactions Transactions
 * The I<SUP>2</SUP>C&trade; standard defines three fundamental transaction formats:
 * There are three fundamental transaction formats:
 * \li Master Write
 *   - The master transmits data packets to the slave after addressing it.
 * \li Master Read
 *   - The slave transmits data packets to the master after being addressed.
 * \li Combined
 *   - A combined transaction consists of several write and read transactions.
 *
 * A data transfer starts with the master issuing a \b Start condition on the
 * bus, followed by the address of the slave together with a bit to indicate
 * whether the master wants to read from or write to the slave.
 * The addressed slave must respond to this by sending an \b ACK back to the
 * master.
 *
 * After this, data packets are sent from the master or slave, according to the
 * read/write bit. Each packet must be acknowledged (ACK) or not
 * acknowledged (NACK) by the receiver.
 *
 * If a slave responds with a NACK, the master must assume that the slave
 * cannot receive any more data and cancel the write operation.
 *
 * The master completes a transaction by issuing a \b Stop condition.
 *
 * A master can issue multiple \b Start conditions during a transaction; this
 * is then called a \b Repeated \b Start condition.
 *
 * \subsubsection asfdoc_samd20_i2c_address_packets Address Packets
 * The slave address consists of seven bits. The 8th bit in the transfer
 * determines the data direction (read or write). An address packet always
 * succeeds a \b Start or \b Repeated \b Start condition. The 8th bit is handled
 * in the driver, and the user will only have to provide the 7 bit address.
 *
 * \subsubsection asfdoc_samd20_i2c_data_packets Data Packets
 * 9 bits long, consists of one data byte and an acknowledgment bit.
 * Data packets succeed either an address packet or another data packet.
 *
 * \subsubsection asfdoc_samd20_i2c_trans_examples Transaction Examples
 * The gray bits in the following examples are sent from master to slave, and
 * the white bits are sent from slave to master.
 * Example of a read transaction is shown below. Here, the master first issues
 * a \b Start condition and gets ownership of the bus. An address packet with
 * the direction flag set to read is then sent and acknowledged by the slave.
 * Then the slave sends one data packet which is acknowledged by the master.
 * The slave sends another packet, which is not acknowledged by the master
 * and indicates that the master will terminate the transaction. In the end,
 * the transaction is terminated by the master issuing a \b Stop condition.
 *
 * <table>
 *   <tr>
 *      <td>0</td>
 *      <td>1</td>
 *      <td>2</td>
 *      <td>3</td>
 *      <td>4</td>
 *      <td>5</td>
 *      <td>6</td>
 *      <td>7</td>
 *      <td>8</td>
 *      <td>9</td>
 *      <td>10</td>
 *      <td>11</td>
 *      <td>12</td>
 *      <td>13</td>
 *      <td>14</td>
 *      <td>15</td>
 *      <td>16</td>
 *      <td>17</td>
 *      <td>18</td>
 *      <td>19</td>
 *      <td>20</td>
 *      <td>21</td>
 *      <td>22</td>
 *      <td>23</td>
 *      <td>24</td>
 *      <td>25</td>
 *      <td>26</td>
 *      <td>27</td>
 *      <td>28</td>
 *   </tr>
 *   <tr>
 *      <td BGCOLOR="lightgray">START</td>
 *      <td COLSPAN="7" BGCOLOR="lightgray">ADDRESS</td>
 *      <td BGCOLOR="lightgray">READ</td>
 *      <td>ACK</td>
 *      <td COLSPAN="8" BGCOLOR="">DATA</td>
 *      <td BGCOLOR="lightgray">ACK</td>
 *      <td COLSPAN="8" BGCOLOR="">DATA</td>
 *      <td BGCOLOR="lightgray">NACK</td>
 *      <td BGCOLOR="lightgray">STOP</td>
 *   </tr>
 * </table>
 *
 * Example of a write transaction is shown below. Here, the master first issues
 * a \b Start condition and gets ownership of the bus. An address packet with
 * the dir flag set to write is then sent and acknowledged by the slave. Then
 * the master sends two data packets, each acknowledged by the slave. In the
 * end, the transaction is terminated by the master issuing a \b Stop
 * condition.
 *
 * <table>
 *   <tr>
 *      <td>0</td>
 *      <td>1</td>
 *      <td>2</td>
 *      <td>3</td>
 *      <td>4</td>
 *      <td>5</td>
 *      <td>6</td>
 *      <td>7</td>
 *      <td>8</td>
 *      <td>9</td>
 *      <td>10</td>
 *      <td>11</td>
 *      <td>12</td>
 *      <td>13</td>
 *      <td>14</td>
 *      <td>15</td>
 *      <td>16</td>
 *      <td>17</td>
 *      <td>18</td>
 *      <td>19</td>
 *      <td>20</td>
 *      <td>21</td>
 *      <td>22</td>
 *      <td>23</td>
 *      <td>24</td>
 *      <td>25</td>
 *      <td>26</td>
 *      <td>27</td>
 *      <td>28</td>
 *   </tr>
 *   <tr>
 *      <td BGCOLOR="lightgray">START</td>
 *      <td COLSPAN="7" BGCOLOR="lightgray">ADDRESS</td>
 *      <td BGCOLOR="lightgray">WRITE</td>
 *      <td>ACK</td>
 *      <td COLSPAN="8" BGCOLOR="lightgray">DATA</td>
 *      <td>ACK</td>
 *      <td COLSPAN="8" BGCOLOR="lightgray">DATA</td>
 *      <td>ACK</td>
 *      <td BGCOLOR="lightgray">STOP</td>
 *   </tr>
 * </table>
 *
 * \subsubsection asfdoc_samd20_i2c_packet_timeout Packet Timeout
 * When a master sends an I<SUP>2</SUP>&trade; packet, there is no way of being sure that a
 * slave will acknowledge the packet. To avoid stalling the device forever
 * while waiting for an acknowledge, a user selectable timeout is provided in
 * the \ref i2c_master_config which lets the driver exit a read or write
 * operation after the specified time. The function will then return the
 * STATUS_ERR_TIMEOUT flag.
 *
 * The time before the timeout occurs, can be found by same formula as
 * provided for \ref timeout "unknown bus state" timeout.
 *
 * \subsubsection asfdoc_samd20_i2c_repeated_start Repeated Start
 * To issue a \b Repeated \b Start, the functions postfixed \c _no_stop must be
 * used.
 * These functions will not send a \b Stop condition when the transfer is done,
 * thus the next transfer will start with a \b Repeated \b Start. To end the
 * transaction, the functions without the \c _no_stop postfix must be used
 * for the last read/write.
 *
 * \subsection asfdoc_samd20_i2c_multi_master Multi Master
 * In a multi master environment, arbitration of the bus is important, as only
 * one master can own the bus at any point.
 *
 * \subsubsection arbitration Arbitration
 *
 * \par Clock stretching
 * The serial clock line is always driven by a master device. However, all
 * devices connected to the bus are allowed stretch the low period of the clock
 * to slow down the overall clock frequency or to insert wait states while
 * processing data.
 * Both master and slave can randomly stretch the clock, which will force the
 * other device into a wait-state until the clock line goes high again.
 *
 * \par Arbitration on the data line
 * If two masters start transmitting at the same time, they will both transmit
 * until one master detects that the other master is pulling the data line low.
 * When this is detected, the master not pulling the line low, will stop the
 * transmission and wait until the bus is idle.
 * As it is the master trying to contact the slave with the lowest address that
 * will get the bus ownership, this will create an arbitration scheme always
 * prioritizing the slaves with the lowest address in case of a bus collision.
 *
 * \subsubsection clock_sync Clock Synchronization
 * In situations where more than one master is trying to control the bus clock
 * line at the same time, a clock synchronization algorithm based on the same
 * principles used for clock stretching is necessary.
 *
 *
 * \subsection asfdoc_samd20_i2c_bus_states Bus States
 * As the I<SUP>2</SUP>&trade; bus is limited to one transaction at the time, a master that
 * wants to perform a bus transaction must wait until the bus is free.
 * Because of this, it is necessary for all masters in a multi-master system to
 * know the current status of the bus to be able to avoid conflicts and to
 * ensure data integrity.
 * \li \b IDLE No activity on the bus (between a \b Stop and a new \b Start
 * condition)
 * \li \b OWNER If the master initiates a transaction successfully
 * \li \b BUSY If another master is driving the bus
 * \li \b UNKNOWN If the master has recently been enabled or connected to
 * the bus. Is forced to \b IDLE after given \ref inactive_bus "timeout" when
 * the master module is enabled.
 *
 * The bus state diagram can be seen below.
 * \li S: Start condition
 * \li P: Stop condition
 * \li Sr: Repeated start condition
 * \dot
 * digraph bus_states{
 *   reset [label="", style=invisible];
 *   unknown [label="UNKNOWN", shape=circle, height=1.5];
 *   idle [label="IDLE", shape=circle, height=1.5];
 *   owner [label="OWNER", shape=circle, height=1.5];
 *   busy [label="BUSY", shape=circle, height=1.5];
 *
 *  {rank=same; idle busy}
 *
 *   reset->unknown [label="RESET"];
 *   unknown->idle [label="P + Timeout"];
 *   idle->owner [label="Write ADDR \n(S)"];
 *   idle->busy [label="S"];
 *   owner->owner [label="Write ADDR (Sr)"];
 *   owner->idle [label="Command P"];
 *   owner->busy [label="Arbitration \nLost"];
 *   busy->busy [label="Sr"];
 *   busy->idle [label="P + Timeout"];
 * }
 * \enddot
 *
 * \subsection asfdoc_samd20_i2c_timeout Bus Timing
 * Inactive bus timeout for the master and SDA hold time is configurable in the
 * drivers.
 *
 * \subsubsection asfdoc_samd20_i2c_unknown_bus_timeout Unknown Bus State Timeout
 * When a master is enabled or connected to the bus, the bus state will be
 * unknown until either a given timeout or a stop command has occurred. The
 * timeout is configurable in the i2c_master_config struct and can
 * roughly be calculated with:
 * \f[
 *    t_{timeout} = \frac{VALUE}{(5-7)*f_{GCLK}}
 * \f]
 *
 * Depending on optimization level.
 *
 * \warning Must be checked with correct toolchain!
 *
 * \subsubsection sda_hold SDA Hold Timeout
 * When using the I<SUP>2</SUP>&trade; in slave mode, it will be important to set a SDA hold
 * time which assures that the master will be able to pick up the bit sent from
 * the slave. The SDA hold time makes sure that this is the case by holding the
 * data line low for a given period after the negative edge on the clock.
 *
 * The SDA hold time is also available for the master driver, but will not be a
 * necessity.
 *
 * \subsection asfdoc_samd20_i2c_sleep_modes Operation in Sleep Modes
 * The I<SUP>2</SUP>&trade; module can operate in all sleep modes by setting the run_in_standby
 * boolean in the \ref i2c_master_config or \ref i2c_slave_config struct.
 * The operation in slave and master Mode is shown in the table below.
 *
 * <table>
 *   <tr>
 *      <th>Run in standby</th>
 *      <th>Slave</th>
 *      <th>Master</th>
 *   </tr>
 *   <tr>
 *      <th>false</th>
 *      <td>Disabled, all reception is dropped</td>
 *      <td>GCLK disabled when master is idle</td>
 *   </tr>
 *   <tr>
 *      <th>true</th>
 *      <td>Wake on address match when enabled</td>
 *      <td>GCLK enabled while in sleep modes</td>
 *   </tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_i2c_special_considerations Special Considerations
 *
 * \subsection asfdoc_samd20_i2c_common_interrupt Interrupt-Driven Operation
 * While an interrupt-driven operation is in progress, subsequent calls to a
 * write or read operation will return the STATUS_BUSY flag, indicating that
 * only one operation is allowed at any given time.
 *
 * To check if another transmission can be initiated, the user can either call
 * another transfer operation, or use the
 * \ref i2c_master_get_job_status/\ref i2c_slave_get_job_status functions
 * depending on mode.
 *
 * If the user would like to get callback from operations while using the
 * interrupt-driven driver, the callback must be registered and then enabled
 * using the "register_callback" and "enable_callback" functions.
 *
 * Documentation for the interrupt-driven drivers can be found here:
 * - \ref asfdoc_samd20_i2c_master_interrupt_group "I<SUP>2</SUP>&trade; Master Interrupt"
 * - \ref asfdoc_samd20_i2c_slave_interrupt_group "I<SUP>2</SUP>&trade; Slave Interrupt"
 *
 * \section asfdoc_samd20_i2c_extra Extra Information
 * For extra information see \ref asfdoc_samd20_i2c_extra_info_page.
 *
 * \section asfdoc_samd20_i2c_examples Examples
 * - \ref asfdoc_samd20_i2c_quickstart
 *
 * \section asfdoc_samd20_i2c_api_overview API Overview
 * @{
 *
 * I<SUP>2</SUP>&trade; Master\tm
 *  - \ref asfdoc_samd20_i2c_master_api_overview "I<SUP>2</SUP>&trade; Master API Overview"
 *  - \ref asfdoc_samd20_i2c_master_interrupt_api_overview
 * "I<SUP>2</SUP>&trade; Master Interrupt API Overview"
 *
 * I<SUP>2</SUP>&trade; Slave
 *  - \ref asfdoc_samd20_i2c_slave_interrupt_api_overview
 * "I<SUP>2</SUP>&trade; Slave Interrupt API Overview"
 */

/**
 * \brief I<SUP>2</SUP>&trade; packet for read/write
 *
 * Structure to be used when transferring I<SUP>2</SUP>&trade; packets. Used both for master and
 * slave.
 */
struct i2c_packet {
	/** Address to slave device  */
	uint8_t address;
	/** Length of data array */
	uint8_t data_length;
	/** Data array containing all data to be transferred */
	uint8_t *data;
};

/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page asfdoc_samd20_i2c_extra_info_page Extra Information
 *
 * \section asfdoc_samd20_i2c_dependencies Dependencies
 * The I<SUP>2</SUP>&trade; driver has the following dependencies:
 * \li \b SERCOM
 * \li \b SYSTEM
 *
 * \section asfdoc_samd20_i2c_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>SDA</td>
 *		<td>Serial Data Line</td>
 *	</tr>
 *	<tr>
 *		<td>SCL</td>
 *		<td>Serial Clock Line</td>
 *	</tr>
 * </table>
 *
 * \section asfdoc_samd20_i2c_workarounds Workarounds implemented by driver
 * Master:
 * - A bug in hardware makes the master go straight from IDLE to BUSY bus state.
 * As a workaround the inactive timeout is enabled, which will force the bus
 * state back to IDLE. If a address is written while the bus state was in BUSY,
 * the hardware will be able to generate a start condition and enter OWNER
 * before the hardware bug makes it go into the internal BUSY state.
 * As result of the timeout, there will be generated at BUSERR, thus the
 * workaround ignores all these errors.
 *
 * Slave:
 * - A bug in hardware makes the Stop interrupt only fire
 * occasionally. This means that the callback for a read will not be called
 * until a new address packet it received. The write callback will be called
 * when the master nacks the data, or when the entire packet is written.
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samd20_i2c_quickstart Quick Start Guides for the SERCOM I<SUP>2</SUP> module
 *
 * This is the quick start guide list for the \ref asfdoc_samd20_i2c_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section use_cases I<SUP>2</SUP>&trade; driver use cases
 * - \subpage asfdoc_samd20_i2c_master_basic_use_case "Quick Start Guide for the
 * I<SUP>2</SUP>&trade; Master module - Basic Use Case"
 * - \subpage asfdoc_samd20_i2c_master_callback_use_case "Quick Start Guide
 * for the I<SUP>2</SUP>&trade; Master Module - Callback Use Case"
 * - \subpage asfdoc_samd20_i2c_slave_basic_use_case "Quick Start Guide for
 * the I<SUP>2</SUP>&trade; Slave module - Callback Use Case"
 */

#endif /* I2C_COMMON_H_INCLUDED */
