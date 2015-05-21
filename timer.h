#ifndef TIMER_H
#define TIMER_H

typedef struct timer timer_t;

timer_t * timer_create();
void timer_destroy(timer_t *timer);

void timer_add(timer_t *timer, unsigned long id, unsigned long elapse);
void timer_delete(timer_t *timer, unsigned long id);
void timer_delete_all(timer_t *timer);

typedef void (* timer_execute_ptr)(unsigned long id, void *userp);

void timer_expire(timer_t *timer, timer_execute_ptr execute, void *userp);

#endif /* TIMER_H */
