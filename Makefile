SRC = \
    src/main.c \
    src/map.c \
    src/player.c \
    parsing/parsing_v0.c \
    parsing/parsing_v1.c \
    parsing/parsing_v2.c \
    parsing/parsing_v3.c \
    parsing/utils_v0.c \
    parsing/utils_v1.c \

OBJ = $(SRC:.c=.o)

MINILIBX_DIR = mlx
MINILIBX = $(MINILIBX_DIR)/libmlx.a

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L$(MINILIBX_DIR) -lmlx -framework OpenGL -framework AppKit

AR = ar rcs

NAME = cub3D

all : $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MINILIBX)
	@echo "compiling $(NAME)"
	@$(CC) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@sleep 1
	@echo "$(NAME) compiled successfully."

$(LIBFT):
	@echo "compiling libft"
	@$(MAKE) -C $(LIBFT_DIR)

$(MINILIBX):
	@echo "compiling minilibx"
	@$(MAKE) -C $(MINILIBX_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -I$(MINILIBX_DIR) -c $< -o $@

clean:
	@echo cleaning object files ...
	@$(RM) -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(MINILIBX_DIR) clean
	@sleep 0.5
	@echo object files cleaned.

fclean : clean
	@echo full cleaning of $(NAME) ...
	@$(RM) -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@sleep 0.5
	@echo $(NAME) cleaned.

re: fclean all