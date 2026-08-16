/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlandi <dlandi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 15:41:43 by dlandi            #+#    #+#             */
/*   Updated: 2026/08/10 20:08:11 by dlandi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*clean_reserve(char *reserve)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (reserve && reserve[i] && reserve[i] != '\n')
		i++;
	if (!reserve || !reserve[i])
	{
		free(reserve);
		return (NULL);
	}
	str = malloc(sizeof(char) * (ft_strlen(reserve) - i + 1));
	if (!str)
	{
		free(reserve);
		return (NULL);
	}
	j = 0;
	while (reserve[++i])
		str[j++] = reserve[i];
	str[j] = '\0';
	free(reserve);
	return (str);
}

char	*extract_line(char *reserve)
{
	int		i;
	char	*str;

	i = 0;
	if (!reserve || !reserve[i])
		return (NULL);
	while (reserve[i] && reserve[i] != '\n')
		i++;
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (reserve[i] && reserve[i] != '\n')
	{
		str[i] = reserve[i];
		i++;
	}
	if (reserve[i] == '\n')
	{
		str[i] = reserve[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*read_to_reserve(int fd, char *reserve)
{
	char	*bucket;
	int		b;

	bucket = malloc(BUFFER_SIZE + 1);
	b = 1;
	while (bucket && !ft_strchr(reserve, '\n') && b > 0)
	{
		b = read(fd, bucket, BUFFER_SIZE);
		if (b == -1)
			break ;
		if (b > 0)
		{
			bucket[b] = '\0';
			reserve = ft_strjoin(reserve, bucket);
			if (!reserve)
				break ;
		}
	}
	free(bucket);
	if (b == -1)
	{
		free(reserve);
		reserve = NULL;
	}
	return (reserve);
}

char	*get_next_line(int fd)
{
	static char	*reserve;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (reserve)
		{
			free(reserve);
			reserve = NULL;
		}
		return (NULL);
	}
	reserve = read_to_reserve(fd, reserve);
	if (!reserve)
		return (NULL);
	line = extract_line(reserve);
	if (!line)
	{
		free(reserve);
		reserve = NULL;
		return (NULL);
	}
	reserve = clean_reserve(reserve);
	return (line);
}
