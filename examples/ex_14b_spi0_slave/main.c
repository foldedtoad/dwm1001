/**
 * Copyright (c) 2019 - Callender-Consulting. LLC
 *
 *   You should have received a copy of the GNU General Public License.
 *   see <https://www.gnu.org/licenses/>.
 * 
 */
#include <zephyr.h>
#include <sys/printk.h>

#define STACKSIZE 1024
#define PRIORITY 99


#ifdef CONFIG_SPI_0
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
#include "spi_slave.h"
void spi_slave_thread(void * id, void * unused1, void * unused2)
{
    printk("%s\n", __func__);

    spi_slave_init();

    while(1) { /* spin */}
}

K_THREAD_DEFINE(spi_slave_id, STACKSIZE, spi_slave_thread, 
                NULL, NULL, NULL, PRIORITY, 0, 0);
#endif
