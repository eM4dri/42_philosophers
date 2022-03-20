/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:13 by emadriga          #+#    #+#             */
/*   Updated: 2022/02/21 18:44:48 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_threads(t_philo *ph, int nbr_philos, int i)
{
	while (++i < nbr_philos)
		pthread_join(ph[i].thread, NULL);
	i = START_ITERATOR;
	while (++i < nbr_philos)
		pthread_mutex_destroy(&ph[i].lfork);
	free(ph);
}

static int	cant_init_threads(t_philo *ph, int i)
{
	i = 0;
	while (i < ph->data->nbr_philos)
	{
		if (pthread_create(&ph[i].thread, NULL, &life_cycle, &ph[i]))
		{
			printf(ERROR, PTHREAD_CREATE_FAILED);
			return (1);
		}
		i += 2;
	}
	usleep(50);
	i = 1;
	while (i < ph->data->nbr_philos)
	{
		if (pthread_create(&ph[i].thread, NULL, &life_cycle, &ph[i]))
		{
			printf(ERROR, PTHREAD_CREATE_FAILED);
			return (1);
		}
		i += 2;
	}
	return (0);
}

static int	cant_init_mutex(t_philo *ph, t_data *data, int i)
{
	if (pthread_mutex_init(&data->m_print, NULL))
	{
		printf(ERROR, PRINT_MUTEX_INIT_FAILED);
		return (1);
	}
	while (++i < data->nbr_philos)
	{
		if (pthread_mutex_init(&ph[i].lfork, NULL))
		{
			printf(ERROR, FORK_MUTEX_INIT_FAILED);
			return (1);
		}
	}
	return (0);
}

static int	cant_init_philos_forks(t_philo *ph, int nbr_philos, int i)
{
	int	right;

	while (++i < nbr_philos)
	{
		if (i == nbr_philos - 1)
			right = 0;
		else
			right = i + 1;
		ph[i].rforkval = &ph[right].lforkval;
		ph[i].rfork = &ph[right].lfork;
	}
	return (0);
}

int	cant_init_philos(t_philo *ph, t_data *data, int i)
{
	if (cant_init_mutex(ph, data, START_ITERATOR))
		return (1);
	while (++i < data->nbr_philos)
	{
		ph[i].id = i + 1;
		ph[i].data = data;
		ph[i].lastmeal = data->start;
		ph[i].meal_count = data->total_meals;
		ph[i].status = THINK;
		ph[i].lforkval = TABLE;
		ph[i].rforkval = NULL;
		ph[i].rfork = NULL;
		ph[i].status_changed = FALSE;
	}
	if (cant_init_philos_forks(ph, data->nbr_philos, START_ITERATOR))
		return (1);
	i = START_ITERATOR;
	if (cant_init_threads(ph, START_ITERATOR))
		return (1);
	wait_threads(ph, data->nbr_philos, START_ITERATOR);
	return (0);
}
