/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:07 by emadriga          #+#    #+#             */
/*   Updated: 2022/08/10 19:04:41 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *str)
{
	int				sign;
	long long		result;

	sign = 1;
	result = 0;
	while (*str != 0 && (*str == 32 || (*str >= 9 && *str <= 13)))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign *= -1;
	while (*str >= '0' && *str <= '9' && result < 2147483648)
		result = 10 * result + (int)(*str++ - '0');
	if (result > 2147483647 && sign > 0)
		return (-1);
	if (result > 2147483648 && sign < 0)
		return (0);
	return ((int)result * sign);
}

static int	not_valid_nbr(const char *s_nbr)
{
	long	l;

	l = 0;
	while (*s_nbr != '\0' && l < MAX_INT)
	{
		if ((*s_nbr < '0' || *s_nbr > '9'))
		{
			printf(ERROR, POSITIVE_INT_ONLY);
			return (1);
		}
		l = l * 10 + *s_nbr - '0';
		s_nbr++;
	}
	if (l > MAX_INT)
	{
		printf(ERROR, POSITIVE_INT_ONLY);
		return (1);
	}
	return (0);
}

static int	validate_ranges(t_data *in)
{
	if (in->nbr_philos > MAX_PHILOS)
	{
		printf(ERROR, MAX_ALLOWED_PHILOS);
		return (1);
	}
	if (in->t2d < MIN_TIMMER || in->t2e < MIN_TIMMER || in->t2s < MIN_TIMMER)
	{
		printf(ERROR, MIN_ALLOWED_TIMMER);
		return (1);
	}
	return (0);
}

static int	cant_parse_philo(int argc, char **argv, t_data *data)
{
	struct timeval	tstart;

	if (not_valid_nbr(argv[1]) || not_valid_nbr(argv[2]) || \
	not_valid_nbr(argv[3]) || not_valid_nbr(argv[4]) || \
	(argc == 6 && not_valid_nbr(argv[5])))
		return (1);
	data->nbr_philos = ft_atoi(argv[1]);
	data->t2d = ft_atoi(argv[2]);
	data->t2e = ft_atoi(argv[3]);
	data->t2s = ft_atoi(argv[4]);
	data->delay = data->t2d << 4;
	data->min_delay = data->t2d >> 2;
	data->off = 0;
	data->total_meals = -1;
	if (argc == 6)
		data->total_meals = ft_atoi(argv[5]);
	gettimeofday(&tstart, NULL);
	data->start = tstart.tv_sec * THOUSAND + tstart.tv_usec / THOUSAND;
	return (validate_ranges(data));
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_philo			*ph;

	if (argc != 5 && argc != 6)
	{
		printf(ERROR, VALID_ARGV);
		return (1);
	}
	if (cant_parse_philo(argc, argv, &data))
		return (1);
	ph = NULL;
	ph = (t_philo *) malloc(sizeof(t_philo) * data.nbr_philos);
	if (ph == NULL)
	{
		printf(ERROR, "Malloc for philo array failed");
		return (1);
	}
	if (cant_init_philos(ph, &data, START_ITERATOR))
		return (1);
	return (0);
}
