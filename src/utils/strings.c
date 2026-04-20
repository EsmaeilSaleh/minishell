#include "minishell.h"

int ft_strlen(const char *s)
{
    int i;

    i = 0;
    while (*s)
    {
        s++;
        i++;
    }
    return (i);
}

char *ft_strcpy(char *dst, const char *src)
{
    int i;

    i = 0;
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

char *ft_strdup(const char *s)
{
    char *copy;

    copy = malloc(ft_strlen(s) + 1);
    if (!copy)
        return (NULL);
    return (ft_strcpy(copy, s));
}

char *ft_substr(const char *s, int start, int len)
{
    char *substr;
    int i;
    int s_len;

    i = 0;
    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > (s_len - start))
        len = s_len - start;
    substr = malloc(len + 1);
    if (!substr)
        return (NULL);
    while (i < len && s[start + i])
    {
        substr[i] = s[start + i];
        i++;
    }
    substr[i] = '\0';
    return (substr);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 || *s2)
    {
        if ((unsigned char)*s1 != (unsigned char)*s2)
            return ((unsigned char)*s1 - (unsigned char)*s2);
        s1++;
        s2++;
    }
    return (0);
}