/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evscheid <evscheid@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 21:57:47 by evscheid          #+#    #+#             */
/*   Updated: 2023/10/02 21:57:47 by evscheid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_death(t_philo *philo)
{
    int current_time;

    current_time = get_current_time();
    pthread_mutex_lock(&philo->data->death_mutex);
    if (philo->data->philo_died >= 1)
    {
        pthread_mutex_unlock(&philo->data->death_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->death_mutex);
    pthread_mutex_lock(&philo->data->meal1);
    if ((current_time - philo->last_meal_time) > philo->data->time_to_die)
    {
        pthread_mutex_lock(&philo->data->death_mutex);
        philo->data->philo_died++;
        pthread_mutex_unlock(&philo->data->death_mutex);
        pthread_mutex_unlock(&philo->data->meal1);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->meal1);
    return (0);
}

void *monitor_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)  // Condition de sortie ajoutée
    {
        if (check_death(philo))
        {
            display_action(philo, "died");
            return (NULL);
        }
        ft_usleep(100);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    t_data data;
    pthread_t *monitor_threads;

    // Analyser les arguments d'entrée
    if (parse_args(argc, argv, &data) != 0) {
        printf("Erreur : arguments invalides.\n");
        return (1);
    }

    // Initialiser les données
    if (init_data(&data) != 0) {
        printf("Erreur : échec de l'initialisation.\n");
        return (1);
    }

    data.start_time = get_current_time();
    monitor_threads = malloc(sizeof(pthread_t) * data.nb_philos);
    for (int i = 0; i < data.nb_philos; i++)
    {
        pthread_create(&data.threads[i], NULL, philo_routine, &data.philos[i]);
        pthread_create(&monitor_threads[i], NULL, monitor_routine, &data.philos[i]);
    }

    // Attendre que tous les threads des philosophes se terminent
    for (int i = 0; i < data.nb_philos; i++)
    {
       pthread_join(data.threads[i], NULL);
       pthread_join(monitor_threads[i], NULL);
    }
    // dans l'ancienne version les joins bloquer a cause des data races, maintenant c'est fix mais les philo ne marche plus vraiment, le pb venait de routine.c
    free(data.forks);
    free(data.philos);
    free(data.threads);
    free(monitor_threads);
    for (int i = 0; i < data.nb_philos; i++)
        pthread_mutex_destroy(&data.forks[i]);
    pthread_mutex_destroy(&data.death_mutex);
    pthread_mutex_destroy(&data.meal1);
    pthread_mutex_destroy(&data.meal2);
    pthread_mutex_destroy(&data.print_mutex);
    return 0;
}
