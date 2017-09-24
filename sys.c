SYSCALL_DEFINE0(hello)
{
	printk( KERN_WARNING "ANINDA AMINUZZAMAN 1001018367\n");
	return 0;
}

SYSCALL_DEFINE2(procstat, int, PID, struct proc_stat*, proccc)
{
	printk( KERN_WARNING "IN DEFINE2\n");
	struct proc_stat kernel;	
	kernel.state = find_task_by_pid_ns(PID, &init_pid_ns)->state;
	long t = kernel.state;
	printk( KERN_WARNING "STATE = %ld \n", t);
	return 0;	
}
