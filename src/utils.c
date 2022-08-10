/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:18 by emadriga          #+#    #+#             */
/*   Updated: 2022/08/10 22:15:24 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

u_int64_t	get_current_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * THOUSAND + now.tv_usec / THOUSAND);
}

int	print_action(t_philo *ph, const char *str, u_int64_t now)
{
	pthread_mutex_lock(&ph->data->m_off);
	if (!ph->data->off)
	{
		pthread_mutex_lock(&ph->data->m_print);
		printf(str, now - ph->data->start, ph->id);
		pthread_mutex_unlock(&ph->data->m_print);
	}
	pthread_mutex_unlock(&ph->data->m_off);
	return (0);
}

int	stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->m_off);
	stop = data->off;
	pthread_mutex_unlock(&data->m_off);
	return (stop);
}

void	performance_delay(t_philo *ph)
{
	if (ph->status_changed)
	{
		usleep(ph->data->delay);
		ph->status_changed = 0;
	}
	else
		usleep(ph->data->min_delay);
}
