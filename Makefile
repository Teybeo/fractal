NAME = fractol

SRCS += main.c
SRCS += app.c
SRCS += app_utils.c
SRCS += t_double2.c
SRCS += t_double2_util.c
SRCS += multithreading.c
SRCS += config.c
SRCS += coloring.c
SRCS += thread_pool.c
SRCS += events.c
SRCS += key_event.c
SRCS += delta_draw.c
SRCS += mouse_events.c
SRCS += drawing.c
SRCS += gradient.c
SRCS += rgb.c
SRCS += gradient_definitions.c
SRCS += work_queue.c
SRCS += fractal_list.c
SRCS += julia_functions.c
SRCS += mandelbrot_functions.c

FLAGS = -Wall -Wextra -Werror
FLAGS += -O2 -march=native

OBJ_DIR = obj
OBJ_LIST = $(SRCS:.c=.o)

OBJS = $(addprefix $(OBJ_DIR)/,$(OBJ_LIST))

LIBS = -l ft -L libft
INCLUDES = -I libft/

LIBS += libmlx.a -framework OpenGL -framework AppKit

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	make -C ./libft
	sh patch_mlx.sh
	gcc -o $(NAME) $(OBJS) $(INCLUDES) $(LIBS)

# $^ is the dependencies of the rule
# $@ is the name of the rule
$(OBJ_DIR)/%.o : %.c
	gcc $(FLAGS) $(INCLUDES) -c $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/libft/

clean :
	make clean -C ./libft
	rm -rf $(OBJ_DIR)
	rm -f libmlx.a

fclean : clean
	make fclean -C ./libft
	rm -f $(NAME)

re : fclean
	make all
