
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	src_len;
	size_t	dst_len;

	i = 0;
	j = 0;
	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	j = dst_len;
	if (size == 0 || size <= dst_len)
	{
		return (src_len + size);
	}
	while (src[i] != '\0' && i < size - dst_len - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (dst_len + src_len);
}
