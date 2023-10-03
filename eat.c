/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evscheid <evscheid@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 13:46:16 by evscheid          #+#    #+#             */
/*   Updated: 2023/10/03 13:46:16 by evscheid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->death_mutex);
    if (philo->data->philo_died)
    {
        pthread_mutex_unlock(&philo->data->death_mutex);
        return 1;
    }
    pthread_mutex_unlock(&philo->data->death_mutex);
    take_fork(philo->right_fork, philo);
    take_fork(philo->left_fork, philo);
    pthread_mutex_lock(&philo->data->meal1);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(&philo->data->meal1);
    display_action(philo, "is eating");
    // Simule le temps que le philosophe passe à manger
    ft_usleep(philo->data->time_to_eat);
    pthread_mutex_lock(&philo->data->meal2);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meal2);
    drop_fork(philo->left_fork);
    drop_fork(philo->right_fork);
    return 0;
}
