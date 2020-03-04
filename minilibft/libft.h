#ifndef LIBFT_H
# define LIBFT_H

# include "../include/diasm.h"
# include "inttypes.h"
# include "stdlib.h"

void	sys_err(char *err);
void	*ft_memccpy(void *dst, const void *src, int c, size_t n);
void	*ft_memset(void *b, int c, size_t len);
void	ft_putchar(char c);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl(char const *s);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putstr(char const *s);
void	ft_putstr_fd(char const *s, int fd);
char	*ft_strcat(char *s1, const char *s2);
void	ft_strdel(char **as);
char	*ft_strdup(const char *s1);
char	*ft_itoa(intmax_t n);
size_t	ft_strlen(const char *s);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnew(size_t size);

#endif
