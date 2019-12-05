/**
 * Copyright (c) 2019 - Callender-Consulting. LLC
 *
 *   You should have received a copy of the GNU General Public License.
 *   see <https://www.gnu.org/licenses/>.
 * 
 */
#include <zephyr.h>
#include <misc/printk.h>

#define STACKSIZE 1024
#define PRIORITY 99

#ifdef CONFIG_I2C_0
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
#include "accel.h"
void accel_thread(void * id, void * unused1, void * unused2)
{
    printk("%s\n", __func__);

    accel_init();

    while (1) { /* spin */}
}

K_THREAD_DEFINE(accel_id, STACKSIZE, accel_thread, 
                NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
#endif
