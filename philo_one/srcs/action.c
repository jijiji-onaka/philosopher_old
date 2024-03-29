/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjinichi <tjinichi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 16:10:59 by tjinichi          #+#    #+#             */
/*   Updated: 2021/06/07 01:02:52 by tjinichi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

void	*eat_spaghetti(t_philosopher *philo)
{
	if (philo->eat_count
		== philo->info->number_of_times_each_philosopher_must_eat)
		return (NULL);
	if (pthread_mutex_lock(&philo->info->forks[philo->right_fork]) != 0)
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	if (take_fork(philo, RIGHT) == NULL)
		return (NULL);
	if (pthread_mutex_lock(&philo->info->forks[philo->left_fork]) != 0)
	{
		if (pthread_mutex_unlock(&philo->info->forks[philo->right_fork]) != 0)
			return (wrap(error_exit(ERR_MUTEX_UNLOCK)));
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	}
	if (take_fork(philo, LEFT) == NULL)
		return (NULL);
	return (eat_spaghetti_2(philo));
}

void	*sleep_after_eating(t_philosopher *philo)
{
	if (philo->eat_count
		== philo->info->number_of_times_each_philosopher_must_eat)
		return (NULL);
	if (philo->info->status == DEATH)
		return (NULL);
	if (pthread_mutex_lock(&(philo->info->mutex)) != 0)
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	if (print_action(philo->number + 1, SLEEP,
			philo->info->status, philo->info->begin_time) == EXIT_FAILURE)
	{
		if (pthread_mutex_unlock(&(philo->info->mutex)) != 0)
			return (wrap(error_exit(ERR_MUTEX_UNLOCK)));
		return (wrap(error_exit(ERR_PRINTF)));
	}
	if (pthread_mutex_unlock(&(philo->info->mutex)) != 0)
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	if (my_usleep(philo->info->time_to_sleep, philo->info) == NULL)
		return (wrap(error_exit(ERR_GETTIMEOFDAY)));
	return ("OK");
}

void	*deep_thought(t_philosopher *philo)
{
	if (philo->eat_count
		== philo->info->number_of_times_each_philosopher_must_eat)
		return (NULL);
	if (philo->info->status == DEATH)
		return (NULL);
	if (pthread_mutex_lock(&(philo->info->mutex)) != 0)
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	if (print_action(philo->number + 1,
			THINK, philo->info->status,
			philo->info->begin_time) == EXIT_FAILURE)
	{
		if (pthread_mutex_unlock(&(philo->info->mutex)) != 0)
			return (wrap(error_exit(ERR_MUTEX_UNLOCK)));
		return (wrap(error_exit(ERR_PRINTF)));
	}
	if (pthread_mutex_unlock(&(philo->info->mutex)) != 0)
		return (wrap(error_exit(ERR_MUTEX_LOCK)));
	return ("OK");
}

void	*take_fork(t_philosopher *phil, int l_or_r)
{
	void	*res;

	res = "OK";
	if (phil->eat_count
		== phil->info->number_of_times_each_philosopher_must_eat)
		res = NULL;
	if (phil->info->status == DEATH)
		res = NULL;
	if (print_action(phil->number + 1,
			TAKE_FORK, phil->info->status,
			phil->info->begin_time) == EXIT_FAILURE)
		res = NULL;
	if (res == NULL)
	{
		if (l_or_r == RIGHT || l_or_r == LEFT)
			if (pthread_mutex_unlock(&phil->info->forks[phil->right_fork]) != 0)
				return (wrap(error_exit(ERR_MUTEX_UNLOCK)));
		if (l_or_r == LEFT)
			if (pthread_mutex_unlock(&phil->info->forks[phil->left_fork]) != 0)
				return (wrap(error_exit(ERR_MUTEX_UNLOCK)));
		if (phil->info->status == DEATH)
			return (NULL);
		return (wrap(error_exit(ERR_PRINTF)));
	}
	return (res);
}

void	*die_check(t_philosopher *philo, time_t cur_time)
{
	if (philo->info->time_to_die + philo->last_eat_time < cur_time)
	{
		if (print_action(philo->number + 1,
				DIED, philo->info->status,
				philo->info->begin_time) == EXIT_FAILURE)
		{
			if (pthread_mutex_unlock(&(philo->info->mutex)) != 0)
				return (wrap(error_exit(ERR_MUTEX_LOCK)));
			return (wrap(error_exit(ERR_PRINTF "5")));
		}
		philo->info->status = DEATH;
	}
	return ("OK");
}
