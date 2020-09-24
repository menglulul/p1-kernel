#include "sched.h"
#include "irq.h"
#include "printf.h"

static struct task_struct init_task = INIT_TASK;
struct task_struct *current = &(init_task);
struct task_struct * task[NR_TASKS] = {&(init_task), };
int nr_tasks = 1;
int timer_cnt = 0;

//void preempt_disable(void)
//{
//	current->preempt_count++;
//}
//
//void preempt_enable(void)
//{
//	current->preempt_count--;
//}


void _schedule(void)
{
//	preempt_disable();
	int next,c;
	struct task_struct * p;
	while (1) {
		c = -1;
		next = 0;
		for (int i = 0; i < NR_TASKS; i++){
			p = task[i];
			if (p && p->state == TASK_RUNNING && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}
		if (c) {
			break;
		}
		for (int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}
	switch_to(task[next]);
//	preempt_enable();
}

void schedule(void)
{
	current->counter = 0;
	_schedule();
}

void switch_to(struct task_struct * next) 
{
	if (current == next) 
		return;
	struct task_struct * prev = current;
	current = next;
	cpu_switch_to(prev, next);
}

void schedule_tail(void) {
//	preempt_enable();
}

void change_state(int i) {
	switch(i) {
		case TASK_RUNNING : current->state = TASK_RUNNING; printf("changed to running\n"); break;
		case TASK_WAIT : current->state = TASK_WAIT; timer_cnt = 5; printf("changed to wait\n"); break;
	}
}

void timer_tick()
{
	timer_cnt--;
	if(timer_cnt>0){
		printf("timer cnt: %d\n", timer_cnt);
	} else {
		timer_cnt=5;
		struct task_struct * p;
		for (int i = 0; i < NR_TASKS; i++){
			p = task[i];
			if(p && p->state == TASK_WAIT) {
				switch_to(task[i]);
				change_state(TASK_RUNNING);
				schedule();
			}
		}
	
	}

}
