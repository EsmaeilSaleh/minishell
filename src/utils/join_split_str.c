#include "minishell.h"

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && (s1[i] || s2[i]))
    {
        if ((unsigned char)s1[i] != (unsigned char)s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return (0);
}

char *ft_strjoin(const char *s1, const char *s2)
{
    char *joined;
    char *p;

    joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!joined)
        return (NULL);
    p = joined;
    while (*s1)
        *p++ = *s1++;
    while (*s2)
        *p++ = *s2++;
    *p = '\0';
    return (joined);
}

char *ft_join_path(char *dir, char *cmd)
{
    char *path;
    size_t i;

    i = 0;
    path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
    if (!path)
        return (NULL);
    while (*dir)
        path[i++] = *dir++;
    path[i++] = '/';
    while (*cmd)
        path[i++] = *cmd++;
    path[i] = '\0';
    return (path);
}

int is_sep(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static size_t count_words(const char *s, char c)
{
    size_t count;

    count = 0;
    while (*s)
    {
        while (*s == c)
            s++;
        if (*s)
        {
            count++;
            while (*s && *s != c)
                s++;
        }
    }
    return (count);
}

static char *word_dup(const char *s, size_t start, size_t len)
{
    char *word;
    size_t i;

    i = 0;
    word = malloc(len + 1);
    if (!word)
        return (NULL);
    while (i < len)
    {
        word[i] = s[start + i];
        i++;
    }
    word[i] = '\0';
    return (word);
}

static void free_all(char **res, size_t j)
{
    while (j > 0)
        free(res[--j]);
    free(res);
}

char **ft_split(const char *s, char c)
{
    char **res;
    size_t i;
    size_t j;
    size_t start;

    if (!s)
        return (NULL);
    res = malloc(sizeof(char *) * (count_words(s, c) + 1));
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    while (s[i])
    {
        while (s[i] == c)
            i++;
        if (s[i])
        {
            start = i;
            while (s[i] && s[i] != c)
                i++;
            res[j] = word_dup(s, start, i - start);
            if (!res[j])
                return (free_all(res, j), NULL);
            j++;
        }
    }
    res[j] = NULL;
    return (res);
}