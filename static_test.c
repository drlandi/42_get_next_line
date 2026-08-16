/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlandi <dlandi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 08:20:36 by dlandi            #+#    #+#             */
/*   Updated: 2026/07/19 08:21:28 by dlandi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void component_test(void)
{
    static int persistent_memory = 0;
    int volatile_memory = 0;

    persistent_memory += 42;
    volatile_memory += 42;

    printf("Persistent (Static): %d | Volatile (Normal): %d\n", persistent_memory, volatile_memory);
}

int main(void)
{
    printf("--- Cycle 1 ---\n");
    component_test();
    
    printf("--- Cycle 2 ---\n");
    component_test();
    
    printf("--- Cycle 3 ---\n");
    component_test();
    
    return (0);
}