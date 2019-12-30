/**
 * Copyright (c) 2019 - Callender-Consulting. LLC
 *
 *   You should have received a copy of the GNU General Public License.
 *   see <https://www.gnu.org/licenses/>.
 * 
 */
#include <zephyr.h>
#include <sys/printk.h>

#include "spi_master.h"

#define STACKSIZE 1024
#define PRIORITY 99


#ifdef CONFIG_SPI_0
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void spi_master_thread(void * id, void * unused1, void * unused2)
{
    printk("%s\n", __func__);

    spi_master_init();

    while(1) { /* spin */}
}

K_THREAD_DEFINE(spi_master_id, STACKSIZE, spi_master_thread, 
                NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
#endif
