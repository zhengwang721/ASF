#include "pinmux_public.h"

/**
 * \brief Configures the output slew rate mode for a group of pins.
 *
 * Configures the output slew rate mode for a group of pins, to
 * control the speed at which the physical output pin can react to
 * logical changes of the I/O pin value.
 *
 * \param[in] port     Base of the PORT module to configure.
 * \param[in] mask     Mask of the port pin(s) to configure.
 * \param[in] mode     New pin slew rate mode to configure.
 */
void system_pinmux_group_set_output_slew_rate(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_slew_rate mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_SLEW_RATE_LIMITED) {
				port->PINCFG[i].reg |=  PORT_PINCFG_SLEWLIM;
			}
			else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_SLEWLIM;
			}
		}
	}
}


/**
 * \brief Configures the output driver strength mode for a group of pins.
 *
 * Configures the output drive strength for a group of pins, to
 * control the amount of current the pad is able to sink/source.
 *
 * \param[in] port     Base of the PORT module to configure.
 * \param[in] mask     Mask of the port pin(s) to configure.
 * \param[in] mode     New output driver strength mode to configure.
 */
void system_pinmux_group_set_output_strength(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_strength mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_STRENGTH_HIGH) {
				port->PINCFG[i].reg |=  PORT_PINCFG_DRVSTR;
			}
			else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_DRVSTR;
			}
		}
	}
}

/**
 * \brief Configures the output driver mode for a group of pins.
 *
 * Configures the output driver mode for a group of pins, to
 * control the pad behavior.
 *
 * \param[in] port Base of the PORT module to configure.
 * \param[in] mask Mask of the port pin(s) to configure.
 * \param[in] mode New pad output driver mode to configure.
 */
void system_pinmux_group_set_output_drive(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_drive mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_DRIVE_OPEN_DRAIN) {
				port->PINCFG[i].reg |= PORT_PINCFG_ODRAIN;
			} else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_ODRAIN;
			}
		}
	}
}


