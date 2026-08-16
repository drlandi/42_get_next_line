/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlandi <dlandi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 15:55:19 by dlandi            #+#    #+#             */
/*   Updated: 2026/07/19 17:04:40 by dlandi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strjoin(char *reserve, char *bucket)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = malloc(sizeof(char) * (ft_strlen(reserve) + ft_strlen(bucket) + 1));
	if (!str)
	{
		free(reserve);
		return (NULL);
	}
	i = -1;
	if (reserve)
		while (reserve[++i])
			str[i] = reserve[i];
	else
		i = 0;
	j = 0;
	while (bucket[j])
		str[i++] = bucket[j++];
	str[i] = '\0';
	free(reserve);
	return (str);
}
