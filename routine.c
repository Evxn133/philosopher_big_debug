/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evscheid <evscheid@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 19:37:25 by evscheid          #+#    #+#             */
/*   Updated: 2023/10/02 19:37:25 by evscheid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *philo_ptr)
{
    t_philo *philo;

    philo = (t_philo *)philo_ptr;
    while (1)
    {
        if (think(philo))
            break ;
        if (eat(philo))
            break ;
        if (sleep_philo(philo))
            break ;
        pthread_mutex_lock(&philo->data->meal2);
        if (philo->data->max_meals != -1 && philo->meals_eaten >= philo->data->max_meals)
        {
            pthread_mutex_unlock(&philo->data->meal2);
            break ;
        }
        pthread_mutex_unlock(&philo->data->meal2);
    }
    return (NULL);
}
