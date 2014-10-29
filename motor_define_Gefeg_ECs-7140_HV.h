/* THE SOURCE CODE AND ITS
 * RELATED DOCUMENTATION IS PROVIDED "AS IS". TOSHIBA CORPORATION MAKES NO OTHER
 * WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR, STATUTORY AND DISCLAIMS ANY
 * AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON
 * INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. THE SOURCE CODE AND
 * DOCUMENTATION MAY INCLUDE ERRORS. TOSHIBA CORPORATION RESERVES THE RIGHT TO
 * INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER REVISIONS OF IT, AND TO
 * MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR THE PRODUCTS OR
 * TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME. TOSHIBA CORPORATION SHALL NOT BE
 * LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGE OR LIABILITY ARISING
 * FROM YOUR USE OF THE SOURCE CODE OR ANY DOCUMENTATION, INCLUDING BUT NOT
 * LIMITED TO, LOST REVENUES, DATA OR PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL
 * OR CONSEQUENTIAL NATURE, PUNITIVE DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS
 * ARISING OUT OF OR IN CONNECTION WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF
 * ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM
 * FOR SUCH DAMAGE IS BASED UPON WARRANTY, CONTRACT, TORT, NEGLIGENCE OR
 * OTHERWISE. (C)Copyright TOSHIBA CORPORATION 2011-2012 All rights reserved
 */
#ifndef _MOTOR_DEFINE_H_
#define _MOTOR_DEFINE_H_

#include "motorctrl.h"

#define  MOTOR_POLE_PAIRS                           2 /* [NONE] */
#define  MOTOR_DIRECTION                            3 /* [NONE]      0 - None
                                                                     1 - CW only
                                                                     2 - CCW only
                                                                     3 - CW and CCW */
#define  MOTOR_ENCODER_TYPE                         0 /* [NONE]      0 - No encoder
                                                                     1 - Use Hall Sensors (UVW)
                                                                     2 - Use Hall Sensors (UV)
                                                                     3 - Use incremental Encoder (ABZ)
                                                                     4 - Use incremental Encoder (AB)
                                                                     5 - Use single pulse input */
#define  MOTOR_ENCODER_USAGE                        0 /* [NONE]      0 - No encoder
                                                                     1 - Use encoder for speed control
                                                                     2 - Use encoder for counting events */
#define  MOTOR_ENCODER_COUNT                        0 /* [NONE]      Incremental Encoder Resolution */
#define  MOTOR_ANGULAR_ACC_MAX_VALUE               50 /* [NONE]      Maximum angular acceleration
                                                                     Speed up limit for the motor */
#define  MOTOR_ANGULAR_ACC_MAX_UNIT                 0 /* [ENUM]      0 - rad/sec²
                                                                     1 - Hz/s
                                                                     2 - RPM/s
                                                                     3 -RPM/min */
#define  MOTOR_TORQUE_FACTOR_VALUE                330 /* [Nm/A]      Torque Factor of the Motor
                                                                     Used only for statistics graph */
#define  MOTOR_TORQUE_FACTOR_UNIT                  -3 /* [ENUM]      0 - mNm/A
                                                                     -3 - Nm/A */
#define  MOTOR_RESISTANCE_VALUE                 40000 /* [Ohm]       Resistance per phase */
#define  MOTOR_RESISTANCE_UNIT                     -3 /* [10^x] */
#define  MOTOR_INDUCTANCE_VALUE                 34000 /* [H]         Inductance per phase */
#define  MOTOR_INDUCTANCE_UNIT                     -6 /* [10^x] */
#define  MOTOR_SPEED_LIMIT_VALUE                  140 /* [NONE]      Maximum rating for motor */
#define  MOTOR_SPEED_LIMIT_UNIT                     0 /* [ENUM]      0 - Hz
                                                                     1 - RPM */
#define  MOTOR_SPEED_CHANGE_VALUE                  20 /* [NONE]      Change frequency from Forced- to Field-Oriented-Commutation */
#define  MOTOR_SPEED_CHANGE_UNIT                    0 /* [ENUM]      0 - Hz
                                                                     1 - RPM */
#define  MOTOR_POSITION_DELAY_VALUE                     200 /* [ms]        Time to reach the start position (DC-Exitation) */
#define  MOTOR_IQ_START_VALUE                     100 /* [A]         Torque current in Forced-Commutation stage
                                                                     Limited by IqLimit */
#define  MOTOR_IQ_START_UNIT                       -3 /* [10^x] */
#define  MOTOR_ID_START_VALUE                     150 /* [A]         Flux current in Forced-Commutation stage
                                                                     Limited by IdLimit */
#define  MOTOR_ID_START_UNIT                       -3 /* [10^x] */
#define  MOTOR_IQ_LIM_VALUE                       500 /* [A]         Torque current maximum */
#define  MOTOR_IQ_LIM_UNIT                         -3 /* [10^x] */
#define  MOTOR_ID_LIM_VALUE                       500 /* [A]         Flux current maximum */
#define  MOTOR_ID_LIM_UNIT                         -3 /* [10^x] */
#define  MOTORID                    "Gefeg ECs-7140 HV"

#define  CONTROL_ID_KI_VALUE                             20 /* [V/As]      Integration part of PI Controller for flux current */
#define  CONTROL_ID_KP_VALUE                             40 /* [V/A]       Proportional part of PI Controller for flux current */
#define  CONTROL_IQ_KI_VALUE                             20 /* [V/As]      Integration part of PI Controller for torque current */
#define  CONTROL_IQ_KP_VALUE                             40 /* [V/A]       Proportional part of PI Controller for torque current */
#define  CONTROL_POSITION_KI_VALUE                        2 /* [Hz/Vs]     Integration part of PI Controller of position estimator */
#define  CONTROL_POSITION_KP_VALUE                    10000 /* [mHz/V]     Proportional part of PI Controller of position estimator */
#define  CONTROL_SPEED_KI_VALUE                          18 /* [mA/Hz*s]   Integration part of PI Controller of speed controller */
#define  CONTROL_SPEED_KP_VALUE                          30 /* [mA/Hz]     Proportional part of PI Controller of speed controller */

#define  SYSTEM_PWM_FREQUENCY_VALUE                   12500 /* [HZ] */
#define  SYSTEM_SHUTDOWN_MODE                       2 /* [NONE]      0 - No signals
                                                                     1 - Short brake
                                                                     2 - Gentle shutdown (with MaxAngAcc) */
#define  SYSTEM_BRAKE_TIME                          2 /* [ms]        Time in ms for braking when short brake is selected */
#define  SYSTEM_RESTART_MODE                        0 /* [NONE]      0 - Switch off motor after stall detection
                                                                     1 - Restart motor after stall detection */
#define  SYSTEM_STALL_VALUE                      1200 /* [NONE]      Stall detect value as vqi in stall_detect.c
                                                                     (0 means switch off)
                                                                     For further information check the 'Firmware Usage Guide' */
#define  SYSTEM_OVERTEMPERATURE_VALUE                     60 /* [NONE]      Motor switch off when temerature exceeded
                                                                     (0 means switch off) */
#define  SYSTEM_SPEED_CONTROL_MODE                  0 /* [NONE]      0 - No external speed control
                                                                     1 - Speed control by external voltage signal
                                                                     2 - Speed control by external PWM Signal */
#define  SYSTEM_SW_OVERVOLTAGE_VALUE                      0 /* [V]         Overvoltage detection by Software
                                                                     (0 means switch off) */
#define  SYSTEM_SW_UNDERVOLTAGE_VALUE                     0 /* [V]         Undervoltage detection by Software
                                                                     (0 means switch off) */
#define  SYSTEM_SW_OVERCURRENT_VALUE                      0 /* [mA]        Stops motor is overcurrent value is reached on input ports */
#define  SYSTEM_SPEED_REDUCTION_VALUE                 100 /* [%]         Percentage of IqLimitit when speed reduction is used */

#endif
