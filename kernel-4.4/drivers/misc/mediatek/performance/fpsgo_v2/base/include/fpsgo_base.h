
#ifndef __FPSGO_BASE_H__
#define __FPSGO_BASE_H__

#include <linux/compiler.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>

#define WINDOW 20
#define RESCUE_TIMER_NUM 2

struct fbt_jerk {
	int id;
	int jerking;
	struct hrtimer timer;
	struct work_struct work;
};
struct fbt_proc {
	int active_jerk_id;
	struct fbt_jerk jerks[RESCUE_TIMER_NUM];
};

struct fbt_frame_info {
	int target_fps;
	int mips_diff;
	long mips;
	unsigned long long running_time;
	int count;
};

struct fbt_thread_loading {
	int pid;
	atomic_t loading;
	atomic_t last_cb_ts;
	atomic_t skip_loading;
	struct list_head entry;
};

struct fbt_thread_blc {
	int pid;
	unsigned int blc;
	int freerun;
	struct list_head entry;
};

struct fbt_boost_info {
	int fstb_target_fps;

	/* rescue*/
	struct fbt_proc proc;

	/* variance control */
	struct fbt_frame_info frame_info[WINDOW];
	unsigned int floor;
	int floor_count;
	int reset_floor_bound;
	int f_iter;

	/* middle_enable */
	int middle_enable;

	/* APP self-control*/
	int sf_check;
	int sf_bound;

	int keep_loading;
};

struct frame_id_info {
	unsigned long long id;
	int draw;
	int complete;
};

struct render_info {
	struct rb_node pid_node;
	struct list_head bufferid_list;
	struct list_head ui_list;

	/*render basic info pid bufferId..etc*/
	int pid;
	unsigned long long buffer_id;
	int ui_pid;		/*vsync aligned frame's UI pid*/
	int tgid;	/*render's process pid*/
	int api;	/*connected API*/
	int frame_type;
	int render_method;	/* HWUI/SWUI/GLSurface */
	int render;	/* vsync aligned render info 1 or 0 */
	struct frame_id_info frame_id[2];

	/*render queue/dequeue/frame time info*/
	unsigned long long t_enqueue_start;
	unsigned long long t_enqueue_end;
	unsigned long long t_dequeue_start;
	unsigned long long t_dequeue_end;
	unsigned long long enqueue_length;
	unsigned long long dequeue_length;
	unsigned long long self_time;
	unsigned long long Q2Q_time;
	unsigned long long sleep_time;

	/*fbt*/
	int linger;
	struct fbt_boost_info boost_info;
	struct fbt_thread_loading *pLoading;
	struct fbt_thread_blc *p_blc;

	struct mutex thr_mlock;
};

struct fteh_loading {
	int pid;
	int loading;
};

#ifdef FPSGO_DEBUG
#define FPSGO_LOGI(...)	pr_debug("FPSGO:" __VA_ARGS__)
#else
#define FPSGO_LOGI(...)
#endif
#define FPSGO_LOGE(...)	pr_debug("FPSGO:" __VA_ARGS__)
#define FPSGO_CONTAINER_OF(ptr, type, member) ((type *)(((char *)ptr) - offsetof(type, member)))

void *fpsgo_alloc_atomic(int i32Size);
void fpsgo_free(void *pvBuf, int i32Size);
unsigned long long fpsgo_get_time(void);

int fpsgo_get_tgid(int pid);
void fpsgo_render_tree_lock(const char *tag);
void fpsgo_render_tree_unlock(const char *tag);
void fpsgo_thread_lock(struct mutex *mlock);
void fpsgo_thread_unlock(struct mutex *mlock);
void fpsgo_lockprove(const char *tag);
void fpsgo_thread_lockprove(const char *tag, struct mutex *mlock);
void fpsgo_delete_render_info(int pid);
struct render_info *fpsgo_search_and_add_render_info(int pid, int force);
int fpsgo_has_bypass(void);
void fpsgo_check_thread_status(void);
void fpsgo_clear(void);

int init_fpsgo_common(void);


enum FPSGO_ERROR {
	FPSGO_OK,
	FPSGO_ERROR_FAIL,
	FPSGO_ERROR_OOM,
	FPSGO_ERROR_OUT_OF_FD,
	FPSGO_ERROR_FAIL_WITH_LIMIT,
	FPSGO_ERROR_TIMEOUT,
	FPSGO_ERROR_CMD_NOT_PROCESSED,
	FPSGO_ERROR_INVALID_PARAMS,
	FPSGO_INTENTIONAL_BLOCK
};

enum FRAME_TYPE {
	NON_VSYNC_ALIGNED_TYPE = 0,
	VSYNC_ALIGNED_TYPE = 1,
	BY_PASS_TYPE = 2,
};

enum {
	WINDOW_DISCONNECT = 0,
	NATIVE_WINDOW_API_EGL = 1,
	NATIVE_WINDOW_API_CPU = 2,
	NATIVE_WINDOW_API_MEDIA = 3,
	NATIVE_WINDOW_API_CAMERA = 4,
};

enum {
	FPSGO_FORCE_OFF = 0,
	FPSGO_FORCE_ON = 1,
	FPSGO_FREE = 2,
};

enum {
	SWUI = 0,
	HWUI,
	GLSURFACE
};

#endif
