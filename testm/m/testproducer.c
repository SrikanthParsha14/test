#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

MODULE_LICENSE("Dual BSD/GPL");

struct semaphore sem_producer; 	/*"��?����?",��?�i�v�L���?�I?��s�\��?�s��?*/
struct semaphore sem_consumer; 	/*"��??"�C�ݗL?�i�I?��(��?��??)�ˉȏ�?*/
char product[10]; 		/*"?�i"�����n*/
int exit_flags; 	  	/*��?????�u*/
int producer(void *product); 	/*��?��*/
int consumer(void *product); 	/*��?��*/

static int procon_init(void)
{
    printk(KERN_INFO"show producer and consumer\n");
    init_MUTEX(&sem_producer); 	/*??"��?����?"�C�󊎏y?��?*/
    init_MUTEX_LOCKED(&sem_consumer); 			/*??"��??"�C�A�s��?*/
    exit_flags=0; 		/*��??��?�H*/
    kernel_thread(consumer,product,CLONE_KERNEL); 	/*??��?��*/
    kernel_thread(producer,product,CLONE_KERNEL); 	/*??��?��*/
    return 0;
}
static void procon_exit(void)
{
    printk(KERN_INFO"exit producer and consumer\n");
}
/*
* ��?�ҁC??��?�\��?�i
*/
int producer(void *p)
{
    char *product=(char *)p;
    int i;
    for(i=0;i<10;i++) { 	/*?����?�\��?�i*/
        /* ?��"��?����?"�C�@��?�i��?���?�C
         * ?�y?��?�B��?�ݍ����Ғ������v��?
         */
        down(&sem_producer);
        snprintf(product,10,"product-%d",i); 		/*��??�i*/
        printk(KERN_INFO"producer produce %s\n",product);/*��?�Ғ񎦛�?��?*/
        up(&sem_consumer); 	/*?��?��?��"��??"*/
    }
    exit_flags=1; 		/*��?��?�C??��??*/
    return 0;
}
/*
* ��?�ҁC�@�ʗL?�i�C?��??�i
*/
int consumer(void *p)
{
    char *product=(char *)p;
    for(;;) {
        if(exit_flags) 		/*�@�ʐ�?�H�ʛ�???�C?��~��?*/
            break;
        /*?��"��??"�C�@�ʗL?�i�C?��?��C
         *?�s��?�B��?���Ғ����L?�i�B
         */
        down(&sem_consumer);
        printk(KERN_INFO"consumer consume %s\n",product);/*��?�Ғ�?����?�i*/
        memset(product,'\0',10);/*��??�i*/
        up(&sem_producer); 	/*����?�Ғ�o��?����*/
    }
    return 0;
}

module_init(procon_init);
module_exit(procon_exit);
MODULE_AUTHOR("Niu Tao");
MODULE_DESCRIPTION("producer and consumer Module");
MODULE_ALIAS("a simplest module");
