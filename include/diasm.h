
#include "../libft/libft.h"
#include "op.h"

# define FIR_ARG 0xc0
# define SEC_ARG 0x30
# define THI_ARG 0xc

typedef struct	s_diasm
{
	char		*name_cor;
	char		*name_s;
	int			fd_cor;
	int			fd_s;
}				t_diasm;
