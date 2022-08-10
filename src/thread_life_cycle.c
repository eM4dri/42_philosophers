/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_life_cycle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:11 by emadriga          #+#    #+#             */
/*   Updated: 2022/08/10 22:00:56 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	kill_philo(t_philo *ph)
{
	u_int64_t	now;

	pthread_mutex_lock(&ph->data->m_off);
	if (!ph->data->off)
	{
		ph->status = DIE;
		ph->data->off = 1;
		usleep(ph->data->delay);
		pthread_mutex_lock(&ph->data->m_print);
		now = ph->lastmeal + ph->data->t2d;
		printf(PHILO_DIES, now - ph->data->start, ph->id);
		pthread_mutex_unlock(&ph->data->m_print);
	}
	pthread_mutex_unlock(&ph->data->m_off);
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
	pthread_mutex_lock(&ph->lfork);
	if (ph->lforkval == TABLE)
	{
		ph->lforkval = LEFT_HAND;
		print_action(ph, LEFT_FORK_TAKEN, now);
	}
	if (ph->data->nbr_philos > 1)
	{
		pthread_mutex_lock(ph->rfork);
		if (*ph->rforkval == TABLE)
		{
			*ph->rforkval = RIGHT_HAND;
			print_action(ph, RIGHT_FORK_TAKEN, now);
		}
		if (ph->lforkval == LEFT_HAND && *ph->rforkval == RIGHT_HAND)
		{
			ph->status = EAT;
			ph->lastmeal = get_current_time();
			print_action(ph, PHILO_EATS, ph->lastmeal);
			ph->status_changed = TRUE;
		}
		pthread_mutex_unlock(ph->rfork);
	}
	pthread_mutex_unlock(&ph->lfork);
}

void	*life_cycle(void *arg)
{
	t_philo		*ph;
	u_int64_t	now;

	ph = (t_philo *) arg;
	ph->lastmeal = get_current_time();
	while (ph->meal_count != 0 && !stop(ph->data))
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
		performance_delay(ph);
	}
	return (0);
}
