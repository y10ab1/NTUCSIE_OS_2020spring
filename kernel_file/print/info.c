#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage int sys_show_info(char *output){
    printk(output);
    return 0;
}
