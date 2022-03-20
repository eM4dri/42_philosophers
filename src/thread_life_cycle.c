/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_life_cycle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:11 by emadriga          #+#    #+#             */
/*   Updated: 2022/02/21 18:44:29 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	kill_philo(t_philo *ph)
{
	u_int64_t	now;

	ph->status = DIE;
	ph->data->off = 1;
	usleep(100);
	pthread_mutex_lock(&ph->data->m_print);
	now = ph->lastmeal + ph->data->t2d;
	printf(PHILO_DIES, now - ph->data->start, ph->id);
	pthread_mutex_unlock(&ph->data->m_print);
	return (0);
}

static int	philo_sleep(t_philo *ph)
{
	ph->status = THINK;
	print_action(ph, PHILO_THINKS, ph->lastmeal + ph->data->t2e \
	+ ph->data->t2s);
	if (ph->meal_count > 0)
		ph->meal_count--;
	ph->status_changed = TRUE;
	return (0);
}

static int	philo_eat(t_philo *ph)
{
	ph->status = SLEEP;
	print_action(ph, PHILO_SLEEPS, (u_int64_t)(ph->lastmeal + ph->data->t2e));
	pthread_mutex_lock(ph->rfork);
	*ph->rforkval = TABLE;
	pthread_mutex_unlock(ph->rfork);
	pthread_mutex_lock(&ph->lfork);
	ph->lforkval = TABLE;
	pthread_mutex_unlock(&ph->lfork);
	ph->status_changed = TRUE;
	return (0);
}

/**
* * try_lock_forks
* Locks left/own's fork if it's free,
* Once left's taken locks right/other fork if its free
* Once both're taken set status EAT
* @param ph	Philosopher
*/
static void	try_lock_forks(t_philo *ph, u_int64_t	now)
{
	if (ph->lforkval == TABLE)
	{
		pthread_mutex_lock(&ph->lfork);
		ph->lforkval = LEFT_HAND;
		pthread_mutex_unlock(&ph->lfork);
		print_action(ph, LEFT_FORK_TAKEN, now);
	}
	if (*ph->rforkval == TABLE)
	{
		pthread_mutex_lock(ph->rfork);
		*ph->rforkval = RIGHT_HAND;
		pthread_mutex_unlock(ph->rfork);
		print_action(ph, RIGHT_FORK_TAKEN, now);
	}
	if (ph->lforkval == LEFT_HAND && *ph->rforkval == RIGHT_HAND)
	{
		ph->status = EAT;
		ph->lastmeal = get_current_time();
		print_action(ph, PHILO_EATS, ph->lastmeal);
		ph->status_changed = TRUE;
	}
}

void	*life_cycle(void *arg)
{
	t_philo		*ph;
	u_int64_t	now;

	ph = (t_philo *) arg;
	ph->lastmeal = get_current_time();
	while (!ph->data->off && ph->meal_count != 0)
	{
		now = get_current_time();
		if (now > ph->lastmeal + ph->data->t2d)
			kill_philo(ph);
		else if (ph->status == THINK)
			try_lock_forks(ph, now);
		else if (ph->status == EAT && now > ph->lastmeal + ph->data->t2e)
			philo_eat(ph);
		else if (ph->status == SLEEP \
		&& now > ph->lastmeal + ph->data->t2e + ph->data->t2s)
			philo_sleep(ph);
		if (ph->status_changed--)
			usleep(5000);
		else
			usleep(50);
	}
	return (0);
}
