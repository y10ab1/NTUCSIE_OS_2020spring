#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_osproj1_gettime(struct timespec *r) {
	getnstimeofday(r);
}
