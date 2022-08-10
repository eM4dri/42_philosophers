/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:37:22 by emadriga          #+#    #+#             */
/*   Updated: 2022/08/10 19:04:56 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <errno.h>
# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# define ERROR "Error\n\t%s\n"
# define VALID_ARGV "Valid execution must have this arguments\n\
\t  * number of philosophers\n\t  * time to die\n\t  * time to eat\n\t\
  * time to sleep\n\t  * number of times each philosopher must eat (optional)"
# define POSITIVE_INT_ONLY "Only positive integers allowed"
# define MAX_ALLOWED_PHILOS "Maximun philos allowed 200"
# define MIN_ALLOWED_TIMMER "Minimun timmer allowed 60ms"
# define PRINT_MUTEX_INIT_FAILED "Print mutex init failed"
# define OFF_MUTEX_INIT_FAILED "Off mutex init failed"
# define FORK_MUTEX_INIT_FAILED "Fork mutex init failed"
# define PTHREAD_CREATE_FAILED "Pthread create failed"
# define MAX_INT 2147483647
# define MAX_PHILOS 200
# define MIN_TIMMER 60
# define THOUSAND 1000
# define LEFT_FORK_TAKEN  "\033[0;36m \
[%lums] \tphilo %d has taken left fork\n\033[0;39m"
# define RIGHT_FORK_TAKEN  "\033[0;36m \
[%lums] \tphilo %d has taken right fork\n\033[0;39m"
# define PHILO_EATS	"\033[0;34m \
[%lums] \tphilo %d is eating\n\033[0;39m"
# define PHILO_SLEEPS "\033[0;35m \
[%lums] \tphilo %d is sleeping\n\033[0;39m"
# define PHILO_THINKS "\033[0;32m \
[%lums] \tphilo %d is thinking\n\033[0;39m"
# define PHILO_DIES "\033[0;31m \
[%llums] \tphilo %d died\n\033[0;39m"
# define TRAZA "Traza n %d \n"
# define SUBTRAZA "\tsubTraza %s n %d \n"
# define START_ITERATOR -1

enum e_status{
	EAT,
	SLEEP,
	THINK,
	DIE
};
enum e_place{
	TABLE,
	LEFT_HAND,
	RIGHT_HAND
};
enum e_bool{
	FALSE,
	TRUE
};
typedef struct s_data
{
	short			nbr_philos;
	int				t2d;
	int				t2e;
	int				t2s;
	int				delay;
	int				min_delay;
	int				total_meals;
	short			off;
	u_int64_t		start;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_off;
}t_data;
typedef struct s_philo
{
	short			id;
	short			status_changed;
	int				meal_count;
	short			status;
	u_int64_t		lastmeal;
	pthread_t		thread;
	short			lforkval;
	short			*rforkval;
	pthread_mutex_t	lfork;
	pthread_mutex_t	*rfork;
	t_data			*data;
}t_philo;

u_int64_t	get_current_time(void);
int			cant_init_philos(t_philo *ph, t_data *data, int i);
void		*life_cycle(void *arg);
int			print_action(t_philo *ph, const char *str, u_int64_t now);
int			stop(t_data *data);
void		performance_delay(t_philo *ph);
#endif
