/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:18 by emadriga          #+#    #+#             */
/*   Updated: 2022/02/21 18:26:15 by emadriga         ###   ########.fr       */
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
	if (!ph->data->off)
	{
		pthread_mutex_lock(&ph->data->m_print);
		printf(str, now - ph->data->start, ph->id);
		pthread_mutex_unlock(&ph->data->m_print);
	}
	return (0);
}
