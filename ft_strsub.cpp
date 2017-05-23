/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abureau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/29 12:24:35 by abureau           #+#    #+#             */
/*   Updated: 2016/01/13 13:28:02 by abureau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
    char			*tronc;
    unsigned int	tmpstart;
    unsigned int	i;

    i = 0;
    tmpstart = start + len;
    tronc = (char *)malloc(sizeof(char) * len + 1);
    if (!tronc || !s)
    {
        if (!tronc)
            free(tronc);
        return (NULL);
    }
    while (start != tmpstart)
    {
        tronc[i] = s[start];
        start++;
        i++;
    }
    tronc[i] = '\0';
    return (tronc);
}
