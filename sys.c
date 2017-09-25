//do this at line 96

SYSCALL_DEFINE0(hello)
{
	printk( KERN_WARNING "ANINDA AMINUZZAMAN 1001018367\n");
	return 0;
}

SYSCALL_DEFINE2(procstat, int, PID, struct proc_stat*, proccc)
{
	if(find_task_by_pid_ns(PID, &init_pid_ns) == NULL)//find_task_by_pid_ns(PID, &init_pid_ns)->parent->pid == NULL && find_task_by_pid_ns(PID, &init_pid_ns)->pid != 1)
	{
		printk(KERN_WARNING "PID IS NULL\n");
		int errno = ESRCH;
		return errno;
	}
	
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

	kernel.policy = find_task_by_pid_ns(PID, &init_pid_ns)->policy;
	long f = kernel.policy;
	printk(KERN_WARNING "POLICY = %ld \n", f);

	struct sched_rt_entity temp = find_task_by_pid_ns(PID, &init_pid_ns)->rt;
	kernel.time_slice = temp.time_slice;
	int g = kernel.time_slice;
	printk(KERN_WARNING "TIME SLICE = %d \n", g);

	kernel.pid = find_task_by_pid_ns(PID, &init_pid_ns)->pid;
	int h = kernel.pid;
	printk(KERN_WARNING "PID = %d \n", h);

	kernel.num_context_switches = (find_task_by_pid_ns(PID, &init_pid_ns)->nvcsw) + (find_task_by_pid_ns(PID, &init_pid_ns)->nivcsw);
	int i = kernel.num_context_switches;
	printk(KERN_WARNING "CONTEXT SWITCHES = %d\n", i);

	kernel.user_time = find_task_by_pid_ns(PID, &init_pid_ns)->utime;
	long j = kernel.user_time;
	printk(KERN_WARNING "USER TIME = %ld\n",j);

	kernel.sys_time = find_task_by_pid_ns(PID, &init_pid_ns)->stime;
	long k = kernel.sys_time;
	printk(KERN_WARNING "SYSTEM TIME = %ld\n", k);

	//kernel.total_pages_mapped = find_task_by_pid_ns(PID, &init_pid_ns)->numa_pages_migrated;
	//long l = kernel.total_pages_mapped;
	//printk(KERN_WARNING "TOTAL PAGES MAPPED = %ld\n", l);
	
	strcpy(kernel.name, find_task_by_pid_ns(PID, &init_pid_ns)->comm);
	printk(KERN_WARNING "NAME = %s", &kernel.name);

	kernel.parent_pid = find_task_by_pid_ns(PID, &init_pid_ns)->real_parent->pid;
	int m = kernel.parent_pid;
	printk(KERN_WARNING "PARENT PID = %d", m);
	
	/*proccc->state = a;//kernel.state;
	proccc->priority = b;//kernel.priority;
	proccc->normal_priority = c;//kernel.priority;
	proccc->static_priority = d;//kernel.static_priority;
	proccc->rt_priority = e;//kernel.rt_priority;
	proccc->time_slice = f;//kernel.time_slice;
	proccc->policy = g;//kernel.policy; */
	

	copy_to_user(proccc, &kernel, sizeof(struct proc_stat));
	
	return 0;
}
