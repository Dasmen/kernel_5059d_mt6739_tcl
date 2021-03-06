
#ifndef _AUTOK_DVFS_H_
#define _AUTOK_DVFS_H_

#include <mt_vcorefs_manager.h>
#include "autok.h"

#define SDIO_DVFS_TIMEOUT       (HZ/100 * 5)    /* 10ms x5 */

/* Enable later@Peter */
/* #define SDIO_HW_DVFS_CONDITIONAL */

enum AUTOK_VCORE {
	AUTOK_VCORE_LOW = 0,
	AUTOK_VCORE_HIGH,
	AUTOK_VCORE_NUM
};

extern u8 sdio_autok_res[2][TUNING_PARAM_COUNT];
extern u8 emmc_autok_res[2][TUNING_PARAM_COUNT];
extern u8 sd_autok_res[2][TUNING_PARAM_COUNT];

extern int sdio_autok_res_apply(struct msdc_host *host, int vcore);
extern int sdio_autok_res_save(struct msdc_host *host, int vcore, u8 *res);
extern void sdio_autok_wait_dvfs_ready(void);

extern int autok_res_check(u8 *res_h, u8 *res_l);
extern void sdio_set_hw_dvfs(int vcore, int done, struct msdc_host *host);
extern void sdio_dvfs_reg_restore(struct msdc_host *host);

#endif /* _AUTOK_DVFS_H_ */

