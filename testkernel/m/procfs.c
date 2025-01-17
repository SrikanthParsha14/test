/*
 *  procfs.c -  create a "file" in /proc, which allows both input and output.
 */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>		/* Necessary because we use file_operations */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/sched.h>	/* add by kevin for compile error*/


/*
 * Here we keep the last message received, to prove
 * that we can process our input
 */
#define MESSAGE_LENGTH 80
static char Message[MESSAGE_LENGTH];
static struct proc_dir_entry *Our_Proc_File;

#define PROC_ENTRY_FILENAME "rw_test"

static ssize_t module_output(struct file *filp,	/* see include/linux/fs.h   */
			     char *buffer,	/* buffer to fill with data */
			     size_t length,	/* length of the buffer     */
			     loff_t * offset)
{
	static int finished = 0;
	int i;
	char message[MESSAGE_LENGTH + 30];

	/*
	 * We return 0 to indicate end of file, that we have
	 * no more information. Otherwise, processes will
	 * continue to read from us in an endless loop.
	 */
	if (finished) {
		finished = 0;
		return 0;
	}

	/*
	 * We use put_user to copy the string from the kernel's
	 * memory segment to the memory segment of the process
	 * that called us. get_user, BTW, is
	 * used for the reverse.
	 */
	sprintf(message, "Last input:%s", Message);
	for (i = 0; i < length && message[i]; i++)
		put_user(message[i], buffer + i);

	/*
	 * Notice, we assume here that the size of the message
	 * is below len, or it will be received cut. In a real
	 * life situation, if the size of the message is less
	 * than len then we'd return len and on the second call
	 * start filling the buffer with the len+1'th byte of
	 * the message.
	 */
	finished = 1;

	return i;		/* Return the number of bytes "read" */
}

static ssize_t
module_input(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	int i;
	/*
	 * Put the input into Message, where module_output
	 * will later be able to use it
	 */
	for (i = 0; i < MESSAGE_LENGTH - 1 && i < len; i++)
		get_user(Message[i], buff + i);

	Message[i] = '\0';	/* we want a standard, zero terminated string */
	return i;
}

/*
 * This function decides whether to allow an operation
 * (return zero) or not allow it (return a non-zero
 * which indicates why it is not allowed).
 *
 * The operation can be one of the following values:
 * 0 - Execute (run the "file" - meaningless in our case)
 * 2 - Write (input to the kernel module)
 * 4 - Read (output from the kernel module)
 *
 * This is the real function that checks file
 * permissions. The permissions returned by ls -l are
 * for referece only, and can be overridden here.
 */

// static int module_permission(struct inode *inode, int op)
// {
// 	/*
// 	 * We allow everybody to read from our module, but
// 	 * only root (uid 0) may write to it
// 	 */
// 	printk(KERN_DEBUG"op=%d current_euid=%d\n", op, __kuid_val(current_euid()));
// 	if (op & MAY_READ || (op & MAY_WRITE && __kuid_val(current_euid()) == 0))
// 		return 0;

// 	/*
// 	 * If it's anything else, access is denied
// 	 */
// 	return -EACCES;
// }

/*
 * The file is opened - we don't really care about
 * that, but it does mean we need to increment the
 * module's reference count.
 */
int module_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

/*
 * The file is closed - again, interesting only because
 * of the reference count.
 */
int module_close(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;		/* success */
}

static struct file_operations File_Ops_4_Our_Proc_File = {
	.read = module_output,
	.write = module_input,
	.open = module_open,
	.release = module_close,
};

/*
 * Inode operations for our proc file. We need it so
 * we'll have some place to specify the file operations
 * structure we want to use, and the function we use for
 * permissions. It's also possible to specify functions
 * to be called for anything else which could be done to
 * an inode (although we don't bother, we just put
 * NULL).
 */

// static struct inode_operations Inode_Ops_4_Our_Proc_File = {
// 	.permission = module_permission,	/* check for permissions */
// };

/*
 * Module initialization and cleanup
 */

int init_module()
{
	int rv = 0;
	// Our_Proc_File = create_proc(PROC_ENTRY_FILENAME, 0644, NULL, &File_Ops_4_Our_Proc_File);
	Our_Proc_File = proc_create(PROC_ENTRY_FILENAME, 0644, NULL, &File_Ops_4_Our_Proc_File);
	// proc_set_user(Our_Proc_File, 0, 0);
	proc_set_size(Our_Proc_File, 80);

	/* After 3.10, definition of struct proc_dir_entry moved to fs/proc/internal.h
	to ensure the modules using the official procfs API.*/
	// Our_Proc_File->owner = THIS_MODULE;
	// Our_Proc_File->proc_iops = &Inode_Ops_4_Our_Proc_File;
	// Our_Proc_File->proc_fops = &File_Ops_4_Our_Proc_File;
	// Our_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
	// Our_Proc_File->uid = 0;
	// Our_Proc_File->gid = 0;
	// Our_Proc_File->size = 80;

	printk(KERN_DEBUG"Init module: procfs\n");

	if (Our_Proc_File == NULL) {
		rv = -ENOMEM;
		remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
		printk(KERN_INFO"Error: Could not initialize /proc/rw_test\n");
	}

	return rv;
}

void cleanup_module()
{
	remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin Qu");
