//do this at line 96

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
	long a = kernel.state;
	printk( KERN_WARNING "STATE = %ld \n", a);
	
	kernel.priority = find_task_by_pid_ns(PID, &init_pid_ns)->prio;
	long b = kernel.priority;
	printk( KERN_WARNING "PRIORITY = %ld \n", b);

	kernel.normal_priority = find_task_by_pid_ns(PID, &init_pid_ns)->normal_prio;
	long c = kernel.normal_priority;
	printk(KERN_WARNING "NORMAL PRIORITY = %ld \n", c);

	kernel.static_priority = find_task_by_pid_ns(PID, &init_pid_ns)->static_prio;
	long d = kernel.static_priority;
	printk(KERN_WARNING "STATIC PRIORITY = %ld \n", d);

	kernel.rt_priority = find_task_by_pid_ns(PID, &init_pid_ns)->rt_priority;
	long e = kernel.rt_priority;
	printk(KERN_WARNING "RT PRIORITY = %ld \n", e);

	//kernel.policy = find_task_by_pid_ns(PID, &init_pid_ns)->policy
	
	return 0;	
}
